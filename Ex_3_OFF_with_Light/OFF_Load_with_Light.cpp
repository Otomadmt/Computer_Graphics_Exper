#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <fstream> 
#include <string>
#include <iostream>
#include <vector>
#include <math.h>
using namespace std;
#define _MAX_VRX_ 2001//最多点信息数
#define _MAX_NOR_ 1501//最多面信息数
#define _MAX_PATH_ 81//最长文件名长度

GLfloat vertices[_MAX_VRX_][3];//存储OFF模型中各点坐标信息
int nFaces, nVertices, nEdges;//OFF模型中点线面总数
int surface[_MAX_NOR_][3];//存储OFF模型中各面点构成信息
vector<vector<int> > pons(_MAX_VRX_);//记录点在哪些面上
GLfloat snor[_MAX_NOR_][3];//面法向量信息
GLfloat vnor[_MAX_VRX_][3];//点法向量信息
int spin = 0;//光源旋转角度
bool type = 1;//明暗处理模式

void normalize(GLfloat(&a)[3])//单位化法向量
{
	float l;
	l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] /= l;
	a[1] /= l;
	a[2] /= l;
}

void calc_normal(int a, int b, int c, int i)
{
	float x1, y1, z1;
	float x2, y2, z2;
	x1 = vertices[b][0] - vertices[a][0];
	x2 = vertices[c][0] - vertices[a][0];
	y1 = vertices[b][1] - vertices[a][1];
	y2 = vertices[c][1] - vertices[a][1];
	z1 = vertices[b][2] - vertices[a][2];
	z2 = vertices[c][2] - vertices[a][2];
	snor[i][0] = y1*z2 - y2*z1;
	snor[i][1] = z1*x2 - x1*z2;
	snor[i][2] = x1*y2 - y1*x2;//根据两边叉乘计算面法向量
	normalize(snor[i]);
}

void calc_vnormal()
{
	int i,j,k,p;
	GLfloat x, y, z;
	for (i = 0; i < nVertices; i++)
	{
		x = 0; y = 0; z = 0;
		k = pons[i].size();
		for (j = 0; j < k; j++)
		{
			p = pons[i][j];
			x += snor[p][0];
			y += snor[p][1];
			z += snor[p][2];
		}
		vnor[i][0] = x;
		vnor[i][1] = y;
		vnor[i][2] = z;//根据该点所在面的法向量之和，求平均得到点法向量
		normalize(vnor[i]);
	}
}

void polygon_surf(int a, int b, int c, int i)
{
	//指定每个面法向量，绘制三角形图元
	glBegin(GL_POLYGON);
	glNormal3fv(snor[i]);
	glVertex3fv(vertices[a]);
	glVertex3fv(vertices[b]);
	glVertex3fv(vertices[c]);
	glEnd();
}

void polygon_smth(int a, int b, int c)
{
	//指定每个点法向量，绘制三角形图元
	glBegin(GL_POLYGON);
	glNormal3fv(vnor[a]);
	glVertex3fv(vertices[a]);
	glNormal3fv(vnor[b]);
	glVertex3fv(vertices[b]);
	glNormal3fv(vnor[c]);
	glVertex3fv(vertices[c]);
	glEnd();
}

void disp_off()//绘制每一个面
{
	int i;
	for (i = 0; i < nFaces; i++)
	{
		if (type)
		{
			polygon_surf(surface[i][0], surface[i][1], surface[i][2], i);//平面处理
		}
		else
		{
			polygon_smth(surface[i][0], surface[i][1], surface[i][2]);//平滑处理
		}
	}
}

bool load_off(string* str)
{
	//读入OFF模型信息
	ifstream fin;
	fin.open(*str);
	if (!fin.is_open())
	{
		printf("File Error!\n");
		return 0;
	}
	string off;
	getline(fin, off);//读入首行字符串"OFF"

	fin >> nVertices >> nFaces >> nEdges; //读入点、面、边数目
	for (int i = 0; i < nVertices; i++)
	{
		fin >> vertices[i][0] >> vertices[i][1] >> vertices[i][2];    //逐行读入顶点坐标;
	}
	int n;
	for (int i = 0; i < nFaces; i++)
	{
		fin >> n >> surface[i][0] >> surface[i][1] >> surface[i][2];  //逐行读入面的顶点序列;
		calc_normal(surface[i][0], surface[i][1], surface[i][2], i);  //计算该平面法向量
		pons[surface[i][0]].push_back(i);
		pons[surface[i][1]].push_back(i);
		pons[surface[i][2]].push_back(i);//记录点在哪些平面上
	}
	calc_vnormal();//计算点法向量
	fin.close();
	disp_off();
	return 1;
}

void display(void)
{
	GLfloat pos1[] = { 0.2, 0.2,1.0, 1.0 };//固定光源位置
	GLfloat position[] = { 0.0, 0.0, 2.0, 1.0 };//可移动光源位置

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	gluLookAt(0.0, 0.0,5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glPushMatrix();
	glTranslated(pos1[0], pos1[1], pos1[2]);
	glLightfv(GL_LIGHT1, GL_POSITION, pos1);//设置固定光源
	glTranslated(0.0, 0.0, 1.5);
	glDisable(GL_LIGHTING);
	glColor3f(1.0, 1.0, 0.0);
	glutWireCube(0.1);
	glEnable(GL_LIGHTING);
	glRotated((GLdouble)spin, 1.0, 0.0, 0.0);
	glLightfv(GL_LIGHT0, GL_POSITION, position);//设置可移动光源
	glTranslated(0.0, 0.0, 1.5);
	glDisable(GL_LIGHTING);
	glColor3f(0.0, 1.0, 1.0);
	glutWireCube(0.1);
	glEnable(GL_LIGHTING);
	glPopMatrix();

	disp_off();
	glPopMatrix();
	glFlush();
}

void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);

	//调整窗口大小后重绘
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			spin = (spin + 20) % 360;//旋转可移动光源
			glutPostRedisplay();
		}
		break;
	default:
		break;
	}
}

void keys(unsigned char key, int x, int y)
{
	switch (key) {
	case 's':
	case 'S':type = false; display(); break;//切换为平面明暗处理方式
	case 'f':
	case 'F':type = true; display(); break;//切换为平滑明暗处理方式
	case 27:
		exit(0);
		break;
	}
}
void myMenu(int value) {
	if (value == 1) {
		type = true;
		glutPostRedisplay();
	}
	if (value == 2) {
		type = false;
		glutPostRedisplay();
	}
	if (value == 3) {
		exit(0);
	}
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_DEPTH_TEST);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	string filepath;
	do
	{
		cout << "Enter the full file name of OFF model:" << endl;
		cin >> filepath;//读入文件名
	} while (!load_off(&filepath));
	glutDisplayFunc(display);
	glutReshapeFunc(myReshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keys);
	glutCreateMenu(myMenu);
	glutAddMenuEntry("Flat", 1);
	glutAddMenuEntry("Smooth", 2);
	glutAddMenuEntry("Exit", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutMainLoop();
	return 0;
}
