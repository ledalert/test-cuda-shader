#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GLFW/glfw3.h>
#include <pthread.h>
#include <png.h>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_A && action == GLFW_PRESS) {

	}
		
	if (key == GLFW_KEY_D && action == GLFW_PRESS) {

	}

	if (key == GLFW_KEY_Z && action == GLFW_PRESS) {

	}
		
	if (key == GLFW_KEY_X && action == GLFW_PRESS) {
	}

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
	}


	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
	}
		
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
	}

	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
	}
		
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
	}

	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
	}
		
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
	}



	if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
	}
		
	if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
	}

	if (key == GLFW_KEY_Z && action == GLFW_RELEASE) {
	}
		
	if (key == GLFW_KEY_X && action == GLFW_RELEASE) {
	}

	if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
	}
		
	if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
	}

	if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE) {
	}
		
	if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE) {
	}

	if (key == GLFW_KEY_UP && action == GLFW_RELEASE) {
	}
		
	if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE) {
	}

}
