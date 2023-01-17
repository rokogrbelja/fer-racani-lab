#pragma once

#include "Transform.h"
#include "Shader.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#include <vector>

struct Particle {
	glm::vec3 position;
	glm::vec4 color;
	glm::vec3 velocity;
	float life;

	Particle() : position(0.0f), color(1.0f), velocity(0.0f, -1.0f, 0.0f), life(0.0f) { }
};

class ParticleGenerator {

private:
	std::vector<Particle> particles;
	unsigned int N;
	float timeSinceSpawn;
	float spawnDelay;
	float decayRate;
	
	void loadTexture(const char * texturePath);
	void setupBuffers();
	void initParticles();
	unsigned int firstUnusedParticle();
	void respawnParticle(Particle &particle, glm::vec3 position);

	glm::mat4 getParticleModelMatrix(Particle p);

public:
	unsigned int vertexArrayID;
	unsigned int vertexBufferID;
	unsigned int textureID;
	Shader* shader;

	ParticleGenerator();
	ParticleGenerator(Shader * shader, unsigned int N, const char * texturePath, float decayRate, float spawnDelay);
	void update(float dt, bool spawnParticle, glm::vec3 position, float speed);
	void draw(glm::mat4 view, glm::mat4 projection);
};