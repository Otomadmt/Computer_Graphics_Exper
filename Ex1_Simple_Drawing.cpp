#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//预定义一些常用语句以提高可读性
#define _RED_COLOR_ _A_;glColor3ub(255, 0, 0)
#define _GREEN_COLOR_ _A_;glColor3ub(0, 255, 0)
#define _BLUE_COLOR_ _A_;glColor3ub(0, 0, 255)
#define _RANDOM_COLOR_ _A_+1;glColor3ub((char)rand() % 256, (char)rand() % 256, (char)rand() % 256)
#define _A_ sgn_sop=0
#define _FILLED_OR_LINE_	if(fill_state){glBegin(GL_LINE_LOOP);}else{glBegin(GL_POLYGON);}

GLsizei wh = 600, ww = 800; //初始窗口大小
GLfloat size = 3.0;   //初始化模式6下的方块大小
int status = 2,npnt=0;//status指定当前绘图模式,npnt记录当前图元已经确定的坐标点
float xm, ym, xmm, ymm;//临时储存坐标以实现Rubber效果
char sgn_sop = 0,fill_state=0;//sgn_sop用于指示是否每次绘图都随机改变颜色,fill_state指示多边形填充方式
const float n_round = 256;//分割圆形的边数
const float PI = 3.1415926;//常量PI

void main_menu(int id);

//窗口坐标结构体
struct vx{
	float x;
	float y;
}vex[4];

//设置颜色
void set_color(int id)
{
	switch (id) {
	case 10:_RED_COLOR_; break;
	case 11:_GREEN_COLOR_; break;
	case 12:_BLUE_COLOR_; break;
	case 13:_RANDOM_COLOR_; break;
	default: break;
	}
}

//模式6下绘制正方形
void drawSquare(int x, int y)
{

	y = wh - y;
	if (sgn_sop)
	{
		glColor3ub((char)rand() % 256, (char)rand() % 256, (char)rand() % 256);
	}
	_FILLED_OR_LINE_
	glVertex2f(x + size, y + size);
	glVertex2f(x - size, y + size);
	glVertex2f(x - size, y - size);
	glVertex2f(x + size, y - size);
	glEnd();
	glFlush();
}

//绘制线段
void drawLine(int x1, int y1,int x2,int y2)
{

	y1 = wh - y1;
	y2 = wh - y2;
	glBegin(GL_LINES);
	glVertex2f(x1,y1);
	glVertex2f(x2,y2);
	glEnd();
	glFlush();
}

//绘制三角形
void drawTriangle(int x1, int y1, int x2, int y2,int x3,int y3)
{

	y1 = wh - y1;
	y2 = wh - y2;
	y3 = wh - y3;
	_FILLED_OR_LINE_
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glVertex2f(x3, y3);
	glEnd();
	glFlush();
}

//通过多边形模拟绘制圆形
void drawCircle(float x1, float y1, float x2, float y2)
{
	int n;
	float ox, oy, r;
	ox = (x1 + x2) / 2.0;//根据起始点之中点确定圆心
	oy = (float)wh-(y1 + y2) / 2.0;
	r = sqrt((x2-x1)*(x2 - x1)+ (y2 - y1)*(y2 - y1))/2.0;//根据起始点距离确定半径
	_FILLED_OR_LINE_
	for (n = 0; n <= n_round; n++)
	{
		glVertex2f(ox + r*cos( 2 * PI * n / n_round), oy + r*sin(n / n_round * 2 * PI));
	}
	glEnd();
	glFlush();
}


//改变窗口大小时调整视窗体属性
void reshape(GLsizei w, GLsizei h)
{

/* adjust clipping box */

   	glMatrixMode(GL_PROJECTION);
   	glLoadIdentity();
   	glOrtho(0.0, (GLdouble)w, 0.0, (GLdouble)h, -1.0, 1.0);
   	glMatrixMode(GL_MODELVIEW);
   	glLoadIdentity();

/* adjust viewport and clear */

	glViewport(0,0,w,h);
    glClearColor (1.0, 1.0, 1.0, 1.0);
   	glClear(GL_COLOR_BUFFER_BIT);
   	glFlush();

/* set global size for use by drawing routine */

	ww = w;
   	wh = h;
}

