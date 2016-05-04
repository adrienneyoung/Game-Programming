#include "Platformer.h"
enum GameState { MAIN_MENU, GAME_LEVEL, GAME_WIN, GAME_LOSE };

void Platformer::buildLevel() {
	ifstream infile("map.txt");
	string line;
	while (getline(infile, line)) {
		if (line == "[header]") {
			if (!readHeader(infile)) {
				return;
			}
		}
		else if (line == "[layer]") {
			readLayerData(infile);
		}
		/*else if (line == "[ObjectsLayer]") {
		readEntityData(infile);
		}*/
	}
}

bool Platformer::readHeader(std::ifstream &stream) {
	string line;
	mapWidth = -1;
	mapHeight = -1;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "width") {
			mapWidth = atoi(value.c_str());
		}
		else if (key == "height"){
			mapHeight = atoi(value.c_str());
		}
	}
	if (mapWidth == -1 || mapHeight == -1) {
		return false;
	}
	else { // allocate our map data
		levelData = new unsigned char*[mapHeight];
		for (int i = 0; i < mapHeight; ++i) {
			levelData[i] = new unsigned char[mapWidth];
		}
		return true;
	}
}

bool Platformer::readLayerData(std::ifstream &stream) {
	string line;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "data") {
			for (int y = 0; y < mapHeight; y++) {
				getline(stream, line);
				istringstream lineStream(line);
				string tile;
				for (int x = 0; x < mapWidth; x++) {
					getline(lineStream, tile, ',');
					unsigned char val = (unsigned char)atoi(tile.c_str());
					if (val > 0) {
						// be careful, the tiles in this format are indexed from 1 not 0
						levelData[y][x] = val - 1;
					}
					else {
						levelData[y][x] = 0;
					}
				}
			}
		}
	}
	return true;
}

/*
bool Platformer::readEntityData(std::ifstream &stream) {
string line;
string type;
while (getline(stream, line)) {
if (line == "") { break; }
istringstream sStream(line);
string key, value;
getline(sStream, key, '=');
getline(sStream, value);
if (key == "type") {
type = value;
}
else if (key == "location") {
istringstream lineStream(value);
string xPosition, yPosition;
getline(lineStream, xPosition, ',');
getline(lineStream, yPosition, ',');
float placeX = atoi(xPosition.c_str()) / 16 * TILE_SIZE;
float placeY = atoi(yPosition.c_str()) / 16 * -TILE_SIZE;
placeEntity(type, placeX, placeY);
}
}
return true;
}
*/

void Platformer::RenderLevel(){
	vector<float> vertexData;
	vector<float> texCoordData;

	int count = 0;

	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
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
					u, v + spriteHeight,
					u + spriteWidth, v + spriteHeight,

					u, v,
					u + spriteWidth, v + spriteHeight,
					u + spriteWidth, v
				});

				count++;
			}
		}
	}

	glUseProgram(program->programID);

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
	glEnableVertexAttribArray(program->texCoordAttribute);

	glBindTexture(GL_TEXTURE_2D, spritesheet);
	glDrawArrays(GL_TRIANGLES, 0, count * 6);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);

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

Platformer::~Platformer()
{
	//Mix_FreeChunk(someSound);
	Mix_FreeMusic(music);

	delete program;
	SDL_Quit();
}

