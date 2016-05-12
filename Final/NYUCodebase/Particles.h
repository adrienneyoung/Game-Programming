#ifndef PARTICLES_H
#define PARTICLES_H

#include "Matrix.h"
#include "Pair.h"
#include "ShaderProgram.h"
#include "Entity.h"

class Particle{
public:

	Pair position;
	Pair velocity;
	float lifetime;

	float width;
	float height;
	Matrix matrix;
	int texture;

	float sizeDeviation;
	float perlinY;
	bool isDead;

	Particle();
	void Render(ShaderProgram * program);
	bool collidesWith(Entity* block);
};

#endif