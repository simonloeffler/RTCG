#pragma once

#include<GL/glew.h>
#include<GLFW/glfw3.h>

class GL_Window
{
	public:

		GL_Window();
		GL_Window(GLint window_width, GLint window_height);

		int initialise_window();

		GLint getbufferwidth() { return buffer_width; }
		GLint getbufferheight() { return buffer_height; }

		bool getshouldclose() { return glfwWindowShouldClose(mainWindow); }

		void swapbuffers() { glfwSwapBuffers(mainWindow); }

		bool* get_keys() { return keys; };
		GLfloat get_change_x();
		GLfloat get_change_y();


		~GL_Window();

	private:

		GLFWwindow *mainWindow;

		GLint width, height;
		GLint buffer_width, buffer_height;

		bool keys[1024];

		GLfloat last_x_position, last_y_position, x_position_change, y_position_change;
		bool mouse_initial_movement;

		static void handle_keys(GLFWwindow* window, int key, int code, int action, int mode);
		static void handle_mouse(GLFWwindow* window, double new_position_x, double new_position_y);

		void create_callbacks();


};