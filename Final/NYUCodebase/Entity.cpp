#include "Entity.h"

Entity::Entity() {}

Entity::Entity(float xPos, float yPos, float width, float height, const char* texPath)
	: xPos(xPos), yPos(yPos), width(width), height(height) {
	tex = LoadTexture(texPath); isHumping = false; humpUp = false;
}

Entity::Entity(float xPos, float yPos, float width, float height, const char* texPath, const char* texPath2)
	: xPos(xPos), yPos(yPos), width(width), height(height) {
	tex = LoadTexture(texPath);
	tex2 = LoadTexture(texPath2);
	isHumping = false;
	humpUp = false;
}

//Draws entity using SpriteSheet::Draw
void Entity::Render(ShaderProgram* program, Matrix& matrix, int index) {
	sprite.Draw(program, matrix, index, xPos, yPos);
}

void Entity::Animate(float fixedElapsed) {
	animationElapsed += fixedElapsed;
	if (animationElapsed > 1.0 / framesPerSecond) {
		currentIndex++;
		animationElapsed = 0.0;
		if (currentIndex > numFrames - 1) {
			currentIndex = 0;
		}
	}
	Hump();
}

void Entity::Update(float fixedElapsed)
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
		matrix.setRotation(fixedElapsed);
	}

	if (isEnemy) {
		yGrav = 0.0f;
	}

	Animate(fixedElapsed);
}

bool Entity::collidesWith(Entity* e) {

	float top = yPos + height / 2;
	float bot = yPos - height / 2;
	float left = xPos - width / 2;
	float right = xPos + width / 2;

	float blockTop = e->yPos + e->height / 2;
	float blockBot = e->yPos - e->height / 2;
	float blockLeft = e->xPos - e->width / 2;
	float blockRight = e->xPos + e->width / 2;

	if (bot < blockTop && top > blockBot && left < blockRight && right > blockLeft)
		return true; //there's a collision
	return false;
}

bool Entity::bulletCollidesWith(Entity* e) {
	float top = yPos + height / 2.5f;
	float bot = yPos - height / 2.5f;
	float left = xPos - width / 2.5f;
	float right = xPos + width / 2.5f;

	float eTop = e->yPos + e->height / 2.5f;
	float eBot = e->yPos - e->height / 2.5f;
	float eLeft = e->xPos - e->width / 2.5f;
	float eRight = e->xPos + e->width / 2.5f;

	if (bot < eTop && top > eBot && left < eRight && right > eLeft)
		//if (yPos - height / 2 < e->yPos + e->height / 2 && yPos + height / 2 > e->yPos - e->height / 2 &&
		//	xPos - width / 2 < e->xPos + e->width / 2 && xPos + width / 2 > e->xPos - e->width / 2)
		return true;
	return false;
}

void Entity::handleCollision(Entity* e) {
	float top = yPos + height / 2;
	float bot = yPos - height / 2;
	float left = xPos - width / 2;
	float right = xPos + width / 2;

	float eTop = e->yPos + e->height / 2;
	float eBot = e->yPos - e->height / 2;
	float eLeft = e->xPos - e->width / 2;
	float eRight = e->xPos + e->width / 2;

	//Penetration
	float xPen = 0.0f;
	float yPen = 0.0f;

	//Handles Y Collision
	if (fabs(xPos - e->xPos) < (width + e->width) / 2.5f) {
		if (e->isEnemy) {
			yVel = 0.0f;
		}

		if (yPos > e->yPos) {
			yPen = fabs(bot - eTop);
			yPos += yPen + 0.0001f;
			collidedBottom = true;
		}

		else if (yPos < e->yPos) {
			yPen = fabs(top - eBot);
			yPos -= yPen + 0.0001f;
		}
	}

	//Handles X Collision
	if (fabs(yPos - e->yPos) < (height + e->height) / 2.5f) {
		if (e->isEnemy) {
			xVel = 0.0f;
		}

		if (xPos > e->xPos) {
			xPen = fabs(left - eRight);
			xPos += xPen + 0.0001f;
		}

		else if (xPos < e->xPos) {
			xPen = fabs(right - eLeft);
			xPos -= xPen + 0.0001f;
		}
	}
}

void Entity::Hump(){
	if (isHumping){
		if (directionFacing == 1){
			//Right
			matrix.setRotation(120);
		}
		if (directionFacing == -1){
			//Left
			matrix.setRotation(150);
		}
		if (directionFacing == 0){
			matrix.setRotation(0);
		}
		if (humpUp){
			yPos += .1f;
			humpUp = false;
		}
		else{
			yPos -= .1f;
			humpUp = true;
		}
	}
	else{
		matrix.setRotation(0);
	}
}

void Entity::humpStop(){
	isHumping = false;
}

void Entity::humpStart(){
	isHumping = true;
}

bool Entity::getHumpingStatus(){
	return isHumping;
}