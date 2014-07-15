#define GLUT_DISABLE_ATEXIT_HACK

#include <GL/glut.h>
#include <math.h>
/*
设五角星的五个顶点分布位置关系如下：
     A
 E       B

   D   C
首先，根据余弦定理列方程，计算五角星的中心到顶点的距离a
（假设五角星对应正五边形的边长为.0）
a = 1 / (2-2*cos(72*Pi/180));
然后，根据正弦和余弦的定义，计算B的x坐标bx和y坐标by，以及C的y坐标
（假设五角星的中心在坐标原点）
bx = a * cos(18 * Pi/180);
by = a * sin(18 * Pi/180);
cy = -a * cos(18 * Pi/180);
五个点的坐标就可以通过以上四个量和一些常数简单的表示出来
*/
const GLfloat Pi = 3.1415926536f;

static int day = 200; // day的变化：从0到359
void myDisplay(void)
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75, 1, 1, 400000000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, -200000000, 200000000, 0, 0, 0, 0, 0, 1);

	// 绘制红色的“太阳”
	glColor3f(1.0f, 0.0f, 0.0f);
	glutSolidSphere(69600000, 20, 20);
	// 绘制蓝色的“地球”
	glColor3f(0.0f, 0.0f, 1.0f);
	glRotatef(day/360.0*360.0, 0.0f, 0.0f, -1.0f);
	glTranslatef(150000000, 0.0f, 0.0f);
	glutSolidSphere(15945000, 20, 20);
	// 绘制黄色的“月亮”
	glColor3f(1.0f, 1.0f, 0.0f);
	glRotatef(day/30.0*360.0 - day/360.0*360.0, 0.0f, 0.0f, -1.0f);
	glTranslatef(38000000, 0.0f, 0.0f);
	glutSolidSphere(4345000, 20, 20);

	glFlush();

}



int main(int argc, char *argv[])
{
	/// glutInit，对GLUT进行初始化，这个函数必须在其它的GLUT使用之前调用一次。其格式比较死板，一般照抄这句glutInit(&argc, argv)就可以了。
	glutInit(&argc, argv);	
	/// 设置显示方式，其中GLUT_RGB表示使用RGB颜色，与之对应的还有GLUT_INDEX（表示使用索引颜色）。GLUT_SINGLE表示使用单缓冲，与之对应的还有GLUT_DOUBLE（使用双缓冲）
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	/// 设置窗口在屏幕中的位置。
	glutInitWindowPosition(100, 100);
	/// 设置窗口的大小。
	glutInitWindowSize(400, 400);
	/// 根据前面设置的信息创建窗口。参数将被作为窗口的标题。注意：窗口被创建后，并不立即显示到屏幕上。需要调用glutMainLoop才能看到窗口。
	glutCreateWindow("第一个OpenGL程序");
	/// 设置一个函数，当需要进行画图时，这个函数就会被调用。
	glutDisplayFunc(&myDisplay);
	/// 进行一个消息循环
	glutMainLoop();
	return 0;
}

