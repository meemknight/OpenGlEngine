#include "ParticleSystem.h"


static const GLfloat vertexBufferData[] = {
 -0.5f, -0.5f, 0.0f,
 0.5f, -0.5f, 0.0f,
 -0.5f, 0.5f, 0.0f,
 0.5f, 0.5f, 0.0f,
};

ParticleSystem::ParticleSystem(unsigned int count, ShaderProgram &sp) :count(count), sp(sp)
{
	buildParticleSystem();
}

void ParticleSystem::draw()
{
	for (int i = 0; i < count; i++)
	{
		ParticlePositions[i].x = 0;
		ParticlePositions[i].y = 4;
		ParticlePositions[i].z = 0;
	}

	//shape
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexShapeId);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//positions
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vertexPositionId);
	glBufferData(GL_ARRAY_BUFFER, count * 3 * sizeof(float), ParticlePositions, GL_STREAM_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//colors
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, vertexColorsId);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);

	sp.bind();
	glm::mat4 projection = camera->getObjectToWorld();
	projection[0][0] = 1;
	projection[1][1] = 1;
	projection[2][2] = 1;
	
	projection[0][1] = 0;
	projection[1][0] = 0;
	projection[0][2] = 0;
	projection[2][0] = 0;
	projection[2][1] = 0;
	projection[1][2] = 0;
	
	projection = camera->getProjectionMatrix() * projection;

	glUniformMatrix4fv(sp.getUniformLocation("projectionMatrix"), 1, GL_FALSE, &projection[0][0]);

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, count);
}

void ParticleSystem::buildParticleSystem()
{

	glGenBuffers(1, &vertexShapeId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexShapeId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW);

	glGenBuffers(1, &vertexPositionId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexPositionId);
	glBufferData(GL_ARRAY_BUFFER, count * 3 * sizeof(float) , nullptr , GL_STREAM_DRAW);

	glGenBuffers(1, &vertexColorsId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexColorsId);
	
	glm::vec3 *colors;
	colors = new glm::vec3[count];

	for(int i=0; i<count; i++)
	{
		colors[i].r = 0.8f;
		colors[i].g = 0.8f;
		colors[i].b = 1.0f;
	}

	glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::vec3), colors, GL_STREAM_DRAW);

	delete[] colors;

	ParticlePositions = new glm::vec3[count];
	ParticleDrag = new glm::vec4[count];



}

/*
static const GLfloat g_vertex_buffer_data[] = {
 -0.5f, -0.5f, 0.0f,
 0.5f, -0.5f, 0.0f,
 -0.5f, 0.5f, 0.0f,
 0.5f, 0.5f, 0.0f,
};

GLuint billboard_vertex_buffer;
glGenBuffers(1, &billboard_vertex_buffer);
glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

// The VBO containing the positions and sizes of the particles
GLuint particles_position_buffer;
glGenBuffers(1, &particles_position_buffer);
glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
// Initialize with empty (NULL) buffer : it will be updated later, each frame.
glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

// The VBO containing the colors of the particles
GLuint particles_color_buffer;
glGenBuffers(1, &particles_color_buffer);
glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
// Initialize with empty (NULL) buffer : it will be updated later, each frame.
glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
*/