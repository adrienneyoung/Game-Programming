#include "Platformer.h"

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
	displayWindow = SDL_CreateWindow("Scrolling Platformer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
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

void Platformer::scrollScreen()
{
	viewMatrix.identity();
	viewMatrix.Translate(-(player->xPos), -(player->yPos), 0.0f);
	program->setViewMatrix(viewMatrix);

	//make it not scroll past a certain point so bg img will always be visible
}

void Platformer::Setup() {
	//Player
	player = new Entity(-2.0f, 2.0f, 0.5f, 0.5f, "akarispritesheet.png");
	player->sprite = SheetSprite(player->tex, 3, 4, player->width, player->height, 0.5f);

	//Animate player
	player->runAnimationLeft = { 3, 4, 5 };
	player->runAnimationRight = { 6, 7, 8 };
	player->runAnimationFront = { 0, 1, 2 };
	player->runAnimationBack = { 9, 10, 11 };

	//Player's bullets
	for (int i = 0; i < player->maxBullets; i++) {
		Entity* bullet = new Entity(player->xPos, player->yPos, 0.25f, 0.25f, "bun.png");
		bullet->sprite = SheetSprite(bullet->tex, 1, 1, bullet->width, bullet->height, 0.5f);
		bullet->isBullet = true;
		player->bullets.push_back(bullet);
	}

	//Blocks
	//block = new Entity(-1.0f, 0.25f, 0.5f, 0.5f, "sheet.png");
	//block->sprite = SheetSprite(block->tex, 14, 7, block->width, block->height, 0.5f);

	/*for (int i = 0; i < 5; i++) {
		Entity* block3 = new Entity(-2.5f + (i*0.5f), -2.5f + (i*0.25f), 0.5f, 0.5f, "sheet.png");
		block3->sprite = SheetSprite(block3->tex, 14, 7, block3->width, block3->height, 0.5f);
		blocks.push_back(block3);
	}*/

	//Walkway
	for (int i = 0; i < 5; i++) {
		Entity* block3 = new Entity(-3.5f + (i*0.5f), -2.5f, 0.5f, 0.5f, "sheet.png");
		block3->sprite = SheetSprite(block3->tex, 14, 7, block3->width, block3->height, 0.5f);
		blocks.push_back(block3);
	}

	//Staircase
	for (int i = 0; i < 5; i++) {
		Entity* block3 = new Entity(-2.5f + (i*0.5f), -1.5f + (i*0.25f), 0.5f, 0.5f, "sheet.png");
		block3->sprite = SheetSprite(block3->tex, 14, 7, block3->width, block3->height, 0.5f);
		blocks2.push_back(block3);
	}
	
	//Randomly generate blocks
	srand(time(NULL));
	for (int i = 0; i < 5; i++) {
		Entity* block3 = new Entity(-5.5f + (i*0.5f), -2.5f + (rand() % 3) / 0.5f, 0.5f, 0.5f, "sheet.png");
		block3->sprite = SheetSprite(block3->tex, 14, 7, block3->width, block3->height, 0.5f);
		staticEntities.push_back(block3);
	}

	//Textures
	font = LoadTexture("font2.png");
	spritesheet = LoadTexture("sheet.png");
	background = LoadTexture("bg2.png");

	//Stuff for background
	vertices = { -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f };
	texCoords = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

	//Main music
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	music = Mix_LoadMUS("knk.mp3");
	Mix_PlayMusic(music, -1);
}

void Platformer::Render() {
	glClearColor(0.9f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program->programID); //Installs a program object as part of current rendering state

	program->setProjectionMatrix(projectionMatrix);
	program->setViewMatrix(viewMatrix);

	//Render background
	program->setModelMatrix(backgroundMatrix);
	glBindTexture(GL_TEXTURE_2D, background);
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
	glEnableVertexAttribArray(program->positionAttribute);
	glEnableVertexAttribArray(program->texCoordAttribute);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	backgroundMatrix.setScale(12.0f, 4.0f, 1.0f);

	//Render player
	//player->matrix.setScale(3.0, 3.0, 1.0);
	if (player->directionFacing == 0)
		player->Render(program, player->matrix, 1);

	else if (player->directionFacing == 1)
		player->Render(program, player->matrix, player->runAnimationRight[player->currentIndex]);

	else if (player->directionFacing == -1)
		player->Render(program, player->matrix, player->runAnimationLeft[player->currentIndex]);

	else if (player->directionFacing == 2)
		player->Render(program, player->matrix, player->runAnimationFront[player->currentIndex]);

	else if (player->directionFacing == -2)
		player->Render(program, player->matrix, player->runAnimationBack[player->currentIndex]);

	//Render player's bullets
	for (int i = 0; i < player->maxBullets; i++) {
		if (player->bullets[i]->display) {
			player->bullets[i]->matrix.setScale(0.5f, 0.5f, 1.0f);
			player->bullets[i]->Render(program, player->bullets[i]->matrix, 0);
		}
	}

	//Render blocks
	//block->Render(program, block->matrix, 10);

	for (int i = 0; i < blocks.size(); i++) {
		blocks[i]->Render(program, blocks[i]->matrix, 1);
		blocks[i]->isStatic = true;
	}

	for (int i = 0; i < blocks2.size(); i++) {
		blocks2[i]->Render(program, blocks[i]->matrix, 30);
		blocks2[i]->isStatic = true;
	}

	for (int i = 0; i < staticEntities.size(); i++) {
		staticEntities[i]->Render(program, staticEntities[i]->matrix, 30);
		staticEntities[i]->isStatic = true;
	}

	RenderLevel(); //Doesn't work
	
	//string s = to_string(player->runAnimationRight[player->currentIndex]);
	//string s = to_string(player->bullets[1]->xPos);
	//string s = to_string(blocks[2]->yPos + blocks[2]->height / 2);

	/*
	//Block top
	program->setModelMatrix(modelMatrixText);
	modelMatrixText.setPosition(0.0f, 0.0f, 0.0f);
	string s = to_string(blocks[1]->yPos + blocks[1]->height / 2);
	DrawText(program, font, s, 0.2f, 0.2f);

	//Player bot normal
	program->setModelMatrix(modelMatrixText2);
	modelMatrixText2.setPosition(0.0f, -0.5f, 0.0f);
	string s2 = to_string((player->yPos - player->height / 2));
	DrawText(program, font, s2, 0.2f, 0.2f);

	//Player bot
	program->setModelMatrix(modelMatrixText3);
	modelMatrixText3.setPosition(0.0f, -1.0f, 0.0f);
	string s3 = to_string((player->yPos - player->height / 2) * 1.1f);
	DrawText(program, font, s3, 0.2f, 0.2f);

	program->setModelMatrix(modelMatrixText4);
	modelMatrixText4.setPosition(0.0f, -1.5f, 0.0f);
	string s4 = to_string(player->collidedBottom);
	DrawText(program, font, s4, 0.2f, 0.2f);
	*/

	SDL_GL_SwapWindow(displayWindow);
}

void Platformer::Update(float elapsed) {
	player->Update(elapsed);

	//Player's bullets
	for (int i = 0; i < player->maxBullets; i++)
	{
		player->bullets[i]->Update(elapsed);

		//If bullets fly off screen
		/*if (player->bullets[i]->xPos + player->bullets[i]->width / 2 > 2.0f ||
			player->bullets[i]->xPos - player->bullets[i]->width / 2 > -4.0f) {
			player->bullets[i]->display = false;
			player->bullets[i]->xAcc = 0.0f;
		}*/

		//Collision
		/*for (int j = 0; j < enemies.size(); j++) {
			if (bullets[i].collidesWith(&enemies[j])) {
				//If bullet hits an enemy that's alive
				if (enemies[j].display) {
					bullets[i].display = false;
					bullets[i].speed = 0.0f;
					enemyCount--;

					Mix_PlayChannel(-1, akarin, 0);
				}

				enemies[j].display = false;
			}
		}*/
	}

	//block->Update(elapsed);

	/*for (int i = 0; i < blocks.size(); i++) {
	blocks[i]->Update(elapsed);
	}*/

	scrollScreen();
	handleCollisions();
}

void Platformer::handleCollisions() {

	/*if (player->collidesWith(block))
		player->handleCollision(block);*/

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

	for (int i = 0; i < blocks2.size(); i++) {
		if (player->collidesWith(blocks2[i])) {
			player->handleCollision(blocks2[i]);
		}
	}

	for (int i = 0; i < blocks2.size(); i++) {
		player->checkCollision(blocks2[i]);

		if (player->collidedBottom)
			break;
	}

	for (int i = 0; i < staticEntities.size(); i++) {
		if (player->collidesWith(staticEntities[i])) {
			player->handleCollision(staticEntities[i]);
		}
	}

	for (int i = 0; i < staticEntities.size(); i++) {
		player->checkCollision(staticEntities[i]);

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
			//Jump
			if (event.key.keysym.scancode == SDL_SCANCODE_D) {
				if (player->collidedBottom)
					player->yVel = 0.1f;	
			}

			///Fire bullets
			if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
				if (player->directionFacing == 1 || player->directionFacing == -1) {
					//If the bullet hasn't been fired yet, display it
					if (!player->bullets[player->bulletCount]->display) {
						player->bullets[player->bulletCount]->display = true;
						player->bullets[player->bulletCount]->xAcc = elapsed * 15.0f * player->directionFacing;

						//Bullet comes out of the front of the player
						if (player->directionFacing == 1)
							player->bullets[player->bulletCount]->xPos = player->xPos + player->width / 3;

						else if (player->directionFacing == -1)
							player->bullets[player->bulletCount]->xPos = player->xPos - player->width / 3;

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
		player->xAcc = elapsed * 2.0f;
		player->directionFacing = 1;
	}

	//Player move left
	else if (keys[SDL_SCANCODE_LEFT]) {
		player->xAcc = elapsed * -2.0f;
		player->directionFacing = -1;
	}
	
	//For player and block collision testing
	else if (keys[SDL_SCANCODE_UP]) {
		player->yAcc = elapsed * 2.0f;
		player->directionFacing = 2;
	}

	else if (keys[SDL_SCANCODE_DOWN]) {
		player->yAcc = elapsed * -2.0f;
		player->directionFacing = -2;
	}

	else if (keys == SDL_GetKeyboardState(NULL)) {
		player->xVel = lerp(player->xVel, 0.0f, FIXED_TIMESTEP * player->xFric);
		player->yVel = lerp(player->yVel, 0.0f, FIXED_TIMESTEP * player->yFric);
	}

	Update(elapsed);
	Render();

	return done;
}