#include "Platformer.h"
enum GameState { MAIN_MENU, GAME_LEVEL, GAME_WIN, GAME_LOSE };

unsigned char level1Data[LEVEL_HEIGHT][LEVEL_WIDTH] = {
	{ 0, 0, 0, 0, 0, 6, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 6, 6, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 6, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 6, 0, 0, 0 },
	{ 6, 6, 6, 6, 0, 0, 0, 6, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 6, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 6 },
	{ 6, 6, 6, 6, 0, 0, 0, 0, 0, 6 }
};

void Platformer::BuildLevel() {
	memcpy(levelData, level1Data, LEVEL_HEIGHT*LEVEL_WIDTH);
}

void Platformer::RenderLevel(){
	vector<float> vertexData;
	vector<float> texCoordData;
	for (int y = 0; y < LEVEL_HEIGHT; y++) {
		for (int x = 0; x < LEVEL_WIDTH; x++) {
			if (levelData[y][x] != 0) {
				float u = (float)(((int)levelData[y][x]) % SPRITE_COUNT_X) / (float)SPRITE_COUNT_X;
				float v = (float)(((int)levelData[y][x]) / SPRITE_COUNT_X) / (float)SPRITE_COUNT_Y;

				float spriteWidth = 1.0f / (float)SPRITE_COUNT_X;
				float spriteHeight = 1.0f / (float)SPRITE_COUNT_Y;

				vertexData.insert(vertexData.end(), {
					TILE_SIZE * x, -TILE_SIZE * y,
					TILE_SIZE * x, (-TILE_SIZE * y) - TILE_SIZE,
					(TILE_SIZE * x) + TILE_SIZE, (-TILE_SIZE * y) - TILE_SIZE,
					TILE_SIZE * x, -TILE_SIZE * y,
					(TILE_SIZE * x) + TILE_SIZE, (-TILE_SIZE * y) - TILE_SIZE,
					(TILE_SIZE * x) + TILE_SIZE, -TILE_SIZE * y
				});

				texCoordData.insert(texCoordData.end(), {
					u, v,
					u, v + (spriteHeight),
					u + spriteWidth, v + (spriteHeight),
					u, v,
					u + spriteWidth, v + (spriteHeight),
					u + spriteWidth, v
				});
			}

			glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
			glEnableVertexAttribArray(program->positionAttribute);

			glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
			glEnableVertexAttribArray(program->texCoordAttribute);

			glBindTexture(GL_TEXTURE_2D, spritesheet);
			glDrawArrays(GL_TRIANGLES, 0, 6 * LEVEL_HEIGHT * LEVEL_WIDTH);

			glDisableVertexAttribArray(program->positionAttribute);
			glDisableVertexAttribArray(program->texCoordAttribute);
		}
	}
}

void Platformer::worldToTileCoordinates(float worldX, float worldY, int *gridX, int *gridY) {
	*gridX = (int)(worldX / TILE_SIZE);
	*gridY = (int)(-worldY / TILE_SIZE);
}

Platformer::Platformer() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Angry Dogs", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 640, 360);
	program = new ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");

	projectionMatrix.setOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);

	Setup();
}

Platformer::~Platformer() {
	//Mix_FreeChunk(someSound);
	Mix_FreeMusic(music);

	delete program;
	SDL_Quit();
}

