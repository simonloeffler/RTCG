#include "pch.h"
#include "Object_Material.h"

Object_Material::Object_Material()
{
	specular_intensity = 0;
	specular_roughness = 0;
}

Object_Material::Object_Material(GLfloat spec_int, GLfloat spec_rough)
{
	specular_intensity = spec_int;
	specular_roughness = spec_rough;
}

void Object_Material::use_object_material(GLint specular_intensity_location, GLint specular_roughness_location)
{
	glUniform1f(specular_intensity_location, specular_intensity);
	glUniform1f(specular_roughness_location, specular_roughness);

}

Object_Material::~Object_Material(){}