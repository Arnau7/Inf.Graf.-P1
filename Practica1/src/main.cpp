//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>
#include <iostream>
#include "Shader.h"
#include "glm.hpp"
#include <SOIL.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

using namespace glm;



using namespace glm;
using namespace std;
const GLint WIDTH = 800, HEIGHT = 600;
bool WIDEFRAME = false;
float vertex1 = 0.5;
float vertex2 = -0.5;
float mixed;
float mixed2;
float gir;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
mat4 matrix = {
	1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1
};
mat4 rotation = matrix;
mat4 finalMatrix = matrix;
mat4 projection;
mat4 vision;
mat4 model;
float angleX, angleY;

vec3 cameraPosition = vec3(0.0f, 0.0f, 3.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp;
GLfloat dt;
GLfloat lastFrame;

GLfloat yaw2 = -90.0f;
GLfloat pitch2 = 0.0f;
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
GLfloat fov = 45.0f;
bool keys[1024];

Shader* shade;

void do_movement()
{
	// Camera controls
	GLfloat cameraSpeed = 1.5f * dt;
	if (keys[GLFW_KEY_W])
		cameraPosition += cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_S])
		cameraPosition -= cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_A])
		cameraPosition -= normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keys[GLFW_KEY_D])
		cameraPosition += normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keys[GLFW_KEY_UP])
	{
		angleX += 2;
		model = rotate(model, radians(5.0f), vec3(1.0f, 0.0f, 0.0f));

		GLint variableRot = glGetUniformLocation(shade->Program, "model");
		glUniformMatrix4fv(variableRot, 1, GL_FALSE, value_ptr(model));
	}
	else if (keys[GLFW_KEY_DOWN])
	{
		angleX -= 2;
		model = rotate(model, radians(-5.0f), vec3(1.0f, 0.0f, 0.0f));

		GLint variableRot = glGetUniformLocation(shade->Program, "model");
		glUniformMatrix4fv(variableRot, 1, GL_FALSE, value_ptr(model));
	}
	if (keys[GLFW_KEY_LEFT])
	{
		angleY += 2;
		//gir += radians(1.0f);
		model = rotate(model, radians(5.0f), vec3(0.0f, 0.0f, 1.0f));

		GLint variableRot = glGetUniformLocation(shade->Program, "model");
		glUniformMatrix4fv(variableRot, 1, GL_FALSE, value_ptr(model));
	}

	else if (keys[GLFW_KEY_RIGHT])
	{
		angleY -= 2;
		//gir += radians(1.0f);
		model = rotate(model, radians(-5.0f), vec3(0.0f, 0.0f, 1.0f));

		GLint variableRot = glGetUniformLocation(shade->Program, "model");

		glUniformMatrix4fv(variableRot, 1, GL_FALSE, value_ptr(model));
	}

}
bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to left
	lastX = xpos;
	lastY = ypos;

	GLfloat sensitivity = 0.05;	// Change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw2 += xoffset;
	pitch2 += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch2 > 89.0f)
		pitch2 = 89.0f;
	if (pitch2 < -89.0f)
		pitch2 = -89.0f;

	vec3 front;
	front.x = cos(radians(yaw2)) * cos(radians(pitch2));
	front.y = sin(radians(pitch2));
	front.z = sin(radians(yaw2)) * cos(radians(pitch2));
	cameraFront = normalize(front);
}


