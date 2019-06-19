#pragma once
#include "pch.h"
#include <GL/glew.h>

class Object_Material
{
public:
	Object_Material();

	Object_Material(GLfloat spec_int, GLfloat spec_rough);

	void use_object_material(GLint specular_intensity_location, GLint specular_roughness_location);

	~Object_Material();

private:
	GLfloat specular_intensity, specular_roughness;
};