#define _USE_MATH_DEFINES
#include <cmath>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm\gtx\quaternion.hpp"
#include "Window.h"
#include "Shader.h"
#include "Texture.h"
#include "Quaternion.h"
#include "Model.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

int main()
{
	int height = 600;
	int width = 600;
	
	Window window(height, width, "Test", NULL, NULL);

	Shader shader("./Resources/Shaders/vertexShader.vs", "./Resources/Shaders/fragmentShader.fs");

	//----------------------Setup data for rending a traingle--------//

	//Consists of vector position, color, and texture coordinates
							//positions			  //colors                   //texture coords
	GLfloat vertices[] = { -0.5f, -0.5f,  0.0f,   0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f,
						    0.5f, -0.5f,  0.0f,   1.0f, 0.0f, 0.0f, 1.0f,    1.0f, 0.0f,
						    0.0f,  0.5f,  0.0f,   0.0f, 1.0f, 0.0f, 1.0f,    0.5f, 1.0f,
						   -1.0f, -1.0f,  0.0f,   0.0f, 1.0f, 0.0f, 1.0f,    0.0f, 0.0f,
						   -1.0f,  0.0f,  0.0f,   0.0f, 1.0f, 0.0f, 1.0f,    1.0f, 2.0f,
						    0.0f,  0.0f,  0.0f,   0.0f, 1.0f, 0.0f, 1.0f,    2.0f, 0.0f };

	//----------------------Setup data for square using indices------//

	unsigned int indices[] = { 0, 1, 2, 3, 4, 5 };

	//-----------------------Setup vertex buffers--------------------//
	GLuint vertexArrayObject, vertexBuffer;
	glGenVertexArrays(1, &vertexArrayObject);
	glGenBuffers(1, &vertexBuffer);

	glBindVertexArray(vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 9 * 6 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	GLuint positionID = glGetAttribLocation(shader.GetProgramID(), "position");
	GLuint vertColorID = glGetAttribLocation(shader.GetProgramID(), "vertColor");
	GLint texCoordsID = glGetAttribLocation(shader.GetProgramID(), "texCoords");
	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), 0);
	glVertexAttribPointer(vertColorID, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(texCoordsID, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void *)(7 * sizeof(GLfloat)));
	glEnableVertexAttribArray(positionID);
	glEnableVertexAttribArray(vertColorID);
	glEnableVertexAttribArray(texCoordsID);
	
	//-----------------------Setup index buffer---------------------//
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	glBindVertexArray(0); //Unbind the vertex array object
	glBindBuffer(GL_ARRAY_BUFFER, 0); //Unbind the vertex buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //Unbind the index buffer
	//UNBINDS ON VBO/IBO ARE CALLED AFTER VAO IS UNBOUNDED

	//-----------------------Setup cube vertex data and buffers---------------------//

	float cubeVertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	GLuint cubeBuffer, cubeVertexArray;
	glGenVertexArrays(1, &cubeVertexArray);
	glGenBuffers(1, &cubeBuffer);

	glBindVertexArray(cubeVertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, cubeBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	
	GLuint cubePositionLocation = glGetAttribLocation(shader.GetProgramID(), "cubePosition");
	GLuint cubeTextureCoordinates = glGetAttribLocation(shader.GetProgramID(), "cubeTexCoords");
	glVertexAttribPointer(cubePositionLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)0);
	glVertexAttribPointer(cubeTextureCoordinates, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(cubePositionLocation);
	glEnableVertexAttribArray(cubeTextureCoordinates);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Define ten locations for ten cubes to be transfered into the world coordinate view
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	//-----------------------Create and load textures------------------//
	Texture beachTexture("./Resources/Textures/Beach.jpg");
	Texture brickWallTexture("./Resources/Textures/Brick Wall.jpg");

	shader.SetUniformToTextureUnit("beachTexture", 0);
	shader.SetUniformToTextureUnit("brickTexture", 1);

	//-----------------------Setup Model Objects------------------//
	Shader modelShader("./Resources/Shaders/modelLoader.vs", "./Resources/Shaders/modelLoader.fs");
	std::string modelPath = "./Resources/Models/Nanosuit/nanosuit.obj";
	Model nanoSuit(&modelPath, modelShader.GetProgramID());
	modelPath = "./Resources/Models/Teapot1/teapot.obj";
	Model teapot(&modelPath, modelShader.GetProgramID());

	//Set intial camera position within the world
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 25.0f); // Set camera 3 units backwards. It is backwards since openGL's coordinate system sets the -z axis as the direction the camer looks
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); // Point camera towards the -z diraction, which is towards the world
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); // Provide vector along the positive y direction to give orientation to the camera

	// Create Model, View, Projection matrices
	glm::mat4 viewMatrix = glm::mat4(1.0f);
	glm::mat4 projectionMatrix = glm::mat4(1.0f);
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	
	while(window.IsWindowClosed())
	{
		window.ClearBuffer();
		glClearColor(0.0f, 0.5f, 0.5f, 1.0f);

		shader.UseProgram();
		beachTexture.BindTexture(0);
		brickWallTexture.BindTexture(1);

		window.RefreshRate();
		window.ProcessUserInput(&cameraPos, &cameraFront, &cameraUp);

		//Transformations must occur after the textures have been activated
		viewMatrix = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
		projectionMatrix = glm::perspective(glm::radians(45.0f), window.GetAspectRatio(), 0.1f, 100.0f);
		
		unsigned int modelMatrixLocation = glGetUniformLocation(shader.GetProgramID(), "modelMatrix");
		unsigned int viewMatrixLocation = glGetUniformLocation(shader.GetProgramID(), "viewMatrix");
		unsigned int projectionMatrixLocation = glGetUniformLocation(shader.GetProgramID(), "projectionMatrix");

		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

		glBindVertexArray(cubeVertexArray);
		for (unsigned int i = 0; i < 10; i++)
		{
			modelMatrix = glm::mat4(1.0);
			modelMatrix = glm::scale(glm::translate(modelMatrix, cubePositions[i]), glm::vec3(1.5f, 1.5f, 1.5f));
			float angle = 20.0f * i;
			modelMatrix = glm::rotate(modelMatrix, glm::radians(angle * (float)glfwGetTime()), glm::vec3(1.0f, 0.3f, 0.5f));

			glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // This is to draw the ebo related to the vbo object
		glBindVertexArray(0);
		shader.TurnOffProgram();
		
		modelShader.UseProgram();

		modelMatrixLocation = glGetUniformLocation(modelShader.GetProgramID(), "model");
		viewMatrixLocation = glGetUniformLocation(modelShader.GetProgramID(), "view");
		projectionMatrixLocation = glGetUniformLocation(modelShader.GetProgramID(), "projection");

		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(15.0f, -10.0f, 0.0f));
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		nanoSuit.Draw(&modelShader);

		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::scale(glm::translate(modelMatrix, glm::vec3(-15.0f, -5.0f, 0.0f)), glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		teapot.Draw(&modelShader);

		modelShader.TurnOffProgram();

		window.SwapBuffers();
		window.PollEvents();
	}
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &cubeBuffer);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vertexArrayObject);
	glDeleteVertexArrays(1, &cubeVertexArray);
}
