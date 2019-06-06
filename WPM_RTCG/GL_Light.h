#pragma once
#include <glm.hpp>
#include <GL/glew.h>

class GL_Light
{
public:
	GL_Light();

	GL_Light(GLfloat color_red, GLfloat color_green, GLfloat color_blue, GLfloat amb_light_int, 
		GLfloat diffuse_position_x, GLfloat diffuse_position_y, GLfloat diffuse_position_z, GLfloat diffuse_intensity);

	void use_light(GLuint ambient_intensity_location, GLuint ambient_color_location, 
		GLint diffuse_intensity_location, GLint general_light_direction);

	~GL_Light();

private:
	glm::vec3 global_light_color;
	GLfloat ambient_light_intensity;

	glm::vec3 direction_general_light;
	GLfloat diffuse_light_intensity;
};