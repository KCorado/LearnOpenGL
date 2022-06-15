#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <iostream>

//callback function that gets called whenever window is resized
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow* window);

//vertex shader 
const char *vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

//fragment shader 1 - orange
const char *fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n";

//Hello Triangle - Exercise 3
//fragment shader 2 with dark grey color
const char *fragmentShaderSource2 = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(0.41f, 0.41f, 0.41f, 1.0f);\n"
	"}\n";


int main() {
	// this functions inits the GLFW lib; returns GL_TRUE if successful, otherwise GL_FALSE
	glfwInit();

	// sets target opengl versions to 3, and specifically core-profile
	// preventing access to pre-version opengl 3.2 backwards compatibility
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// this windowHint is necessary for mac osx systems to initialize
	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL){
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//make the created window the main context on the current thread
	glfwMakeContextCurrent(window);

	//initalize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initalize GLAD" << std::endl;
		return -1;
	}
	
	//tell opengl the size of the rendering view window
	glViewport(0, 0, 800, 600);

	//calling callback function
	glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

	///setting up shader 1
	//setting up the vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//checking for compile time errors with vertex shader
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//setting up the fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//checking for compile time errors with fragment shader
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//compiling the full shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//check if the compilation of the shader program failed
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	///setting up shader 2
	//note: vertex shader unchanged, use with shader two before destruction

	//set up of fragment shader
	unsigned int fragmentShader2;
	fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
	glCompileShader(fragmentShader2);

	//checking for compile time errors with fragment shader
	glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT2::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//compiling second shader program with new fragment shader
	unsigned int shaderProgram2;
	shaderProgram2 = glCreateProgram();
	glAttachShader(shaderProgram2, vertexShader);
	glAttachShader(shaderProgram2, fragmentShader2);
	glLinkProgram(shaderProgram2);

	//checking if compilation of second shader program failed
	glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::SHADER2::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//since the shader is activated, make sure to delete the other shader objects used
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(fragmentShader2);


	///Hello Triangle - Exercise 2	
	//vertex arrays for two triangles
	float firstTriangle[] = {
		-0.6f, 0.0f, 0.0f,
		-0.1f, 0.5f, 0.0f,
		-0.1f, -0.5f, 0.0f,	
	};

	float secondTriangle[] = {
		0.1f, -0.5f, 0.0f,
		0.6f, 0.0f, 0.0f,
		0.1f, 0.5f, 0.0f
	};

	unsigned int VBOs[2], VAOs[2];
	glGenVertexArrays(2, VAOs); //generating multiple VAO or VBO(buffers) at the same time
	glGenBuffers(2, VBOs);

	//first triangle, with binding VAO #1
	glBindVertexArray(VAOs[0]);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]); //setup of buffer (VBO)
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //setup of how vertex is defined
	glEnableVertexAttribArray(0);
	//glBindVertexArray(0); no need to unbind at all as we directly bind a different VAO in the next few lines

	//second triangle setup, directly binding VAO #2 (because we bind directly, no need to unbind explicitly)
	glBindVertexArray(VAOs[1]);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//glBindVertexArray(0); //not necessary as well, BUT be aware of calls that could affect VAOs while this one is bound
	//ex/ like binding element buffer objects, or enabling/disabling vertex attributes
	//will unbinded for the sake of it
	glBindVertexArray(0);

	///add-on to draw in wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	//this is the render loop, this will keep running until we tell GLFW to stop
	//	-- glfwWindowShouldClose is a check to see if GLFW has been instructed to close, returns true if so
	while (!glfwWindowShouldClose(window)) {
		// input
		processInput(window);

		//rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //specifies color to clear buffer with
		glClear(GL_COLOR_BUFFER_BIT); //clears color buffer

		///Drawing code (in render loop) ref:Hello Triangle
		//using first program with first fragment shader
		glUseProgram(shaderProgram);

		///Hello Triangle - EXERCISE 2
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//using second shader program for second triangle
		glUseProgram(shaderProgram2);

		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		///

		//swaps the color buffer
		glfwSwapBuffers(window);
		//check for any events are triggered
		glfwPollEvents();
	}

	//deallocate resources once theyve outlived their purpose
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	glDeleteProgram(shaderProgram);
	glDeleteProgram(shaderProgram2);

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