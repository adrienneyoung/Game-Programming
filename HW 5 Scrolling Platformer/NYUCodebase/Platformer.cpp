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

	glBindTexture(GL_TEXTURE_2D, sheet);
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
	delete program;
	SDL_Quit();
}

void Platformer::scrollScreen()
{
	viewMatrix.identity();
	viewMatrix.Translate(-(player->xPos), -(player->yPos), 0.0f);
	program->setViewMatrix(viewMatrix);
}

void Platformer::Setup() {
	player = new Entity(0.0f, 1.5f, 0.5f, 0.5f, "akarispritesheet.png");
	player->sprite = SheetSprite(player->tex, 3, 4, player->width, player->height, 0.5f);

	//block = new Entity(-1.0f, 0.25f, 0.5f, 0.5f, "sheet.png");
	//block->sprite = SheetSprite(block->tex, 14, 7, block->width, block->height, 0.5f);

	for (int i = 0; i < 5; i++) {
		Entity* block3 = new Entity(-2.5f + (i*0.5f), -2.5f + (i*0.25f), 0.5f, 0.5f, "sheet.png");
		block3->sprite = SheetSprite(block3->tex, 14, 7, block3->width, block3->height, 0.5f);
		blocks.push_back(block3);
	}
	
	
	srand(time(NULL));
	//Randomly generate blocks
	for (int i = 0; i < 10; i++) {
		Entity* block3 = new Entity(-2.5f + (i*0.5f), -2.5f + (rand() % 2) / 0.5f, 0.5f, 0.5f, "sheet.png");
		block3->sprite = SheetSprite(block3->tex, 14, 7, block3->width, block3->height, 0.5f);
		staticEntities.push_back(block3);
	}

	font = LoadTexture("font1.png");
	sheet = LoadTexture("sheet.png");
}

void Platformer::Render() {
	glClearColor(0.9f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program->programID); //Installs a program object as part of current rendering state

	program->setProjectionMatrix(projectionMatrix);
	program->setViewMatrix(viewMatrix);
	//program->setViewMatrix(viewMatrix);
	player->Render(program, player->matrix, 1);

	//block->Render(program, block->matrix, 10);

	for (int i = 0; i < blocks.size(); i++) {
		blocks[i]->Render(program, blocks[i]->matrix, 1);
		blocks[i]->isStatic = true;
	}

	for (int i = 0; i < staticEntities.size(); i++) {
		staticEntities[i]->Render(program, staticEntities[i]->matrix, 24);
		staticEntities[i]->isStatic = true;
	}

	RenderLevel(); //Doesn't work

	/*program->setModelMatrix(modelMatrixText);
	string s = to_string();
	DrawText(program, font, s, 0.2f, 0.2f);*/
	

	SDL_GL_SwapWindow(displayWindow);
}

void Platformer::Update(float elapsed) {
	player->Update(elapsed);
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

	for (int i = 0; i < blocks.size(); i++)
		if (player->collidesWith(blocks[i]))
			player->handleCollision(blocks[i]);

	for (int i = 0; i < staticEntities.size(); i++)
		if (player->collidesWith(staticEntities[i]))
			player->handleCollision(staticEntities[i]);
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
		}
	}

	//Keyboard polling
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	//Player move right
	if (keys[SDL_SCANCODE_RIGHT]) {
		player->xAcc = elapsed * 5;
	}

	//Player move left
	else if (keys[SDL_SCANCODE_LEFT]) {
		player->xAcc = elapsed * -5;
	}

	else if (keys[SDL_SCANCODE_UP]) {
		player->yAcc = elapsed * 5;
	}

	else if (keys[SDL_SCANCODE_DOWN]) {
		player->yAcc = elapsed * -5;
	}

	else if (keys[SDL_SCANCODE_SPACE]) {
		player->jump();
	}

	else if (keys == SDL_GetKeyboardState(NULL)) {
		player->xVel = lerp(player->xVel, 0.0f, FIXED_TIMESTEP * player->xFric);
		player->yVel = lerp(player->yVel, 0.0f, FIXED_TIMESTEP * player->yFric);
	}

	Update(elapsed);
	Render();

	return done;
}