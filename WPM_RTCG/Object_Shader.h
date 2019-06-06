#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <GL/glew.h>
class Object_Shader
{
	public:

		Object_Shader();

		void Create_From_String(const char* vertex_Code, const char* fragment_Code);
		void Create_From_File(const char* location_vertex_shader, const char* location_fragment_shader);

		GLuint Get_Projection_Location();
		GLuint Get_Model_Location();
		GLuint Get_View_Location();
		GLuint Get_Ambient_Color_Location();
		GLuint Get_Ambient_Intensity_Location();
		GLuint Get_Diffuse_Intensity_Location();
		GLuint Get_Light_Direction_Location();

		void Use_Object_Shader();

		void Clear_Object_Shader();

		~Object_Shader();

	private:

		GLuint obj_shader_ID, uniform_model, uniform_projection, uniform_view, uniform_ambient_intensity, uniform_ambient_color,
			uniform_diffuse_intensity, uniform_light_direction;
		void compileObjectShader(const char* vertex_Code, const char* fragment_Code);
		void addingObjectShaders(GLuint shader_program, const char* shader_Code, GLenum shader_type);
		std::string Read_File(const char* file_location);

};