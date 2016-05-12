#include "Platformer.h"
enum GameState { MAIN_MENU, GAME_LEVEL, GAME_WIN, GAME_LOSE };

unsigned char level1Data[LEVEL_HEIGHT][LEVEL_WIDTH] = {

	{ 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, },
	{ 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, },
	{ 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, },
	{ 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 123, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, },
	{ 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 43, 140, 140, },
	{ 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 123, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, },
	{ 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 74, 50, 39, 74, 140, 140, 140, 140, },
	{ 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 39, 74, 50, 39, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, },
	{ 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, },
	{ 140, 140, 140, 140, 140, 140, 140, 123, 140, 140, 140, 140, 140, 140, 140, 43, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 123, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 74, 39, 39, 74, 50, 39, 39, 74, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, },
	{ 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, },
	{ 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, },
	{ 79, 79, 79, 79, 79, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 79, 79, 79, 79, },
	{ 88, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 83, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 83, 121, 121, 121, 121, 103, 114, 114, 114, 114, 114, 114, 111, 114, 114, 114, 114, 114, 114, 114, 114, 114, 125, 114, 114, 106, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 95, },
};


void Platformer::initializeEmitter(){
	maxLifetime = 2;
	position.x = 5.0f;
	position.y = -7.5f;
	velocity.x = 4.0f;
	velocity.y = 3.0f;
	velocityDeviation.x = 1.0f;
	velocityDeviation.y = 1.0f;
	startSize = 0.1f;
	endSize = 1.0f;
	sizeDeviation = .5;
	isRunning = false;
	particleTexture = LoadTexture("homura.png");
	direction = 1;
	isLaser = false;
	for (int i = 0; i < 100; i++){
		//Random since we dont want particles to reset together
		Particle * p = new Particle();
		randomize(*p, position.x, position.y, isLaser);
		particles.push_back(p);
	}

}
void Platformer::turnOn(){
	isRunning = true;
}
void Platformer::turnOff(){
	isRunning = false;
}
bool Platformer::getRunningStatus(){
	return isRunning;
}
void Platformer::changeDirection(int dir){
	//Use with player->directionFacing
	direction = dir;
}
void Platformer::randomize(Particle& p, float x, float y, bool laser){
	p.perlinY = rand() / (RAND_MAX);
	p.lifetime = rand() / (RAND_MAX / maxLifetime);
	p.position.x = x;
	p.position.y = y;
	p.sizeDeviation = rand() / (RAND_MAX / sizeDeviation);
	if (isLaser){
		p.velocity.y = 0;
		p.velocity.x = (direction)*(velocity.x + rand() / (RAND_MAX / velocityDeviation.x * 5));

	}
	else{
		p.velocity.y = velocity.y + rand() / (RAND_MAX / velocityDeviation.y) + .5;
		p.velocity.x = (direction)*(velocity.x + rand() / (RAND_MAX / velocityDeviation.x));

	}
}

void Platformer::laser(){
	velocity.x = 8.0f;
	velocity.y = 0.0f;
	velocityDeviation.x = 1.0f;
	velocityDeviation.y = 1.0f;
	particleTexture = LoadTexture("laser.png");
	isLaser = true;
}

void Platformer::bunFountain(){
	velocity.x = 4.0f;
	velocity.y = 3.0f;
	velocityDeviation.x = 1.0f;
	velocityDeviation.y = 1.0f;
	particleTexture = LoadTexture("bun.png");
	isLaser = false;
}
void Platformer::setPosition(float x, float y){
	position.x = x;
	position.y = y;
}

