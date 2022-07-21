#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "Shader.h"

#include <iostream>

//callback function that gets called whenever window is resized
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow* window);

//vertex shader 
const char *vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	
	"out vec4 vertexColor;"
	
	"void main()\n"
	"{\n"
		"gl_Position = vec4(aPos, 1.0); \n"
		"vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
	"}\0";

//second vertex shader
const char *vertexShaderSource2 = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"	//the position variable has attr pos 0
	"layout (location = 1) in vec3 aColor;\n"	//the color variable has attr pos 1
	
	"out vec3 ourColor;\n"					//output a color to the fragment shader

	"void main(){\n"
		"gl_Position = vec4(aPos, 1.0);\n"
		"ourColor = aColor;\n" //set ourColor to the input color we got from the vertex data
	"}\0";

//fragment shader 1 - orange
const char *fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";

//Hello Triangle - Exercise 3
//fragment shader 2 with dark grey color
const char *fragmentShaderSource2 = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(0.41f, 0.41f, 0.41f, 1.0f);\n"
	"}\0";

const char *fragmentShaderSource3 =
	"#version 330 core\n"
	
	"out vec4 FragColor;\n"
	"in vec3 ourColor;\n"

	"void main()\n"
	"{\n"
		"FragColor = vec4(ourColor, 1.0); \n"
	"}\0";


int main() {
	///setup

	//initalize GLFW lib, true if success
	glfwInit();


	//targeting opengl version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//extra preprocessor content for mac systems
	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	GLFWwindow* window = glfwCreateWindow(800, 600, "Shader Time!", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//make the created window the main context on the current thread
	glfwMakeContextCurrent(window);

	//init GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to init GLAD" << std::endl;
		return -1;
	}

	//set opengl size of rendering view window
	glViewport(0, 0, 800, 600);

	//set window resize callback func
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	///shader compiling
	Shader ourShader1("VertexShaderPosColor.vs", "FragmentShaderExternalColorFromVS.fs");

	///data 
	float fourthTriangle[] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
	};

	unsigned int VBOs[1], VAOs[1];
	glGenVertexArrays(1, VAOs);
	glGenBuffers(1, VBOs);

	glBindVertexArray(VAOs[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fourthTriangle), fourthTriangle, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//unbind the VAO that was in use
	glBindVertexArray(0);

	//add-on to draw in wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	///rendering loop
	while (!glfwWindowShouldClose(window)) {
		// input
		processInput(window);

		//rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //specifies color to clear buffer with
		glClear(GL_COLOR_BUFFER_BIT); //clears color buffer

		ourShader1.use();
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);


		//swaps the color buffer
		glfwSwapBuffers(window);
		//check for any events are triggered
		glfwPollEvents();
	}

	//deallocate resources once they've outlived their purposes

	glDeleteVertexArrays(1, VAOs);
	glDeleteBuffers(1, VBOs);
	ourShader1.deallocateShader();

	//function to remove remaining windows, release allocated resources, and sets the libary to an uninatlized state
	glfwTerminate();
	return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height){
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

//checks if the user pressed the esc key (if not, glfwGetKey returns GLFW_RELEASE
void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}