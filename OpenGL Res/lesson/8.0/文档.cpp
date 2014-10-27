我们已经知道，使用OpenGL其实只要调用一系列的OpenGL函数就可以了。然而，这种方式在一些时候可能导致问题。
比如某个画面中，使用了数千个多边形来表现一个比较真实的人物，OpenGL为了产生这数千个多边形，就需要不停
的调用glVertex*函数，每一个多边形将至少调用三次（因为多边形至少有三个顶点），于是绘制一个比较真实的人
物就需要调用上万次的glVertex*函数。更糟糕的是，如果我们需要每秒钟绘制60幅画面，则每秒调用的glVertex*函数次数就会超过数十万次，乃至接近百万次。这样的情况是我们所不愿意看到的。
同时，考虑这样一段代码：
const int segments = 100;
const GLfloat pi = 3.14f;
int i;
glLineWidth(10.0);
glBegin(GL_LINE_LOOP);
for(i=0; i<segments; ++i)
{
    GLfloat tmp = 2 * pi * i / segments;
    glVertex2f(cos(tmp), sin(tmp));
}
glEnd();

这段代码将绘制一个圆环。如果我们在每次绘制图象时调用这段代码，则虽然可以达到绘制圆环的目的，但是cos、
sin等开销较大的函数被多次调用，浪费了CPU资源。如果每一个顶点不是通过cos、sin等函数得到，而是使用更复
杂的运算方式来得到，则浪费的现象就更加明显。

经过分析，我们可以发现上述两个问题的共同点：程序多次执行了重复的工作，导致CPU资源浪费和运行速度的下降。
使用显示列表可以较好的解决上述两个问题。
在编写程序时，遇到重复的工作，我们往往是将重复的工作编写为函数，在需要的地方调用它。类似的，在编写OpenGL
程序时，遇到重复的工作，可以创建一个显示列表，把重复的工作装入其中，并在需要的地方调用这个显示列表。
使用显示列表一般有四个步骤：分配显示列表编号、创建显示列表、调用显示列表、销毁显示列表。

一、分配显示列表编号
OpenGL允许多个显示列表同时存在，就好象C语言允许程序中有多个函数同时存在。C语言中，不同的函数用不同的
名字来区分，而在OpenGL中，不同的显示列表用不同的正整数来区分。
你可以自己指定一些各不相同的正整数来表示不同的显示列表。但是如果你不够小心，可能出现一个显示列表将另
一个显示列表覆盖的情况。为了避免这一问题，使用glGenLists函数来自动分配一个没有使用的显示列表编号。
glGenLists函数有一个参数i，表示要分配i个连续的未使用的显示列表编号。返回的是分配的若干连续编号中最小
的一个。例如，glGenLists(3);如果返回20，则表示分配了20、21、22这三个连续的编号。如果函数返回零，表示
分配失败。
可以使用glIsList函数判断一个编号是否已经被用作显示列表。

二、创建显示列表

创建显示列表实际上就是把各种OpenGL函数的调用装入到显示列表中。使用glNewList开始装入，使用glEndList结
束装入。glNewList有两个参数，第一个参数是一个正整数表示装入到哪个显示列表。第二个参数有两种取值，如果
为GL_COMPILE，则表示以下的内容只是装入到显示列表，但现在不执行它们；如果为GL_COMPILE_AND_EXECUTE，表
示在装入的同时，把装入的内容执行一遍。
例如，需要把“设置颜色为红色，并且指定一个坐标为(0, 0)的顶点”这两条命令装入到编号为list的显示列表中，
并且在装入的时候不执行，则可以用下面的代码：
glNewList(list, GL_COMPILE);
glColor3f(1.0f, 0.0f, 0.0f);
glVertex2f(0.0f, 0.0f);
glEnd();

注意：显示列表只能装入OpenGL函数，而不能装入其它内容。例如：
int i = 3;
glNewList(list, GL_COMPILE);
if( i > 20 )
    glColor3f(1.0f, 0.0f, 0.0f);
