#include "Vertex.h"

Vertex::Vertex()
{
	basicShader = new GLShader("../shaders/main.vert", "../shaders/main.frag");

	position = glm::vec3(0.0f, 0.0f, 0.0f);

	// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	float xsize = 1;
	float ysize = 1;
	float zsize = 1;

	GLfloat vertex_array_data[] = {
		//fram
		xsize / 2, -ysize / 2, zsize / 2, 0, 0, 1, 1, 0, //0
		xsize / 2, ysize / 2, zsize / 2, 0, 0, 1, 1, 1, //1
		-xsize / 2, ysize / 2, zsize / 2, 0, 0, 1, 0, 1, //2
		-xsize / 2, -ysize / 2, zsize / 2, 0, 0, 1, 0, 0, //3
		//bak
		xsize / 2, -ysize / 2, -zsize / 2, 0, 0, -1, 0, 0, //4
		xsize / 2, ysize / 2, -zsize / 2, 0, 0, -1, 0, 1, //5
		-xsize / 2, ysize / 2, -zsize / 2, 0, 0, -1, 1, 1, //6
		-xsize / 2, -ysize / 2, -zsize / 2, 0, 0, -1, 1, 0, //7
		//topp
		xsize / 2, ysize / 2, zsize / 2, 0, 1, 0, 1, 0, //8
		xsize / 2, ysize / 2, -zsize / 2, 0, 1, 0, 1, 1, //9
		-xsize / 2, ysize / 2, -zsize / 2, 0, 1, 0, 0, 1, //10
		-xsize / 2, ysize / 2, zsize / 2, 0, 1, 0, 0, 0, //11
		//botten
		xsize / 2, -ysize / 2, zsize / 2, 0, -1, 0, 0, 0, //12
		xsize / 2, -ysize / 2, -zsize / 2, 0, -1, 0, 1, 0, //13
		-xsize / 2, -ysize / 2, -zsize / 2, 0, -1, 0, 1, 1, //14
		-xsize / 2, -ysize / 2, zsize / 2, 0, -1, 0, 0, 1, //15
		//vänster
		-xsize / 2, -ysize / 2, zsize / 2, -1, 0, 0, 1, 0, //16
		-xsize / 2, ysize / 2, zsize / 2, -1, 0, 0, 1, 1, //17
		-xsize / 2, ysize / 2, -zsize / 2, -1, 0, 0, 0, 1, //18
		-xsize / 2, -ysize / 2, -zsize / 2, -1, 0, 0, 0, 0, //19
		//höger
		xsize / 2, -ysize / 2, zsize / 2, 1, 0, 0, 0, 0, //20
		xsize / 2, ysize / 2, zsize / 2, 1, 0, 0, 0, 1, //21
		xsize / 2, ysize / 2, -zsize / 2, 1, 0, 0, 1, 1, //22
		xsize / 2, -ysize / 2, -zsize / 2, 1, 0, 0, 1, 0, //23
	};
	GLuint index_array_data[] = {
		0, 1, 2, //framsida
		2, 3, 0,

		6, 5, 4, //baksida
		4, 7, 6,

		8, 9, 10, //topp
		10, 11, 8,

		14, 13, 12, //botten
		12, 15, 14,

		16, 17, 18, //vänster
		18, 19, 16,

		20, 23, 22, //höger
		22, 21, 20
	};

	vertexarray = vertex_array_data;
	indexarray = index_array_data;

	numberOfVertices = 24;
	numberOfTriangles = 12;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &indexbuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

	glBufferData(GL_ARRAY_BUFFER, 8 * numberOfVertices*sizeof(GLfloat), vertexarray, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		8 * sizeof(GLfloat), (void*)0); // xyz coordinates
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
		8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat))); // normals
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
		8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))); // texcoords

	// Activate the index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
	// Present our vertex indices to OpenGL
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		3 * numberOfTriangles*sizeof(GLuint), indexarray, GL_STATIC_DRAW);

	// Deactivate (unbind) the VAO and the buffers again.
	// Do NOT unbind the buffers while the VAO is still bound.
	// The index buffer is an essential part of the VAO state.
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Vertex::Vertex(glm::vec3 pos)
{
	position = pos;
}

Vertex::~Vertex()
{
	delete basicShader;
}

void Vertex::draw()
{
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
	glm::mat4 Model = glm::mat4(1.0f);  // Changes for each model!

	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

	GLuint MatrixID = glGetUniformLocation(basicShader->programID, "MVP");
	// Send our transformation to the currently bound shader,
	// in the "MVP" uniform
	// For each model you render, since the MVP will be different (at least the M part)
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	glBindVertexArray(vao);
	glUseProgram(basicShader->programID);
	glDrawElements(GL_TRIANGLES, 3 * numberOfTriangles, GL_UNSIGNED_INT, (void*)0);
	// (mode, vertex count, type, element array buffer offset)
	glBindVertexArray(0);
}

glm::vec3 Vertex::getPosition()
{
	return position;
}