void Platformer::Setup() {
	//Pug
	pug = new Entity(0.0f, -0.9f, 3.5f, 2.0f, "pug.png");
	pug->sprite = SheetSprite(pug->tex, 4, 1, pug->width, pug->height, 2.0f);

	//Animate pug
	pug->runAnimationLeft = { 0, 1, 2, 3 };
	pug->numFrames = 4;

	//Player
	//player = new Entity(-2.0f, 1.5f, 1.0f, 1.0f, "akarispritesheet.png");
	//player->sprite = SheetSprite(player->tex, 3, 4, player->width, player->height, 1.0f);

	player = new Entity(-2.0f, 1.5f, 1.0f, 1.0f, "corgiLR.png", "corgiUD.png");
	//player->sprite = SheetSprite(player->tex, 3, 4, player->width, player->height, 1.0f);

	//Animate player
	//player->runAnimationLeft = { 3, 4, 5 };
	//player->runAnimationRight = { 6, 7, 8 };

	player->runAnimationLeft = { 0, 1, 2 };
	player->runAnimationRight = { 3, 4, 5 };
	player->runAnimationBack = { 0, 1, 2 };
	player->runAnimationFront = { 3, 4, 5 };

	//Player's bullets
	/*for (int i = 0; i < player->maxBullets; i++) {
		Entity* bullet = new Entity(player->xPos, player->yPos, 0.25f, 0.25f, "bun.png");
		bullet->sprite = SheetSprite(bullet->tex, 1, 1, bullet->width, bullet->height, 0.5f);
		bullet->isBullet = true;
		player->bullets.push_back(bullet);
	}*/

	for (int i = 0; i < player->maxBullets; i++) {
		Entity* bullet = new Entity(player->xPos, player->yPos, 0.3f, 0.15f, "stick.png");
		bullet->sprite = SheetSprite(bullet->tex, 1, 1, bullet->width, bullet->height, 0.25f);
		bullet->isBullet = true;
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
	spritesheet = LoadTexture("sheet.png");
	background = LoadTexture("park.png");

	//Stuff for background
	vertices = { -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f };
	texCoords = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

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
	glClearColor(0.9f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program->programID); //Installs a program object as part of current rendering state

	program->setProjectionMatrix(projectionMatrix);
	program->setViewMatrix(viewMatrix);

	if (state == MAIN_MENU) {
		DrawText(program, mainMenuText, font, "ANGRY DOGS", 0.4f, 0.2f, -2.6f, 1.0f);
		DrawText(program, mainMenuText2, font, "Press space to start", 0.2f, 0.1f, -2.7f, 0.3f);
		pug->Render(program, pug->matrix, pug->runAnimationLeft[pug->currentIndex]);
	}
	
	else if (state == GAME_LEVEL) {
		//Render background
		program->setModelMatrix(backgroundMatrix);
		glBindTexture(GL_TEXTURE_2D, background);
		glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
		glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
		glEnableVertexAttribArray(program->positionAttribute);
		glEnableVertexAttribArray(program->texCoordAttribute);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		backgroundMatrix.setScale(7.0f, 3.5f, 1.0f);

		//Render player
		/*if (player->directionFacing == 0)
			player->Render(program, player->matrix, 1);

			else if (player->directionFacing == 1)
			player->Render(program, player->matrix, player->runAnimationRight[player->currentIndex]);

			else if (player->directionFacing == -1)
			player->Render(program, player->matrix, player->runAnimationLeft[player->currentIndex]);*/

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
				//player->bullets[i]->matrix.setScale(0.5f, 0.5f, 1.0f);
				player->bullets[i]->Render(program, player->bullets[i]->matrix, 0);
			}
		}

		//Render walkway
		for (int i = 0; i < blocks.size(); i++) {
			blocks[i]->Render(program, blocks[i]->matrix, 1);
			blocks[i]->isStatic = true;
		}

		RenderLevel(); //Doesn't work

		string s = to_string(player->xPos);
		DrawText(program, gameText, font, "xPos: " + s, 0.2f, 0.2f, 0.0f, -0.5f);
		gameText.setPosition(-8.0f, 0.0f, 1.0f);
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

void Platformer::Update(float elapsed) {
	if (state == MAIN_MENU)
		pug->Animate(elapsed);

	else if (state == GAME_LEVEL) {
		mainMenuText.identity();
		mainMenuText2.identity();
		pug->matrix.identity();

		player->Update(elapsed);

		//Player's bullets
		for (int i = 0; i < player->maxBullets; i++)
		{
			player->bullets[i]->Update(elapsed);

			//bullet collision

			if (player->bullets[i]->xPos > 6.0f || player->bullets[i]->xPos < -10.0f) {
				player->bullets[i]->display = false;
				player->bullets[i]->xVel = 0.0f;
			}
		}

		if (player->yPos < -5.0f)
			state = GAME_LOSE;

		scrollScreen();
		handleCollisions();
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
							player->bullets[player->bulletCount]->xVel = 0.5f;
							player->bullets[player->bulletCount]->xPos += player->bullets[player->bulletCount]->xVel * FIXED_TIMESTEP;
						}

						else if (player->directionFacing == -1) {
							player->bullets[player->bulletCount]->xPos = player->xPos - player->width / 2 - player->bullets[player->bulletCount]->width / 2;
							player->bullets[player->bulletCount]->xVel = -0.5f;
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
		player->xAcc = elapsed * 10.0f;
		player->directionFacing = 1;
	}

	//Player move left
	else if (keys[SDL_SCANCODE_LEFT]) {
		player->xAcc = elapsed * -10.0f;
		player->directionFacing = -1;
	}

	else if (keys == SDL_GetKeyboardState(NULL)) {
		player->xVel = lerp(player->xVel, 0.0f, FIXED_TIMESTEP * player->xFric);
	}

	Update(elapsed);
	Render();

	return done;
}