void Platformer::Setup() {
	//BuildLevel();

	player = new Entity(-2.0f, 1.5f, 1.0f, 1.0f, "corgiLR.png", "corgiUD.png");
	//player->entityType = PLAYER;

	player->runAnimationLeft = { 0, 1, 2 };
	player->runAnimationRight = { 3, 4, 5 };
	player->runAnimationBack = { 0, 1, 2 };
	player->runAnimationFront = { 3, 4, 5 };

	//Player's bullets
	for (int i = 0; i < player->maxBullets; i++) {
		Entity* bullet = new Entity(player->xPos, player->yPos, 0.3f, 0.15f, "stick.png");
		bullet->sprite = SheetSprite(bullet->tex, 1, 1, bullet->width, bullet->height, 0.25f);
		bullet->isBullet = true;
		//bullet->entityType = BULLET;
		player->bullets.push_back(bullet);
	}

	//Walkway
	for (int i = 0; i < 35; i++) {
		Entity* block3 = new Entity(-8.5f + (i*0.5f), -3.25f, 0.5f, 0.5f, "sheet.png");
		block3->sprite = SheetSprite(block3->tex, 14, 7, block3->width, block3->height, 0.5f);
		blocks.push_back(block3);
	}

	//Textures
	font = LoadTexture("font2.png");
	spritesheet = LoadTexture("bun.png");
	gameBackground = LoadTexture("park2.png");
	mainBackground = LoadTexture("city.png");

	//Stuff for scrolling background
	/*svertices = { -12.0f, -2.0f, 0.0f, -2.0f, 0.0f, 2.0f, -12.0f, -2.0f, 0.0f, 2.0f, -12.0f, 2.0f };
	stexCoords = { 0.0f + textureOffsetX, 1.0f,
		2.0f + textureOffsetX, 1.0f,
		2.0f + textureOffsetX, 0.0f,
		0.0f + textureOffsetX, 1.0f,
		2.0f + textureOffsetX, 0.0f,
		0.0f + textureOffsetX, 0.0f };*/
	//svertices = { -15.0f, -2.0f, 15.0f, -2.0f, 15.0f, 2.0f, -15.0f, -2.0f, 15.0f, 2.0f, -15.0f, 2.0f };
	svertices = { -32.0f, -2.0f, 
				0.0f, -2.0f, 
				0.0f, 2.0f, 
				-32.0f, -2.0f, 
				0.0f, 2.0f, 
				-32.0f, 2.0f };
	stexCoords = { 0.0f + textureOffsetX, 1.0f,
		2.0f + textureOffsetX, 1.0f,
		2.0f + textureOffsetX, 0.0f,
		0.0f + textureOffsetX, 1.0f,
		2.0f + textureOffsetX, 0.0f,
		0.0f + textureOffsetX, 0.0f }; 

	//Stuff for background
	vertices = { -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f };
	texCoords = { 0.0, 1.0, 
				1.0, 1.0, 
				1.0, 0.0, 
				0.0, 1.0, 
				1.0, 0.0, 
				0.0, 0.0 };

	//Main music
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	music = Mix_LoadMUS("fate.mp3");
	//music = Mix_LoadMUS("fate2.mp3");
	//music = Mix_LoadMUS("fate3.mp3");
	//music = Mix_LoadMUS("fate4.mp3");
	//music = Mix_LoadMUS("fate5.mp3"); 

	//music = Mix_LoadMUS("homura.mp3"); 
	//music = Mix_LoadMUS("sayaka.mp3"); 
	//music = Mix_LoadMUS("pmmm.mp3"); 
	//music = Mix_LoadMUS("pmmm2.mp3"); I REALLY LIKE THIS ONE. I THINK IT WAS PLAYED WHEN HOMURA WAS FIGHTING THE HUGE BOSS WITCH
	//music = Mix_LoadMUS("pmmm3.mp3"); THIS ONE IS KIND OF CREEPY 

	//music = Mix_LoadMUS("knk.mp3");
	//music = Mix_LoadMUS("knk2.mp3"); I LIKE THIS ONE A LOT
	//music = Mix_LoadMUS("knk3.mp3"); THIS ONE IS REALLY GOOD

	//music = Mix_LoadMUS("klk.mp3");
	//music = Mix_LoadMUS("klk2.mp3");

	//music = Mix_LoadMUS("snk.mp3");
	//music = Mix_LoadMUS("snk2.mp3");

	//CREEPY SONGS
	//music = Mix_LoadMUS("drrr.mp3");
	//music = Mix_LoadMUS("drrr2.mp3");

	Mix_PlayMusic(music, -1);
}




