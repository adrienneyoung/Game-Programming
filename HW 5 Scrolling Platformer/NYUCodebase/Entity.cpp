#include "Entity.h"

Entity::Entity() {}
Entity::Entity(float xPos, float yPos, float width, float height, float speed, const char* texPath)
	: xPos(xPos), yPos(yPos), width(width), height(height) { tex = LoadTexture(texPath); }
Entity::Entity(float xPos, float yPos, float width, float height, const char* texPath)
	: xPos(xPos), yPos(yPos), width(width), height(height) { tex = LoadTexture(texPath); }

//Draws entity using SpriteSheet::Draw
void Entity::Render(ShaderProgram* program, Matrix& matrix, int index) {
	sprite.Draw(program, matrix, index, xPos, yPos);
}

void Entity::Update(float elapsed)
{
	xVel += xAcc * FIXED_TIMESTEP;
	xPos += xVel * FIXED_TIMESTEP;

	yVel += yAcc * FIXED_TIMESTEP;
	yPos += yVel * FIXED_TIMESTEP;

	matrix.Translate(xVel * FIXED_TIMESTEP, yVel * FIXED_TIMESTEP, 0.0f);
}

//Jump if currently touching the ground
void Entity::jump() {
	xVel = 2.0f;
}

bool Entity::collidesWith(Entity* block) {
	/*
	is player’s bottom higher than block’s top ?
	is player’s top lower than block’s bottom ?
	is player’s left larger than block’s right ?
	is player’s right smaller than block’s left ?
	If ANY of the above are true, then the two rectangles are NOT intersecting!
	The rectangles are intersecting if NONE of the above are true.

	if(player bot < block top && player top > block bot && player left < block right && player right > block left)
		return true if there's a collision
	*/
	
	float top = yPos+ height / 2;
	float bot = yPos- height / 2;
	float left = xPos - width / 2;
	float right = xPos + width / 2;

	float blockTop = block->yPos + block->height / 2;
	float blockBot = block->yPos - block->height / 2;
	float blockLeft = block->xPos - block->width / 2;
	float blockRight = block->xPos + block->width / 2;

	//Penetration
	float yPen = 0.0f;
	float xPen = 0.0f;

	if (bot <= blockTop) {
		collidedBottom = true;
		yPen = fabs(yPos - block->yPos - height / 2 - block->height / 2);
		yPos += yPen + 0.0001f;
	}

	//if (yPos - height / 2 < e->yPos + e->height / 2 && yPos + height / 2 > e->yPos - e->height / 2 &&
	//	xPos - width / 2 < e->xPos + e->width / 2 && xPos + width / 2 > e->xPos - e->width / 2) 
	//	return true;

	return false;
}
