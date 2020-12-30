#pragma once

#include <stdio.h>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);

	int Initialise();

	GLfloat getBufferWidth() {
		return bufferWidth;
	}

	GLfloat getBufferHeight() {
		return bufferHeight;
	}

	bool getShouldClose() {
		return glfwWindowShouldClose(mainWindow);
	}

	bool* getKeys() {
		return keys;
	}

	// The camera will poll the movements of the mouse every single frame and after detecting
	// a change in x or y axes the values for xChange and yChange are set, but then never reset
	// back to 0 - we would use a boolean in a games context to test the validity of the value
	// however for the purpose of this example we reset the values
	GLfloat getXChange();
	GLfloat getYChange();

	void swapBuffers() {
		glfwSwapBuffers(mainWindow);
	}

	~Window();

private:
	GLFWwindow* mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024]; // Covers all ASCII characters

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;

	bool mouseFirstMoved; // Check if it is the very first movement to disregard it

	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode); // We only need to use the static modifier in the declaration

	static void handleMouse(GLFWwindow* window, double xPos, double yPos);

	void createCallbacks();
};