glVertex2f(0.0f, 0.0f);
glEnd();
其中if这个判断就没有被装入到显示列表。以后即使修改i的值，使i>20的条件成立，则glColor3f这个函数也不会
被执行。因为它根本就不存在于显示列表中。

另外，并非所有的OpenGL函数都可以装入到显示列表中。例如，各种用于查询的函数，它们无法被装入到显示列表，
因为它们都具有返回值，而glCallList和glCallLists函数都不知道如何处理这些返回值。在网络方式下，设置客户
端状态的函数也无法被装入到显示列表，这是因为显示列表被保存到服务器端，各种设置客户端状态的函数在发送
到服务器端以前就被执行了，而服务器端无法执行这些函数。分配、创建、删除显示列表的动作也无法被装入到另
一个显示列表，但调用显示列表的动作则可以被装入到另一个显示列表。

三、调用显示列表
使用glCallList函数可以调用一个显示列表。该函数有一个参数，表示要调用的显示列表的编号。例如，要调用编
号为10的显示列表，直接使用glCallList(10);就可以了。
使用glCallLists函数可以调用一系列的显示列表。该函数有三个参数，第一个参数表示了要调用多少个显示列表。
第二个参数表示了这些显示列表的编号的储存格式，可以是GL_BYTE（每个编号用一个GLbyte表示），GL_UNSIGNED_BYTE
（每个编号用一个GLubyte表示），GL_SHORT，GL_UNSIGNED_SHORT，GL_INT，GL_UNSIGNED_INT，GL_FLOAT。第三个
参数表示了这些显示列表的编号所在的位置。在使用该函数前，需要用glListBase函数来设置一个偏移量。假设偏
移量为k，且glCallLists中要求调用的显示列表编号依次为l1, l2, l3, ...，则实际调用的显示列表为l1+k, l2+k, l3+k, ...。
例如：
GLuint lists[] = {1, 3, 4, 8};
glListBase(10);
glCallLists(4, GL_UNSIGNED_INT, lists);
则实际上调用的是编号为11, 13, 14, 18的四个显示列表。
注：“调用显示列表”这个动作本身也可以被装在另一个显示列表中。

四、销毁显示列表
销毁显示列表可以回收资源。使用glDeleteLists来销毁一串编号连续的显示列表。
例如，使用glDeleteLists(20, 4);将销毁20，21，22，23这四个显示列表。

使用显示列表将会带来一些开销，例如，把各种动作保存到显示列表中会占用一定数量的内存资源。但如果使用得
当，显示列表可以提升程序的性能。这主要表现在以下方面：
1、明显的减少OpenGL函数的调用次数。如果函数调用是通过网络进行的（Linux等操作系统支持这样的方式，即由应
用程序在客户端发出OpenGL请求，由网络上的另一台服务器进行实际的绘图操作），将显示列表保存在服务器端，可
以大大减少网络负担。
2、保存中间结果，避免一些不必要的计算。例如前面的样例程序中，cos、sin函数的计算结果被直接保存到显示列表
中，以后使用时就不必重复计算。
3、便于优化。我们已经知道，使用glTranslate*、glRotate*、glScale*等函数时，实际上是执行矩阵乘法操作，由
于这些函数经常被组合在一起使用，通常会出现矩阵的连乘。这时，如果把这些操作保存到显示列表中，则一些复杂
的OpenGL版本会尝试先计算出连乘的一部分结果，从而提高程序的运行速度。在其它方面也可能存在类似的例子。
同时，显示列表也为程序的设计带来方便。我们在设置一些属性时，经常把一些相关的函数放在一起调用，（比如，
把设置光源的各种属性的函数放到一起）这时，如果把这些设置属性的操作装入到显示列表中，则可以实现属性的成
组的切换。
当然了，即使使用显示列表在某些情况下可以提高性能，但这种提高很可能并不明显。毕竟，在硬件配置和大致的软
件算法都不变的前提下，性能可提升的空间并不大。

