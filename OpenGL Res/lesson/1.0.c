#define GLUT_DISABLE_ATEXIT_HACK

#include <GL/glut.h>

void myDisplay(void)
{
	/// �����GL_COLOR_BUFFER_BIT��ʾ�����ɫ��glClear������������������Ķ�����
	glClear(GL_COLOR_BUFFER_BIT);
	/// ��һ�����Ρ��ĸ������ֱ��ʾ��λ�ڶԽ����ϵ�������ĺᡢ������
	glRectf(-0.5f, -0.5f, 0.5f, 0.5f);
	/// ��֤ǰ���OpenGL��������ִ�У��������������ڻ������еȴ����������ø�fflush(stdout)���ơ�
	glFlush();
}

int main(int argc, char *argv[])
{
	/// glutInit����GLUT���г�ʼ�����������������������GLUTʹ��֮ǰ����һ�Ρ����ʽ�Ƚ����壬һ���ճ����glutInit(&argc, argv)�Ϳ����ˡ�
	glutInit(&argc, argv);	
	/// ������ʾ��ʽ������GLUT_RGB��ʾʹ��RGB��ɫ����֮��Ӧ�Ļ���GLUT_INDEX����ʾʹ��������ɫ����GLUT_SINGLE��ʾʹ�õ����壬��֮��Ӧ�Ļ���GLUT_DOUBLE��ʹ��˫���壩
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	/// ���ô�������Ļ�е�λ�á�
	glutInitWindowPosition(100, 100);
	/// ���ô��ڵĴ�С��
	glutInitWindowSize(400, 400);
	/// ����ǰ�����õ���Ϣ�������ڡ�����������Ϊ���ڵı��⡣ע�⣺���ڱ������󣬲���������ʾ����Ļ�ϡ���Ҫ����glutMainLoop���ܿ������ڡ�
	glutCreateWindow("��һ��OpenGL����");
	/// ����һ������������Ҫ���л�ͼʱ����������ͻᱻ���á�
	glutDisplayFunc(&myDisplay);
	/// ����һ����Ϣѭ��
	glutMainLoop();
	return 0;
}

