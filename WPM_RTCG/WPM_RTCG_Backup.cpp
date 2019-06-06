
#include "pch.h"
#include <iostream>
#include <string.h>

#include <cmath>

#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

const GLint WINDOW_WIDTH = 1024, WINDOW_HEIGHT = 768;
static const char* vertex_shader = "																\n\
																									\n\
#version 330																						\n\
																									\n\
layout(location = 0) in vec3 position;																\n\
uniform mat4 model;																					\n\
uniform mat4 projection;																			\n\
out vec4 vertex_colors;																				\n\
																									\n\
void main(){																						\n\
	gl_Position = projection * model * vec4(0.5 * position, 1.0);									\n\
	vertex_colors = vec4(clamp(position, 0.0f, 1.0f), 1.0f);										\n\
}						 																			\n\
";

static const char* fragment_shader = "		\n\
											\n\
#version 330								\n\
											\n\
in vec4 vertex_colors;						\n\
out vec4 colors;							\n\
											\n\
void main(){								\n\
colors = vertex_colors;						\n\
}											\n\
";

GLuint obj_vao, obj_vbo, obj_ibo, obj_shader, uniform_model, uniform_projection;

bool direction = true;
float obj_offset = 0;
float obj_maximum_offset = 0.5f;
float obj_increment = 0.005f;

float actual_angle = 0.0f;

float actual_size = 0.5f;
float maximum_size = 0.9f;
float minimum_size = 0.2f;


void createObjects()
{
	GLfloat vertices[]{
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	unsigned int indices[]{
		0,3,1,
		1,3,2,
		2,3,0,
		0,1,2
	};

	glGenVertexArrays(1, &obj_vao);
	glBindVertexArray(obj_vao);

	//binding ibo
	glGenBuffers(1, &obj_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glGenBuffers(1, &obj_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, obj_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//unbinding ibo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void addingObjectShaders(GLuint shader_program, const char* shader_Code, GLenum shader_type)
{
	GLuint a_Shader = glCreateShader(shader_type);

	const GLchar* a_Code[1];
	a_Code[0] = shader_Code;

	GLint code_length[1];
	code_length[0] = strlen(shader_Code);

	glShaderSource(a_Shader, 1, a_Code, code_length);
	glCompileShader(a_Shader);

	GLint test_result = 0;
	GLchar error_log[1024] = { 0 };

	glGetShaderiv(a_Shader, GL_COMPILE_STATUS, &test_result);
	if (!test_result) 
	{
		glGetShaderInfoLog(a_Shader, sizeof(error_log), NULL, error_log);
		printf("Fehler bei der Kompilierung des '%d' Shaders: '%s'\n", shader_type, error_log);
		return;
	}

	glAttachShader(shader_program, a_Shader);

}

void compileObjectShaders()
{
	obj_shader = glCreateProgram();

	if (!obj_shader) 
	{
		printf("Erstellung des Shader-Programms fehlgeschlagen!");
		return;
	}

	addingObjectShaders(obj_shader, vertex_shader, GL_VERTEX_SHADER);
	addingObjectShaders(obj_shader, fragment_shader, GL_FRAGMENT_SHADER);

	GLint test_result = 0;
	GLchar error_log[1024] = { 0 };

	glLinkProgram(obj_shader);
	glGetProgramiv(obj_shader, GL_LINK_STATUS, &test_result);
	if (!test_result) 
	{
		glGetProgramInfoLog(obj_shader, sizeof(error_log), NULL, error_log);
		printf("Fehler beim Linking des Shader Programms: '%s'\n", error_log);
		return;
	}

	glValidateProgram(obj_shader);
	glGetProgramiv(obj_shader, GL_VALIDATE_STATUS, &test_result);
	if (!test_result) 
	{
		glGetProgramInfoLog(obj_shader, sizeof(error_log), NULL, error_log);
		printf("Fehler bei der Validierung des Shader Programms: '%s'\n", error_log);
		return;
	}

	uniform_model = glGetUniformLocation(obj_shader, "model");
	uniform_projection = glGetUniformLocation(obj_shader, "projection");

}

int main()
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

	GLFWwindow *main_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hauptfenster", NULL, NULL);
	if (!main_window) 
	{
		printf("Erstellen des Hauptfensters fehlgeschlagen!");
		glfwTerminate();
		return 1;
	}

	GLint buffer_width, buffer_length;
	glfwGetFramebufferSize(main_window, &buffer_width, &buffer_length);
	glfwMakeContextCurrent(main_window);
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) 
	{
		printf("GLEW Initialisierung fehgeschlagen!");
		glfwDestroyWindow(main_window);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, buffer_width, buffer_length);

	createObjects();
	compileObjectShaders();

	glm::mat4 projection = glm::perspective(50.0f, (GLfloat)buffer_width / (GLfloat)buffer_length, 0.1f, 250.0f);

	while (!glfwWindowShouldClose(main_window)) 
	{
		glfwPollEvents();
		if (direction) 
		{
			obj_offset += obj_increment;
		}
		else 
		{
			obj_offset -= obj_increment;
		}
		if (abs(obj_offset) >= obj_maximum_offset) 
		{
			direction = !direction;
		}

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				//Framebuffer
		glUseProgram(obj_shader);

		glm::mat4 model();

		//Transaltion
		model = glm::translate(model, glm::vec3(obj_offset, 0.0f, -5.0f));
		//Rotation
		model = glm::rotate(model, glm::radians(actual_angle), glm::vec3(0.0f, 1.0f, 1.0f));
		actual_angle += 0.5f;
		if (actual_angle >= 360) 
		{
			actual_angle -= 360;
		}

		//Scaling
		if (direction)
		{
			actual_size += 0.002f;
		}
		else
		{
			actual_size -= 0.002f;
		}
		model = glm::scale(model, glm::vec3(actual_size, actual_size, 1.0f));
		
		glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(projection));
		
		glBindVertexArray(obj_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj_ibo);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glUseProgram(0);

		glfwSwapBuffers(main_window);
	}

	return 0;

}