void Platformer::UpdateEmitter(float elapsed, float x, float y){
	for (int i = 0; i < particles.size(); i++){
		particles[i]->lifetime += elapsed;
		if (particles[i]->lifetime > maxLifetime){
			if (!isRunning){
				//if not running, then make this dead
				particles[i]->isDead = true;
			}
			else{
				particles[i]->isDead = false;
				randomize(*particles[i], x, y, isLaser);
			}
			//not sure if particles[i]->position = position; does the same job
		}
		else{
			particles[i]->position.x += elapsed * particles[i]->velocity.x;
			particles[i]->position.y += elapsed * particles[i]->velocity.y;
			if (!isLaser){
				particles[i]->velocity.y += -9.8 * elapsed;
			}
		}
	}
}
void Platformer::RenderTexture(ShaderProgram * program){
	float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
	for (int i = 0; i < particles.size(); i++){
		if (!particles[i]->isDead){
			GLfloat vertices[] =
			{
				particles[i]->position.x - particles[i]->width / 2, particles[i]->position.y - particles[i]->height / 2,
				particles[i]->position.x + particles[i]->width / 2, particles[i]->position.y - particles[i]->height / 2,
				particles[i]->position.x + particles[i]->width / 2, particles[i]->position.y + particles[i]->height / 2,
				particles[i]->position.x - particles[i]->width / 2, particles[i]->position.y - particles[i]->height / 2,
				particles[i]->position.x + particles[i]->width / 2, particles[i]->position.y + particles[i]->height / 2,
				particles[i]->position.x - particles[i]->width / 2, particles[i]->position.y + particles[i]->height / 2 //0,0
			};

			program->setModelMatrix(particles[i]->matrix);

			if (isLaser){
				glBlendFunc(GL_SRC_ALPHA, GL_ONE); //white
			}
			else{
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //not white(?)
			}
			glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
			glEnableVertexAttribArray(program->positionAttribute);
			glBindTexture(GL_TEXTURE_2D, particleTexture);

			glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
			glEnableVertexAttribArray(program->texCoordAttribute);

			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //white

		}
		else{
			//do not render cuz it ded
		}
	}
}

void Platformer::BuildLevel() {
	memcpy(levelData, level1Data, LEVEL_HEIGHT*LEVEL_WIDTH);
}


void Platformer::RenderLevel(){
	vector<float> vertexData;
	vector<float> texCoordData;

	float spriteWidth;
	float spriteHeight;

	for (int y = 0; y < LEVEL_HEIGHT; y++) {
		for (int x = 0; x < LEVEL_WIDTH; x++) {

			float u = (float)(((int)levelData[y][x]) % SPRITE_COUNT_X) / (float)SPRITE_COUNT_X;
			float v = (float)(((int)levelData[y][x]) / SPRITE_COUNT_X) / (float)SPRITE_COUNT_Y;

			if (levelData[y][x] != 140) {
				Matrix matrixtest;
				program->setModelMatrix(matrixtest);

				spriteWidth = 1.0f / (float)SPRITE_COUNT_X;
				spriteHeight = 1.0f / (float)SPRITE_COUNT_Y;
			}

			else {
				spriteWidth = 0.0f;
				spriteHeight = 0.0f;
			}

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
		}
	}

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
	glEnableVertexAttribArray(program->texCoordAttribute);

	glBindTexture(GL_TEXTURE_2D, spritesheet);
	glDrawArrays(GL_TRIANGLES, 0, 6 * LEVEL_HEIGHT * LEVEL_WIDTH);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);

}


void Platformer::worldToTileCoordinates(float worldX, float worldY, int *gridX, int *gridY) {
	*gridX = (int)(worldX / TILE_SIZE);
	*gridY = (int)(-worldY / TILE_SIZE);
}

bool Platformer::isSolid(int x, int y, unsigned char levelData[LEVEL_HEIGHT][LEVEL_WIDTH]) {
	if (levelData[y][x] != 140)
		return true;
	return false;
}

Platformer::Platformer() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Extreme Housekeeping: The Sequel", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 900, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 900, 600);
	program = new ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");

	projectionMatrix.setOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);

	Setup();
}

Platformer::~Platformer() {
	Mix_FreeChunk(woof);
	Mix_FreeChunk(jump);
	Mix_FreeChunk(laserSound);
	Mix_FreeChunk(akarin);
	Mix_FreeMusic(music);

	delete program;
	SDL_Quit();
}

