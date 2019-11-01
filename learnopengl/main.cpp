#include <glad.h>
#include<glfw3.h>
#include <iostream>

#include "Shader.h"

// function prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
	// instantiate the GLFW window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // tell GLFW that 3.3 is the OpenGL version we want to use
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // tell GLFW we want to explicitly use the core-profile

	// create a window object
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); //? tell GLFW to make the context of the window. What is context?

	//? initailize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader program
	//------------------------------------
	Shader ourShader("shader.vs", "shader.sf");

	// set up vertex data (and buffer(s)) and configure vertex attribute
	// -----------------------------------------------------------------
	float vertices[] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
	};

	unsigned int VBO, VAO; // Vertex Buffer Object & Vertex Array Object & Element buffer object reference id
	glGenVertexArrays(1, &VAO); // Generate a VAO
	glGenBuffers(1, &VBO); // Generate a VBO

	//Bind the vertex data first,then bind and set vertex and element buffer(s), and then configure vertex and eleement attributes(s)
	glBindVertexArray(VAO); // bind the VAO

	glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind the vertex buffer

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // copy our vertices array in a vertex buffer for OpenGL to use

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // tell OpenGL how it should interpret the vertex data (per vertex attribute)
	glEnableVertexAttribArray(0); // enable the vertex attribute
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	ourShader.use();

	// set dimensions
	glViewport(0, 0, 800, 600);

	// When the user resize the window...
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// implement a render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw code in render loop
		// --------------------------
		// render traingle
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	//? clean all of GLFW's resources that were allocated. What is a resource?
	// -------------------------------------------------------------
	glfwTerminate();
	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	// if user press the escape button...
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		// then close the window
		glfwSetWindowShouldClose(window, true);
}


/*遇到的坑*/
// 1.unresolved external: glfw3...
	// glfw3.lib是x64格式的，与target machine的x86格式冲突了
		// 什么是target machine?

/*疑惑*/
// 1.How to link vertex buffer to vertex shader?
// 2.How maintain the buffer? CPU or GPU?
// 3.What is vertex attribute?
	// This vertex's data is represented using vertex attributes that can contain any data we'd like
// 4.VAO? VBO?
// 5.buffer? GPU memory?
// 6.为什么能够unbind VAO却不能unbind VBO?
	// http://www.photoneray.com/opengl-vao-vbo/
// 7.顶点数据怎么传到shader里去？