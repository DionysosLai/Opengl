#include <GL/glew.h>
#include <gl/freeglut.h>
#include "shader.h"

#include <assert.h>

#pragma comment(lib, "glew32.lib")

#define BUFFER_OFFSET(offset) ((void *)(offset))
GLuint vertexArrayID;
GLuint program;

struct Matrix4f {
    float m[4][4];
};

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(vertexArrayID);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glutSwapBuffers();
	glFlush();
}

void setUniform()
{
	/// 获取着色器中 gScale uniform变量值
	GLuint gWorldLocation = glGetUniformLocation(program, "gWorld");
	assert(gWorldLocation != 0XFFFFFFFF);

	/// 设置 uniform 变量值
	static GLfloat fscale = 0.0f;
	fscale += 0.0001f;
	//fscale = fscale > 1.0 ? 0.0f : fscale;

	/// 围绕z轴旋转
	Matrix4f World;
	World.m[0][0] = cosf(fscale); World.m[0][1] = -sinf(fscale); World.m[0][2] = 0.0f; World.m[0][3] = 0.0f;
	World.m[1][0] = sinf(fscale); World.m[1][1] = cosf(fscale);  World.m[1][2] = 0.0f; World.m[1][3] = 0.0f;
	World.m[2][0] = 0.0f;        World.m[2][1] = 0.0f;         World.m[2][2] = 1.0f; World.m[2][3] = 0.0f;
	World.m[3][0] = 0.0f;        World.m[3][1] = 0.0f;         World.m[3][2] = 0.0f; World.m[3][3] = 1.0f;


	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &World.m[0][0]);
	/// 刷新窗口
	glutPostRedisplay();
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

	program = LoadShaders("triangles.vert", "triangles.frag");
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
	glutCreateWindow("07");

	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		return 1;
	}

	init();

	glutDisplayFunc(display);
	glutIdleFunc(setUniform);
	glutMainLoop();

	return 0;
}