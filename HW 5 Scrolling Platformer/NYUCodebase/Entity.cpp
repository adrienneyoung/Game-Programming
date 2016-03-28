#include "Entity.h"

Entity::Entity() {}
Entity::Entity(float xPos, float yPos, float width, float height, float speed, const char* texPath)
	: xPos(xPos), yPos(yPos), width(width), height(height) { tex = LoadTexture(texPath); }

//Draws entity using SpriteSheet::Draw
void Entity::Render(ShaderProgram* program, Matrix& matrix, int index) {
	sprite.Draw(program, matrix, index, xPos, yPos);
}

void Entity::Update(float elapsed)
{
	
}

//Jump if currently touching the ground
void Entity::jump() {

}

bool Entity::collidesWith(Entity* e) {
	/*
	is R1’s bottom higher than R2’s top ?
	is R1’s top lower than R2’s bottom ?
	is R1’s left larger than R2’s right ?
	is R1’s right smaller than R2’s left ?
	If ANY of the above are true, then the two rectangles are NOT intersecting!
	The rectangles are intersecting if NONE of the above are true.

	R1 top = y + height/2
	R1 bot = y - height/2
	R1 left = x - width/2
	R1 right = x + width/2

	R2 top = e->y + e->height/2
	R2 bot = e->y - e->height/2
	R2 left = e->x - e->width/2
	R2 right = e->x + e->width/2

	if(R1 bot < R2 top && R1 top > R2 bot && R1 left < R2 right && R1 right > R2 left)
		return true if there's a collision
	*/

	if (yPos - height / 2 < e->yPos + e->height / 2 && yPos + height / 2 > e->yPos - e->height / 2 &&
		xPos - width / 2 < e->xPos + e->width / 2 && xPos + width / 2 > e->xPos - e->width / 2)
		return true;

	return false;
}

