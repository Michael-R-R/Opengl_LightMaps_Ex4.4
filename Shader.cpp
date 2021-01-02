#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "Shader.h"
#include "stb_image.h"

using namespace std;


Shader::Shader()
{
	shader_program = glCreateProgram();

	success = 0;
	info_log[511] = { };

	VAO = 0;
	VBO = 0;

	texture = { };
}


void Shader::Create(const char* vertex_path, const char* fragment_path)
{
	// Retrieve shaders from file path
	string vertex_code;
	string fragment_code;

	ifstream v_shader_file;
	ifstream f_shader_file;

	// Make sure files can throw exceptions
	v_shader_file.exceptions(ifstream::failbit | ifstream::badbit);
	f_shader_file.exceptions(ifstream::failbit | ifstream::badbit);

	try
	{
		v_shader_file.open(vertex_path);
		f_shader_file.open(fragment_path);
		stringstream v_shader_stream, f_shader_stream;

		// Read the file's buffer contents into streams
		v_shader_stream << v_shader_file.rdbuf();
		f_shader_stream << f_shader_file.rdbuf();

		// Close file handlers
		v_shader_file.close();
		f_shader_file.close();

		// Convert stream to string
		vertex_code = v_shader_stream.str();
		fragment_code = f_shader_stream.str();
	}
	catch (ifstream::failure e)
	{
		cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
	}

	const char* v_shader_code = vertex_code.c_str();
	const char* f_shader_code = fragment_code.c_str();

	// --------------------------------------------------------------- //

	// Compiling the Vertex Shader
	unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &v_shader_code, NULL);
	glCompileShader(vertex_shader);
	// Check for vertex compile errors
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
		cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << endl;
	}

	// Compiling the Fragment Shader
	unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &f_shader_code, NULL);
	glCompileShader(fragment_shader);
	// Check for fragment compile errors
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
		cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << endl;
	}

	// Linking the shaders
    shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);
	// Check compiling erros for the Program
	if (!success)
	{
		glGetProgramInfoLog(shader_program, 512, NULL, info_log);
		cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << endl;
	}

	// Delete shaders
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}


void Shader::Config(vector<float> vertices)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 *sizeof(float)));
	glEnableVertexAttribArray(2);
}

void Shader::Load_Texture(string texture1_path, unsigned int texture_ID)
{
	// Find out the type of the uploaded texture
	string tex_type;
	double found = texture1_path.find_first_of(".");
	for (int i = 0; i < 4; i++)
	{
		tex_type += texture1_path[found + i];
	}

	// Load and create texture
	glGenTextures(1, &texture_ID);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load image, create textures, create mipmaps
	int width, height, nr_channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(texture1_path.c_str(), &width, &height, &nr_channels, 0);
	if (data)
	{
		if (tex_type == ".png")
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else if(tex_type == ".jpg")
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			cerr << "Unknown image type" << endl;
		}
		
	}
	else
	{
		cerr << "Failed to load texture" << endl;
	}
	stbi_image_free(data);

	texture.push_back(texture_ID);
}

void Shader::Use()
{
	glUseProgram(shader_program);
}

void Shader::Unbind()
{
	// Safely unbind VAO, VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Shader::Clean_Up()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shader_program);
}

// Uniform Utilities
void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(shader_program, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(shader_program, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(shader_program, name.c_str()), value);
}

void Shader::setVec3(const std::string& name, const glm::vec3 value) const
{
	glUniform3fv(glGetUniformLocation(shader_program, name.c_str()), 1, &value[0]);
}

void Shader::setMatrix4fv(const std::string& name, glm::mat4 transform) const
{
	glUniformMatrix4fv(glGetUniformLocation(shader_program, name.c_str()),
											1, GL_FALSE, glm::value_ptr(transform));
}
