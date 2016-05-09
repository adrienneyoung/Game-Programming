#include "Entity.h"
#include <vector>
#include <SDL_mixer.h>

// 60 FPS (1.0f/60.0f)
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

#define SPRITE_COUNT_X 14
#define SPRITE_COUNT_Y 7

//#define SPRITE_COUNT_X 30
//#define SPRITE_COUNT_Y 30

#define LEVEL_HEIGHT 9
#define LEVEL_WIDTH 12
#define TILE_SIZE 1.0f

class Platformer {
public:
	Platformer();
	~Platformer();

	void scrollScreen();
	void handleCollisions();

	void Setup();
	void Render();
	void Update(float fixedElapsed);
	bool Run();

	void BuildLevel();
	void RenderLevel();
	void worldToTileCoordinates(float worldX, float worldY, int *gridX, int *gridY);

private:
	SDL_Window* displayWindow;
	ShaderProgram* program;
	Mix_Music* music;
	int state;

	unsigned char levelData[LEVEL_HEIGHT][LEVEL_WIDTH];

	//Matrices
	Matrix projectionMatrix;
	Matrix viewMatrix;
	Matrix backgroundMatrix;
	Matrix backgroundMatrix2;

	Matrix mainMenuText;
	Matrix mainMenuText2;
	Matrix gameText;

	GLuint font;
	GLuint spritesheet;
	GLuint gameBackground;
	GLuint mainBackground;
	
	vector<float> svertices;
	vector<float> stexCoords; 
	vector<float> vertices;
	vector<float> texCoords;

	float textureOffsetX = 3.55f;

	//Entities
	Entity* player;
	//vector<Entity*> staticEntities;

	bool done = false;
	float lastFrameTicks = 0.0f;
};