#include "pch.h"
#include "GL_Light.h"

GL_Light::GL_Light()
{
	global_light_color = glm::vec3(1.0f, 1.0f, 1.0f);
	ambient_light_intensity = 1.0f;

	diffuse_light_intensity = 0.5f;
	direction_general_light = glm::vec3(0.0f, -1.0f, 0.0f);
}

GL_Light::GL_Light(GLfloat color_red, GLfloat color_green, GLfloat color_blue, GLfloat amb_light_int, 
	GLfloat diffuse_position_x, GLfloat diffuse_position_y, GLfloat diffuse_position_z, GLfloat diffuse_intensity)
{
	global_light_color = glm::vec3(color_red, color_green, color_blue);
	ambient_light_intensity = amb_light_int;

	direction_general_light = glm::vec3(diffuse_position_x, diffuse_position_y, diffuse_position_z);
	diffuse_light_intensity = diffuse_intensity;
}

void GL_Light::use_light(GLuint ambient_intensity_location, GLuint ambient_color_location, 
	GLint diffuse_intensity_location, GLint general_light_direction_location)
{
	glUniform3f(ambient_color_location, global_light_color.x, global_light_color.y, global_light_color.z);
	glUniform1f(ambient_intensity_location, ambient_light_intensity);

	glUniform3f(general_light_direction_location, direction_general_light.x, direction_general_light.y, direction_general_light.z);
	glUniform1f(diffuse_intensity_location, diffuse_light_intensity);
}

GL_Light::~GL_Light()
{
}