//初始化
void init(void)
{

   	glViewport(0,0,ww,wh);

	/* Pick 2D clipping window to match size of screen window. This choice avoids having to scale object coordinates each time window is resized */
   	glMatrixMode(GL_PROJECTION);
   	glLoadIdentity();
   	glOrtho(0.0, (GLdouble) ww , 0.0, (GLdouble) wh , -1.0, 1.0);

   	glClearColor (1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

//鼠标动作回调函数
void mousemotion(int px, int py)
{
	if (px&&py&&status==6)
	{
		drawSquare(px, py);
	}
}

//Passive状态鼠标动作回调函数,用于实现Rubber效果
void passmmo(int x, int y)
{
	xmm = xm;
	ymm = ym;
	if(x)xm=x;
	if(y)ym=y;
	if (npnt)//根据npnt与status的值判断当前图形是否绘制完成
	{
		switch (status)
		{
		case 2:
			if (xmm&&ymm)
			{
				drawLine(vex[0].x, vex[0].y, xmm, ymm);
			}
			   drawLine(vex[0].x, vex[0].y, xm, ym); break;
		case 3:
			if (xmm&&ymm)
			{
				glRectf(vex[0].x, wh-vex[0].y, xmm, wh-ymm);
				glFlush();
			}
			glRectf(vex[0].x, wh-vex[0].y, xm, wh-ym);
			glFlush();break;
		case 4:
				if (xmm&&ymm)
				{
					drawCircle(vex[0].x, vex[0].y, xmm, ymm);
				}
				drawCircle(vex[0].x, vex[0].y, xm, ym); break;
		case 5:
				if (npnt == 2)
				{
					if (xmm&&ymm)
					{
						drawTriangle(vex[0].x, vex[0].y, vex[1].x, vex[1].y, xmm, ymm);
					}
					drawTriangle(vex[0].x, vex[0].y, vex[1].x, vex[1].y, xm, ym); break;
				}

		}
	}
}

//鼠标点击回调函数
void mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (npnt==0)
		{
			if (status != 6)
			{
				vex[0].x = x;
				vex[0].y = y;
				npnt++;
				glLogicOp(GL_XOR);//XOR写入实现Rubber效果
			}else
			{
				drawSquare(x, y);
				npnt = 0;
				return;
			}
		}
		else if (npnt == 1)
		{
			vex[1].x = x;
			vex[1].y = y;
			npnt++;
			if (status == 2)
			{
				drawLine(vex[0].x, vex[0].y, xm,ym);
				glLogicOp(GL_COPY);
				drawLine(vex[0].x, vex[0].y, vex[1].x, vex[1].y);
				npnt = 0;
			}
			else if(status==3)
			{
				glRectf(vex[0].x, wh-vex[0].y, xm, wh-ym); glFlush();
				glLogicOp(GL_COPY);
				glRectf(vex[0].x, wh-vex[0].y, vex[1].x, wh-vex[1].y); glFlush();
				npnt = 0;
			}
			else if (status == 4)
			{
				drawCircle(vex[0].x, vex[0].y, xm, ym);
				glLogicOp(GL_COPY);
				drawCircle(vex[0].x, vex[0].y, vex[1].x,vex[1].y);
				npnt = 0;
			}
		}
		else if (npnt == 2 && status == 5)
		{
			vex[2].x = x;
			vex[2].y = y;
			npnt++;
			drawTriangle(vex[0].x, vex[0].y, vex[1].x, vex[1].y, xm, ym); glFlush();
			glLogicOp(GL_COPY);
			drawTriangle(vex[0].x, vex[0].y, vex[1].x, vex[1].y, vex[2].x, vex[2].y); glFlush();
			npnt = 0;
		}
	}
}