void Platformer::Setup() {
	BuildLevel();
	initializeEmitter();

	akari = new Entity(2.0, -0.5, 1.0f, 1.0f, "mybae.png", "mybae2.png");
	akari->runAnimationFront = { 0, 1, 2, 3, 4, 5, 6, 7 };
	akari->numFrames = 8;

	//Player
	player = new Entity(5.0f, -5.5f, 1.0f, 1.0f, "corgiLR.png", "corgiUD.png");
	player->runAnimationLeft = { 0, 1, 2 };
	player->runAnimationRight = { 3, 4, 5 };
	player->runAnimationBack = { 0, 1, 2 };
	player->runAnimationFront = { 3, 4, 5 };

	//Player's bullets
	for (int i = 0; i < player->maxBullets; i++) {
		Entity* bullet = new Entity(player->xPos, player->yPos, 0.3f, 0.15f, "stick.png");
		bullet->sprite = SheetSprite(bullet->tex, 1, 1, bullet->width, bullet->height, 0.25f);
		bullet->isBullet = true;
		player->bullets.push_back(bullet);
	}

	//Enemies
	for (int y = 0; y < LEVEL_HEIGHT; y++) {
		for (int x = 0; x < LEVEL_WIDTH; x++) {
			if (levelData[y][x] == 83 || levelData[y][x] == 111 || levelData[y][x] == 50) {
				Entity* enemy = new Entity((float)x * TILE_SIZE + TILE_SIZE / 2.0f, (float)y * -TILE_SIZE + TILE_SIZE / 2.0f + 0.3f, 1.0f, 1.0f, "mum.png", "mum2.png");
				enemy->isEnemy = true;
				enemy->isDead = false;
				enemy->health = 100.0f;
				enemies.push_back(enemy);
			}

			else if (levelData[y][x] == 43) {
				Entity* staticEntity = new Entity((float)x * TILE_SIZE + TILE_SIZE / 2.0f, (float)y * -TILE_SIZE + TILE_SIZE / 2.0f + 0.3f, 1.0f, 1.0f, "bae.png");
				staticEntity->sprite = SheetSprite(staticEntity->tex, 1, 1, staticEntity->width, staticEntity->height, 1.3f);
				staticEntities.push_back(staticEntity);
			}

			else if (levelData[y][x] == 123) {
				Entity* staticEntity = new Entity((float)x * TILE_SIZE + TILE_SIZE / 2.0f + 0.25f, (float)y * -TILE_SIZE + TILE_SIZE / 2.0f + 0.7f, 1.5f, 0.4f, "kyubey.png");
				staticEntity->sprite = SheetSprite(staticEntity->tex, 5, 5, staticEntity->width, staticEntity->height + 1.1f, 3.0f);
				staticEntity->runAnimationFront = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
					18, 19, 20, 21, 22, 23, 24 };
				staticEntity->numFrames = 25;
				staticEntities.push_back(staticEntity);
			}
		}
	}

	//Textures
	font = LoadTexture("font2.png");
	spritesheet = LoadTexture("sheet4.png");
	levelOne = LoadTexture("park2.png");
	levelTwo = LoadTexture("bathroom.png");
	levelThree = LoadTexture("andre2.png");
	mainBackground = LoadTexture("city.png");

	//Stuff for scrolling background
	svertices = { -48.0f, -2.0f,
		0.0f, -2.0f,
		0.0f, 2.0f,
		-48.0f, -2.0f,
		0.0f, 2.0f,
		-48.0f, 2.0f };

	stexCoords = { 0.0f + textureOffsetX, 1.0f,
		3.0f + textureOffsetX, 1.0f,
		3.0f + textureOffsetX, 0.0f,
		0.0f + textureOffsetX, 1.0f,
		3.0f + textureOffsetX, 0.0f,
		0.0f + textureOffsetX, 0.0f };

	//Stuff for game level background
	vertices = { 0.0f, -2.47f,
		2.0f, -2.47f,
		2.0f, 0.52f,
		0.0f, -2.47f,
		2.0f, 0.52f,
		0.0f, 0.52f };

	vertices2 = { 4.25f, -2.27f,
		6.25f, -2.27f,
		6.25f, 0.72f,
		4.25f, -2.27f,
		6.25f, 0.72f,
		4.25f, 0.72f };

	vertices3 = { 6.25f, -2.67f,
		8.25f, -2.67f,
		8.25f, 0.32f,
		6.25f, -2.67f,
		8.25f, 0.32f,
		6.25f, 0.32f };

	texCoords = { 0.0, 1.0,
		1.0, 1.0,
		1.0, 0.0,
		0.0, 1.0,
		1.0, 0.0,
		0.0, 0.0 };

	//Main music
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	music = Mix_LoadMUS("fate.mp3");
	woof = Mix_LoadWAV("woof.wav");
	jump = Mix_LoadWAV("jump.wav");
	laserSound = Mix_LoadWAV("laser.wav");
	akarin = Mix_LoadWAV("akarin.wav");
	Mix_PlayMusic(music, -1);
}