显示列表的内容就是这么多了，下面我们看一个例子。
假设我们需要绘制一个旋转的彩色正四面体，则可以这样考虑：设置一个全局变量angle，然后让它的值不断的增加（
到达360后又恢复为0，周而复始）。每次需要绘制图形时，根据angle的值进行旋转，然后绘制正四面体。这里正四面
体采用显示列表来实现，即把绘制正四面体的若干OpenGL函数装到一个显示列表中，然后每次需要绘制时，调用这个显示列表即可。
将正四面体的四个顶点颜色分别设置为红、黄、绿、蓝，通过数学计算，将坐标设置为：
(-0.5, -5*sqrt(5)/48,  sqrt(3)/6),
( 0.5, -5*sqrt(5)/48,  sqrt(3)/6),
(   0, -5*sqrt(5)/48, -sqrt(3)/3),
(   0, 11*sqrt(6)/48,          0)
2007年4月24日修正：以上结果有误，通过计算AB, AC, AD, BC, BD, CD的长度，发现AD, BD, CD的长度与1.0有较大偏差。正确的坐标应该是：
   A点：(  0.5,   -sqrt(6)/12, -sqrt(3)/6)
   B点：( -0.5,   -sqrt(6)/12, -sqrt(3)/6)
   C点：(    0,   -sqrt(6)/12,  sqrt(3)/3)
   D点：(    0,    sqrt(6)/4,           0)
   程序代码中也做了相应的修改
   
   在程序中，我们将绘制正四面体的OpenGL函数装到了一个显示列表中，但是，关于旋转的操作却在显示列表之外
   进行。这是因为如果把旋转的操作也装入到显示列表，则每次旋转的角度都是一样的，不会随着angle的值的变化
   而变化，于是就不能表现出动态的旋转效果了。
程序运行时，可能感觉到画面的立体感不足，这主要是因为没有使用光照的缘故。如果将glColor3fv函数去掉，改为
设置各种材质，然后开启光照效果，则可以产生更好的立体感。大家可以自己试着使用光照效果，唯一需要注意的
地方就是法线向量的计算。由于这里的正四面体四个顶点坐标选取得比较特殊，使得正四面体的中心坐标正好是(0, 0, 0)，
因此，每三个顶点坐标的平均值正好就是这三个顶点所组成的平面的法线向量的值。
void setNormal(GLfloat* Point1, GLfloat* Point2, GLfloat* Point3)
{
    GLfloat normal[3];
    int i;
    for(i=0; i<3; ++i)
        normal[i] = (Point1[i]+Point2[i]+Point3[i]) / 3;
    glNormal3fv(normal);
}

限于篇幅，这里就不给出完整的程序了。不过，大家可以自行尝试，看看使用光照后效果有何种改观。尤其是注意
四面体各个表面交界的位置，在未使用光照前，几乎看不清轮廓，在使用光照后，可比较容易的区分各个平面，因
此立体感得到加强。（见图1，图2）当然了，这样的效果还不够。如果在各表面的交界处设置很多细小的平面，进
行平滑处理，则光照后的效果将更真实。但这已经远离本课的内容了。
小结
本课介绍了显示列表的知识和简单的应用。
可以把各种OpenGL函数调用的动作装到显示列表中，以后调用显示列表，就相当于调用了其中的OpenGL函数。显示
列表中除了存放对OpenGL函数的调用外，不会存放其它内容。
使用显示列表的过程是：分配一个未使用的显示列表编号，把OpenGL函数调用装入显示列表，调用显示列表，销毁
显示列表。
使用显示列表有可能带来程序运行速度的提升，但是这种提升并不一定会很明显。显示列表本身也存在一定的开销。
把绘制固定的物体的OpenGL函数放到一个显示列表中，是一种不错的编程思路。本课最后的例子中使用了这种思路。