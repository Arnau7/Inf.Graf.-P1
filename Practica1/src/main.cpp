//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>
#include <iostream>
#include "Shader.h"
#include "glm.hpp"

using namespace glm;
using namespace std;
const GLint WIDTH = 800, HEIGHT = 600;
bool WIDEFRAME = false;
float vertex1 = -0.5;
float vertex2 = 0.5;




void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

Shader* shade;
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
	shade = new Shader("./src/SimpleVertexShader.vertexshader", "./src/SimpleFragmentShader.fragmentshader");
	GLint variableShader = glGetUniformLocation(shade->Program, "shade");

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
	GLint Element[]{
		0,1,3,
		1,2,3
	};
	
	GLfloat VertexBufferObject[] = {
		0.5f,  0.5f, 0.0f,  // Top Right
		0.5f, -0.5f, 0.0f,  // Bottom Right
		-0.5f, -0.5f, 0.0f,  // Bottom Left
		-0.5f,  0.5f, 0.0f   // Top Left 
	};
	
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
		glBindBuffer(GL_ARRAY_BUFFER,VBO );
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBufferObject), VertexBufferObject, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Element), Element, GL_STATIC_DRAW);

		//Enlazar el buffer con openGL
		

	
		//Establecer las propiedades de los vertices
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		//liberar el buffer
	
		glBindVertexArray(0);
		//liberar el buffer de vertices

		
	//bucle de dibujado
		while (!glfwWindowShouldClose(window))
		{
			/*float time = glfwGetTime();
			vertex1 = abs(sin(time)/2) - 1;
			vertex2 = abs(cos(time)/2) + 0.5;
			
			printf("%f\n", vertex2);
			printf("%f\n", vertex1);
			GLfloat Vertex[]{
				vertex1,0,0,
				-0.5,1,0,
				0.5,1,0,
				vertex2,0,0
			};
		

			// Definir el EBO
			
			// Crear los VBO, VAO y EBO
			

			//Establecer el objeto
			glBindVertexArray(VAO);
			//Declarar el VBO y el EBO
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex), Vertex, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
			glEnableVertexAttribArray(0);
			//liberar el buffer
			glBindVertexArray(0);*/
			glClear(GL_COLOR_BUFFER_BIT);

			//definir la matriz de proyeccion
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(-10, 10, -10.f, 10.f, -1.0f, 10.f);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			//establecer el shader
			//glUseProgram(programID);
			shade->USE();
			
			glBindVertexArray(VAO);
			//pitar el VAO
			if (WIDEFRAME) {
				
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				//glBindVertexArray(0);
				
			}
			else {
				//glBindVertexArray(VAO);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				
			}
			glBindVertexArray(0);
			// Swap the screen buffers
			glfwSwapBuffers(window);
			glfwPollEvents();
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
}