//主菜单调用
void main_menu(int id)
{
    switch(id)
    {
    case 0: exit(0);
			break;
    case 1: glClear(GL_COLOR_BUFFER_BIT);
			glFlush();
			break;
	case 7:fill_state = 0; break;
	case 8:fill_state++; break;
	case 10:case 11:case 12:case 13:
		set_color(id);
		break;
	case 3831:
		printf("An OpenGL Experiment by X. Huang.,\nComputer Science Dept., Xiamen University @2016.\nAll rights not reserved : ) \n"); break;
	default:status = id; npnt = 0; break;
    }
    glutPostRedisplay();
}

//键盘快捷键
void keyboard(unsigned char key, int x, int y)
{
	if (key >= '0'&&key <= '6')
	{
		main_menu(key - '0');
		return;
	}
   switch (key) {
    case 27:case 'Q':case 'q':exit(0);break;//Press 'ESC' or 'Q' to quit.
	case'l':case'L':size = 2 * size;break;//Press 'L' to enlarge the size of simple square under mode 'Square on Path',
	case's':case'S':						// and 'S' to shrink it.
		  if (size > 1)
			   {
				   size = size/2;}break;
	case'r':case'R':_RED_COLOR_; break;//'R' for change color to Red,
	case'g':case'G':_GREEN_COLOR_; break;//'G' for change color to Green,
	case'b':case'B':_BLUE_COLOR_; break;//'B' for change color to Blue,
	case'w':case'W':_RANDOM_COLOR_; break;//and 'W' to change color to another - whatever it is.
   }
}

//显示函数
void display(void)
{}

int main(int argc, char** argv)
{
int sub_menu_set_type, sub_menu_set_color,sub_menu_set_fill;

   	glutInit(&argc,argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(ww,wh);
	glutCreateWindow("Simple Drawer - CG Experiment");
   	init ();
   	glutReshapeFunc (reshape);
   	glutMouseFunc (mouse);
	glutPassiveMotionFunc(passmmo);
	glutMotionFunc(mousemotion);
	glutDisplayFunc(display);
    glutKeyboardFunc (keyboard);
	glEnable(GL_COLOR_LOGIC_OP);

	//设置右键子菜单
    sub_menu_set_type = glutCreateMenu(main_menu);
    glutAddMenuEntry("Line(2)",2);
    glutAddMenuEntry("Rectangle(3)",3);
	glutAddMenuEntry("Circle(4)",4);
    glutAddMenuEntry("Triangle(5)",5);
	glutAddMenuEntry("Square On Path(6)",6);

	sub_menu_set_color = glutCreateMenu(main_menu);
    glutAddMenuEntry("Red(R)",10);
    glutAddMenuEntry("Green(G)",11);
	glutAddMenuEntry("Blue(B)",12);
	glutAddMenuEntry("Random(W)",13);

	sub_menu_set_fill = glutCreateMenu(main_menu);
	glutAddMenuEntry("Filled", 7);
	glutAddMenuEntry("Lines Only", 8);

	glutCreateMenu(main_menu);
    glutAddSubMenu("Set Type", sub_menu_set_type);
	glutAddSubMenu("Set Color", sub_menu_set_color);
	glutAddSubMenu("Set Fill Method", sub_menu_set_fill);
	glutAddMenuEntry("Clear(1)", 1);
	glutAddMenuEntry("About", 3831);
	glutAddMenuEntry("Exit(0/Q/Esc)", 0);

	//初始参数
    glutAttachMenu(GLUT_RIGHT_BUTTON);
	glLogicOp(GL_COPY);
	glColor3ub((char)rand() % 256, (char)rand() % 256, (char)rand() % 256);

	//在控制台打印用户帮助
	printf("Default Settings\n-------------------\n Drawing Line/ Filled Polygon/ Pixel Writing Method: COPY\n");
	printf("\nShort Cuts\n-------------------\nSystem: 0/Q/Esc - Quit 1 - Clear\nColor : R - Red B - Blue G - Green W - Random\nType  : 2 - Line 3 - Rectangle 4 - Circle\n        5 - Triangle 6 - Square on Mouse Path(L - Larger S - Smaller)\n");

	//进入主函数循环
   	glutMainLoop();
    return 0;
}