void Platformer::Render() {
	glClearColor(0.2f, 0.17f, 0.33f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program->programID); //Installs a program object as part of current rendering state
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	program->setProjectionMatrix(projectionMatrix);
	program->setViewMatrix(viewMatrix);

	if (state == MAIN_MENU) {
		//Render background
		program->setModelMatrix(backgroundMatrix);
		glBindTexture(GL_TEXTURE_2D, mainBackground);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, svertices.data());
		glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, stexCoords.data());
		glEnableVertexAttribArray(program->positionAttribute);
		glEnableVertexAttribArray(program->texCoordAttribute);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		string s = to_string(textureOffsetX);
		DrawText(program, mainMenuText, font, "ANGRY DOGS", 0.6f, 0.1f, -3.0f, 1.3f);
		DrawText(program, mainMenuText2, font, "Press space to start", 0.15f, 0.07f, -2.0f, -1.7f);
		DrawText(program, gameText, font, s, 0.15f, 0.07f, -2.0f, -1.4f);
		player->sprite = SheetSprite(player->tex, 3, 2, player->width, player->height, 1.0f);
		player->Render(program, player->matrix, player->runAnimationLeft[player->currentIndex]);
	}
	
	else if (state == GAME_LEVEL) {
		//RenderLevel();

		//Render background
		program->setModelMatrix(backgroundMatrix2);
		glBindTexture(GL_TEXTURE_2D, gameBackground);
		glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
		glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
		glEnableVertexAttribArray(program->positionAttribute);
		glEnableVertexAttribArray(program->texCoordAttribute);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		backgroundMatrix2.setScale(14.0f, 3.5f, 1.0f);

		//Render player
		if (player->directionFacing == 0) {
			player->sprite = SheetSprite(player->tex2, 3, 2, player->width, player->height, 1.0f);
			player->Render(program, player->matrix, 3);
		}

		else if (player->directionFacing == 1) {
			player->sprite = SheetSprite(player->tex, 3, 2, player->width, player->height, 1.0f);
			player->Render(program, player->matrix, player->runAnimationRight[player->currentIndex]);
		}

		else if (player->directionFacing == -1) {
			player->sprite = SheetSprite(player->tex, 3, 2, player->width, player->height, 1.0f);
			player->Render(program, player->matrix, player->runAnimationLeft[player->currentIndex]);
		}

		//Render player's bullets
		for (int i = 0; i < player->maxBullets; i++) {
			if (player->bullets[i]->display) {
				player->bullets[i]->Render(program, player->bullets[i]->matrix, 0);
			}
		}

		//Render walkway
		for (int i = 0; i < blocks.size(); i++) {
			blocks[i]->Render(program, blocks[i]->matrix, 1);
			blocks[i]->isStatic = true;
		}

		//BuildLevel();
		//RenderLevel(); //Doesn't work

		//string s = to_string(player->xPos);
		//DrawText(program, gameText, font, "xPos: " + s, 0.2f, 0.2f, 0.0f, -0.5f);
		//gameText.setPosition(-8.0f, 0.0f, 1.0f);
	}

	else if (state = GAME_LOSE) {
		DrawText(program, gameText, font, "You died", 0.2f, 0.2f, 5.0f, -0.5f);
	}

	SDL_GL_SwapWindow(displayWindow);
}

void Platformer::scrollScreen()
{
	viewMatrix.identity();
	/*if (player->xPos > -3.39999f && player->yPos > -1.49999f)
	viewMatrix.Translate(-(player->xPos), -(player->yPos), 0.0f);
	else
	viewMatrix.setPosition(3.4f, 1.5f, 0.0f);*/

	if (player->yPos < -1.49999f)
		//viewMatrix.Translate(-(player->xPos), -(player->yPos), 0.0f);
		viewMatrix.setPosition(-(player->xPos), 1.5f, 0.0f);
	//if (player->xPos < -3.29999f)
	//	viewMatrix.setPosition(-3.3f, -(player->yPos), 0.0f);
	//else
	//	viewMatrix.setPosition(-(player->xPos), 1.5f, 0.0f);
	else
		viewMatrix.Translate(-(player->xPos), -(player->yPos), 0.0f);

	program->setViewMatrix(viewMatrix);
}

