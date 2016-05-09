#include "Entity.h"

Entity::Entity() {}

Entity::Entity(float xPos, float yPos, float width, float height, const char* texPath)
	: xPos(xPos), yPos(yPos), width(width), height(height) { tex = LoadTexture(texPath); }

Entity::Entity(float xPos, float yPos, float width, float height, const char* texPath, const char* texPath2)
	: xPos(xPos), yPos(yPos), width(width), height(height) {
	tex = LoadTexture(texPath);
	tex2 = LoadTexture(texPath2);
}

//Draws entity using SpriteSheet::Draw
void Entity::Render(ShaderProgram* program, Matrix& matrix, int index) {
	sprite.Draw(program, matrix, index, xPos, yPos);
}

void Entity::Animate(float elapsed) {
	animationElapsed += elapsed;
	if (animationElapsed > 1.0 / framesPerSecond) {
		currentIndex++;
		animationElapsed = 0.0;
		if (currentIndex > numFrames - 1) {
			currentIndex = 0;
		}
	}
}

void Entity::Update(float elapsed)
{
	collidedTop = false;
	collidedBottom = false;
	collidedLeft = false;
	collidedRight = false;

	xPos += xVel * FIXED_TIMESTEP;
	yPos += yVel * FIXED_TIMESTEP;

	xVel += xAcc * FIXED_TIMESTEP;
	yVel += yAcc * FIXED_TIMESTEP;

	yAcc = yGrav * FIXED_TIMESTEP;

	if (isBullet) {
		//Bullets don't accelerate (xAcc is 0 and yAcc is 0)
		//yPos is always the same as the point it was fired at (height of the player's head)
		//yVel is 0

		yGrav = 0.0f;
		xFric = 0.0f;
		matrix.Rotate(-elapsed * 20.0f);
	}

	if (isEnemy) {
		yGrav = 0.0f;
	}

	Animate(elapsed);
}

