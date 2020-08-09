#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>

#include <shaders/shaders.h>

// settings
unsigned int scr_height = 800;
unsigned int scr_width = (16.0 / 9.0) * scr_height;

unsigned int max_iterations = 250;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
	glfwSetCursor(window, cursor);

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

float vertices[] = {
	// positions       
	-1.0f, -1.0f, 0.0f, // bottom left
	 0.0f,  1.0f, 0.0f, // top right
	-1.0f,  1.0f, 0.0f, // top left
	 0.0f, -1.0f, 0.0f  // bottom right
};

float vertices1[] = {
	// positions       
	 0.0f, -1.0f, 0.0f, // bottom left
	 1.0f,  1.0f, 0.0f, // top right
	 0.0f,  1.0f, 0.0f, // top left
	 1.0f, -1.0f, 0.0f  // bottom right
};

unsigned int indices[] =
{
	// 2----1
	// |  / |
	// | /  |
	// 0----3

	0, 1, 2,
	0, 3, 1

};

unsigned int indices1[] =
{
	// 2----1
	// |  / |
	// | /  |
	// 0----3

	0, 1, 2,
	0, 3, 1

};


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWwindow* window = glfwCreateWindow(scr_width, scr_height, "Mandelbrot and Julia Set Explorer", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	Shader leftShader("C:/Programming/C++/OpenGL Projects/Mandelbrotandjulia/Mandelbrotandjulia/Shaders/leftVShader.txt", "C:/Programming/C++/OpenGL Projects/Mandelbrotandjulia/Mandelbrotandjulia/Shaders/leftFShader.txt");
	Shader rightShader("C:/Programming/C++/OpenGL Projects/Mandelbrotandjulia/Mandelbrotandjulia/Shaders/rightVShader.txt", "C:/Programming/C++/OpenGL Projects/Mandelbrotandjulia/Mandelbrotandjulia/Shaders/rightFShader.txt");
	unsigned int leftVAO, rightVAO;
	unsigned int leftVBO, rightVBO;
	unsigned int leftEBO, rightEBO;

	// left side
	glGenVertexArrays(1, &leftVAO);
	glGenBuffers(1, &leftVBO);
	glGenBuffers(1, &leftEBO);
	glBindVertexArray(leftVAO);

	glBindBuffer(GL_ARRAY_BUFFER, leftVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, leftEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// right side
	glGenVertexArrays(1, &rightVAO);
	glGenBuffers(1, &rightVBO);
	glGenBuffers(1, &rightEBO);
	glBindVertexArray(rightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, rightVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rightEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	glEnable(GL_MULTISAMPLE);


	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		int sWidth, sHeight;
		glfwGetFramebufferSize(window, &sWidth, &sHeight);
		glm::vec2 resolution = glm::vec2(sWidth, sHeight);

		double cXpos, cYpos;
		glfwGetCursorPos(window, &cXpos, &cYpos);
		glm::vec2 cursorPos = glm::vec2(cXpos, sHeight - cYpos);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		leftShader.use();
		leftShader.setFloat("time", glfwGetTime());
		leftShader.setVec2("resolution", resolution);
		leftShader.setVec2("cursorPos", cursorPos);
		leftShader.setFloat("max_iterations", max_iterations);

		glBindVertexArray(leftVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		rightShader.use();
		rightShader.setFloat("time", glfwGetTime());
		rightShader.setVec2("resolution", resolution);
		rightShader.setVec2("cursorPos", cursorPos);
		rightShader.setFloat("max_iterations", max_iterations);

		glBindVertexArray(rightVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}