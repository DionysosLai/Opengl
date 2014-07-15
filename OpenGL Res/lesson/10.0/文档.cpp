BMP文件与像素操作

1、BMP文件格式简单介绍
BMP文件是一种像素文件，它保存了一幅图象中所有的像素。这种文件格式可以保存单色位图、16色或256色索引模
式像素图、24位真彩色图象，每种模式种单一像素的大小分别为1/8 字节，1/2 字节，1字节和3字节。目前最常见的
是256色BMP和24位色BMP。这种文件格式还定义了像素保存的几种方法，包括不压缩、RLE压缩等。常见的BMP文件大多是不压缩的。
这里为了简单起见，我们仅讨论24位色、不使用压缩的BMP。（如果你使用Windows自带的画图程序，很容易绘制出一个符合以上要求的BMP）
Windows所使用的BMP文件，在开始处有一个文件头，大小为54字节。保存了包括文件格式标识、颜色数、图象大小、
压缩方式等信息，因为我们仅讨论24位色不压缩的BMP，所以文件头中的信息基本不需要注意，只有“大小”这一项对
我们比较有用。图象的宽度和高度都是一个32位整数，在文件中的地址分别为0x0012和0x0016，于是我们可以使用以下代码来读取图象的大小信息：

GLint width, height; // 使用OpenGL的GLint类型，它是32位的。
                     // 而C语言本身的int则不一定是32位的。
FILE* pFile;
// 在这里进行“打开文件”的操作
fseek(pFile, 0x0012, SEEK_SET);         // 移动到0x0012位置
fread(&width, sizeof(width), 1, pFile); // 读取宽度
fseek(pFile, 0x0016, SEEK_SET);         // 移动到0x0016位置
                                        // 由于上一句执行后本就应该在0x0016位置
                                        // 所以这一句可省略
fread(&height, sizeof(height), 1, pFile); // 读取高度

54个字节以后，如果是16色或256色BMP，则还有一个颜色表，但24位色BMP没有这个，我们这里不考虑。接下来就是
实际的像素数据了。24位色的BMP文件中，每三个字节表示一个像素的颜色。
注意，OpenGL通常使用RGB来表示颜色，但BMP文件则采用BGR，就是说，顺序被反过来了。
另外需要注意的地方是：像素的数据量并不一定完全等于图象的高度乘以宽度乘以每一像素的字节数，而是可能略
大于这个值。原因是BMP文件采用了一种“对齐”的机制，每一行像素数据的长度若不是4的倍数，则填充一些数据使
它是4的倍数。这样一来，一个17*15的24位BMP大小就应该是834字节（每行17个像素，有51字节，补充为52字节，
乘以15得到像素数据总长度780，再加上文件开始的54字节，得到834字节）。分配内存时，一定要小心，不能直接
使用“图象的高度乘以宽度乘以每一像素的字节数”来计算分配空间的长度，否则有可能导致分配的内存空间长度不
足，造成越界访问，带来各种严重后果。
一个很简单的计算数据长度的方法如下：

int LineLength, TotalLength;
LineLength = ImageWidth * BytesPerPixel; // 每行数据长度大致为图象宽度乘以
                                         // 每像素的字节数
while( LineLength % 4 != 0 )             // 修正LineLength使其为4的倍数
    ++LineLenth;
TotalLength = LineLength * ImageHeight;  // 数据总长 = 每行长度 * 图象高度

这并不是效率最高的方法，但由于这个修正本身运算量并不大，使用频率也不高，我们就不需要再考虑更快的方法了。

2、简单的OpenGL像素操作
OpenGL提供了简洁的函数来操作像素：
glReadPixels：读取一些像素。当前可以简单理解为“把已经绘制好的像素（它可能已经被保存到显卡的显存中）读取到内存”。
glDrawPixels：绘制一些像素。当前可以简单理解为“把内存中一些数据作为像素数据，进行绘制”。
glCopyPixels：复制一些像素。当前可以简单理解为“把已经绘制好的像素从一个位置复制到另一个位置”。虽然从
功能上看，好象等价于先读取像素再绘制像素，但实际上它不需要把已经绘制的像素（它可能已经被保存到显卡的
显存中）转换为内存数据，然后再由内存数据进行重新的绘制，所以要比先读取后绘制快很多。
这三个函数可以完成简单的像素读取、绘制和复制任务，但实际上也可以完成更复杂的任务。当前，我们仅讨论一
些简单的应用。由于这几个函数的参数数目比较多，下面我们分别介绍。

