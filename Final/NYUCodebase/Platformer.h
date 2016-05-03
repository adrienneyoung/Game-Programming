#include "Entity.h"
#include <vector>
#include <SDL_mixer.h>

// 60 FPS (1.0f/60.0f)
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
#define TILE_SIZE .70f
#define SPRITE_COUNT_X 14
#define SPRITE_COUNT_Y 7

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

	void buildLevel();
	bool readHeader(std::ifstream &stream);
	bool readLayerData(std::ifstream &stream);
	//bool readEntityData(std::ifstream &stream);
	void RenderLevel();


private:
	SDL_Window* displayWindow;
	ShaderProgram* program;
	Mix_Music* music;
	int state;

	//Matrices
	Matrix projectionMatrix;
	Matrix viewMatrix;
	Matrix backgroundMatrix;

	Matrix mainMenuText;
	Matrix mainMenuText2;
	Matrix gameText;

	GLuint font;
	GLuint spritesheet;
	GLuint background;

	vector<float> vertices;
	vector<float> texCoords;

	//Entities
	Entity* pug;
	Entity* player;
	vector<Entity*> staticEntities;

	//Map stuff
	int mapWidth;
	int mapHeight;
	unsigned char** levelData;

	bool done = false;
	float lastFrameTicks = 0.0f;

	vector<Entity*> blocks; //this is for testing purposes
};