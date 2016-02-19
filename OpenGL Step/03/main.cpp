#include <GL/glew.h>
#include <gl/freeglut.h>

#pragma comment(lib, "glew32.lib")

#define BUFFER_OFFSET(offset) ((void *)(offset))
GLuint vertexArrayID;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glutSwapBuffers();
	glFlush();
}

void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	/// »º´æÊý¾Ý
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	GLfloat vertices[][3] = {
		{ -1.0f, -1.0f, 0.0f },
		{ 1.0f, -1.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f }
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("03");

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