void Platformer::Render() {
	glClearColor(0.2f, 0.17f, 0.33f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//	p->Render(program);
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

		//Title
		DrawText(program, mainMenuText, font, "EXTREME HOUSEKEEPING", 0.2f, 0.1f, -3.0f, 0.6f);
		DrawText(program, mainMenuText, font, "The Sequel", 0.15f, 0.1f, -3.0f, 1.3f);
		DrawText(program, mainMenuText2, font, "Press space to start", 0.15f, 0.07f, -2.0f, -1.7f);

		//Corgi
		player->sprite = SheetSprite(player->tex, 3, 2, player->width, player->height, 1.0f);
		player->Render(program, player->matrix, player->runAnimationLeft[player->currentIndex]);
	}

	else if (state == GAME_LEVEL) {
		//Render backgrounds
		program->setModelMatrix(levelOneMatrix);
		glBindTexture(GL_TEXTURE_2D, levelOne);
		glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
		glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
		glEnableVertexAttribArray(program->positionAttribute);
		glEnableVertexAttribArray(program->texCoordAttribute);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		levelOneMatrix.setScale(14.0f, 3.5f, 1.0f);

		program->setModelMatrix(levelTwoMatrix);
		glBindTexture(GL_TEXTURE_2D, levelTwo);
		glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices2.data());
		glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
		glEnableVertexAttribArray(program->positionAttribute);
		glEnableVertexAttribArray(program->texCoordAttribute);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		levelTwoMatrix.setScale(6.0f, 4.0f, 1.0f);

		program->setModelMatrix(levelThreeMatrix);
		glBindTexture(GL_TEXTURE_2D, levelThree);
		glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices3.data());
		glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
		glEnableVertexAttribArray(program->positionAttribute);
		glEnableVertexAttribArray(program->texCoordAttribute);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		levelThreeMatrix.setScale(6.0f, 3.0f, 1.0f);

		RenderTexture(program);

		//Render enemies
		for (int i = 0; i < enemies.size(); i++) {
			if (!enemies[i]->isDead) {
				enemies[i]->sprite = SheetSprite(enemies[i]->tex, 1, 1, enemies[i]->width, enemies[i]->height, 1.5f);
				enemies[i]->Render(program, enemies[i]->matrix, 0);
			}

			else {
				enemies[i]->sprite = SheetSprite(enemies[i]->tex2, 1, 1, enemies[i]->width, enemies[i]->height, 1.5f);
				enemies[i]->Render(program, enemies[i]->matrix, 0);
			}
		}

		//Render static entities
		for (int i = 0; i < staticEntities.size(); i++) {
			if (staticEntities[i]->runAnimationFront.size() != 0)
				staticEntities[i]->Render(program, staticEntities[i]->matrix, staticEntities[i]->runAnimationFront[staticEntities[i]->currentIndex]);
			else
				staticEntities[i]->Render(program, staticEntities[i]->matrix, 0);
		}

		RenderLevel();

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

		else if (player->directionFacing == 2) {
			player->sprite = SheetSprite(player->tex2, 3, 2, player->width, player->height, 1.0f);
			player->Render(program, player->matrix, player->runAnimationFront[player->currentIndex]);
		}

		else if (player->directionFacing == -2) {
			player->sprite = SheetSprite(player->tex2, 3, 2, player->width, player->height, 1.0f);
			player->Render(program, player->matrix, player->runAnimationBack[player->currentIndex]);
		}

		//Render player's bullets
		for (int i = 0; i < player->maxBullets; i++) {
			if (player->bullets[i]->display) {
				player->bullets[i]->Render(program, player->bullets[i]->matrix, 0);
			}
		}

		//Debug
		string s = to_string(killCount);
		DrawText(program, gameText, font, s + " / 6 Kyubeys killed", 0.2f, 0.05f, 0.0f, -0.5f);
		gameText.setPosition(player->xPos - 2.5f, -7.95f, 1.0f);
	}

	else if (state == GAME_WIN) {
		DrawText(program, gameText, font, "You win!", 0.2f, 0.2f, 5.0f, -0.5f);
		akari->sprite = SheetSprite(akari->tex, 4, 2, akari->width, akari->height, 1.0f);
		akari->matrix.setScale(4.0f, 4.0f, 1.0f);
		akari->matrix.setPosition(-2.0f, 0.0f, 1.0f);
		akari->Render(program, akari->matrix, akari->runAnimationFront[akari->currentIndex]);
	}

	else if (state == GAME_LOSE) {
		DrawText(program, gameText, font, "You died", 0.2f, 0.2f, 5.0f, -0.5f);
		akari->sprite = SheetSprite(akari->tex2, 1, 1, akari->width, akari->height, 1.0f);
		akari->matrix.setScale(4.0f, 4.0f, 1.0f);
		akari->matrix.setPosition(-2.0f, 0.0f, 1.0f);
		akari->Render(program, akari->matrix, 0);
	}

	SDL_GL_SwapWindow(displayWindow);
}

