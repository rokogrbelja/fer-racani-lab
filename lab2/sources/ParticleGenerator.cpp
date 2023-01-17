#include "ParticleGenerator.h"
#include <glm/gtc/type_ptr.hpp>

void ParticleGenerator::loadTexture(const char * texturePath) {
	// Data read from the header of the BMP file
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;   // = width*height*3
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = fopen(texturePath, "rb");
	if (!file) { printf("Image could not be opened\n"); }

	if (fread(header, 1, 54, file) != 54) { // If not 54 bytes read : problem
		printf("Not a correct BMP file\n");
	}

	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
	}

	std::cout << "great.." << std::endl;
	// Read ints from the byte array
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	std::cout << "Image Size: " << imageSize << std::endl;
	width = *(int*)&(header[0x12]);
	std::cout << "width: " << width << std::endl;
	height = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)    imageSize = width * height * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

	// Create a buffer
	data = new unsigned char[imageSize];

	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);

	//Everything is in memory now, the file can be closed
	fclose(file);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cout << "Failed to load texture" << std::endl;
	}

	free(data);
}

void ParticleGenerator::setupBuffers() {

	float vertices[30] = {
		 0.5f,  0.5f, 0.0f,		1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,		1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f,		1.0f, 1.0f,
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,
	};

	glGenVertexArrays(1, &vertexArrayID);
	glGenBuffers(1, &vertexBufferID);

	glBindVertexArray(vertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// vrhovi
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// tekstura
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void ParticleGenerator::initParticles() {
	// dodaj N cestica s zivotom 0
	std::cout << "generiranje cestica...";
	for (unsigned int i = 0; i < this->N; ++i) {
		std::cout << i << " ";
		this->particles.push_back(Particle());
	}

	std::cout << " gotovo!" << std::endl;
}

// indeks posljednje zauzete cestice
unsigned int lastUsedParticle = 0;
unsigned int ParticleGenerator::firstUnusedParticle() {
	// trazimo od zadnje zauzete nadalje - najcesce puno brze nego iscrpno pretrazivanje
	for (unsigned int i = lastUsedParticle; i < this->N; ++i) {
		if (this->particles[i].life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// iscrpno pretrazivanje
	for (unsigned int i = 0; i < lastUsedParticle; ++i) {
		if (this->particles[i].life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// sve cestice zauzete, prepisi preko prve
	lastUsedParticle = 0;
	return 0;
}

void ParticleGenerator::respawnParticle(Particle &particle, glm::vec3 position) {
	// dodavanje nasumicnog suma u brzinu i mjesto stvaranja cestice
	float random_x = ((rand() % 100) - 50) / 50.0f;
	float random_y = ((rand() % 100) - 50) / 50.0f;
	float random_z = ((rand() % 100) - 50) / 50.0f;
	float random_v = ((rand() % 100) - 50) / 100.0f;
	particle.position.x = position.x + random_x;
	particle.position.y = position.y + random_y;
	particle.position.z = position.z + random_z;
	particle.velocity.y = 1.0f + random_v;
	particle.life = 1.0f;
}

glm::mat4 ParticleGenerator::getParticleModelMatrix(Particle p) {
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 translateMatrix = Transform::translate3D(p.position);
	float scale = 1.0f + (1.0f - p.life) * 2.0f;
	glm::mat4 scaleMatrix = Transform::scale3D(glm::vec3(scale, scale, scale));

	return translateMatrix * scaleMatrix * model;
}

ParticleGenerator::ParticleGenerator(Shader * shader, unsigned int N, const char * texturePath, float decayRate, float spawnDelay) {
	this->shader = shader;
	this->decayRate = decayRate;
	this->spawnDelay = spawnDelay;
	this->timeSinceSpawn = spawnDelay;
	this->N = N;
	setupBuffers();
	loadTexture(texturePath);
	initParticles();
}

ParticleGenerator::ParticleGenerator() {}

void ParticleGenerator::update(float dt, bool spawnParticle, glm::vec3 position, float speed) {
	timeSinceSpawn += dt*0.05;
	// ako je kliknut mis, stvori novu cesticu
	if (spawnParticle) {
		if (timeSinceSpawn >= spawnDelay) {
			std::cout << "spawning particle...";
			int unusedParticle = firstUnusedParticle();
			respawnParticle(particles[unusedParticle], position);
			timeSinceSpawn = 0;
		}
	}

	// azuriraj sve cestice
	for (unsigned int i = 0; i < this->N; ++i) {
		Particle &p = this->particles[i];
		p.life -= dt*decayRate; // reduce life
		if (p.life > 0.0f) {
			p.position += p.velocity * dt * speed;
			p.color.a = p.life;
		}
	}
}

void ParticleGenerator::draw(glm::mat4 view, glm::mat4 projection) {

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	unsigned int shaderID = this->shader->ID;
	glUseProgram(shaderID);
	for (Particle p : this->particles) {
		if (p.life > 0.0f) {
			// postavi vrijednost matrice modela u shaderu
			unsigned int modelLoc = glGetUniformLocation(shaderID, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(getParticleModelMatrix(p)));

			// postavi vrijednost matrice pogleda u shaderu
			unsigned int viewLoc = glGetUniformLocation(shaderID, "view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

			// postavi vrijednost matrice projekcije u shaderu
			unsigned int projLoc = glGetUniformLocation(shaderID, "projection");
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

			// postavi poziciju cestice
			unsigned int billboardPositionLoc = glGetUniformLocation(shaderID, "billboard_position");
			glUniform4f(billboardPositionLoc, p.position.x, p.position.y, p.position.z, 0.0f);

			// postavi prozirnost cestice
			unsigned int alphaLoc = glGetUniformLocation(shaderID, "alpha");
			glUniform1f(alphaLoc, p.life);

			// bind Texture
			glBindTexture(GL_TEXTURE_2D, textureID);

			glBindVertexArray(vertexArrayID);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
