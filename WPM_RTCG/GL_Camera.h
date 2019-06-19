#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>

class GL_Camera
{
public:

	GL_Camera();

	GL_Camera(glm::vec3 initial_camera_position, glm::vec3 initial_up_direction, GLfloat initial_yaw, GLfloat initial_pitch, GLfloat initial_speed_movement, GLfloat initial_speed_turn);

	void control_structure_keys(bool* keys, GLfloat delta_time);
	void control_stucture_mouse(GLfloat change_x_position, GLfloat change_y_position);

	glm::vec3 get_camera_position();
	glm::mat4 view_matrix_calculation();

	~GL_Camera();

private:

	glm::vec3 camera_position, front_direction, right_direction, up_direction, world_up_direction;
	GLfloat yaw, pitch, speed_movement, speed_turn;

	void update();
};