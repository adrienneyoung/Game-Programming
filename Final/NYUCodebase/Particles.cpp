#include "Particles.h"
Particle::Particle(){
	texture = LoadTexture("bun.png");
	position.x = 5.0f;
	position.y = -7.5f;
	width = .5f;
	height = .5f;
}
void Particle::Render(ShaderProgram* program){
	float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
	GLfloat vertices[] =
	{
		position.x - width / 2, position.y - height / 2,
		position.x + width / 2, position.y - height / 2,
		position.x + width / 2, position.y + height / 2,
		position.x - width / 2, position.y - height / 2,
		position.x + width / 2, position.y + height / 2,
		position.x - width / 2, position.y + height / 2 //0,0
	};

	program->setModelMatrix(matrix);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //not white(?)
	//			glBlendFunc(GL_SRC_ALPHA, GL_ONE); //white
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);
	glBindTexture(GL_TEXTURE_2D, texture);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}
bool Particle::collidesWith(Entity* e) {

	float top = position.y + height / 2;
	float bot = position.y - height / 2;
	float left = position.x - width / 2;
	float right = position.x + width / 2;

	float blockTop = e->yPos + e->height / 2;
	float blockBot = e->yPos - e->height / 2;
	float blockLeft = e->xPos - e->width / 2;
	float blockRight = e->xPos + e->width / 2;

	if (bot < blockTop && top > blockBot && left < blockRight && right > blockLeft)
		return true; //there's a collision
	return false;
}
