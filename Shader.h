#pragma once
#include <vector>

class Shader
{
public:
	Shader();

	// Variables
	unsigned int shader_program;

	int success;
	char info_log[512];

	std::vector<float> vertices;
	unsigned int VAO, VBO;

	// Store all texture ID's in a vector in order to access them
	// during the rendering of them
	std::vector<unsigned int> texture;

	// Fucntions
	void Create(const char* vertex_path, const char* fragment_path);
	void Config(std::vector<float> vertices);
	void Load_Texture(std::string texture1_path, unsigned int texture_ID);
	void Use();
	void Unbind();
	void Clean_Up();

	// Utility Uniform
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec3(const std::string& name, glm::vec3 value) const;
	void setMatrix4fv(const std::string& name, glm::mat4 transform) const;

private:
	

};

