#include "Platformer.h"

Platformer::Platformer() {
	Setup();

	player = new Entity(0.0f, 1.5f, 0.5f, 0.5f, 2.5f, "akarispritesheet.png");
}


Platformer::~Platformer()
{
	delete program;
	SDL_Quit();
}


void Platformer::Setup() {
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
}

void Platformer::Render() {
	glClearColor(0.9f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program->programID); //Installs a program object as part of current rendering state

	program->setProjectionMatrix(projectionMatrix);
	program->setViewMatrix(viewMatrix);

	player->sprite = SheetSprite(player->tex, 3, 4, player->width, player->height, 0.5f);
	player->Render(program, player->matrix, 1);

	SDL_GL_SwapWindow(displayWindow);
}

void Platformer::Update(float elapsed) {	

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
			//Reset back to 0 so xMovement won't be a huge number since it is constantly being added by elapsed time
			player->xMovement = 0;
		}
	}

	//Keyboard polling
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	//Player move right
	if (keys[SDL_SCANCODE_RIGHT]) {
		//Prevent player from going out of view
		if (player->xPos + player->width / 2 < 3.3f) {
			player->xDir = 1.0f;
			player->xMovement = elapsed * player->speed * player->xDir;
			player->xPos += player->xMovement;
			player->matrix.Translate(player->xMovement, 0.0f, 0.0f);
		}
	}

	//Player move left
	if (keys[SDL_SCANCODE_LEFT]) {
		//Prevent player from going out of view
		if (player->xPos - player->width / 2 / 2 > -3.3f) {
			player->xDir = -1.0f;
			player->xMovement = elapsed * player->speed * player->xDir;
			player->xPos += player->xMovement;
			player->matrix.Translate(player->xMovement, 0.0f, 0.0f);
		}
	}
	

	Update(elapsed);
	Render();

	return done;
}
