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
#define _MAX_VRX_ 2001//������Ϣ��
#define _MAX_NOR_ 1501//�������Ϣ��
#define _MAX_PATH_ 81//��ļ�������

GLfloat vertices[_MAX_VRX_][3];//�洢OFFģ���и���������Ϣ
int nFaces, nVertices, nEdges;//OFFģ���е���������
int surface[_MAX_NOR_][3];//�洢OFFģ���и���㹹����Ϣ
vector<vector<int> > pons(_MAX_VRX_);//��¼������Щ����
GLfloat snor[_MAX_NOR_][3];//�淨������Ϣ
GLfloat vnor[_MAX_VRX_][3];//�㷨������Ϣ
int spin = 0;//��Դ��ת�Ƕ�
bool type = 1;//��������ģʽ

void normalize(GLfloat(&a)[3])//��λ��������
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
	snor[i][2] = x1*y2 - y1*x2;//�������߲�˼����淨����
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
		vnor[i][2] = z;//���ݸõ�������ķ�����֮�ͣ���ƽ���õ��㷨����
		normalize(vnor[i]);
	}
}

void polygon_surf(int a, int b, int c, int i)
{
	//ָ��ÿ���淨����������������ͼԪ
	glBegin(GL_POLYGON);
	glNormal3fv(snor[i]);
	glVertex3fv(vertices[a]);
	glVertex3fv(vertices[b]);
	glVertex3fv(vertices[c]);
	glEnd();
}

void polygon_smth(int a, int b, int c)
{
	//ָ��ÿ���㷨����������������ͼԪ
	glBegin(GL_POLYGON);
	glNormal3fv(vnor[a]);
	glVertex3fv(vertices[a]);
	glNormal3fv(vnor[b]);
	glVertex3fv(vertices[b]);
	glNormal3fv(vnor[c]);
	glVertex3fv(vertices[c]);
	glEnd();
}

void disp_off()//����ÿһ����
{
	int i;
	for (i = 0; i < nFaces; i++)
	{
		if (type)
		{
			polygon_surf(surface[i][0], surface[i][1], surface[i][2], i);//ƽ�洦��
		}
		else
		{
			polygon_smth(surface[i][0], surface[i][1], surface[i][2]);//ƽ������
		}
	}
}

bool load_off(string* str)
{
	//����OFFģ����Ϣ
	ifstream fin;
	fin.open(*str);
	if (!fin.is_open())
	{
		printf("File Error!\n");
		return 0;
	}
	string off;
	getline(fin, off);//���������ַ���"OFF"

	fin >> nVertices >> nFaces >> nEdges; //����㡢�桢����Ŀ
	for (int i = 0; i < nVertices; i++)
	{
		fin >> vertices[i][0] >> vertices[i][1] >> vertices[i][2];    //���ж��붥������;
	}
	int n;
	for (int i = 0; i < nFaces; i++)
	{
		fin >> n >> surface[i][0] >> surface[i][1] >> surface[i][2];  //���ж�����Ķ�������;
		calc_normal(surface[i][0], surface[i][1], surface[i][2], i);  //�����ƽ�淨����
		pons[surface[i][0]].push_back(i);
		pons[surface[i][1]].push_back(i);
		pons[surface[i][2]].push_back(i);//��¼������Щƽ����
	}
	calc_vnormal();//����㷨����
	fin.close();
	disp_off();
	return 1;
}

void display(void)
{
	GLfloat pos1[] = { 0.2, 0.2,1.0, 1.0 };//�̶���Դλ��
	GLfloat position[] = { 0.0, 0.0, 2.0, 1.0 };//���ƶ���Դλ��

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	gluLookAt(0.0, 0.0,5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glPushMatrix();
	glTranslated(pos1[0], pos1[1], pos1[2]);
	glLightfv(GL_LIGHT1, GL_POSITION, pos1);//���ù̶���Դ
	glTranslated(0.0, 0.0, 1.5);
	glDisable(GL_LIGHTING);
	glColor3f(1.0, 1.0, 0.0);
	glutWireCube(0.1);
	glEnable(GL_LIGHTING);
	glRotated((GLdouble)spin, 1.0, 0.0, 0.0);
	glLightfv(GL_LIGHT0, GL_POSITION, position);//���ÿ��ƶ���Դ
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

	//�������ڴ�С���ػ�
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
			spin = (spin + 20) % 360;//��ת���ƶ���Դ
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
	case 'S':type = false; display(); break;//�л�Ϊƽ����������ʽ
	case 'f':
	case 'F':type = true; display(); break;//�л�Ϊƽ����������ʽ
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
		cin >> filepath;//�����ļ���
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
