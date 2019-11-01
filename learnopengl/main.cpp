#include <glad.h>
#include<glfw3.h>
#include <iostream>
#include <stb_image.h>

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
		// positions         // colors        // texture coords
		 0.5f, 0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // bottom right
		 -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // bottom left
		 -0.5f, 0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f // top left
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3 // second triangle
	};

	unsigned int VBO, VAO, EBO; // Vertex Buffer Object & Vertex Array Object & Element buffer object reference id
	glGenVertexArrays(1, &VAO); // Generate a VAO
	glGenBuffers(1, &VBO); // Generate a VBO
	glGenBuffers(1, &EBO); // Generate a EBO

	glBindVertexArray(VAO); // bind the VAO

	glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind the vertex buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // copy our vertices array in a vertex buffer for OpenGL to use

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // bind the element buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // sizeof(indices)?

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // tell OpenGL how it should interpret the vertex data (per vertex attribute)
	glEnableVertexAttribArray(0); // enable the vertex attribute
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// load and create a texture
	// -------------------------
	unsigned int texture;
	glGenTextures(1, &texture); //? unclear
	glBindTexture(GL_TEXTURE_2D, texture); //? all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //? set texture wrapping to GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //?
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	//unsigned char* data = stbi_load("resources/texturese/container.jpg", &width, &height, &nrChannels, 0); //? Failed
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); //?
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

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

		// bind Texture
		glBindTexture(GL_TEXTURE_2D, texture); // bind texture again?

		// Draw code in render loop
		// --------------------------
		// render container
		glBindVertexArray(VAO); // bind vertex array again?
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

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