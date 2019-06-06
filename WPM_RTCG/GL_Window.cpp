#include "pch.h"

#include "GL_Window.h"

#include <iostream>
#include <string.h>

GL_Window::GL_Window()
{
	width = 1024;
	height = 768;

	for (size_t i = 0; i < 1024; i++) {
		keys[i] = 0;
	}

}

GL_Window::GL_Window(GLint window_width, GLint window_height) 
{
	width = window_width;
	height = window_height;

	for (size_t i = 0; i < 1024; i++) {
		keys[i] = 0;
	}

}

int GL_Window::initialise_window() 
{
	if (!glfwInit())
	{
		printf("Initialisierung von GLFW fehgeschlagen!");
		glfwTerminate();
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	mainWindow = glfwCreateWindow(width, height, "Hauptfenster", NULL, NULL);   //erste NULL = glfwGetPrimaryMonitor() oder so für fullscreen
	if (!mainWindow)
	{
		printf("Erstellen des Hauptfensters fehlgeschlagen!");
		glfwTerminate();
		return 1;
	}

	GLint buffer_width, buffer_length;
	glfwGetFramebufferSize(mainWindow, &buffer_width, &buffer_length);
	glfwMakeContextCurrent(mainWindow);

	create_callbacks();
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("GLEW Initialisierung fehgeschlagen!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, buffer_width, buffer_length);

	glfwSetWindowUserPointer(mainWindow, this);
}

void GL_Window::handle_keys(GLFWwindow* window, int key, int code, int action, int mode) 
{

	GL_Window* actual_window = static_cast<GL_Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && GLFW_PRESS) 
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			actual_window->keys[key] = true;
			printf("Pressed Key: %d\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			actual_window->keys[key] = false;
			printf("Released Key: %d\n", key);
		}
	}
}

void GL_Window::handle_mouse(GLFWwindow* window, double new_position_x, double new_position_y) 
{
	GL_Window* actual_window = static_cast<GL_Window*>(glfwGetWindowUserPointer(window));

	if (actual_window->mouse_initial_movement) 
	{
		actual_window->last_x_position = new_position_x;
		actual_window->last_y_position = new_position_y;
		actual_window->mouse_initial_movement = false;
	}

	actual_window->x_position_change = new_position_x - actual_window -> last_x_position;
	actual_window->y_position_change = actual_window->last_y_position - new_position_y;

	actual_window->last_x_position = new_position_x;
	actual_window->last_y_position = new_position_y;

	printf("Change X:%.2f, Change Y:%.2f\n", actual_window->x_position_change, actual_window->y_position_change);
}

void GL_Window::create_callbacks() 
{
	glfwSetKeyCallback(mainWindow, handle_keys);
	glfwSetCursorPosCallback(mainWindow, handle_mouse);
}

GLfloat GL_Window::get_change_x() 
{
	GLfloat actual_change = x_position_change;
	x_position_change = 0.0f;

	return actual_change;
}

GLfloat GL_Window::get_change_y() 
{
	GLfloat actual_change = y_position_change;
	y_position_change = 0.0f;

	return actual_change;
}

GL_Window::~GL_Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}