#pragma once

#include "pch.h"
#include <iostream>
#include <string.h>

#include<GL/glew.h>


class Mesh_Object
{
	public:

		Mesh_Object();

		void Create_Object_Mesh(GLfloat *vertices, unsigned int *indices, unsigned int count_vertices, unsigned int count_indices);
		void Render_Object_Mesh();
		void Clear_Object_Mesh();

		~Mesh_Object();

	private:

		GLuint obj_vao, obj_vbo, obj_ibo;
		GLsizei indices_count;

};