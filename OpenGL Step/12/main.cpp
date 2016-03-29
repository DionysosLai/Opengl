#include <GL/glew.h>
#include <gl/freeglut.h>
#include "shader.h"

#include <assert.h>
#include <math.h>

#pragma comment(lib, "glew32.lib")

#define M_PI 3.1415926
#define ToRadian(x) ((x) * M_PI / 180.0f)
#define ToDegree(x) ((x) * 180.0f / M_PI)

#define BUFFER_OFFSET(offset) ((void *)(offset))
GLuint vertexArrayID;
GLuint indicesbuffer;
GLuint program;

struct Matrix4f {
	float m[4][4];
};

inline Matrix4f mul(const Matrix4f& left, const Matrix4f& Right)
{
	Matrix4f Ret;
	for (unsigned int i = 0; i < 4; i++) {
		for (unsigned int j = 0; j < 4; j++) {
			Ret.m[i][j] = left.m[i][0] * Right.m[0][j] +
				left.m[i][1] * Right.m[1][j] +
				left.m[i][2] * Right.m[2][j] +
				left.m[i][3] * Right.m[3][j];
		}
	}

	return Ret;
}

void initPerspectiveProj(Matrix4f& m)
{
	const float ar = 4 / 3;
	const float zNear = 0.1f;
	const float zFar = 1000.f;
	const float zRange = zNear - zFar;
	const float tanHalfFOV = tanf(ToRadian(30.f / 2.0));

	m.m[0][0] = 1.0f / (tanHalfFOV * ar);
	m.m[0][1] = 0.0f;
	m.m[0][2] = 0.0f;
	m.m[0][3] = 0.0f;

	m.m[1][0] = 0.0f;
	m.m[1][1] = 1.0f / tanHalfFOV;
	m.m[1][2] = 0.0f;
	m.m[1][3] = 0.0f;

	m.m[2][0] = 0.0f;
	m.m[2][1] = 0.0f;
	m.m[2][2] = (-zNear - zFar) / zRange;
	m.m[2][3] = 2.0f * zFar * zNear / zRange;

	m.m[3][0] = 0.0f;
	m.m[3][1] = 0.0f;
	m.m[3][2] = 1.0f;
	m.m[3][3] = 0.0f;
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(vertexArrayID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesbuffer);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glutSwapBuffers();
	glFlush();
}

void setUniform()
{
	/// 获取着色器中 gScale uniform变量值
	GLuint gWorldLocation = glGetUniformLocation(program, "gWorld");
	assert(gWorldLocation != 0XFFFFFFFF);

	/// 设置 uniform 变量值
	static GLfloat fscale = 0.5f;
	//fscale += 0.0001f;
	//fscale = fscale > 1.0 ? 0.0f : fscale;

	/// 围绕z轴旋转
	Matrix4f scale;
	scale.m[0][0] = cosf(fscale); scale.m[0][1] = -sinf(fscale); scale.m[0][2] = 0.0f; scale.m[0][3] = 0.0f;
	scale.m[1][0] = sinf(fscale); scale.m[1][1] = cosf(fscale);  scale.m[1][2] = 0.0f; scale.m[1][3] = 0.0f;
	scale.m[2][0] = 0.0f;        scale.m[2][1] = 0.0f;         scale.m[2][2] = 1.0f; scale.m[2][3] = 0.0f;
	scale.m[3][0] = 0.0f;        scale.m[3][1] = 0.0f;         scale.m[3][2] = 0.0f; scale.m[3][3] = 1.0f;

	/// 平移
	Matrix4f tran;
	tran.m[0][0] = 1.0f; tran.m[0][1] = 0.0f; tran.m[0][2] = 0.0f; tran.m[0][3] = sinf(fscale);
	tran.m[1][0] = 0.0f; tran.m[1][1] = 1.0f; tran.m[1][2] = 0.0f; tran.m[1][3] = 0.0f;
	tran.m[2][0] = 0.0f; tran.m[2][1] = 0.0f; tran.m[2][2] = 1.0f; tran.m[2][3] = 0.0f;
	tran.m[3][0] = 0.0f; tran.m[3][1] = 0.0f; tran.m[3][2] = 0.0f; tran.m[3][3] = 1.0f;

	/// 透视投影
	Matrix4f m;
	initPerspectiveProj(m);

	Matrix4f World;

	World = mul(scale, tran);
	World = mul(World, m);



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
		{ -1.0f, -1.0f, 0.0f, },
		{ 0.0f, -0.5f, 1.0f, },
		{ 1.0f, -1.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f }
	};
	/// 缓存数据
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	/// 将数据载入缓存对象中
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/// 缓存数据
	unsigned int Indices[] = {
		0, 3, 2,
		0, 3, 1,
		3, 2, 1,
		0, 2, 1,
	};
	glGenBuffers(1, &indicesbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	program = LoadShaders("triangles.vert", "triangles.frag");
	glValidateProgram(program);
	glUseProgram(program);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(600, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("10");

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