void Platformer::scrollScreen()
{
	viewMatrix.identity();

	if (player->yPos < -6.09999f)
		viewMatrix.setPosition(-(player->xPos), 6.1f, 0.0f);
	else
		viewMatrix.Translate(-(player->xPos), -(player->yPos), 0.0f);

	program->setViewMatrix(viewMatrix);
}

void Platformer::Update(float fixedElapsed) {
	if (state == MAIN_MENU) {
		if (textureOffsetX > 43.0f)
			textureOffsetX = 3.55;

		textureOffsetX += fixedElapsed;
		backgroundMatrix.setPosition(textureOffsetX, 0.0f, 0.0f);

		player->matrix.identity();
		player->Animate(fixedElapsed);
		player->matrix.setPosition(-0.15f, -1.0f, 1.0f);
	}
	else if (state == GAME_LEVEL) {
		//p->position.x += fixedElapsed;
		mainMenuText.identity();
		mainMenuText2.identity();

		player->Update(fixedElapsed);

		if (player->directionFacing == -1){
			if (!isLaser && getRunningStatus()){
				changeDirection(player->directionFacing);
				player->matrix.setRotation(150);
				UpdateEmitter(fixedElapsed, player->xPos - player->width / 3, player->yPos - player->height / 3);
			}
			else{
				changeDirection(player->directionFacing);
				UpdateEmitter(fixedElapsed, player->xPos - player->width / 3, player->yPos + player->height / 5.5);
			}
		}
		if (player->directionFacing == 0){
			if (isLaser && getRunningStatus()){
				player->matrix.setRotation(0);
			}
			UpdateEmitter(fixedElapsed, player->xPos, player->yPos);
		}
		if (player->directionFacing == 1){
			if (!isLaser && getRunningStatus()){
				changeDirection(player->directionFacing);
				player->matrix.setRotation(120);
				UpdateEmitter(fixedElapsed, player->xPos + player->width / 3, player->yPos - player->height / 3);
			}
			else{
				changeDirection(player->directionFacing);
				UpdateEmitter(fixedElapsed, player->xPos + player->width / 3, player->yPos + player->height / 5.5);
			}
		}

		//fountain->Update(fixedElapsed);
		for (int i = 0; i < staticEntities.size(); i++) {
			if (staticEntities[i]->runAnimationFront.size() != 0)
				staticEntities[i]->Animate(fixedElapsed);
		}

		killCount = 0;

		for (int i = 0; i < enemies.size(); i++) {
			if (enemies[i]->isDead) {
				killCount++;
			}

			if (enemies[i]->health <= 0.0f) {
				enemies[i]->isDead = true;
			}

			for (Particle * p : particles){
				if (p->collidesWith(enemies[i]) && getRunningStatus() && !enemies[i]->isDead){
					enemies[i]->health -= .05f;
				}
			}

			if (player->isHumping && player->collidesWith(enemies[i]) && !enemies[i]->isDead) {
				enemies[i]->health--;
			}
		}
		//Collisions between player and blocks
		int gridX;
		int gridY;

		//Penetration
		float xPen = 0.0f;
		float yPen = 0.0f;

		//Top collision
		worldToTileCoordinates(player->xPos, player->yPos + player->height / 2.2f, &gridX, &gridY);
		if (isSolid(gridX, gridY, levelData)) {
			yPen = (player->yPos + player->height / 2.2f) - ((-TILE_SIZE * gridY) - TILE_SIZE);
			player->yPos -= yPen + 0.0001f;
			player->yVel = 0.0f;
			player->collidedTop = true;
		}

		//Bottom collision
		worldToTileCoordinates(player->xPos, player->yPos - player->height / 2.1f, &gridX, &gridY);
		if (isSolid(gridX, gridY, levelData)) {
			yPen = (-TILE_SIZE * gridY) - (player->yPos - player->height / 2.1f);
			player->yPos += yPen + 0.000000001f;
			player->yVel = 0.0f;
			player->collidedBottom = true;
		}

		//Left collision
		worldToTileCoordinates(player->xPos - player->width / 2.0f, player->yPos, &gridX, &gridY);
		if (isSolid(gridX, gridY, levelData)) {
			xPen = ((TILE_SIZE * gridX) + TILE_SIZE) - (player->xPos - player->width / 2.0f);
			player->xPos += xPen + 0.0001f;
			player->xVel = 0.0f;
			player->collidedLeft = true;
		}

		//Right collision
		worldToTileCoordinates(player->xPos + player->width / 2.0f, player->yPos, &gridX, &gridY);
		if (isSolid(gridX, gridY, levelData)) {
			xPen = (player->xPos + player->width / 2.0f) - (TILE_SIZE * gridX);
			player->xPos -= xPen + 0.0001f;
			player->xVel = 0.0f;
			player->collidedRight = true;
		}

		//Player's bullets
		for (int i = 0; i < player->maxBullets; i++)
		{
			player->bullets[i]->Update(fixedElapsed);

			//Bullet collision
			for (int j = 0; j < enemies.size(); j++) {
				if (player->bullets[i]->bulletCollidesWith(enemies[j])) { //If bullet hits an enemy that's alive
					if (!enemies[j]->isDead) {
						player->bullets[i]->handleCollision(enemies[j]);
						player->bullets[i]->display = false;
						player->bullets[i]->xVel = 0.0f;
						Mix_VolumeMusic(200.0);
						Mix_PlayChannel(-1, woof, 1);
						enemies[j]->health -= 30.0f;
					}
				}
			}
			//Don't display bullets when out of bounds
			if (player->bullets[i]->xPos > player->xPos + 10.0f || player->bullets[i]->xPos < player->xPos - 10.0f) {
				player->bullets[i]->display = false;
				player->bullets[i]->xVel = 0.0f;
			}
		}

		if (player->yPos < -10.0f)
			state = GAME_LOSE;

		if (killCount == enemies.size())
			state = GAME_WIN;

		if (killCount == 1)
			state = GAME_LOSE;

		//Enemy collisions
		for (int i = 0; i < enemies.size(); i++) {
			if (player->collidesWith(enemies[i]) && !enemies[i]->isDead) {
				player->handleCollision(enemies[i]);
			}
		}

		//Static entities collisions
		for (int i = 0; i < staticEntities.size(); i++) {
			if (player->collidesWith(staticEntities[i])) {
				player->handleCollision(staticEntities[i]);
			}
		}

		scrollScreen();
	}

	else if (state == GAME_LOSE) {
		backgroundMatrix.identity();
		gameText.identity();
		viewMatrix.identity();
		akari->matrix.identity();
	}

	else if (state == GAME_WIN) {
		backgroundMatrix.identity();
		gameText.identity();
		viewMatrix.identity();
		akari->matrix.identity();

		akari->Animate(fixedElapsed);
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
			if (event.key.keysym.scancode == SDL_SCANCODE_SPACE && state == MAIN_MENU) {
				state = GAME_LEVEL;
			}

			//Jump
			if (event.key.keysym.scancode == SDL_SCANCODE_UP  && state == GAME_LEVEL) {
				if (player->collidedBottom) {
					player->yVel = 4.0f;
					Mix_PlayChannel(-1, jump, 1);
				}
			}
			
			//Fires load
			if (event.key.keysym.scancode == SDL_SCANCODE_2 && state == GAME_LEVEL) {
				if (getRunningStatus()){
					if (isLaser){
						bunFountain();
					}
					else{
						player->matrix.setRotation(0);
						turnOff();
					}
				}
				else{
					bunFountain();
					turnOn();
					Mix_PlayChannel(-1, akarin, 1);
				}
			}

			//lazors
			if (event.key.keysym.scancode == SDL_SCANCODE_3 && state == GAME_LEVEL) {
				if (getRunningStatus()){
					//If spraying, then turn off
					if (isLaser){
						turnOff();
					}
					else{
						laser();
					}
				}
				else{
					laser();
					turnOn();
					Mix_PlayChannel(-1, laserSound, 1);
				}
			}

			//Humps
			if (event.key.keysym.scancode == SDL_SCANCODE_4 && state == GAME_LEVEL) {
				if (player->getHumpingStatus()){
					//If humping, then turn off
					player->humpStop();
				}
				else{
					player->humpStart();
				}
			}


			///Fire bullets
			if (event.key.keysym.scancode == SDL_SCANCODE_1 && state == GAME_LEVEL) {
				if (player->directionFacing == 1 || player->directionFacing == -1) {
					//If the bullet hasn't been fired yet, display it
					if (!player->bullets[player->bulletCount]->display) {
						player->bullets[player->bulletCount]->display = true;

						//Bullet comes out of the front of the player
						if (player->directionFacing == 1) {
							player->bullets[player->bulletCount]->xPos = player->xPos + player->width / 2 + player->bullets[player->bulletCount]->width / 2;
							player->bullets[player->bulletCount]->xVel = 3.0f;
							player->bullets[player->bulletCount]->xPos += player->bullets[player->bulletCount]->xVel * FIXED_TIMESTEP;
						}

						else if (player->directionFacing == -1) {
							player->bullets[player->bulletCount]->xPos = player->xPos - player->width / 2 - player->bullets[player->bulletCount]->width / 2;
							player->bullets[player->bulletCount]->xVel = -3.0f;
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
		player->xAcc = fixedElapsed * 35.0f;
		player->directionFacing = 1;
	}

	//Player move left
	else if (keys[SDL_SCANCODE_LEFT]) {
		player->xAcc = fixedElapsed * -35.0f;
		player->directionFacing = -1;
	}

	else if (keys == SDL_GetKeyboardState(NULL)) {
		player->xVel = lerp(player->xVel, 0.0f, FIXED_TIMESTEP * player->xFric);
		player->yVel = lerp(player->yVel, 0.0f, FIXED_TIMESTEP * player->yFric);
	}

	Update(fixedElapsed);
	Render();

	return done;
}