3、glReadPixels的用法和举例
3.1 函数的参数说明
该函数总共有七个参数。前四个参数可以得到一个矩形，该矩形所包括的像素都会被读取出来。（第一、二个参数
表示了矩形的左下角横、纵坐标，坐标以窗口最左下角为零，最右上角为最大值；第三、四个参数表示了矩形的宽
度和高度）
第五个参数表示读取的内容，例如：GL_RGB就会依次读取像素的红、绿、蓝三种数据，GL_RGBA则会依次读取像素的
红、绿、蓝、alpha四种数据，GL_RED则只读取像素的红色数据（类似的还有GL_GREEN，GL_BLUE，以及GL_ALPHA）。
如果采用的不是RGBA颜色模式，而是采用颜色索引模式，则也可以使用GL_COLOR_INDEX来读取像素的颜色索引。目
前仅需要知道这些，但实际上还可以读取其它内容，例如深度缓冲区的深度数据等。
第六个参数表示读取的内容保存到内存时所使用的格式，例如：GL_UNSIGNED_BYTE会把各种数据保存为GLubyte，
GL_FLOAT会把各种数据保存为GLfloat等。
第七个参数表示一个指针，像素数据被读取后，将被保存到这个指针所表示的地址。注意，需要保证该地址有足够
的可以使用的空间，以容纳读取的像素数据。例如一幅大小为256*256的图象，如果读取其RGB数据，且每一数据被
保存为GLubyte，总大小就是：256*256*3 = 196608字节，即192千字节。如果是读取RGBA数据，则总大小就是256*256*4 = 262144字节，即256千字节。

注意：glReadPixels实际上是从缓冲区中读取数据，如果使用了双缓冲区，则默认是从正在显示的缓冲（即前缓冲）
中读取，而绘制工作是默认绘制到后缓冲区的。因此，如果需要读取已经绘制好的像素，往往需要先交换前后缓冲。

再看前面提到的BMP文件中两个需要注意的地方：
3.2 解决OpenGL常用的RGB像素数据与BMP文件的BGR像素数据顺序不一致问题
可以使用一些代码交换每个像素的第一字节和第三字节，使得RGB的数据变成BGR的数据。当然也可以使用另外的方
式解决问题：新版本的OpenGL除了可以使用GL_RGB读取像素的红、绿、蓝数据外，也可以使用GL_BGR按照相反的顺
序依次读取像素的蓝、绿、红数据，这样就与BMP文件格式相吻合了。即使你的gl/gl.h头文件中没有定义这个GL_BGR，
也没有关系，可以尝试使用GL_BGR_EXT。虽然有的OpenGL实现（尤其是旧版本的实现）并不能使用GL_BGR_EXT，但
我所知道的Windows环境下各种OpenGL实现都对GL_BGR提供了支持，毕竟Windows中各种表示颜色的数据几乎都是使
用BGR的顺序，而非RGB的顺序。这可能与IBM-PC的硬件设计有关。

3.3 消除BMP文件中“对齐”带来的影响
实际上OpenGL也支持使用了这种“对齐”方式的像素数据。只要通过glPixelStore修改“像素保存时对齐的方式”就可
以了。像这样：
int alignment = 4;
glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
第一个参数表示“设置像素的对齐值”，第二个参数表示实际设置为多少。这里像素可以单字节对齐（实际上就是不
使用对齐）、双字节对齐（如果长度为奇数，则再补一个字节）、四字节对齐（如果长度不是四的倍数，则补为四
的倍数）、八字节对齐。分别对应alignment的值为1, 2, 4, 8。实际上，默认的值是4，正好与BMP文件的对齐方式相吻合。
glPixelStorei也可以用于设置其它各种参数。但我们这里并不需要深入讨论了。


