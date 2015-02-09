#include "Vertex.h"

Vertex::Vertex()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);

	// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	static const GLfloat vertex_buffer_data[] = {
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};

	vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), vertex_buffer_data, GL_STATIC_DRAW);

	vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

Vertex::Vertex(glm::vec3 pos)
{
	position = pos;
}

Vertex::~Vertex()
{

}

void Vertex::draw()
{
	

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

glm::vec3 Vertex::getPosition()
{
	return position;
}
