#include "pch.h"
#include "Mesh_Object.h"

Mesh_Object::Mesh_Object()
{
	obj_vao = 0;
	obj_vbo = 0;
	obj_ibo = 0;

	indices_count = 0;
}

void Mesh_Object::Create_Object_Mesh(GLfloat *vertices, unsigned int *indices, unsigned int count_vertices, unsigned int count_indices)
{
	indices_count = count_indices;
	glGenVertexArrays(1, &obj_vao);

	glBindVertexArray(obj_vao);
	glGenBuffers(1, &obj_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj_ibo);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * count_indices, indices, GL_STATIC_DRAW);
	glGenBuffers(1, &obj_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, obj_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * count_vertices, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 6, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 6, (void*)(sizeof(vertices[0]) * 3));
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void Mesh_Object::Render_Object_Mesh()
{
	glBindVertexArray(obj_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj_ibo);

	glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void Mesh_Object::Clear_Object_Mesh()
{
	if (obj_ibo != 0)
	{
		glDeleteBuffers(1, &obj_ibo);
		obj_ibo = 0;
	}

	if (obj_vbo != 0)
	{
		glDeleteBuffers(1, &obj_vbo);
		obj_vbo = 0;
	}

	if (obj_vao != 0)
	{
		glDeleteVertexArrays(1, &obj_vao);
		obj_vao = 0;
	}

	indices_count = 0;
}

Mesh_Object::~Mesh_Object()
{
	Clear_Object_Mesh();
}

