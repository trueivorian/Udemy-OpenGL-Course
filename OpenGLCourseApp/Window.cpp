#include "Window.h"

Window::Window() : width(800), height(600), xChange(0.0f), yChange(0.0f){
	for (size_t i = 0; i < 1024; i++) {
		keys[i] = 0;
	}
}

Window::Window(GLint windowWidth, GLint windowHeight) : width(windowWidth), height(windowHeight), xChange(0.0f), yChange(0.0f) {
	for (size_t i = 0; i < 1024; i++) {
		keys[i] = 0;
	}
}

int Window::Initialise() {
	// Initialise GLFW
	if (!glfwInit()) {
		printf("GLFW initialisation failed");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW window properties
	// OpenGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core profile = No backwards compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);
	if (!mainWindow) {
		printf("GLFW window creation failed");
		glfwTerminate();
		return 1;
	}

	// Get Buffer size information - The buffer is the area in the middle of the window that
	// holds the OpenGL data as its being passed to the window
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context for GLEW to use - let glew know that this window is the one the OpenGL window
	// is tied to
	glfwMakeContextCurrent(mainWindow);

	// Handle Keyboard and Mouse Input
	createCallbacks();
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Lock the cursor to the screen

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	GLenum error = glewInit();

	if (error != GLEW_OK) {
		printf("Error: %s", glewGetErrorString(error));
		glfwDestroyWindow(mainWindow); // get rid of the window
		glfwTerminate();
		return 1;
	}

	// Enables depth testing to test what elements are deeper into the image
	// and what needs to be drawn over another element
	glEnable(GL_DEPTH_TEST);

	// Setup Viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	// Allows us to run the callback
	// Because the callback goes to a static function handleKeys(), the function
	// won't know which object has called it. The generated user pointer will
	// allow the program to identify that object
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks() {
	glfwSetKeyCallback(mainWindow, handleKeys);
	glfwSetCursorPosCallback(mainWindow, handleMouse);
}

GLfloat Window::getXChange() {
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange() {
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode) {
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) { // If the user has pressed the escape key
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if(key >= 0 && key < 1024){ // If the key is valid
		if (action == GLFW_PRESS) { // If the user has pressed a key
			theWindow->keys[key] = true;
		}
		else if (action == GLFW_RELEASE) {
			theWindow->keys[key] = false;
		}
	}
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos) {
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved) {
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos; // To avoid the inverted up/down movement

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;

	//printf("x:%.6f, y:%.6f\n", theWindow->xChange, theWindow->yChange);

}

Window::~Window() {
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
