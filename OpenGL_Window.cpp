#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include "Shader.h"
#include "Camera.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);
void Process_Input(GLFWwindow* window);

// Screen
int scr_width = 1200, scr_height = 800;

// Used for changing values for testing purposes
float change_value = 0.1f;

// Camera object
Camera camera;

int main()
{
	// Initalize glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create Window
	GLFWwindow* window = glfwCreateWindow(scr_width, scr_height, "Learn OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // enable vsync
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Tell glfw to capture the mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initalize GLAD" << std::endl;
		return -1;
	}

	// Configure global OpenGL state
	glEnable(GL_DEPTH_TEST);

	vector<float> vertices = 
	{
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	// Create Shader program, config vertices
	Shader cube_shader;
	cube_shader.Create("vShader.vs", "fShader.fs");
	cube_shader.Config(vertices);
	cube_shader.Load_Texture("container2.png", 0); // diffuse
	cube_shader.Load_Texture("edges.png", 1); // specular
	cube_shader.Load_Texture("matrix.jpg", 2); // emission
	cube_shader.Unbind();

	// Set texture int
	cube_shader.Use();
	cube_shader.setInt("material.diffuse", 0);
	cube_shader.setInt("material.specular", 1);
	cube_shader.setInt("material.emission", 2);

	// Create Shader program, config vertices
	Shader light_shader;
	light_shader.Create("light_cube.vs", "light_cube.fs");
	light_shader.Config(vertices);
	light_shader.Unbind();


	// Render Loop
	while (!glfwWindowShouldClose(window))
	{
		// per frame time logic
		camera.Calculate_DeltaTime();

		Process_Input(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Light Cube Position
		float cos_val = cos(glfwGetTime()); // use to rotate light
		float sin_val = sin(glfwGetTime()); // use to rotate light
		glm::vec3 light_pos(1.2f, 1.0f, 2.0f); // 1.2f, 1.0f, 2.0f = old light pos values


		// ------ Start Drawing ------

		// 1 - Texture config
		glActiveTexture(GL_TEXTURE0); // diffse
		glBindTexture(GL_TEXTURE_2D, cube_shader.texture[0]);
		glActiveTexture(GL_TEXTURE1); // specular
		glBindTexture(GL_TEXTURE_2D, cube_shader.texture[1]);
		glActiveTexture(GL_TEXTURE2); // emission
		glBindTexture(GL_TEXTURE_2D, cube_shader.texture[2]);

		// 1 - Normal Cube
		cube_shader.Use();
		// 1 - Light and view positions
		cube_shader.setVec3("light.position", light_pos);
		cube_shader.setVec3("view_position", camera.position);

		// 1 - Light properties
		cube_shader.setVec3("light.ambient", glm::vec3(0.05f)); // light intensity
		cube_shader.setVec3("light.diffuse", glm::vec3(0.5f)); // light intensity
		cube_shader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

		// 1 - Material properties
		cube_shader.setFloat("material.shininess", 32.0f);

		// 1 - M/V/P properties
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glm::mat4 view = glm::lookAt(camera.position, camera.position + camera.front, camera.up);
		glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)scr_width / (float)scr_height, 0.1f, 100.0f);
		
		// 1 - Set the M/V/P uniforms
		cube_shader.setMatrix4fv("model", model); // Set the model matrix uniform
		cube_shader.setMatrix4fv("view", view); // Set the view matrix unifom
		cube_shader.setMatrix4fv("projection", projection); // Set the projection matrix uniform

		// 1 - Draw
		glBindVertexArray(cube_shader.VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// 2 - Light Source Cube
		light_shader.Use();
		model = glm::mat4(1.0f); // Reset model to identity matrix
		model = glm::translate(model, light_pos);
		model = glm::scale(model, glm::vec3(0.2f));

		// 2 - Set the uniforms
		light_shader.setMatrix4fv("model", model); // Set the model matrix uniform
		light_shader.setMatrix4fv("view", view); // Set the view matrix unifom
		light_shader.setMatrix4fv("projection", projection); // Set the projection matrix uniform

		// 2 - Draw
		glBindVertexArray(light_shader.VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// ------ End Drawing ------

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Clean up VAO, VBO, Program
	cube_shader.Clean_Up();

	// Clean up
	glfwTerminate();
	return 0;
}

// Keeps window formatted for when user resizes the application
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	// Checks to see if this is the first time
	// the window is capturing the mouse
	// eliminates the sudden screen jump
	if (camera.first_mouse_enter)
	{
		camera.last_x = xpos;
		camera.last_y = ypos;
		camera.first_mouse_enter = false;
	}

	float x_offset = xpos - camera.last_x;
	float y_offset = camera.last_y - ypos;
	camera.last_x = xpos;
	camera.last_y = ypos;

	x_offset *= camera.sensitivity;
	y_offset *= camera.sensitivity;

	camera.yaw += x_offset;
	camera.pitch += y_offset;

	// Clamps looking up and down to 89 degrees
	if (camera.pitch > 89.0f) { camera.pitch = 89.0f; }
	if (camera.pitch < -89.0f) { camera.pitch = -89.0f; }

	// Calculate a new camera front everytime the user
	// rotates the screen with the mouse
	glm::vec3 front;
	front.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
	front.y = sin(glm::radians(camera.pitch));
	front.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
	camera.front = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
{
	camera.fov -= (float)y_offset;
	if (camera.fov < 1.0f) { camera.fov = 1.0f; }
	if (camera.fov > 45.0f) { camera.fov = 45.0f; }
}

// Handles keyboard input
void Process_Input(GLFWwindow* window)
{
	// Closes window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	// Key to increase and decrese value for 
	// testing purposes
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		change_value += 0.01f;
		cout << change_value << endl;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		change_value -= 0.01f;
		cout << change_value << endl;
	}

	// Move camera
	camera.Camera_Input(window);
}