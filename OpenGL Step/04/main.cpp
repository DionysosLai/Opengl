#include <GL/glew.h>
#include <gl/freeglut.h>
#include "shader.h"

#pragma comment(lib, "glew32.lib")

#define BUFFER_OFFSET(offset) ((void *)(offset))
GLuint vertexArrayID;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(vertexArrayID);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glutSwapBuffers();
	glFlush();
}

void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	/// 顶点数据
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	GLfloat vertices[][3] = {
		{ -1.0f, -1.0f, 0.0f },
		{ 1.0f, -1.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f }
	};
	/// 缓存数据
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

	/// 将数据载入缓存对象中
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint program = LoadShaders("triangles.vert", "triangles.frag");
	glValidateProgram(program);
	glUseProgram(program);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("04");

	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		return 1;
	}

	init();

	glutDisplayFunc(display);

	glutMainLoop();

	return 0;
}