#include "Mesh.h"

Mesh::Mesh() : VAO(0), VBO(0), IBO (0), indexCount(0) {}

void Mesh::CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices) {
	indexCount = numOfIndices;

	// Create the VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Create the IBO
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

	// Create a VBO and assign to the 
	// previously created VAO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Using GL_STATIC_DRAW tells the graphics card we won't
	// move the triangle once it is drawn
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

	// parameters:-
	// GLuint index: The location of the attribute we're creating the pointer for (0 = position, 1 = texture coords - for this example)
	// GLint size: The size of each value that will be passed (x,y,z) = 3
	// GLenum type: The type of the entered values
	// GLboolean normalized: If we want to normalise the values
	// GLsizei stride: Allows us to splice data together (e.g. vertices and colours)
	// const void *pointer: offset - where the data starts
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 5, 0);
	// GLuint index: The location of the same attribute referenced in the glVertexAttribPointer()
	// argument
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 5, (void*)sizeof(vertices[0] * 3));
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind the VAO so the next object can be drawn
	glBindVertexArray(0);

	// IBO must be unbound after the VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// TODO: Add test to see if the index count is 0 to avoid drawing objects with
// no vertices
void Mesh::RenderMesh() {

	glBindVertexArray(VAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	// parameters:-
	// GLenum mode: There are different modes for different shapes
	// GLsizei count: The number of indices (vertices on each triangle comprising the shape)
	// GLenum type: Format of the indices
	// const void *indices: pointer to the index
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// paraeters:-
	// GLenum mode: There are different modes for different shapes
	// GLint first: Where in the array we are starting
	// GLsizei count: Number of points
	//glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(0);
}

void Mesh::ClearMesh() {
	if (IBO != 0) {
		// Deletes the buffer from the graphics card memory
		// otherwise buffers will stack up and we will have
		// buffer overflow
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}

	if (VBO != 0) {
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	if (VAO != 0) {
		glDeleteBuffers(1, &VAO);
		VAO = 0;
	}

	indexCount = 0;
}

Mesh::~Mesh() {
	ClearMesh();
}