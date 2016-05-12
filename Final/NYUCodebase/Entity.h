#include "Funcs.h"
#define FIXED_TIMESTEP 0.0166666f

class Entity {
public:
	Entity();
	Entity(float xPos, float yPos, float width, float height, const char* texPath);
	Entity(float xPos, float yPos, float width, float height, const char* texPath, const char* texPath2);

	void Animate(float fixedElapsed);
	void Update(float fixedElapsed);
	void Render(ShaderProgram* program, Matrix& matrix, int index);

	//Collisions
	bool collidesWith(Entity* e);
	bool bulletCollidesWith(Entity* e);
	void handleCollision(Entity* e);

	//Abilities
	void Hump();
	void humpStop();
	void humpStart();
	bool getHumpingStatus();

	//Texture stuff
	Matrix matrix;
	GLuint tex;
	GLuint tex2;
	SheetSprite sprite;

	//(xPos,yPos) is the center of the entity
	float xPos = 0.0f;
	float yPos = 0.0f;
	float width;
	float height;

	//Velocity (speed + direction)
	float xVel = 2.0f;
	float yVel = 0.0f;

	//Acceleration
	float xAcc = 0.0f;
	float yAcc = 0.0f;

	//Friction
	float xFric = 8.0f;
	float yFric = 0.0f;
	//float yFric = 0.7f;

	//Gravity
	float xGrav = 0.0f;
	float yGrav = -150.0f;
	//float yGrav = 0.0f;

	//Collision detection
	bool collidedTop;
	bool collidedBottom;
	bool collidedLeft;
	bool collidedRight;

	//Flags
	bool display = false;
	bool isBullet = false;
	bool isEnemy = false;
	bool isDead = false;
	bool isHumping = false;
	bool humpUp = false;

	//Animate player
	vector<int> runAnimationLeft;
	vector<int> runAnimationRight;
	vector<int> runAnimationFront;
	vector<int> runAnimationBack;

	int numFrames = 3;
	float animationElapsed = 0.0f;
	float framesPerSecond = 10.0f;
	int currentIndex = 0;
	int directionFacing = 0;

	//Player's bullets
	vector<Entity*> bullets;
	int maxBullets = 10;
	int bulletCount = 0;

	//Health
	float health;
};
