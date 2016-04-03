
/* Rotating cube with viewer movement from Chapter 5 */
/* Cube definition and display similar to rotating--cube program */

/* We use the Lookat function in the display callback to point
the viewer, whose position can be altered by the x,X,y,Y,z, and Z keys.
The perspective view is set in the reshape callback */

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
#define _MAX_VRX_ 2001
#define _MAX_NOR_ 1501
#define _MAX_PATH_ 81
#define _CLR_ c%3

float vertices[_MAX_VRX_][3];
int nFaces, nVertices, nEdges;
float normals[_MAX_NOR_][3];
int colorset[3][3] = { { 62,88,94 },{ 58,70,81 },{ 129,133,126 } };

float colors[][3] = { { colorset[0][0] / 255.0,colorset[0][1] / 255.0,colorset[0][2] / 255.0 },
{ colorset[1][0] / 255.0,colorset[1][1] / 255.0,colorset[1][2] / 255.0 },
{ colorset[2][0] / 255.0,colorset[2][1] / 255.0,colorset[2][2] / 255.0 } };

void polygon(int a, int b, int c)
{
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
	int i;
	for (i = 0; i<nFaces; i++)
	{
		polygon((int)normals[i][0], (int)normals[i][1], (int)normals[i][2]);
	}
}

bool load_off(string* str)
{
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
																	  //cout<< vertices[i][0] << vertices[i][1] << vertices[i][2]<<'\n';
	}
	int n;
	for (int i = 0; i<nFaces; i++)
	{
		fin >> n >> normals[i][0] >> normals[i][1] >> normals[i][2];  //逐行读入面的顶点序列;
																	  //cout<< normals[i][0] << normals[i][1] << normals[i][2]<<'\n';
	}
	fin.close();
	disp_off();
	return 1;
}

static GLfloat Tran[] = { 0,0,0 };
static GLfloat theta[] = { 0.0,0.0,0.0 };
static GLint axis = 2;
static GLdouble viewer[] = { 0.0, 0.0, 5.0 }; /* initial viewer location */

void init()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glFlush();
}

void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Update viewer position in modelview matrix */

	glLoadIdentity();
	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	
	/* rotate cube */

	glRotatef(theta[0], 1.0, 0.0, 0.0);
	glRotatef(theta[1], 0.0, 1.0, 0.0);
	glRotatef(theta[2], 0.0, 0.0, 1.0);
	glTranslatef(Tran[0], Tran[1],Tran[2]);
	//init();
	disp_off();
	glFlush();
	glutSwapBuffers();
}

void mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) axis = 0;
	if (btn == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) axis = 1;
	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) axis = 2;
	theta[axis] += 3.0;
	if (theta[axis] > 360.0) theta[axis] -= 360.0;
	display();
}

void reset_color()
{
	cout << "Enter 3 sets of RGB color values you want to set:\n";
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			cin >> colorset[i][j];
		}
	}
	init();
	display();
}

void keys(unsigned char key, int x, int y)
{

	/* Use x, X, y, Y, z, and Z keys to move viewer */

	/*if (key == 'x') viewer[0] -= 1.0;
	if (key == 'X') viewer[0] += 1.0;
	if (key == 'y') viewer[1] -= 1.0;
	if (key == 'Y') viewer[1] += 1.0;
	if (key == 'z') viewer[2] -= 1.0;
	if (key == 'Z') viewer[2] += 1.0;*/
	if (key == 38 ) { Tran[0] += 1; }
	if (key == 40 ) { Tran[0] -= 1; }
	if (key == 39) { Tran[1] -= 1; }
	if (key == 37) { Tran[1] += 1; }
	if (key == 'o') { Tran[2] += 0.1; }
	if (key == 'i') { Tran[2] -= 0.1; }
	if (key == 'r' || key == 'R') reset_color();
	//cout << (int)key <<"Here"<< endl;
	display();
}

void skeys(int key, int x, int y)
{
	if (key == 101) { Tran[1] += 0.1; }
	if (key == 103) { Tran[1] -= 0.1; }
	if (key == 100) { Tran[0] -= 0.1; }
	if (key == 102) { Tran[0] += 0.1; }
	//cout << (int)key << "SHere" << endl;
	display();
}

void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);

	/* Use a perspective view */

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w >= h) glFrustum(-2.0, 2.0, -2.0 * (GLfloat)h / (GLfloat)w,
		2.0* (GLfloat)h / (GLfloat)w, 2.0, 20.0);
	else glFrustum(-2.0, 2.0, -2.0 * (GLfloat)w / (GLfloat)h,
		2.0* (GLfloat)w / (GLfloat)h, 2.0, 20.0);

	/* Or we can use gluPerspective */

	/* gluPerspective(45.0, w/h, -10.0, 10.0); */
	cout << "Reshape\n";
	glMatrixMode(GL_MODELVIEW);
}

void main(int argc, char **argv)
{
	std::string filepath = { "horse.off" };
	glutInit(&argc, argv);
	do
	{
		cout << "Enter the full file name of OFF model:" << endl;
		//cin >> filepath;
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