int main() {
	//initGLFW
	//TODO
	if (!glfwInit()) {
		cout << "Error GLFW" << endl;
		exit(EXIT_FAILURE);
	}
	//set GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//create a window
	//TODO
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Window", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	glewExperimental = GL_TRUE;
	//set GLEW and inicializate
	//TODO
	if (GLEW_OK != glewInit()) {
		std::cout << "Error al cargar glew" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	//set function when callback
	//TODO
	glfwSetKeyCallback(window, key_callback);
	//set windows and viewport
	//TODO

	//fondo
	//TODO
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.0, 1.0, 0.8, 1.0);

	//cargamos los shader
	//shade = new Shader("./src/textureVertex.vertexshader", "./src/textureFragment.fragmentshader");
	shade = new Shader("./src/fovVertexShader.vertexshader", "./src/fovFragmentShader.fragmentshader");
	//GLint variableShader = glGetUniformLocation(shade->Program, "shade");

	GL_MAX_VERTEX_ATTRIBS;
	GLint nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	vec3 vectprop = vec3(1, 2, 2);

	// Definir el buffer de vertices
	/*GLfloat Vertex[]{
	vertex1,0,0,
	-0.5,1,0,
	0.5,1,0,
	vertex2,0,0
	};*/
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	int width, height;
	unsigned char* image = SOIL_load_image("./src/texture.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);

	GLuint texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	int width2, height2;
	unsigned char* image2 = SOIL_load_image("./src/texture2.png", &width2, &height2, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image2);

	GLint Element[]{
		1,0,3,
		3,2,1

	};

	/*GLfloat VertexBufferObject[] = {
	0.5f,  0.5f, 0.0f,  // Top Right
	0.5f, -0.5f, 0.0f,  // Bottom Right
	-0.5f, -0.5f, 0.0f,  // Bottom Left
	-0.5f,  0.5f, 0.0f   // Top Left
	};*/
	GLfloat vertices[] = {
		// Positions          // Colors           // Texture Coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left 
	};
	GLfloat VertexBufferCube[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f , -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	vec3 CubesPositionBuffer[] = {
		vec3(0.0f ,  0.0f,  0.0f),
		vec3(2.0f ,  5.0f, -15.0f),
		vec3(-1.5f, -2.2f, -2.5f),
		vec3(-3.8f, -2.0f, -12.3f),
		vec3(2.4f , -0.4f, -3.5f),
		vec3(-1.7f,  3.0f, -7.5f),
		vec3(1.3f , -2.0f, -2.5f),
		vec3(1.5f ,  2.0f, -2.5f),
		vec3(1.5f ,  0.2f, -1.5f),
		vec3(-1.3f,  1.0f, -1.5f)
	};

	mat4 escaled = scale(matrix, vec3(0.5f, -0.5f, 0));
	mat4 translated = translate(matrix, vec3(0.5f, 0.5f, 0));

	// Definir el EBO
	GLuint EBO;
	// Crear los VBO, VAO y EBO
	GLuint VBO;
	GLuint VAO;
	//reservar memoria para el VAO, VBO y EBO
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);
	//Establecer el objeto
	glBindVertexArray(VAO);
	//Declarar el VBO y el EBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBufferCube), VertexBufferCube, GL_STATIC_DRAW);

	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(CubesPositionBuffer), CubesPositionBuffer, GL_STATIC_DRAW);*/

	//Enlazar el buffer con openGL



	//Establecer las propiedades de los vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	//liberar el buffer

	glBindVertexArray(0);
	//liberar el buffer de vertices





	float aspectRatio = WIDTH / HEIGHT;
	float FOV = 60;
	float nearPlane = 0.1f;
	float farPlane = 100.f;


	projection = perspective(radians(FOV), aspectRatio, nearPlane, farPlane);

	vision = translate(vision, vec3(0, 0, -3.0f));

	model = rotate(model, radians(50.0f), vec3(1.0f, 0.0f, 0.0f));
	model = translate(model, vec3(0.0f, -0.5f, 0.0f));
	model = scale(model, vec3(1.0f, 1.0f, 1.0f));

	//finalMatrix = proj*vision*model;


	glEnable(GL_DEPTH_TEST);

	vec3 up = vec3(0.0f, 1.0f, 0.0f);
	vec3 cameraDirection = normalize(cameraPosition - cameraFront);
	vec3 cameraRight = normalize(cross(up, cameraFront));
	cameraUp = cross(cameraDirection, cameraRight);
	

	//bucle de dibujado
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		do_movement();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GLfloat actualTime = glfwGetTime();
		dt = actualTime - lastFrame;
		lastFrame = actualTime;


		/*GLint variableFov = glGetUniformLocation(shade->Program, "model");
		glUniformMatrix4fv(variableFov, 1, GL_FALSE, value_ptr(model));*/

		GLint variableFov2 = glGetUniformLocation(shade->Program, "vision");
		glUniformMatrix4fv(variableFov2, 1, GL_FALSE, value_ptr(vision));

		GLint variableFov3 = glGetUniformLocation(shade->Program, "projection");
		glUniformMatrix4fv(variableFov3, 1, GL_FALSE, value_ptr(projection));

		
		//definir la matriz de proyeccion
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//glOrtho(-10, 10, -10.f, 10.f, -1.0f, 10.f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//establecer el shader
		//glUseProgram(programID);
		//shade->USE();

		//glBindVertexArray(VAO);
		//pitar el VAO
		/*if (WIDEFRAME) {

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);

		}*/

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(shade->Program, "Texture1"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(shade->Program, "Texture2"), 1);

		/*glBindTexture(GL_TEXTURE_2D, texture);
		glBindTexture(GL_TEXTURE_2D, texture2);*/
		shade->USE();
		glBindVertexArray(VAO);

		//CAMERA
		vision = lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);

		//manual
		mat4 translated, rotation, rotationX, rotationY;
		translated = translate(translated, CubesPositionBuffer[0]);
		rotationX = rotate(rotationX, radians(angleX), vec3(1.0f, 0.0f, 0.0f));
		rotationY = rotate(rotationY, radians(angleY), vec3(0.0f, 1.0f, 0.0f));
		rotation = rotationX * rotationY;
		model = translated * rotation;
		glUniformMatrix4fv(glGetUniformLocation(shade->Program, "model"), 1, GL_FALSE, value_ptr(model));
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//automatic
		for (int i = 1; i < 10; i++) {
			mat4 trans, rot;
			trans = translate(trans, CubesPositionBuffer[i]);
			rot = rotate(rot, (float)glfwGetTime()*radians(30.f), vec3(1.0f, 1.0f, 0.0f));
			model = trans * rot;
			glUniformMatrix4fv(glGetUniformLocation(shade->Program, "model"), 1, GL_FALSE, value_ptr(model));
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//glBindVertexArray(0);



		glBindVertexArray(0);
		// Swap the screen buffers
		glfwSwapBuffers(window);

	}
	// liberar la memoria de los VAO, EBO y VBO
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	//TODO, comprobar que la tecla pulsada es escape para cerrar la aplicación y la tecla w para cambiar a modo widwframe
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	else if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		WIDEFRAME = !WIDEFRAME;
		//cout << "pressed" << endl;
	}
	if (key == GLFW_KEY_1) {
		if (mixed != 1) {
			mixed = 1;
		}
		GLfloat variableShader = glGetUniformLocation(shade->Program, "mixed");
		glUniform1f(variableShader, mixed);
	}
	else if (key == GLFW_KEY_2){
		if (mixed != 0) {
			mixed = 0;
		}
		GLfloat variableShader = glGetUniformLocation(shade->Program, "mixed");
		glUniform1f(variableShader, mixed);
	}
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
	
	//if (key == GLFW_KEY_UP)
	//{
	//	angleX += 2;
	//	model = rotate(model, radians(5.0f), vec3(1.0f, 0.0f, 0.0f));
	//	
	//	GLint variableRot = glGetUniformLocation(shade->Program, "model");
	//	glUniformMatrix4fv(variableRot, 1, GL_FALSE, value_ptr(model));
	//}
	//else if (key == GLFW_KEY_DOWN)
	//{
	//	angleX -= 2;
	//	model = rotate(model, radians(-5.0f), vec3(1.0f, 0.0f, 0.0f));
	//	
	//	GLint variableRot = glGetUniformLocation(shade->Program, "model");
	//	glUniformMatrix4fv(variableRot, 1, GL_FALSE, value_ptr(model));
	//}
	//if (key == GLFW_KEY_LEFT) 
	//{
	//	angleY += 2;
	//	//gir += radians(1.0f);
	//	model = rotate(model, radians(5.0f), vec3(0.0f, 0.0f, 1.0f));
	//	
	//	GLint variableRot = glGetUniformLocation(shade->Program, "model");
	//	glUniformMatrix4fv(variableRot, 1, GL_FALSE, value_ptr(model));
	//}

	//else if(key == GLFW_KEY_RIGHT)
	//{
	//	angleY -= 2;
	//	//gir += radians(1.0f);
	//	model = rotate(model, radians(-5.0f), vec3(0.0f, 0.0f, 1.0f));
	//	
	//	GLint variableRot = glGetUniformLocation(shade->Program, "model");

	//	glUniformMatrix4fv(variableRot, 1, GL_FALSE, value_ptr(model));
	//}
	
	/*if (key == GLFW_KEY_W) {
		cameraPosition += cameraSpeed*cameraFront;
	}
	else if (key == GLFW_KEY_S) {
		cameraPosition -= cameraSpeed*cameraFront;
	}
	if (key == GLFW_KEY_A) {
		cameraPosition -= normalize(cross(cameraFront, cameraUp))*cameraSpeed;
	}
	else if (key == GLFW_KEY_D) {
		cameraPosition += normalize(cross(cameraFront, cameraUp))*cameraSpeed;
	}*/

}

