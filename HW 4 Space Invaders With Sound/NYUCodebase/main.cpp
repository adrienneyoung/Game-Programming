#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include "Matrix.h"
#include <SDL_mixer.h>
#include <vector>
using namespace std;

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;

GLuint LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(surface);

	return textureID;
}

void DrawText(ShaderProgram *program, int fontTexture, std::string text, float size, float spacing) {
	float texture_size = 1.0 / 16.0f;
	std::vector<float> vertexData;
	std::vector<float> texCoordData;
	for (int i = 0; i < text.size(); i++) {
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
		vertexData.insert(vertexData.end(), {
			((size + spacing) * i) + (-7.55f * size), 0.5f * size,
			((size + spacing) * i) + (-7.55f * size), -0.5f * size,
			((size + spacing) * i) + (-6.55f * size), 0.5f * size,
			((size + spacing) * i) + (-6.55f * size), -0.5f * size,
			((size + spacing) * i) + (-6.55f * size), 0.5f * size,
			((size + spacing) * i) + (-7.55f * size), -0.5f * size,
		});
		texCoordData.insert(texCoordData.end(), {
			texture_x, texture_y,
			texture_x, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x + texture_size, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x, texture_y + texture_size,
		});
	}
	glUseProgram(program->programID);
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
	glEnableVertexAttribArray(program->positionAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
	glEnableVertexAttribArray(program->texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glDrawArrays(GL_TRIANGLES, 0, text.size() * 6);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}

void DrawSpriteSheetSprite(ShaderProgram *program, int index, int spriteCountX,
	int spriteCountY, float vertices[], GLuint texture) {
	float u = (float)(((int)index) % spriteCountX) / (float)spriteCountX;
	float v = (float)(((int)index) / spriteCountX) / (float)spriteCountY;
	float spriteWidth = 1.0 / (float)spriteCountX;
	float spriteHeight = 1.0 / (float)spriteCountY;

	GLfloat texCoords[] = {
		u, v + spriteHeight, //0,1
		u + spriteWidth, v + spriteHeight, //1,1
		u + spriteWidth, v, //1,0
		u, v + spriteHeight, //0,1
		u + spriteWidth, v, //1,0
		u, v //0,0
	};

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);

	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

class Entity {
public:
	Entity(float x, float y, float width, float height, float speed, const char* texPath, float scale)
		: x(x), y(y), width(width), height(height), speed(speed), scale(scale)
	{
		tex = LoadTexture(texPath);
		xMovement = 0.0f;
		yMovement = 0.0f;
		xDir = 1.0f;
		yDir = 1.0f;
		vertices = { -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f }; //around origin
	}

	float x, y, xMovement, yMovement, xDir, yDir, width, height, speed, scale; // (x,y) is the center of the entity
	bool display = false;
	Matrix matrix;
	GLuint tex;
	vector<float> vertices;

	void transform(ShaderProgram& program) {
		program.setModelMatrix(matrix);
		matrix.setPosition(x, y, 0.0f);
		matrix.setScale(scale, scale, 1.0f);
	}

	bool collidesWith(Entity* e) {
		//is R1’s bottom higher than R2’s top ?
		//is R1’s top lower than R2’s bottom ?
		//is R1’s left larger than R2’s right ?
		//is R1’s right smaller than R2’s left ?
		//If ANY of the above are true, then the two rectangles are NOT intersecting!
		//The rectangles are intersecting if NONE of the above are true.

		if (y - height / 2 < e->y + e->height / 2 && y + height / 2 > e->y - e->height / 2 &&
			x - width / 2 < e->x + e->width / 2 && x + width / 2 > e->x - e->width / 2)
			return true;

		return false;
	}
};

void setup() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
	glewInit();
#endif
}

void cleanup(ShaderProgram& program) {
	glDisableVertexAttribArray(program.positionAttribute);
	glDisableVertexAttribArray(program.texCoordAttribute);

	SDL_Quit();
}

int main(int argc, char *argv[])
{
	setup();

	glViewport(0, 0, 640, 360);
	ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	glUseProgram(program.programID);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Matrix projectionMatrix;
	Matrix viewMatrix;
	Matrix modelMatrixText;

	projectionMatrix.setOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);

	//Music
	//Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

	//Mix_Music* chinatsu;
	//chinatsu = Mix_LoadMUS("chinatsu.mp3");
	//Mix_PlayMusic(chinatsu, -1);

	//Mix_Chunk* akarin;
	//akarin = Mix_LoadWAV("akarin.wav");
	//Mix_Music *akarin;
	//akarin = Mix_LoadMUS("akarin.mp3");

	GLuint font = LoadTexture("font1.png");
	float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
	float lastFrameTicks = 0.0f;
	int gameState = 0;

	//Player
	Entity player(0.0f, -1.5f, 0.5f, 0.5f, 2.5f, "akarispritesheet.png", 0.25f);
	player.display = true;

	//Player's bullets
	vector<Entity> bullets;
	int maxBullets = 5;
	int bulletCount = 0;

	//Store player's bullets in vector
	for (int i = 0; i < maxBullets; i++) {
		Entity bullet(0.0f, 4.0f, 0.2f, 0.2f, 2.5f, "bun.png", 0.1f);
		bullets.push_back(bullet);
	}

	//Enemies
	vector<Entity> enemies;
	int enemyRows = 3;
	int enemyColumns = 6;
	int enemyCount = enemyRows * enemyColumns;
	float offsetX = 0.35f;
	float offsetY = 0.15f;

	//Store enemies in vector
	for (int i = 0; i < enemyRows; i++) {
		for (int j = 0; j < enemyColumns; j++) {
			//Space apart the enemies 
			Entity enemy(-3.0f + (j*(0.4f + offsetX)), 1.5f - (i*(0.4f + offsetY)), 0.4f, 0.4f, 0.8f, "kyubeyspritesheet.png", 0.25f);
			enemy.display = true;
			enemies.push_back(enemy);
		}
	}


	vector<Entity> enemyBullets;
	int maxEnemyBullets = enemies.size();
	int enemyBulletCount = 0;

	//Store enemy bullets in vector
	for (int i = 0; i < maxEnemyBullets; i++) {
		Entity eBullet(0.0f, 0.0f, 0.2f, 0.2f, 2.5f, "kyubeyspritesheet.png", 0.1f);
		enemyBullets.push_back(eBullet);

		//Bullet comes out of the bottom of the enemy
		enemyBullets[i].x = enemies[i].x;
		enemyBullets[i].y = enemies[i].y - enemies[i].height / 2;
		enemyBullets[i].transform(program);
	}

	float time = 0.0f;
	int random = 0;

	SDL_Event event;
	bool done = false;
	while (!done) {
		glClearColor(0.9f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		program.setProjectionMatrix(projectionMatrix);
		program.setViewMatrix(viewMatrix);
		glEnableVertexAttribArray(program.positionAttribute);
		glEnableVertexAttribArray(program.texCoordAttribute);

		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		/*
		random = rand() % enemies.size();
		time += elapsed;
		if (time > 3.0f) {


		//Draw enemy's bullets
		//if (i == random) {
		enemyBullets[random].display = true;
		enemyBullets[random].transform(program);
		DrawSpriteSheetSprite(&program, 7, 3, 4, enemyBullets[random].vertices.data(), enemyBullets[random].tex);
		enemyBullets[random].yDir = -1.0f;
		enemyBullets[random].yMovement = elapsed * enemyBullets[random].speed * enemyBullets[random].yDir;
		enemyBullets[random].y += enemyBullets[random].yMovement;
		enemyBullets[random].matrix.Translate(0.0f, enemyBullets[random].yMovement, 0.0f);
		//Enemy bullet hits player
		if (enemyBullets[random].collidesWith(&player) && enemyBullets[random].display)
		gameState = 2;
		//}


		enemyBullets[0].display = true;
		enemyBullets[0].transform(program);
		DrawSpriteSheetSprite(&program, 7, 3, 4, enemyBullets[0].vertices.data(), enemyBullets[0].tex);
		enemyBullets[0].yDir = -1.0f;
		enemyBullets[0].yMovement = elapsed * enemyBullets[0].speed * enemyBullets[0].yDir;
		enemyBullets[0].y += enemyBullets[0].yMovement;
		enemyBullets[0].matrix.Translate(0.0f, enemyBullets[0].yMovement, 0.0f);
		//Enemy bullet hits player
		if (enemyBullets[0].collidesWith(&player) && enemyBullets[0].display)
		gameState = 2;

		}
		if (time > 4.0f)
		time = 0;
		*/
		//Title screen
		if (gameState == 0) {
			program.setModelMatrix(modelMatrixText);
			DrawText(&program, font, "Press g to start", 0.3f, 0.05f);
		}

		//Loser screen
		else if (gameState == 2) {
			program.setModelMatrix(modelMatrixText);
			DrawText(&program, font, "You lose", 0.3f, 0.05f);
		}

		//Winner screen
		else if (gameState == 3) {
			program.setModelMatrix(modelMatrixText);
			DrawText(&program, font, "You win", 0.3f, 0.05f);
		}

		//Game is running
		else if (gameState = 1) {
			/*time += elapsed * 20.0f;
			program.setModelMatrix(modelMatrixText);
			string s = to_string(time);
			DrawText(&program, font, s, 0.2f, 0.2f);*/

			//Draw player
			if (player.display) {
				player.transform(program);
				DrawSpriteSheetSprite(&program, 1, 3, 4, player.vertices.data(), player.tex);
			}

			//Draw enemies
			for (int i = 0; i < enemies.size(); i++) {
				if (enemies[i].display) {
					enemies[i].transform(program);
					DrawSpriteSheetSprite(&program, 2, 3, 4, enemies[i].vertices.data(), enemies[i].tex);

					//Move enemies
					enemies[i].xMovement = elapsed * enemies[i].speed * enemies[i].xDir;
					enemies[i].x += enemies[i].xMovement;
					enemies[i].matrix.Translate(enemies[i].xMovement, 0.0f, 0.0f);

					//Enemies hit right side
					if (enemies[i].x + enemies[i].width / 2 > 3.2f) {
						for (int j = 0; j < enemies.size(); j++) {
							enemies[j].xDir = -1.0f; //Go back to the left
							enemies[j].yDir = -1.0f; //Enemies move down
							enemies[j].y += elapsed * 200.0f * enemies[j].yDir;
							enemies[j].matrix.Translate(0.0f, elapsed * 200.0f * enemies[j].yDir, 0.0f);
						}
					}

					//Enemies hit left side
					else if (enemies[i].x - enemies[i].width / 2 < -3.2f) {
						for (int j = 0; j < enemies.size(); j++) {
							enemies[j].xDir = 1.0f; //Go back to the right
							enemies[j].yDir = -1.0f; //Enemies move down
							enemies[j].y += elapsed * 200.0f * enemies[j].yDir;
							enemies[j].matrix.Translate(0.0f, elapsed * 200.0f * enemies[j].yDir, 0.0f);
						}
					}

					//Enemies hit player
					if (enemies[i].y - enemies[i].height / 2 < player.y + player.height / 2)
						gameState = 2;
				}
			}

			//Draw player's bullets
			for (int i = 0; i < maxBullets; i++) {
				if (bullets[i].display) {
					bullets[i].transform(program);

					glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
					glBindTexture(GL_TEXTURE_2D, bullets[i].tex);
					glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, bullets[i].vertices.data());
					glDrawArrays(GL_TRIANGLES, 0, 6);

					bullets[i].yMovement = elapsed * bullets[i].speed * bullets[i].yDir;
					bullets[i].y += bullets[i].yMovement;
					bullets[i].matrix.Translate(0.0f, bullets[i].yMovement, 0.0f);

					//If bullets fly off screen
					if (bullets[i].y + bullets[i].height / 2 > 2.0f) {
						bullets[i].display = false;
						bullets[i].speed = 0.0f;
					}

					//Collision
					for (int j = 0; j < enemies.size(); j++) {
						if (bullets[i].collidesWith(&enemies[j])) {
							//If bullet hits an enemy that's alive
							if (enemies[j].display) {
								bullets[i].display = false;
								bullets[i].speed = 0.0f;
								enemyCount--;

								//Mix_PlayChannel(-1, akarin, 0);
							}

							enemies[j].display = false;
						}
					}
				}
			}

			if (enemyCount == 0)
				gameState = 3;
		}

		//Keyboard polling
		const Uint8* keys = SDL_GetKeyboardState(NULL);

		if (gameState == 1) {
			//Player move right
			if (keys[SDL_SCANCODE_RIGHT]) {
				//Prevent player from going out of view
				if (player.x + player.width / 2 < 3.3f) {
					player.xDir = 1.0f;
					player.xMovement = elapsed * player.speed * player.xDir;
					player.x += player.xMovement;
					player.matrix.Translate(player.xMovement, 0.0f, 0.0f);
				}
			}

			//Player move left
			if (keys[SDL_SCANCODE_LEFT]) {
				//Prevent player from going out of view
				if (player.x - player.width / 2 / 2 > -3.3f) {
					player.xDir = -1.0f;
					player.xMovement = elapsed * player.speed * player.xDir;
					player.x += player.xMovement;
					player.matrix.Translate(player.xMovement, 0.0f, 0.0f);
				}
			}
		}

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}

			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.scancode == SDL_SCANCODE_G) {
					if (gameState == 0)
						gameState = 1;
				}
			}

			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
					if (gameState == 1) {
						//If the bullet hasn't been fired yet, display it
						if (!bullets[bulletCount].display)
						{
							bullets[bulletCount].display = true;
							bullets[bulletCount].speed = 2.0f;

							//Bullet comes out of the top of the player
							bullets[bulletCount].x = player.x;
							bullets[bulletCount].y = player.y + player.height / 2;
							bullets[bulletCount].matrix.setPosition(player.x, player.y + player.height / 2, 0.0f);
						}

						bulletCount++;

						if (bulletCount > maxBullets - 1)
							bulletCount = 0;
					}
				}
			}

			if (event.type == SDL_KEYUP) {
				if (gameState == 1) {
					//Reset back to 0 so xMovement won't be a huge number since it is constantly being added by elapsed time
					player.xMovement = 0;
				}
			}
		}

		SDL_GL_SwapWindow(displayWindow);
	}
	//Mix_FreeMusic(chinatsu);
	//Mix_FreeChunk(akarin);
	cleanup(program);
	return 0;
}