#include "pch.h"

#include "Object_Shader.h"

Object_Shader::Object_Shader()
{
	obj_shader_ID = 0;
	uniform_model = 0;
	uniform_projection = 0;

	uniform_ambient_color = 0;
	uniform_ambient_intensity = 0;

	uniform_diffuse_intensity = 0;
	uniform_light_direction = 0;
}

void Object_Shader::Create_From_String(const char* vertex_Code, const char* fragment_Code) 
{
	compileObjectShader(vertex_Code, fragment_Code);
}

std::string Object_Shader::Read_File(const char* file_location)
{
	std::string shader_file_content;
	std::ifstream file_stream(file_location, std::ios::in);

	if (!file_stream.is_open())
	{
		printf("Fehler beim Lesen der Datei. Datei nicht vorhanden!", file_location);
		return "";
	}

	std::string content_line = "";

	while (!file_stream.eof()) {

		std::getline(file_stream, content_line);
		shader_file_content.append(content_line + "\n");

	}

	file_stream.close();
	return shader_file_content;
}

void Object_Shader::compileObjectShader(const char* vertex_Code, const char* fragment_Code)
{
	obj_shader_ID = glCreateProgram();

	if (!obj_shader_ID)
	{
		printf("Erstellung des Shader-Programms fehlgeschlagen!");
		return;
	}

	addingObjectShaders(obj_shader_ID, vertex_Code, GL_VERTEX_SHADER);
	addingObjectShaders(obj_shader_ID, fragment_Code, GL_FRAGMENT_SHADER);

	GLint test_result = 0;
	GLchar error_log[1024] = { 0 };

	glLinkProgram(obj_shader_ID);
	glGetProgramiv(obj_shader_ID, GL_LINK_STATUS, &test_result);
	if (!test_result)
	{
		glGetProgramInfoLog(obj_shader_ID, sizeof(error_log), NULL, error_log);
		printf("Fehler beim Linking des Shader Programms: '%s'\n", error_log);
		return;
	}

	glValidateProgram(obj_shader_ID);
	glGetProgramiv(obj_shader_ID, GL_VALIDATE_STATUS, &test_result);
	if (!test_result)
	{
		glGetProgramInfoLog(obj_shader_ID, sizeof(error_log), NULL, error_log);
		printf("Fehler bei der Validierung des Shader Programms: '%s'\n", error_log);
		return;
	}

	uniform_model = glGetUniformLocation(obj_shader_ID, "model");
	uniform_projection = glGetUniformLocation(obj_shader_ID, "projection");
	uniform_view = glGetUniformLocation(obj_shader_ID, "view");

	uniform_ambient_color = glGetUniformLocation(obj_shader_ID, "directional_light.light_color");
	uniform_ambient_intensity = glGetUniformLocation(obj_shader_ID, "directional_light.ambient_light_intensity");

	uniform_diffuse_intensity = glGetUniformLocation(obj_shader_ID, "directional_light.diffuse_light_intensity");
	uniform_light_direction = glGetUniformLocation(obj_shader_ID, "directional_light.light_direction");

}

void Object_Shader::Create_From_File(const char* location_vertex_shader, const char* location_fragment_shader)
{
	std::string string_vertex_shader = Read_File(location_vertex_shader);
	std::string string_fragment_shader = Read_File(location_fragment_shader);

	const char* vertex_Code = string_vertex_shader.c_str();
	const char* fragment_Code = string_fragment_shader.c_str();

	compileObjectShader(vertex_Code, fragment_Code);
}

void Object_Shader::addingObjectShaders(GLuint shader_program, const char* shader_Code, GLenum shader_type) {
	GLuint a_Shader = glCreateShader(shader_type);

	const GLchar* a_Code[1];
	a_Code[0] = shader_Code;

	GLint code_length[1];
	code_length[0] = strlen(shader_Code);

	glShaderSource(a_Shader, 1, a_Code, code_length);
	glCompileShader(a_Shader);

	GLint test_result = 0;
	GLchar error_log[1024] = { 0 };

	glGetShaderiv(a_Shader, GL_COMPILE_STATUS, &test_result);
	if (!test_result)
	{
		glGetShaderInfoLog(a_Shader, sizeof(error_log), NULL, error_log);
		printf("Fehler bei der Kompilierung des '%d' Shaders: '%s'\n", shader_type, error_log);
		return;
	}

	glAttachShader(shader_program, a_Shader);
}

GLuint Object_Shader::Get_Projection_Location() 
{
	return uniform_projection;
}

GLuint Object_Shader::Get_Model_Location() 
{
	return uniform_model;
}

GLuint Object_Shader::Get_View_Location()
{
	return uniform_view;
}

void Object_Shader::Use_Object_Shader() 
{
	glUseProgram(obj_shader_ID);
}

void Object_Shader::Clear_Object_Shader() 
{
	if (obj_shader_ID != 0) 
	{
		glDeleteProgram(obj_shader_ID);
		obj_shader_ID = 0;
	}
	uniform_model = 0;
	uniform_projection = 0;
}

GLuint Object_Shader::Get_Ambient_Color_Location() 
{
	return uniform_ambient_color;
}GLuint Object_Shader::Get_Ambient_Intensity_Location() 
{
	return uniform_ambient_intensity;
}GLuint Object_Shader::Get_Diffuse_Intensity_Location() 
{
	return uniform_diffuse_intensity;
}
GLuint Object_Shader::Get_Light_Direction_Location() 
{
	return uniform_light_direction;
}
Object_Shader::~Object_Shader()
{
	Clear_Object_Shader();
}