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
	collidedTop = false;
	collidedBottom = false;
	collidedLeft = false;
	collidedRight = false;

	xVel += xAcc * FIXED_TIMESTEP;
	xPos += xVel * FIXED_TIMESTEP;

	yVel += yAcc * FIXED_TIMESTEP;
	yPos += yVel * FIXED_TIMESTEP;

	//matrix.Translate(xVel * FIXED_TIMESTEP, yVel * FIXED_TIMESTEP, 0.0f);
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
	
	if (bot < blockTop) {
		if (left < blockRight && right > blockLeft && top > blockBot) { //fix top > blockBot since it conflicts
			collidedBottom = true;

			if (block->isStatic) {
				yVel = 0.0f;
			}

			//Akari's amount of penetration = abs(bottom of Akari - blockTop) 
			yPen = fabs((yPos - height / 2) - (block->yPos + block->height / 2));
			yPos += yPen + 0.0001f;
		}
	}
	
	
	else if (top > blockBot) {
		if (left < blockRight && right > blockLeft && bot < blockTop) { //fix bot < blockTop since it conflicts
			collidedTop = true;

			if (block->isStatic) {
				yVel = 0.0f;
			}

			//Akari's amount of penetration = abs(top of Akari - blockBot)
			yPen = fabs((yPos + height / 2) - (block->yPos - block->height / 2));
			yPos -= yPen + 0.0001f;
		}
	}
	/*
	
	if (left < blockRight) {
		if (top > blockBot && bot < blockTop && right > blockLeft) {
			collidedLeft = true;
			if (block->isStatic) {
				xVel = 0.0f;
				xAcc = 0.0f;
			}

			//Akari's amount of penetration = abs(left of Akari - blockRight)
			xPen = fabs((xPos - width / 2) - (block->xPos + block->width / 2));
			xPos += xPen + 0.0001f;
		}
	}

	
	else if (right > blockLeft) {
		if (top > blockBot && bot < blockTop && left < blockRight) {
			collidedRight = true;
			if (block->isStatic) {
				xVel = 0.0f;
				xAcc = 0.0f;
			}
			
			//Akari's amount of penetration = abs(right of Akari - blockLeft)
			xPen = fabs((xPos + width / 2) - (block->xPos - block->width / 2));
			xPos -= xPen + 0.0001f;
		}
	}
	*/
	
	if (bot < blockTop && top > blockBot && left < blockRight && right > blockLeft)
		return true; //there's a collision
	return false;
}

