#include <glad.h>
#include<glfw3.h>
#include <iostream>

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

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

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