void Platformer::Update(float fixedElapsed) {
	if (state == MAIN_MENU) {
		if (textureOffsetX > 28.45f)
			textureOffsetX = 3.55;

		textureOffsetX += fixedElapsed;
		backgroundMatrix.setPosition(textureOffsetX, 0.0f, 0.0f);

		player->matrix.identity();
		player->Animate(fixedElapsed);
		player->matrix.setPosition(-0.15f, -1.0f, 1.0f);
	}

	else if (state == GAME_LEVEL) {
		mainMenuText.identity();
		mainMenuText2.identity();

		player->Update(fixedElapsed);

		//Player's bullets
		for (int i = 0; i < player->maxBullets; i++)
		{
			player->bullets[i]->Update(fixedElapsed);

			//bullet collision

			if (player->bullets[i]->xPos > 10.0f || player->bullets[i]->xPos < -10.0f) {
				player->bullets[i]->display = false;
				player->bullets[i]->xVel = 0.0f;
			}
		}

		if (player->yPos < -5.0f)
			state = GAME_LOSE;

		scrollScreen();
		handleCollisions();
	}

	else if (state == GAME_LOSE) {
		backgroundMatrix.identity();
		gameText.identity();
		viewMatrix.identity();
	}
}

void Platformer::handleCollisions() {
	for (int i = 0; i < blocks.size(); i++) {
		if (player->collidesWith(blocks[i])) {
			player->handleCollision(blocks[i]);
		}
	}

	for (int i = 0; i < blocks.size(); i++) {
		player->checkCollision(blocks[i]);

		if (player->collidedBottom)
			break;
	}
}

bool Platformer::Run()
{
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	float fixedElapsed = elapsed;
	if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) {
		fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
	}
	while (fixedElapsed >= FIXED_TIMESTEP) {
		fixedElapsed -= FIXED_TIMESTEP;
		Update(FIXED_TIMESTEP);
	}

	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}

		if (event.type == SDL_KEYUP) {
			player->yAcc = 0.0f;
			player->xAcc = 0.0f;
			player->directionFacing = 0;
		}

		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_SPACE && state == MAIN_MENU)
				state = GAME_LEVEL;

			//Jump
			if (event.key.keysym.scancode == SDL_SCANCODE_UP  && state == GAME_LEVEL) {
				if (player->collidedBottom)
					player->yVel = 0.1f;
			}

			///Fire bullets
			if (event.key.keysym.scancode == SDL_SCANCODE_SPACE && state == GAME_LEVEL) {
				if (player->directionFacing == 1 || player->directionFacing == -1) {
					//If the bullet hasn't been fired yet, display it
					if (!player->bullets[player->bulletCount]->display) {
						player->bullets[player->bulletCount]->display = true;

						//Bullet comes out of the front of the player
						if (player->directionFacing == 1) {
							player->bullets[player->bulletCount]->xPos = player->xPos + player->width / 2 + player->bullets[player->bulletCount]->width / 2;
							player->bullets[player->bulletCount]->xVel = 0.3f;
							player->bullets[player->bulletCount]->xPos += player->bullets[player->bulletCount]->xVel * FIXED_TIMESTEP;
						}

						else if (player->directionFacing == -1) {
							player->bullets[player->bulletCount]->xPos = player->xPos - player->width / 2 - player->bullets[player->bulletCount]->width / 2;
							player->bullets[player->bulletCount]->xVel = -0.3f;
							player->bullets[player->bulletCount]->xPos -= player->bullets[player->bulletCount]->xVel * FIXED_TIMESTEP;
						}

						player->bullets[player->bulletCount]->yPos = player->yPos;

						player->bullets[player->bulletCount]->matrix.setPosition(player->xPos, player->yPos, 0.0f);
					}
				}

				player->bulletCount++;

				if (player->bulletCount > player->maxBullets - 1)
					player->bulletCount = 0;
			}
		}
	}

	//Keyboard polling
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	//Player move right
	if (keys[SDL_SCANCODE_RIGHT]) {
		player->xAcc = elapsed * 7.0f;
		player->directionFacing = 1;
	}

	//Player move left
	else if (keys[SDL_SCANCODE_LEFT]) {
		player->xAcc = elapsed * -7.0f;
		player->directionFacing = -1;
	}

	else if (keys == SDL_GetKeyboardState(NULL)) {
		player->xVel = lerp(player->xVel, 0.0f, FIXED_TIMESTEP * player->xFric);
	}

	Update(elapsed);
	Render();

	return done;
}