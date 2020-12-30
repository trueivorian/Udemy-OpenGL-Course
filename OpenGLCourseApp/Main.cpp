#define STB_IMAGE_IMPLEMENTATION  // Setting up stb to treat the header file as a code file

#include <stdio.h>
#include <string.h> // Used for error reporting
#include <cmath> // Standard library to use the abs function
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp> // Type pointers will be used to attach the value to the shader

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"

// Window dimensions
//const GLint WIDTH = 800, HEIGHT = 600;

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

Texture brickTexture;
Texture dirtTexture;

GLfloat deltaTime = 0.0f; // deltaTime in seconds - TODO: make more accurate for a game scenario (fixed to a particular frame rate)
GLfloat lastTime = 0.0f;

// Vertex Shader - Allows us to modify vertex values
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader - Allows us to modify pixel attributes and 
// add effects (e.g. hazy air over lava) - interpolates between
// vertex values in the colour gradient example
static const char* fShader = "Shaders/shader.frag";


void CreateObjects() {

	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	// Here we create a pointer to a mesh to avoid calling the destructor when going
	// out of scope
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj2);
}

void CreateShaders() {
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main() {

	mainWindow = Window(800, 600);
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	// Create camera object
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTexture();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTexture();

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;

	// This is outside the loop because we only want to create this once
	// parameters:-
	// fov: Field of view (degrees)
	// aspect: Aspect ratio - For SDL this will depend on the width and height of the window
	// zNear: Distance of the near plane from the origin
	// zFar: Distance of the far plane from the origin
	glm::mat4 projection = glm::perspective(45.0f, mainWindow.getBufferWidth()/mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// Loop until window closed
	while (!mainWindow.getShouldClose()) {
		GLfloat now = glfwGetTime(); // For SDL we would use SDL_GetPerformanceCounter()
		deltaTime = now - lastTime; // For SDL we do (now - lastTime)*1000/SDL_GetPerformanceFrequency() to get deltatime in milliseconds
		lastTime = now;

		// Get + Handle user input events
		glfwPollEvents();

		// Check for key presses
		camera.keyControl(mainWindow.getKeys(), deltaTime);

		// Handle mouse movements
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear window - colours are between 0-1 (divide rgb by 256)
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		// Enums can be combined using bitwise or
		// Here we clear the colour and depth buffer bits
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Each pixel stores depth, stencil... more than just colour!

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();

		// model is automatically set to an identity matrix
		glm::mat4 model(1.0f);

		// Apply an x translation 
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		// The uniform is a 4x4 matrix of floats
		// GL_FALSE means we don't want to flip the matrix
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[0]->RenderMesh();

		// Re-initialise model
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		glUseProgram(0);

		// There are two scenes going on at once - OpenGL draws to the unseen scene and then the buffers are swapped
		mainWindow.swapBuffers();
	}

	return 0;
}