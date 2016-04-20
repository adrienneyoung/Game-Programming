#include "Funcs.h"
#define FIXED_TIMESTEP 0.0166666f

class Entity {
public:
	Entity();
	Entity(float xPos, float yPos, float width, float height, const char* texPath);

	void Update(float elapsed);
	void Render(ShaderProgram* program, Matrix& matrix, int index);
	void jump();
	bool collidesWith(Entity* block);
	void handleCollision(Entity* block);

	//Texture stuff
	Matrix matrix;
	GLuint tex;
	SheetSprite sprite;

	//(xPos,yPos) is the center of the entity
	float xPos = 0.0f; 
	float yPos = 0.0f;
	float width;
	float height;

	//Velocity (speed + direction)
	float xVel = 0.0f;
	float yVel = 0.0f;

	//Acceleration
	float xAcc = 0.0f;
	float yAcc = 0.0f;

	//Friction
	float xFric = 0.08f;
	float yFric = 0.08f;

	//Gravity
	float xGrav = 0.0f;
	float yGrav = -0.5f; 

	//Collision detection
	bool collidedTop;
	bool collidedBottom;
	bool collidedLeft;
	bool collidedRight;

	//Flags
	bool display = false;
	bool isStatic = false; //no gravity, no movement, no collision checks ex: a block
	
};
