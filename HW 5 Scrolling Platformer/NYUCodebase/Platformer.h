#include "Entity.h"

// 60 FPS (1.0f/60.0f)
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

class Platformer {
public:
	Platformer();
	~Platformer();

	void scrollScreen();
	void handleCollisions();

	void Setup();
	void Render();
	void Update(float elapsed);
	bool Run();

private:
	SDL_Window* displayWindow;

	ShaderProgram* program;

	//Matrices
	Matrix projectionMatrix;
	Matrix viewMatrix;
	Matrix modelMatrixText;

	GLuint font;

	//Entities
	Entity* player;
	Entity* block;
	Entity* block2;
	vector<Entity*> blocks;

	bool done = false;
	float lastFrameTicks = 0.0f;

	//Gravity
	float xGrav = 0.0f;
	float yGrav = -0.5f;
};