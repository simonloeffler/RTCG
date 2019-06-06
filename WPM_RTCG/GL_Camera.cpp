#include "pch.h"
#include "GL_Camera.h"

GL_Camera::GL_Camera(){}

GL_Camera::GL_Camera(glm::vec3 initial_camera_position, glm::vec3 initial_up_direction, GLfloat initial_yaw, GLfloat initial_pitch, GLfloat initial_speed_movement, GLfloat initial_speed_turn)
{
	camera_position = initial_camera_position;
	world_up_direction = initial_up_direction;
	yaw = initial_yaw;
	pitch = initial_pitch;
	front_direction = glm::vec3(0.0f, 0.0f, -1.0f);
	speed_movement = initial_speed_movement;
	speed_turn = initial_speed_turn;

	update();
}

void GL_Camera::update()
{
	front_direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front_direction.y = sin(glm::radians(pitch));
	front_direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front_direction = glm::normalize(front_direction);

	right_direction = glm::normalize(glm::cross(front_direction, world_up_direction));
	up_direction = glm::normalize(glm::cross(right_direction, front_direction));
}

void GL_Camera::control_structure_keys(bool* keys, GLfloat delta_time)
{
	GLfloat velocity = speed_movement * delta_time;

	if (keys[GLFW_KEY_W])
	{
		camera_position += front_direction * velocity;
	}

	if (keys[GLFW_KEY_A])
	{
		camera_position -= right_direction * velocity;
	}

	if (keys[GLFW_KEY_S])
	{
		camera_position -= front_direction * velocity;
	}

	if (keys[GLFW_KEY_D])
	{
		camera_position += right_direction * velocity;
	}
}

void GL_Camera::control_stucture_mouse(GLfloat change_x_position, GLfloat change_y_position)
{
	change_x_position *= speed_turn;
	change_y_position *= speed_turn;

	yaw += change_x_position;
	pitch += change_y_position;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	update();
}

glm::mat4 GL_Camera::view_matrix_calculation()
{
	return glm::lookAt(camera_position, camera_position + front_direction, up_direction);
}

GL_Camera::~GL_Camera(){}