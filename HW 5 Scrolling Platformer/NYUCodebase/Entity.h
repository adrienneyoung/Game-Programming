#include "Funcs.h"

class Entity {
public:
	Entity();
	Entity(float xPos, float yPos, float width, float height, float speed, const char* texPath);

	void Update(float elapsed);
	void Render(ShaderProgram* program, Matrix& matrix, int index);
	void jump();
	bool collidesWith(Entity* e);

	//Texture stuff
	Matrix matrix;
	GLuint tex;
	SheetSprite sprite;

	//(xPos,yPos) is the center of the entity
	float xPos = 0.0f; 
	float yPos = 0.0f;
	float width;
	float height;

	//delete this later
	float speed = 2.0f;

	float xMovement = 0.0f;
	float yMovement = 0.0f;
	float xDir = 1.0f;
	float yDir = 1.0f;

	//Velocity
	float xVel = 0.0f;
	float yVel = 0.0f;

	//Acceleration
	float xAcc = 0.0f;
	float yAcc = 0.0f;

	//Collision detection
	
	//Flags
	bool display = false;
	bool isStatic = false; //no gravity, no movement, no collision checks ex: a block
};
