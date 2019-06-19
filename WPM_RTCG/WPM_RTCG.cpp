
#include "pch.h"
#include <iostream>
#include <string.h>

#include <cmath>
#include <vector>

#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Mesh_Object.h"
#include "Object_Shader.h"
#include "GL_Window.h"
#include "GL_Camera.h"
#include "GL_Light.h"
#include "Object_Material.h"

#define GLM_FORCE_CTOR_INIT

const GLint WINDOW_WIDTH = 1024, WINDOW_HEIGHT = 768;
static const char* vertex_shader = "Shader/Shader.vertex";

static const char* fragment_shader = "Shader/Shader.fragment";

GLuint uniform_model, uniform_projection;

bool direction = true;
float obj_offset = 0;
float obj_maximum_offset = 0.5f;
float obj_increment = 0.005f;

float actual_angle = 0.0f;

float actual_size = 0.5f;
float maximum_size = 0.9f;
float minimum_size = 0.2f;

GLfloat delta_time = 0.0f;
GLfloat last_time_step = 0.0f;

std::vector<Mesh_Object*> list_objects;
std::vector<Object_Shader> list_shaders;

GL_Window mainwindow;
GL_Camera maincamera;

GL_Light base_light;

Object_Material shiny_material;
Object_Material rough_material;

void average_object_normals(unsigned int *indices, unsigned int count_indices, GLfloat *vertices, unsigned int count_vertices,
	unsigned int lenght_vertices, unsigned int offset_normals)
{
	for (size_t i = 0; i < count_indices; i += 3)
	{
		unsigned int idx0 = indices[i] * lenght_vertices;
		unsigned int idx1 = indices[i + 1] * lenght_vertices;
		unsigned int idx2 = indices[i + 2] * lenght_vertices;

		glm::vec3 edge_one(vertices[idx1] - vertices[idx0], vertices[idx1 + 1] - vertices[idx0 + 1], vertices[idx1 + 2] -
			vertices[idx0 + 2]);
		glm::vec3 edge_two(vertices[idx2] - vertices[idx0], vertices[idx2 + 1] - vertices[idx0 + 1], vertices[idx2 + 2] -
			vertices[idx0 + 2]);

		glm::vec3 normals = glm::cross(edge_one, edge_two);
		normals = glm::normalize(normals);

		idx0 += offset_normals;
		idx1 += offset_normals;
		idx2 += offset_normals;

		vertices[idx0] += normals.x;
		vertices[idx0 + 1] += normals.y;
		vertices[idx0 + 2] += normals.z;

		vertices[idx1] += normals.x;
		vertices[idx1 + 1] += normals.y;
		vertices[idx1 + 2] += normals.z;

		vertices[idx2] += normals.x;
		vertices[idx2 + 1] += normals.y;
		vertices[idx2 + 2] += normals.z;
	}
	for (size_t i = 0; i < count_vertices / lenght_vertices; i++)
	{
		unsigned int n_offset_ind = i * lenght_vertices + offset_normals;

		glm::vec3 vector_ind(vertices[n_offset_ind], vertices[n_offset_ind + 1], vertices[n_offset_ind + 2]);
		vector_ind = glm::normalize(vector_ind);

		vertices[n_offset_ind] = vector_ind.x;
		vertices[n_offset_ind + 1] = vector_ind.y;
		vertices[n_offset_ind + 2] = vector_ind.z;
	}
}

void createObjects()
{
	GLfloat vertices[]{
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f
	};

	unsigned int indices[]{
		0,3,1,
		1,3,2,
		2,3,0,
		0,1,2
	};

	average_object_normals(indices, 12, vertices, 24, 6, 3);

	Mesh_Object *new_Object = new Mesh_Object();
	new_Object->Create_Object_Mesh(vertices, indices, 24, 12);
	list_objects.push_back(new_Object);

	//hier würde 2.Objekt erstellt werden (aus gleichen vertices und indices oder aus anderen)
}

void Create_Object_Shaders() 
{
	Object_Shader *pyramide_shader = new Object_Shader();
	pyramide_shader->Create_From_File(vertex_shader, fragment_shader);
	list_shaders.push_back(*pyramide_shader);
}

int main()
{
	mainwindow = GL_Window(1024, 768);
	mainwindow.initialise_window();

	createObjects();
	Create_Object_Shaders();

	maincamera = GL_Camera(glm::vec3(0.0f, 0.0f, -7.0f), glm::vec3(0.0f, 1.0f, 0.0f), 75.0f, 0.0f, 0.1f, 0.1f);

	GLuint uniform_projection = 0;
	GLuint uniform_model = 0;
	GLuint uniform_view = 0;

	shiny_material = Object_Material(1.0f, 64);
	rough_material = Object_Material(0.25f, 2);

	base_light = GL_Light(1.0f, 1.0f, 1.0f, 0.5f, 0.0f, -3.0f, 0.0f, 1.0f);
	GLuint uniform_ambient_intensity = 0;
	GLuint uniform_ambient_color = 0;

	GLuint uniform_light_direction = 0;
	GLuint uniform_diffuse_intensity = 0;

	GLuint uniform_specular_intensity = 0;
	GLuint uniform_specular_roughness = 0;
	GLuint uniform_eye_position = 0;

	glm::mat4 projection = glm::perspective(45.0f,(GLfloat)mainwindow.getbufferwidth() /(GLfloat)mainwindow.getbufferheight(), 0.1f, 150.0f);

	while (!mainwindow.getshouldclose()) 
	{
		GLfloat time_step_now = glfwGetTime();
		delta_time = time_step_now - last_time_step;
		last_time_step = time_step_now;

		glfwPollEvents();

		maincamera.control_structure_keys(mainwindow.get_keys(), delta_time);
		maincamera.control_stucture_mouse(mainwindow.get_change_x(), mainwindow.get_change_y());

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

		list_shaders[0].Use_Object_Shader();
		uniform_projection = list_shaders[0].Get_Projection_Location();
		uniform_model = list_shaders[0].Get_Model_Location();
		uniform_view = list_shaders[0].Get_View_Location();

		uniform_ambient_color = list_shaders[0].Get_Ambient_Color_Location();
		uniform_ambient_intensity = list_shaders[0].Get_Ambient_Intensity_Location();

		uniform_light_direction = list_shaders[0].Get_Light_Direction_Location();
		uniform_diffuse_intensity = list_shaders[0].Get_Diffuse_Intensity_Location();

		uniform_eye_position = list_shaders[0].Get_Eye_Position_Location();
		uniform_specular_intensity = list_shaders[0].Get_Specular_Intensity_Location();
		uniform_specular_roughness = list_shaders[0].Get_Specular_Roughness_Location();

		base_light.use_light(uniform_ambient_intensity, uniform_ambient_color, uniform_diffuse_intensity, uniform_light_direction);

		glm::mat4 model(1);												//Identitätsmatrix

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
		glUniformMatrix4fv(uniform_view, 1, GL_FALSE, glm::value_ptr(maincamera.view_matrix_calculation()));

		shiny_material.use_object_material(uniform_specular_intensity, uniform_specular_roughness);
		list_objects[0]->Render_Object_Mesh();
		//list_objects[0]->Clear_Object_Mesh();

		glUseProgram(0);

		mainwindow.swapbuffers();
	}

	return 0;

}


