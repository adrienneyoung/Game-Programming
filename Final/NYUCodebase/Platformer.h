#include "Entity.h"
#include <vector>
#include <SDL_mixer.h>

// 60 FPS (1.0f/60.0f)
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

#define SPRITE_COUNT_X 12
#define SPRITE_COUNT_Y 12

#define LEVEL_HEIGHT 14
#define LEVEL_WIDTH 27
#define TILE_SIZE 0.6f

class Platformer {
public:
	Platformer();
	~Platformer();

	void scrollScreen();

	void Setup();
	void Render();
	void Update(float fixedElapsed);
	bool Run();

	void BuildLevel();
	void RenderLevel();
	void worldToTileCoordinates(float worldX, float worldY, int *gridX, int *gridY);
	bool isSolid(int x, int y, unsigned char levelData[LEVEL_HEIGHT][LEVEL_WIDTH]);

	//ParticleEmitter stuff
	int particleTexture;
	Pair position;
	Pair velocity;
	Pair velocityDeviation;
	float maxLifetime;

	float startSize;
	float endSize;
	float sizeDeviation;

	void initializeEmitter();
	void randomize(Particle& p, float x, float y);
	void RenderTexture(ShaderProgram * program);
	void UpdateEmitter(float elapsed, float x, float y);
	void setPosition(float x, float y);
	void turnOn();
	void turnOff();
	bool getRunningStatus();
	void checkActionWithEnemy();

	bool isRunning;

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
	float offsetX = 0.5f;
	float offsetY = 0.5f;

	//Entities
	Entity* player;
	vector<Entity*> enemies;
	vector<Entity*> staticEntities;
	vector<Particle*> particles;

	bool done = false;
	float lastFrameTicks = 0.0f;
};