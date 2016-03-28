#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include "Matrix.h"
#include <vector>
using namespace std;

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;

class Player {
public:
	Player(float x) : x(x) {}
	float x; //current x position of player
	float y = 0.0f; //current y position of player
	float height = 1.0f, width = 0.65f;
	float yDist = 0.0f; //distance moved along y-axis
	float speed = 0.1f;
};

class Ball {
public:
	float x = 0.0f, y = 0.0f; //current position of ball
	float width = 0.2f, height = 0.2f;
	float xDir = 1.0f, yDir = 1.0f; //flies upwards to the right
	float xDist = 0.0f, yDist = 0.0f; //distance ball travelled
	float speed = 2.0f;
};

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

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 640, 360);
	ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	glUseProgram(program.programID);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Matrix projectionMatrix;
	Matrix modelMatrixAkari;
	Matrix modelMatrixLeft;
	Matrix modelMatrixRight;
	Matrix modelMatrixBall;
	Matrix modelMatrixText;
	Matrix viewMatrix;

	projectionMatrix.setOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);

	GLuint akariTex = LoadTexture("akari2.png");
	GLuint playerTex = LoadTexture("blue.png");
	GLuint ballTex = LoadTexture("ballBlue.png");
	GLuint font = LoadTexture("font1.png");

	float akari[] = { -2.75f, -2.0f, 2.75f, -2.0f, 2.75f, 2.0f, -2.75f, -2.0f, 2.75f, 2.0f, -2.75f, 2.0f };
	float left[] = { -4.0f, -0.5f, -3.35f, -0.5f, -3.35f, 0.5f, -4.0f, -0.5f, -3.35f, 0.5f, -4.0f, 0.5f };
	float right[] = { 3.35f, -0.5f, 4.0f, -0.5f, 4.0f, 0.5f, 3.35f, -0.5f, 4.0f, 0.5f, 3.35f, 0.5f };
	float ball[] = { -0.1f, -0.1f, 0.1f, -0.1f, 0.1f, 0.1f, -0.1f, -0.1f, 0.1f, 0.1f, -0.1f, 0.1f };
	float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

	float lastFrameTicks = 0.0f;

	Player rightPlayer(3.675f);
	Player leftPlayer(-3.675f);
	Ball b;

	SDL_Event event;
	bool done = false;
	while (!done) {
		glClearColor(0.9f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		program.setProjectionMatrix(projectionMatrix);
		program.setViewMatrix(viewMatrix);
		glEnableVertexAttribArray(program.positionAttribute);
		glEnableVertexAttribArray(program.texCoordAttribute);
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);

		//Akari
		program.setModelMatrix(modelMatrixAkari);
		glBindTexture(GL_TEXTURE_2D, akariTex);
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, akari);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		/*modelMatrixAkari.Translate(2.75f, 2.0f, 0.0f);
		modelMatrixAkari.Rotate(30*3.1415/180);
		modelMatrixAkari.Translate(2.75f, -2.0f, 0.0f);*/

		//Left
		program.setModelMatrix(modelMatrixLeft);
		glBindTexture(GL_TEXTURE_2D, playerTex);
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, left);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//Right
		program.setModelMatrix(modelMatrixRight);
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, right);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//Ball
		program.setModelMatrix(modelMatrixBall);
		glBindTexture(GL_TEXTURE_2D, ballTex);
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, ball);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);

		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		b.xDist = b.xDir * elapsed * b.speed;
		b.yDist = b.yDir * elapsed * b.speed;
		b.x += b.xDist;
		b.y += b.yDist;

		//If ball hits rightPlayer
		if (b.x > rightPlayer.x - rightPlayer.width / 2 - b.width / 2 &&
			b.y < rightPlayer.y + rightPlayer.height / 2 + b.height / 2 &&
			b.y > rightPlayer.y - rightPlayer.height / 2 - b.height / 2) {
			b.xDir = -1.0f; //reverse direction
			b.xDist = b.xDir * elapsed * b.speed;
		}

		//If ball hits leftPlayer
		if (b.x < leftPlayer.x + leftPlayer.width / 2 + b.width / 2 &&
			b.y < leftPlayer.y + leftPlayer.height / 2 + b.height / 2 &&
			b.y > leftPlayer.y - leftPlayer.height / 2 - b.height / 2) {
			b.xDir = 1.0f; //reverse direction
			b.xDist = b.xDir * elapsed * b.speed;
		}

		//If ball hits top
		if (b.y > 2.0f - b.height / 2) {
			b.yDir = -1.0f;
			b.yDist = b.yDir * elapsed * b.speed;
		}

		//If ball hits bottom
		if (b.y < -2.0f + b.height / 2) {
			b.yDir = 1.0f;
			b.yDist = b.yDir * elapsed * b.speed;
		}

		//If ball flies out right
		if (b.x > 3.55f + b.width / 2) {
			b.speed = 0.0f;
			//Hide player bars
			modelMatrixRight.Translate(2.0f, 0.0f, 0.0f);
			modelMatrixLeft.Translate(-2.0f, 0.0f, 0.0f);
			program.setModelMatrix(modelMatrixText);
			DrawText(&program, font, "Left Side Wins", 0.4f, 0.02f);
		}

		//If ball flies out left
		if (b.x < -3.55f - b.width / 2) {
			b.speed = 0.0f;
			//Hide player bars
			modelMatrixRight.Translate(2.0f, 0.0f, 0.0f);
			modelMatrixLeft.Translate(-2.0f, 0.0f, 0.0f);
			program.setModelMatrix(modelMatrixText);
			DrawText(&program, font, "Right Side Wins", 0.4f, 0.02f);
		}

		modelMatrixBall.Translate(b.xDist, b.yDist, 0.0f);

		const Uint8* keys = SDL_GetKeyboardState(NULL);


		//Keyboard polling

		//playerRight Up
		if (keys[SDL_SCANCODE_UP]) {
			//Prevent player bar from going out of view
			if (rightPlayer.y + rightPlayer.height / 2 < 2.0f) {
				rightPlayer.yDist += elapsed*rightPlayer.speed;
				rightPlayer.y += rightPlayer.yDist;
				modelMatrixRight.Translate(0.0f, rightPlayer.yDist, 0.0f);
			}
		}

		//playerRight Down
		if (keys[SDL_SCANCODE_DOWN]) {
			//Prevent player bar from going out of view
			if (rightPlayer.y - rightPlayer.height / 2 > -2.0f) {
				rightPlayer.yDist -= elapsed*rightPlayer.speed;
				rightPlayer.y += rightPlayer.yDist;
				modelMatrixRight.Translate(0.0f, rightPlayer.yDist, 0.0f);
			}
		}

		//playerLeft Up
		if (keys[SDL_SCANCODE_W]) {
			//Prevent player bar from going out of view
			if (leftPlayer.y + leftPlayer.height / 2 < 2.0f) {
				leftPlayer.yDist += elapsed*leftPlayer.speed;
				leftPlayer.y += leftPlayer.yDist;
				modelMatrixLeft.Translate(0.0f, leftPlayer.yDist, 0.0f);
			}

			/*leftPlayer.yDist += elapsed / 10.0f;
			leftPlayer.y += leftPlayer.yDist;
			if (leftPlayer.y > 2.0f - leftPlayer.height / 2) {
			leftPlayer.y = leftPlayer.height / 2;
			}
			else {

			modelMatrixLeft.Translate(0.0f, leftPlayer.yDist, 0.0f);
			}*/
		}

		//playerLeft Down
		if (keys[SDL_SCANCODE_S]) {
			//Prevent player bar from going out of view
			if (leftPlayer.y - leftPlayer.height / 2 > -2.0f) {
				leftPlayer.yDist -= elapsed*leftPlayer.speed;
				leftPlayer.y += leftPlayer.yDist;
				modelMatrixLeft.Translate(0.0f, leftPlayer.yDist, 0.0f);
			}
			/*leftPlayer.yDist -= elapsed / 10.0f;
			leftPlayer.y += leftPlayer.yDist;
			if (leftPlayer.y < -2.0f + leftPlayer.height / 2) {
			leftPlayer.y = leftPlayer.height / 2;
			}
			else {

			modelMatrixLeft.Translate(0.0f, leftPlayer.yDist, 0.0f);
			}*/
		}


		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}

			else if (event.type == SDL_KEYUP) {
				//Reset back to 0 so yDist won't be a huge number since it is constantly being added by elapsed time
				rightPlayer.yDist = 0;
				leftPlayer.yDist = 0;
			}
		}

		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}