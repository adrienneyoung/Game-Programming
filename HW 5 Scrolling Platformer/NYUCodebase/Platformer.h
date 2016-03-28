#include "Entity.h"

class Platformer {
public:
	Platformer();
	~Platformer();

	void Setup();
	void Render();
	void Update(float elapsed);
	bool Run();

private:
	SDL_Window* displayWindow;

	ShaderProgram* program;

	Matrix projectionMatrix;
	Matrix viewMatrix;
	Matrix modelMatrixText;

	//Entities
	Entity* player;

	bool done = false;
	float lastFrameTicks = 0.0f;
};