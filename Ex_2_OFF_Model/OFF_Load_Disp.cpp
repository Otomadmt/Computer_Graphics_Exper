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
#define _MAX_VRX_ 2001//������Ϣ��
#define _MAX_NOR_ 1501//�������Ϣ��
#define _MAX_PATH_ 81//��ļ�������
#define _CLR_ c%3//��ɫָ��

float vertices[_MAX_VRX_][3];//�洢OFFģ���и���������Ϣ
int nFaces, nVertices, nEdges;//OFFģ���е���������
GLfloat  scale = 1;//���ű���
float normals[_MAX_NOR_][3];//�洢OFFģ���и��湹����Ϣ
int colorset[3][3] = { { 62,88,94 },{ 58,70,81 },{ 129,133,126 } };//��ʼ��ɫ(RGB 256��ʾ)

float colors[][3] = { { colorset[0][0] / 255.0,colorset[0][1] / 255.0,colorset[0][2] / 255.0 },
{ colorset[1][0] / 255.0,colorset[1][1] / 255.0,colorset[1][2] / 255.0 },
{ colorset[2][0] / 255.0,colorset[2][1] / 255.0,colorset[2][2] / 255.0 } };//��������ʾ��RGB��ɫ��Ϣ

void polygon(int a, int b, int c)
{
	//����������ͼԪ��Ⱦɫ
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
	//����ÿһ����
	int i;
	for (i = 0; i<nFaces; i++)
	{
		polygon((int)normals[i][0], (int)normals[i][1], (int)normals[i][2]);
	}
}

bool load_off(string* str)
{
	//����OFFģ����Ϣ
	std::ifstream fin;
	fin.open(*str);
	if (!fin.is_open())
	{
		printf("File Error!\n");
		return 0;
	}
	std::string off;
	std::getline(fin, off);//���������ַ���"OFF"

	fin >> nVertices >> nFaces >> nEdges; //����㡢�桢����Ŀ
	for (int i = 0; i<nVertices; i++)
	{
		fin >> vertices[i][0] >> vertices[i][1] >> vertices[i][2];    //���ж��붥������;
	}
	int n;
	for (int i = 0; i<nFaces; i++)
	{
		fin >> n >> normals[i][0] >> normals[i][1] >> normals[i][2];  //���ж�����Ķ�������;
	}
	fin.close();
	disp_off();
	return 1;
}

static GLfloat Tran[] = { 0,0,0 };//ƽ�ƾ���
static GLfloat theta[] = { 0.0,0.0,0.0 };//��ת����
static GLint axis = 2;//����
static GLdouble viewer[] = { 0.0, 0.0, 5.0 }; //��ʼ���λ��

 
void init()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);//�������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glFlush();
}

void display(void)
{
	init();
	
	glLoadIdentity();//���뵥λ��
	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);//���λ�ñ任
	glScalef(scale, scale, scale);//�ȱ�����
	glTranslatef(Tran[0], Tran[1], Tran[2]);//ƽ�Ʊ任
	glRotatef(theta[0], 1.0, 0.0, 0.0);//��x����ת
	glRotatef(theta[1], 0.0, 1.0, 0.0);//��y����ת
	glRotatef(theta[2], 0.0, 0.0, 1.0);//��z����ת
	glTranslatef(0, 0, 0);//��ģ���ƻ�ԭ����ִ����ת����
	disp_off();//����ͼԪ
	glFlush();
	glutSwapBuffers();
}

void mouse(int btn, int state, int x, int y)
{
	GLfloat thep = 2.0 / scale;//�������ų߶ȵ���������ת�Ƕ�
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
	//�Զ�����ɫ��ͼ
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
	double tranp = 0.1 / scale;//�������ų߶ȵ������������������
	GLfloat thep = 2.0 / scale;//�������ų߶ȵ���������ת�Ƕ�
	/* ʹ�� g, G, v, V, b and B ���ƶ���� */
	/* ʹ�� x, X, y, Y, z and Z ���ƶ�ģ�� */
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
					cout << "Camera position is too close." << endl;//��������������
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
					cout << "Maxmium scaling level reached."<<endl;//����������ű���
				} break;
	case('S') : if (scale > 0.1)
	{
		scale -= 0.1;
	}
				else
				{
					cout << "Minimum scaling level reached." << endl;//������С���ű���
				} break;
	case('r') : case('R') : reset_camera(); break;
	case('i') : case('I') : reset_all(); break;	
	case('c') : case('C') : reset_color(); break;//�����Զ�����ɫ
	}

	if (theta[axis] > 360.0)
	{
		theta[axis] -= 360.0;
	}//��ԭ��ת�Ƕ�
	display(); 
}

void skeys(int key, int x, int y)
{
	/* ʹ�÷����/F1/F2���ƶ�ģ�� */
	double tranp = 0.1/scale; //�������ų߶ȵ�������ƽ�ƾ���
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

	//�������ڴ�С���ػ�

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
		cin >> filepath;//�����ļ���
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
