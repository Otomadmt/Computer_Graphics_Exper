#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <fstream> 
#include <string>
#include <iostream>
using namespace std;
#define _MAX_VRX_ 2001//最多点信息数
#define _MAX_NOR_ 1501//最多面信息数
#define _MAX_PATH_ 81//最长文件名长度
#define _CLR_ c%3//颜色指定

float vertices[_MAX_VRX_][3];//存储OFF模型中各点坐标信息
int nFaces, nVertices, nEdges;//OFF模型中点线面总数
GLfloat  scale = 1;//缩放倍数
float normals[_MAX_NOR_][3];//存储OFF模型中各面构成信息
int colorset[3][3] = { { 62,88,94 },{ 58,70,81 },{ 129,133,126 } };//初始颜色(RGB 256表示)

float colors[][3] = { { colorset[0][0] / 255.0,colorset[0][1] / 255.0,colorset[0][2] / 255.0 },
{ colorset[1][0] / 255.0,colorset[1][1] / 255.0,colorset[1][2] / 255.0 },
{ colorset[2][0] / 255.0,colorset[2][1] / 255.0,colorset[2][2] / 255.0 } };//浮点数表示的RGB颜色信息

void polygon(int a, int b, int c)
{
	//绘制三角形图元并染色
	glBegin(GL_POLYGON);
	glColor3fv(colors[_CLR_]);
	glNormal3fv(normals[a]);
	glVertex3fv(vertices[a]);
	glColor3fv(colors[_CLR_]);
	glNormal3fv(normals[b]);
	glVertex3fv(vertices[b]);
	glColor3fv(colors[_CLR_]);
	glNormal3fv(normals[c]);
	glVertex3fv(vertices[c]);
	glEnd();
}

void disp_off()
{
	//绘制每一个面
	int i;
	for (i = 0; i<nFaces; i++)
	{
		polygon((int)normals[i][0], (int)normals[i][1], (int)normals[i][2]);
	}
}

bool load_off(string* str)
{
	//读入OFF模型信息
	std::ifstream fin;
	fin.open(*str);
	if (!fin.is_open())
	{
		printf("File Error!\n");
		return 0;
	}
	std::string off;
	std::getline(fin, off);//读入首行字符串"OFF"

	fin >> nVertices >> nFaces >> nEdges; //读入点、面、边数目
	for (int i = 0; i<nVertices; i++)
	{
		fin >> vertices[i][0] >> vertices[i][1] >> vertices[i][2];    //逐行读入顶点坐标;
	}
	int n;
	for (int i = 0; i<nFaces; i++)
	{
		fin >> n >> normals[i][0] >> normals[i][1] >> normals[i][2];  //逐行读入面的顶点序列;
	}
	fin.close();
	disp_off();
	return 1;
}

static GLfloat Tran[] = { 0,0,0 };//平移矩阵
static GLfloat theta[] = { 0.0,0.0,0.0 };//旋转矩阵
static GLint axis = 2;//轴标记
static GLdouble viewer[] = { 0.0, 0.0, 5.0 }; //初始相机位置

 
void init()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);//清除背景
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glFlush();
}

void display(void)
{
	init();
	
	glLoadIdentity();//读入单位阵
	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);//相机位置变换
	glScalef(scale, scale, scale);//等倍缩放
	glTranslatef(Tran[0], Tran[1], Tran[2]);//平移变换
	glRotatef(theta[0], 1.0, 0.0, 0.0);//绕x轴旋转
	glRotatef(theta[1], 0.0, 1.0, 0.0);//绕y轴旋转
	glRotatef(theta[2], 0.0, 0.0, 1.0);//绕z轴旋转
	glTranslatef(0, 0, 0);//将模型移回原点以执行旋转操作
	disp_off();//绘制图元
	glFlush();
	glutSwapBuffers();
}

void mouse(int btn, int state, int x, int y)
{
	GLfloat thep = 2.0 / scale;//根据缩放尺度调整单次旋转角度
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) axis = 0;
	if (btn == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) axis = 1;
	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) axis = 2;
	theta[axis] += thep;
	if (theta[axis] > 360.0) theta[axis] -= 360.0;
	display();
}

