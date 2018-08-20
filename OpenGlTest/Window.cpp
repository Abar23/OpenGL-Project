#include "Window.h"
#include <cstdio>
#include <cstdlib>

void setFrameBufferSize(GLFWwindow *window, int width, int height)
{
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
}

Window::Window(int height, int width, const char * title, GLFWmonitor * montior, GLFWwindow * share)
{
	if (glfwInit() == NULL)
	{
		printf("GLFW could not be initialized!\n");
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->window = glfwCreateWindow(width, height, title, montior, share);
	if (this->window == NULL)
	{
		printf("GLFW could not be initialized!\n");
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(this->window);

	glfwSetFramebufferSizeCallback(this->window, setFrameBufferSize);
	
	glewExperimental = GL_TRUE;
	GLenum glew = glewInit();
	if ( GLEW_OK != glew)
	{
		printf("Could not initialize GLEW!!");
		exit(EXIT_FAILURE);
	}

	glEnable(GL_DEPTH_TEST);

	printf("openGL Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL Language Version : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	printf("openGL Vendor: %s\n", glGetString(GL_VENDOR));
	printf("openGL Renderer: %s\n", glGetString(GL_RENDERER));
}

Window::~Window()
{
	glfwDestroyWindow(this->window);
	glfwTerminate();
}

bool Window::IsWindowClosed()
{
	return !glfwWindowShouldClose(this->window);
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(this->window);
}

void Window::ClearBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::PollEvents()
{
	glfwPollEvents();
}

float Window::GetAspectRatio()
{
	int width, height;
	glfwGetFramebufferSize(this->window, &width, &height);
	
	return (float)width / (float)height;
}

void Window::ProcessUserInput(glm::vec3 *camerPosition, glm::vec3 *cameraFront, glm::vec3 *cameraUp)
{
	float cameraSpeed = 2.5f * this->timeBetweenFrames; // Makes camera movement uniform between differing hardware across user machines

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		*camerPosition += cameraSpeed * *cameraFront;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		*camerPosition -= cameraSpeed * *cameraFront;
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		*camerPosition -= cameraSpeed * glm::normalize(glm::cross(*cameraFront, *cameraUp));
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		*camerPosition += cameraSpeed * glm::normalize(glm::cross(*cameraFront, *cameraUp));
	}
}

// This corrects how fast openGL runs between hardware so that movement with the camera becomes uniform for all users
void Window::RefreshRate()
{
	float timeAtCurrentFrame = glfwGetTime();
	this->timeBetweenFrames = timeAtCurrentFrame - this->timeAtLastFrame;
	this->timeAtLastFrame = timeAtCurrentFrame;
}
