#include "Entity.h"

Entity::Entity() {}
Entity::Entity(float xPos, float yPos, float width, float height, const char* texPath)
	: xPos(xPos), yPos(yPos), width(width), height(height) { tex = LoadTexture(texPath); }

//Draws entity using SpriteSheet::Draw
void Entity::Render(ShaderProgram* program, Matrix& matrix, int index) {
	sprite.Draw(program, matrix, index, xPos, yPos);
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

	yAcc = yGrav * FIXED_TIMESTEP;
	yVel += yAcc * FIXED_TIMESTEP;
}

//Jump if currently touching the ground
void Entity::jump() {
	yVel = 0.25f;
}

bool Entity::collidesWith(Entity* block) {
	/*
	is player’s bottom higher than block’s top ?
	is player’s top lower than block’s bottom ?
	is player’s left larger than block’s right ?
	is player’s right smaller than block’s left ?
	If ANY of the above are true, then the two rectangles are NOT intersecting!
	The rectangles are intersecting if NONE of the above are true.
	*/

	//if (bot < blockTop && top > blockBot && left < blockRight && right > blockLeft)
	if (yPos - height / 2 < block->yPos + block->height / 2 &&
		yPos + height / 2 > block->yPos - block->height / 2 &&
		xPos - width / 2 < block->xPos + block->width / 2 &&
		xPos + width / 2 > block->xPos - block->width / 2)
		return true; //there's a collision
	return false;
}


void Entity::handleCollision(Entity* block) {
	float top = yPos + height / 2;
	float bot = yPos - height / 2;
	float left = xPos - width / 2;
	float right = xPos + width / 2;

	float blockTop = block->yPos + block->height / 2;
	float blockBot = block->yPos - block->height / 2;
	float blockLeft = block->xPos - block->width / 2;
	float blockRight = block->xPos + block->width / 2;

	//Penetration
	float xPen = 0.0f;
	float yPen = 0.0f;

	//2.5f instead of 2.0f so that the player has to be deeper inside the block for the collision to occur
	if (fabs(xPos - block->xPos) < (block->width + width) / 2.5f) {
		if (block->isStatic) {
			yVel = 0.0f;
		}

		if (bot > block->yPos)
		{
			collidedBottom = true;
			yPen = fabs((bot) - (blockTop));
			yPos += yPen + 0.0001f;
		}

		else if (top < block->yPos)
		{
			collidedTop = true;
			yPen = fabs((top) - (blockBot));
			yPos -= yPen + 0.0001f;
		}
	}

	 if (fabs(yPos - block->yPos) < (block->height + height) / 2.5f) {
		if (block->isStatic) {
			xVel = 0.0f;
		}

		if (left > block->xPos)
		{
			collidedLeft = true;
			xPen = fabs((left) - (blockRight));
			xPos += xPen + 0.0001f;
		}

		else if (right < block->xPos)
		{
			collidedRight = true;
			xPen = fabs((right) - (blockLeft));
			xPos -= xPen + 0.0001f;
		}
	}
}