void reset_camera()
{
	viewer[0] = 0;
	viewer[1] = 0;
	viewer[2] = 5;
	cout << "Camera reset finished." << endl;
}

void reset_all()
{
	reset_camera();
	scale = 1;
	theta[0] = 0;
	theta[1] = 0;
	theta[2] = 0;
	Tran[0] = 0;
	Tran[1] = 0;
	Tran[2] = 0;
	cout << "All reset finished." << endl;
}

void reset_color()
{
	//自定义颜色绘图
	cout << "Enter 3 sets of RGB color values you want to set:\n";
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			cin >> colorset[i][j];
			colors[i][j] = colorset[i][j] / 255.0;
		}
	}
	display();
}


void keys(unsigned char key, int x, int y)
{
	double tranp = 0.1 / scale;//根据缩放尺度调整单次相机调整距离
	GLfloat thep = 2.0 / scale;//根据缩放尺度调整单次旋转角度
	/* 使用 g, G, v, V, b and B 键移动相机 */
	/* 使用 x, X, y, Y, z and Z 键移动模型 */
	switch (key)
	{
	case('g') : viewer[0] -= tranp; break;
	case('G') : viewer[0] += tranp; break;
	case('v') : viewer[1] -= tranp; break;
	case('V') : viewer[1] += tranp; break;
	case('B') : viewer[2] += tranp; break;
	case('b') : 
				if (scale + 5 - viewer[2] < 4.0)
				{
					viewer[2] -= tranp;
				}
				else
				{
					cout << "Camera position is too close." << endl;//限制相机最近距离
				} break; 
	case('x') : axis = 0; theta[0] += thep; break;
	case('X') : axis = 0; theta[0] -= thep; break;
	case('y') : axis = 1; theta[1] += thep; break;
	case('Y') : axis = 1; theta[1] -= thep; break;
	case('z') : axis = 2; theta[2] += thep; break;
	case('Z') : axis = 2; theta[2] -= thep; break;
	case('s') : if (scale+5-viewer[2] < 4.0)
				{ scale += 0.1; }
				else 
				{
					cout << "Maxmium scaling level reached."<<endl;//限制最大缩放倍数
				} break;
	case('S') : if (scale > 0.1)
	{
		scale -= 0.1;
	}
				else
				{
					cout << "Minimum scaling level reached." << endl;//限制最小缩放倍数
				} break;
	case('r') : case('R') : reset_camera(); break;
	case('i') : case('I') : reset_all(); break;	
	case('c') : case('C') : reset_color(); break;//输入自定义颜色
	}

	if (theta[axis] > 360.0)
	{
		theta[axis] -= 360.0;
	}//还原旋转角度
	display(); 
}

void skeys(int key, int x, int y)
{
	/* 使用方向键/F1/F2键移动模型 */
	double tranp = 0.1/scale; //根据缩放尺度调整单次平移距离
	if (key == GLUT_KEY_UP) { Tran[1] += tranp; }
	if (key == GLUT_KEY_DOWN) { Tran[1] -= tranp; }
	if (key == GLUT_KEY_LEFT) { Tran[0] -= tranp; }
	if (key == GLUT_KEY_RIGHT) { Tran[0] += tranp; }
	if (key == GLUT_KEY_F1) { Tran[2] += tranp; }
	if (key == GLUT_KEY_F2) { Tran[2] -= tranp; }
	display();
}

void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);

	//调整窗口大小后重绘

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w >= h) glFrustum(-2.0, 2.0, -2.0 * (GLfloat)h / (GLfloat)w,
		2.0* (GLfloat)h / (GLfloat)w, 2.0, 20.0);
	else glFrustum(-2.0, 2.0, -2.0 * (GLfloat)w / (GLfloat)h,
		2.0* (GLfloat)w / (GLfloat)h, 2.0, 20.0);

	glMatrixMode(GL_MODELVIEW);
}



void main(int argc, char **argv)
{
	std::string filepath;
	glutInit(&argc, argv);
	do
	{
		cout << "Enter the full file name of OFF model:" << endl;
		cin >> filepath;//读入文件名
	} while (!load_off(&filepath));
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("OFF Model");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutSpecialFunc(skeys);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keys);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
}