现在，我们已经可以把屏幕上的像素读取到内存了，如果需要的话，我们还可以将内存中的数据保存到文件。正确
的对照BMP文件格式，我们的程序就可以把屏幕中的图象保存为BMP文件，达到屏幕截图的效果。
我们并没有详细介绍BMP文件开头的54个字节的所有内容，不过这无伤大雅。从一个正确的BMP文件中读取前54个字
节，修改其中的宽度和高度信息，就可以得到新的文件头了。假设我们先建立一个1*1大小的24位色BMP，文件名为dummy.bmp，
又假设新的BMP文件名称为grab.bmp。则可以编写如下代码：

FILE* pOriginFile = fopen("dummy.bmp", "rb);
FILE* pGrabFile = fopen("grab.bmp", "wb");
char  BMP_Header[54];
GLint width, height;

/* 先在这里设置好图象的宽度和高度，即width和height的值，并计算像素的总长度 */

// 读取dummy.bmp中的头54个字节到数组
fread(BMP_Header, sizeof(BMP_Header), 1, pOriginFile);
// 把数组内容写入到新的BMP文件
fwrite(BMP_Header, sizeof(BMP_Header), 1, pGrabFile);

// 修改其中的大小信息
fseek(pGrabFile, 0x0012, SEEK_SET);
fwrite(&width, sizeof(width), 1, pGrabFile);
fwrite(&height, sizeof(height), 1, pGrabFile);

// 移动到文件末尾，开始写入像素数据
fseek(pGrabFile, 0, SEEK_END);

/* 在这里写入像素数据到文件 */

fclose(pOriginFile);
fclose(pGrabFile);

我们给出完整的代码，演示如何把整个窗口的图象抓取出来并保存为BMP文件。

#define WindowWidth  400
#define WindowHeight 400

#include <stdio.h>
#include <stdlib.h>

/* 函数grab
 * 抓取窗口中的像素
 * 假设窗口宽度为WindowWidth，高度为WindowHeight
 */
#define BMP_Header_Length 54
void grab(void)
{
    FILE*    pDummyFile;
    FILE*    pWritingFile;
    GLubyte* pPixelData;
    GLubyte  BMP_Header[BMP_Header_Length];
    GLint    i, j;
    GLint    PixelDataLength;

    // 计算像素数据的实际长度
    i = WindowWidth * 3;   // 得到每一行的像素数据长度
    while( i%4 != 0 )      // 补充数据，直到i是的倍数
        ++i;               // 本来还有更快的算法，
                           // 但这里仅追求直观，对速度没有太高要求
    PixelDataLength = i * WindowHeight;

    // 分配内存和打开文件
    pPixelData = (GLubyte*)malloc(PixelDataLength);
    if( pPixelData == 0 )
        exit(0);

    pDummyFile = fopen("dummy.bmp", "rb");
    if( pDummyFile == 0 )
        exit(0);

    pWritingFile = fopen("grab.bmp", "wb");
    if( pWritingFile == 0 )
        exit(0);

    // 读取像素
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glReadPixels(0, 0, WindowWidth, WindowHeight,
        GL_BGR_EXT, GL_UNSIGNED_BYTE, pPixelData);

    // 把dummy.bmp的文件头复制为新文件的文件头
    fread(BMP_Header, sizeof(BMP_Header), 1, pDummyFile);
    fwrite(BMP_Header, sizeof(BMP_Header), 1, pWritingFile);
    fseek(pWritingFile, 0x0012, SEEK_SET);
    i = WindowWidth;
    j = WindowHeight;
    fwrite(&i, sizeof(i), 1, pWritingFile);
    fwrite(&j, sizeof(j), 1, pWritingFile);

    // 写入像素数据
    fseek(pWritingFile, 0, SEEK_END);
    fwrite(pPixelData, PixelDataLength, 1, pWritingFile);

    // 释放内存和关闭文件
    fclose(pDummyFile);
    fclose(pWritingFile);
    free(pPixelData);
}


把这段代码复制到以前任何课程的样例程序中，在绘制函数的最后调用grab函数，即可把图象内容保存为BMP文件了。
（在我写这个教程的时候，不少地方都用这样的代码进行截图工作，这段代码一旦写好，运行起来是很方便的。）

4、glDrawPixels的用法和举例
glDrawPixels函数与glReadPixels函数相比，参数内容大致相同。它的第一、二、三、四个参数分别对应于glReadPixels函数的第三、四、五、六个参数，依次表示图象宽度、图象高度、像素数据内容、像素数据在内存中的格式。两个函数的最后一个参数也是对应的，glReadPixels中表示像素读取后存放在内存中的位置，glDrawPixels则表示用于绘制的像素数据在内存中的位置。
注意到glDrawPixels函数比glReadPixels函数少了两个参数，这两个参数在glReadPixels中分别是表示图象的起始位置。在glDrawPixels中，不必显式的指定绘制的位置，这是因为绘制的位置是由另一个函数glRasterPos*来指定的。glRasterPos*函数的参数与glVertex*类似，通过指定一个二维/三维/四维坐标，OpenGL将自动计算出该坐标对应的屏幕位置，并把该位置作为绘制像素的起始位置。
很自然的，我们可以从BMP文件中读取像素数据，并使用glDrawPixels绘制到屏幕上。我们选择Windows XP默认的桌面背景Bliss.bmp作为绘制的内容（如果你使用的是Windows XP系统，很可能可以在硬盘中搜索到这个文件。当然你也可以使用其它BMP文件来代替，只要它是24位的BMP文件。注意需要修改代码开始部分的FileName的定义），先把该文件复制一份放到正确的位置，我们在程序开始时，就读取该文件，从而获得图象的大小后，根据该大小来创建合适的OpenGL窗口，并绘制像素。
绘制像素本来是很简单的过程，但是这个程序在骨架上与前面的各种示例程序稍有不同，所以我还是打算给出一份完整的代码。

#include <gl/glut.h>

#define FileName "Bliss.bmp"

static GLint    ImageWidth;
static GLint    ImageHeight;
static GLint    PixelLength;
static GLubyte* PixelData;

#include <stdio.h>
#include <stdlib.h>

void display(void)
{
    // 清除屏幕并不必要
    // 每次绘制时，画面都覆盖整个屏幕
    // 因此无论是否清除屏幕，结果都一样
    // glClear(GL_COLOR_BUFFER_BIT);

    // 绘制像素
    glDrawPixels(ImageWidth, ImageHeight,
        GL_BGR_EXT, GL_UNSIGNED_BYTE, PixelData);

    // 完成绘制
    glutSwapBuffers();
}

int main(int argc, char* argv[])
{
    // 打开文件
    FILE* pFile = fopen("Bliss.bmp", "rb");
    if( pFile == 0 )
        exit(0);

    // 读取图象的大小信息
    fseek(pFile, 0x0012, SEEK_SET);
    fread(&ImageWidth, sizeof(ImageWidth), 1, pFile);
    fread(&ImageHeight, sizeof(ImageHeight), 1, pFile);

    // 计算像素数据长度
    PixelLength = ImageWidth * 3;
    while( PixelLength % 4 != 0 )
        ++PixelLength;
    PixelLength *= ImageHeight;

    // 读取像素数据
    PixelData = (GLubyte*)malloc(PixelLength);
    if( PixelData == 0 )
        exit(0);

    fseek(pFile, 54, SEEK_SET);
    fread(PixelData, PixelLength, 1, pFile);

    // 关闭文件
    fclose(pFile);

    // 初始化GLUT并运行
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(ImageWidth, ImageHeight);
    glutCreateWindow(FileName);
    glutDisplayFunc(&display);
    glutMainLoop();

    // 释放内存
    // 实际上，glutMainLoop函数永远不会返回，这里也永远不会到达
    // 这里写释放内存只是出于一种个人习惯
    // 不用担心内存无法释放。在程序结束时操作系统会自动回收所有内存
    free(PixelData);

    return 0;
}


这里仅仅是一个简单的显示24位BMP图象的程序，如果读者对BMP文件格式比较熟悉，也可以写出适用于各种BMP图象
的显示程序，在像素处理时，它们所使用的方法是类似的。
OpenGL在绘制像素之前，可以对像素进行若干处理。最常用的可能就是对整个像素图象进行放大/缩小。使用glPixelZoom
来设置放大/缩小的系数，该函数有两个参数，分别是水平方向系数和垂直方向系数。例如设置glPixelZoom(0.5f, 0.8f);
则表示水平方向变为原来的50%大小，而垂直方向变为原来的80%大小。我们甚至可以使用负的系数，使得整个图象进行水
平方向或垂直方向的翻转（默认像素从左绘制到右，但翻转后将从右绘制到左。默认像素从下绘制到上，但翻转后将从
上绘制到下。因此，glRasterPos*函数设置的“开始位置”不一定就是矩形的左下角）。

5、glCopyPixels的用法和举例
从效果上看，glCopyPixels进行像素复制的操作，等价于把像素读取到内存，再从内存绘制到另一个区域，因此可
以通过glReadPixels和glDrawPixels组合来实现复制像素的功能。然而我们知道，像素数据通常数据量很大，例如
一幅1024*768的图象，如果使用24位BGR方式表示，则需要至少1024*768*3字节，即2.25兆字节。这么多的数据要进
行一次读操作和一次写操作，并且因为在glReadPixels和glDrawPixels中设置的数据格式不同，很可能涉及到数据
格式的转换。这对CPU无疑是一个不小的负担。使用glCopyPixels直接从像素数据复制出新的像素数据，避免了多余
的数据的格式转换，并且也可能减少一些数据复制操作（因为数据可能直接由显卡负责复制，不需要经过主内存），因此效率比较高。
glCopyPixels函数也通过glRasterPos*系列函数来设置绘制的位置，因为不需要涉及到主内存，所以不需要指定数
据在内存中的格式，也不需要使用任何指针。
glCopyPixels函数有五个参数，第一、二个参数表示复制像素来源的矩形的左下角坐标，第三、四个参数表示复制
像素来源的举行的宽度和高度，第五个参数通常使用GL_COLOR，表示复制像素的颜色，但也可以是GL_DEPTH或GL_STENCIL，
分别表示复制深度缓冲数据或模板缓冲数据。
值得一提的是，glDrawPixels和glReadPixels中设置的各种操作，例如glPixelZoom等，在glCopyPixels函数中同样有效。
下面看一个简单的例子，绘制一个三角形后，复制像素，并同时进行水平和垂直方向的翻转，然后缩小为原来的一半，
并绘制。绘制完毕后，调用前面的grab函数，将屏幕中所有内容保存为grab.bmp。其中WindowWidth和WindowHeight是表示窗口宽度和高度的常量。

void display(void)
{
    // 清除屏幕
    glClear(GL_COLOR_BUFFER_BIT);

    // 绘制
    glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f);    glVertex2f(0.0f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f);    glVertex2f(1.0f, 0.0f);
        glColor3f(0.0f, 0.0f, 1.0f);    glVertex2f(0.5f, 1.0f);
    glEnd();
    glPixelZoom(-0.5f, -0.5f);
    glRasterPos2i(1, 1);
    glCopyPixels(WindowWidth/2, WindowHeight/2,
        WindowWidth/2, WindowHeight/2, GL_COLOR);

    // 完成绘制，并抓取图象保存为BMP文件
    glutSwapBuffers();
    grab();
}

小结：
本课结合Windows系统常见的BMP图象格式，简单介绍了OpenGL的像素处理功能。包括使用glReadPixels读取像素、
glDrawPixels绘制像素、glCopyPixels复制像素。
本课仅介绍了像素处理的一些简单应用，但相信大家已经可以体会到，围绕这三个像素处理函数，还存在一些“外围”函数，
比如glPixelStore*，glRasterPos*，以及glPixelZoom等。我们仅使用了这些函数的一少部分功能。
本课内容并不多，例子足够丰富，三个像素处理函数都有例子，大家可以结合例子来体会。