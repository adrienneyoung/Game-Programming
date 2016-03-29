#include "Platformer.h"

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

	block = new Entity(-1.0f, 0.25f, 0.5f, 0.5f, "sheet.png");
	block->sprite = SheetSprite(block->tex, 14, 7, block->width, block->height, 0.5f);

	for (int i = 0; i < 10; i++) {
		Entity* block3 = new Entity(-2.5f + (i*0.5f), -2.5f + (i*0.25f), 0.5f, 0.5f, "sheet.png");
		//block3->isStatic = true;
		block3->sprite = SheetSprite(block3->tex, 14, 7, block3->width, block3->height, 0.5f);
		blocks.push_back(block3);
	}

	font = LoadTexture("font1.png");
}

void Platformer::Render() {
	glClearColor(0.9f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program->programID); //Installs a program object as part of current rendering state

	program->setProjectionMatrix(projectionMatrix);
	program->setViewMatrix(viewMatrix);

	player->Render(program, player->matrix, 1);

	block->Render(program, block->matrix, 10);

	for (int i = 0; i < blocks.size(); i++) {
		blocks[i]->Render(program, blocks[i]->matrix, 1);
		blocks[i]->isStatic = true;
	}

	//program->setModelMatrix(modelMatrixText);
	//string s = to_string(player->height);
	//DrawText(program, font, s, 0.2f, 0.2f);

	SDL_GL_SwapWindow(displayWindow);
}

void Platformer::Update(float elapsed) {	
	player->Update(elapsed);
	block->Update(elapsed);

	/*for (int i = 0; i < blocks.size(); i++) {
		blocks[i]->Update(elapsed);
	}*/

	scrollScreen();
	handleCollisions();

	
}

void Platformer::handleCollisions() {
	//for (int i = 0; i < blocks.size(); i++)
	//	player->collidesWith(blocks[i]);

	player->collidesWith(block);
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

	else if (keys == SDL_GetKeyboardState(NULL)) {
		player->xVel = lerp(player->xVel, 0.0f, FIXED_TIMESTEP * player->xFric);
		player->yVel = lerp(player->yVel, 0.0f, FIXED_TIMESTEP * player->yFric);
	}

	Update(elapsed);
	Render();

	

	return done;
}
