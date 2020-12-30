#include "Camera.h"

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed) : 
	position(startPosition), worldUp(startUp), yaw(startYaw), pitch(startPitch), moveSpeed(startMoveSpeed), turnSpeed(startTurnSpeed), 
	front(glm::vec3(0.0f, 0.0f, -1.0f)){
	update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime) {
	// Update does not need to be called here because we're not changing the yaw or pitch with key presses (only mouse presses)
	
	GLfloat velocity = moveSpeed * deltaTime;

	if (keys[GLFW_KEY_W]) { // If a W key has been pressed
		position += front * velocity; 
	}

	if (keys[GLFW_KEY_S]) { 
		position -= front * velocity;
	}

	if (keys[GLFW_KEY_A]) {
		position -= right * velocity;
	}

	if (keys[GLFW_KEY_D]) { 
		position += right * velocity;
	}
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange) {
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange; // We're limiting the pitch so that the head can't roll all the way back

	if (pitch > 89.0f) { // We limit the pitch < |89.0| rather than 90.0 because we would observe strange behaviour at 90 degrees
		pitch = 89.0f;
	}

	if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	update();

}

glm::mat4 Camera::calculateViewMatrix() {

	// parameters:-
	// const glm::tvec3<T,P>& eye: Where we're looking from
	// const glm::tvec3<T,P>& center: Where the thing we're looking at is
	// const glm::tvec3<T,P>& up: Which way is up? (for yaw and pitch calculations)
	return glm::lookAt(position, position + front, up); // Designed for cameras to generate a matrix at looks at an object for you

}

void Camera::update() {
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front); // Sets the magnitude of the vector since we only need the direction

	right = glm::normalize(glm::cross(front, worldUp)); // We use world up since the local up variable can change
	up = glm::normalize(glm::cross(right, front));
}

Camera::~Camera() {

}