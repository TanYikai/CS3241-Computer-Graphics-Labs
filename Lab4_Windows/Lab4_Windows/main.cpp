// CS3241Lab4.cpp : Defines the entry point for the console application.
//#include <cmath>
#include "math.h"
#include <iostream>
#include <fstream>

/* Include header files depending on platform */
#ifdef _WIN32
	#include "glut.h"
	#define M_PI 3.14159
#elif __APPLE__
	#include <OpenGL/gl.h>
	#include <GLUT/GLUT.h>
#endif

#define MAXPTNO 100000000 //originally 1000
#define NLINESEGMENT 32
#define NOBJECTONCURVE 8

using namespace std;

// Global variables that you can use
struct Point {
	int x,y;
};

// Storage of control points
int nPt = 0;
Point ptList[MAXPTNO];
Point C1PointArr[MAXPTNO];

// Variable for Bezier Curve Points
Point curvePt;
Point tanPt;//for tangent
float t;
float angle;
int nthDegree = 3;

//animation
int refreshMill = 30;
float threshold = 1;
bool animate = false;
bool animate2 = false;

// Display options
bool displayControlPoints = true;
bool displayControlLines = true;
bool displayTangentVectors = false;
bool displayObjects = false;
bool C1Continuity = false;

void drawFilledCircle(float x, float y, float red, float green, float blue, float ellipseIndexX, float ellipseIndexY, double radius) {
	int i;

	glColor4f(red, green, blue, 0.55);
	double twicePi = 2.0 * 3.142;

	glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
	glVertex2f(x, y); // center of circle
	for (i = 0; i <= 20; i++) {
		glVertex2f(
			(x + (ellipseIndexX * radius * cos(i * twicePi / 20))), (y + (ellipseIndexY * radius * sin(i * twicePi / 20)))
		);
	}
	glEnd(); //END
}

void drawSpirit() {
	glPushMatrix();
	glScalef(0.5, 0.5, 0);
	//Limbs
	glTranslatef(-5, -3, 0);
	drawFilledCircle(5, 5, 0.9, 0.9, 0.9, 0.9, 7, 0.3);
	drawFilledCircle(5.05, 4.3, 0.8, 0.8, 0.85, 0.9, 7, 0.17);//shading
	drawFilledCircle(4.5, 5, 0.9, 0.9, 0.9, 0.9, 7, 0.3);
	drawFilledCircle(4.55, 4.3, 0.8, 0.8, 0.85, 0.9, 7, 0.17);//shading	

															  //Body
	glTranslatef(5, 3, 0);
	glRotatef(9, 0, 0, 1);
	drawFilledCircle(0.85, 2, 0.7, 0.7, 0.75, 0.5, 3, 0.3);//arm right
	glRotatef(-9, 0, 0, 1);
	drawFilledCircle(-0.3, 2.3, 0.9, 0.9, 0.9, 1, 2, 0.8);
	drawFilledCircle(-0.25, 2.3, 0.8, 0.8, 0.85, 1, 2, 0.65);//shading
															 //drawFilledCircle(0, 0, 0.7, 0.7, 0.7, 10, 0.9, 0.1);//shadow	
	drawFilledCircle(-1, 2, 0.85, 0.85, 0.85, 0.5, 3, 0.3);//arm left
	glScalef(2, 2, 0);

	//Head
	glTranslatef(-0.2, 2.8, 0);

	glRotatef(180, 0, 0, 1);
	drawFilledCircle(0, 0, 0.9, 0.9, 0.9, 1, 1, 1);
	drawFilledCircle(0.17, 0, 0.8, 0.8, 0.85, 1, 1, 0.9);//Shading												   

	drawFilledCircle(-0.5, 0, 0.6, 0.6, 0.6, 1, 1, 0.3);
	drawFilledCircle(-0.45, 0, 0.2, 0.2, 0.2, 1, 1, 0.25);//shading
	drawFilledCircle(0.58, -0.15, 0.6, 0.6, 0.6, 1, 1, 0.37);
	drawFilledCircle(0.53, -0.15, 0.2, 0.2, 0.2, 1, 1, 0.32);//shading
	drawFilledCircle(0.2, .7, 0.6, 0.6, 0.6, 1, 1, 0.2);
	drawFilledCircle(0.15, .7, 0.2, 0.2, 0.2, 1, 1, 0.15);//shading

	glPopMatrix();

}

void drawRightArrow()
{
	glColor3f(0,1,0);
	glBegin(GL_LINE_STRIP);
		glVertex2f(0,0);
		glVertex2f(100,0);
		glVertex2f(95,5);
		glVertex2f(100,0);
		glVertex2f(95,-5);
	glEnd();
}

void bezierCoefficients(int n, int *c)
{
	int k, i;
	for (k = 0; k <= n; k++)
	{
		c[k] = 1;
		for (i = n; i >= k + 1; i--)
			c[k] *= i;
		for (i = n - k; i >= 2; i--)
			c[k] /= i;

	}
}

void drawNthBezierCurve(bool loop, int n, int startPoint)
{

	int c[100], k;
	float x, y, u, blend;
	bezierCoefficients(n, c);
	glColor3f(0.0, 0.0, 0.0);

	if (loop)
		glBegin(GL_LINE_LOOP);
	else
		glBegin(GL_LINE_STRIP);

	for (u = 0; u<1.0; u += 0.01)
	{
		x = 0;
		y = 0;
		for (k = 0; k<n + 1; k++)
		{
			blend = c[k] * pow(u, k)*pow(1 - u, n - k);
			if (!C1Continuity) {
				x += ptList[startPoint + k].x * blend;
				y += ptList[startPoint + k].y * blend;
			}
			else {
				x += C1PointArr[startPoint + k].x * blend;
				y += C1PointArr[startPoint + k].y * blend;
			}
		}
		glVertex2f(x, y);

	}
	glEnd();
	glFlush();
}

void drawBezierCurve() {
	glPushMatrix();
	for (int i = 1; i < nPt; i++)
	{	
		if (i % 3 == 0) {//Draw the curve at every 4th point 
			if (!C1Continuity) {
				//No C1 continuity
				glPointSize(5);
				glBegin(GL_LINE_STRIP);
				for (int j = 0; j <= NLINESEGMENT; j++) {
					//spliting parameter into N line segments
					t = (1.0 / (float)NLINESEGMENT) * j;

					//calculating bezier curve points
					curvePt.x = pow((1 - t), 3) * ptList[i - 3].x
						+ 3 * pow((1 - t), 2) * t * ptList[i - 2].x
						+ 3 * (1 - t) * pow(t, 2) * ptList[i - 1].x
						+ pow(t, 3) * ptList[i].x;

					curvePt.y = pow((1 - t), 3) * ptList[i - 3].y
						+ 3 * pow((1 - t), 2) * t * ptList[i - 2].y
						+ 3 * (1 - t) * pow(t, 2) * ptList[i - 1].y
						+ pow(t, 3) * ptList[i].y;

					//plotting bezier curve points
					glColor3f(0, 0, 0);
					glVertex2d(curvePt.x, curvePt.y);
				}
				glEnd();
				glPointSize(1);
			}
			else {
				//Have C1 continuity
				glPointSize(5);
				glBegin(GL_LINE_STRIP);
				for (int j = 0; j <= NLINESEGMENT; j++) {
					//spliting parameter into N line segments
					t = (1.0 / (float)NLINESEGMENT) * j;

					//calculating bezier curve points
					curvePt.x = pow((1 - t), 3) * ptList[i - 3].x
						+ 3 * pow((1 - t), 2) * t * C1PointArr[i -2].x
						+ 3 * (1 - t) * pow(t, 2) * ptList[i - 1].x
						+ pow(t, 3) * ptList[i].x;

					curvePt.y = pow((1 - t), 3) * ptList[i - 3].y
						+ 3 * pow((1 - t), 2) * t * C1PointArr[i - 2].y
						+ 3 * (1 - t) * pow(t, 2) * ptList[i - 1].y
						+ pow(t, 3) * ptList[i].y;

					//plotting bezier curve points
					glColor3f(0, 0, 0);
					glVertex2d(curvePt.x, curvePt.y);
				}
				glEnd();
				glPointSize(1);
			}
		}
	}
	glPopMatrix();
}

void C1Continue() {
	int i;
	for (i = nthDegree + 1; i < nPt; i++) {
		if (i % nthDegree == 1) {
			//calculating C1point of next curve
			C1PointArr[i].x = 2 * ptList[i - 1].x - ptList[i - 2].x;
			C1PointArr[i].y = 2 * ptList[i - 1].y - ptList[i - 2].y;
		}
	}
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	//To update the C1Continuity points
	if (C1Continuity) {
		for (int i = 0; i < nPt; i++) {
			C1PointArr[i].x = ptList[i].x;
			C1PointArr[i].y = ptList[i].y;
		}
		C1Continue();
	}
	
	if(displayControlPoints)
	{
		glPointSize(5);
		glBegin(GL_POINTS);
		for(int i=0;i<nPt; i++)
		{
			if (!C1Continuity) {
				glColor3f(0, 0, 0);
				glVertex2d(ptList[i].x, ptList[i].y);
			}
			else {
				if (i % nthDegree == 1 && i != 1) {
					glColor3f(1, 0, 0);
					glVertex2d(C1PointArr[i].x, C1PointArr[i].y);
					glColor3f(0.5, 0.5, 0.5);
				}
				else {
					glColor3f(0, 0, 0);
				}
				glVertex2d(ptList[i].x, ptList[i].y);
			}
		}
		glEnd();
		glPointSize(1);

	}

	if(displayControlLines)
	{	
		glPointSize(5);
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i<nPt; i++)
		{
			if (!C1Continuity) {
				glColor3f(1, 0, 0);
				glVertex2d(ptList[i].x, ptList[i].y);
			}
			else {
				glColor3f(1, 0, 0);
				glVertex2d(C1PointArr[i].x, C1PointArr[i].y);
			}
		}
		glEnd();
		glPointSize(1);
	}

	if (displayTangentVectors) {
		for (int i = 1; i < nPt; i++)
		{	
			if (i % 3 == 0) {//Draw the curve at every 4th point 
				if (!C1Continuity) {
					//No C1 continuity
					for (int j = 0; j < NOBJECTONCURVE; j++) {
						//spliting parameter into N line segments
						t = (1.0 / (float)NOBJECTONCURVE) * j;

						//calculating bezier curve points
						curvePt.x = pow((1 - t), 3) * ptList[i - 3].x
							+ 3 * pow((1 - t), 2) * t * ptList[i - 2].x
							+ 3 * (1 - t) * pow(t, 2) * ptList[i - 1].x
							+ pow(t, 3) * ptList[i].x;

						curvePt.y = pow((1 - t), 3) * ptList[i - 3].y
							+ 3 * pow((1 - t), 2) * t * ptList[i - 2].y
							+ 3 * (1 - t) * pow(t, 2) * ptList[i - 1].y
							+ pow(t, 3) * ptList[i].y;

						//calculating tangent points and angle
						tanPt.x = 3 * pow((1 - t), 2) * (ptList[i - 2].x - ptList[i - 3].x)
							+ 6 * (1 - t) * t * (ptList[i - 1].x - ptList[i - 2].x)
							+ 3 * pow(t, 2) * (ptList[i].x - ptList[i - 1].x);

						tanPt.y = 3 * pow((1 - t), 2) * (ptList[i - 2].y - ptList[i - 3].y)
							+ 6 * (1 - t) * t * (ptList[i - 1].y - ptList[i - 2].y)
							+ 3 * pow(t, 2) * (ptList[i].y - ptList[i - 1].y);

						angle = atan2(tanPt.y, tanPt.x) * (180.0 / M_PI);

						//plotting arrows
						glPushMatrix();
						glTranslatef(curvePt.x, curvePt.y, 0);
						glRotatef(angle, 0, 0, 1);
						drawRightArrow();
						glPopMatrix();
					}
				}
				else {
					//Have C1 continuity
					for (int j = 0; j < NOBJECTONCURVE; j++) {
						//spliting parameter into N line segments
						t = (1.0 / (float)NOBJECTONCURVE) * j;

						//calculating bezier curve points
						curvePt.x = pow((1 - t), 3) * ptList[i - 3].x
							+ 3 * pow((1 - t), 2) * t * C1PointArr[i - 2].x
							+ 3 * (1 - t) * pow(t, 2) * ptList[i - 1].x
							+ pow(t, 3) * ptList[i].x;

						curvePt.y = pow((1 - t), 3) * ptList[i - 3].y
							+ 3 * pow((1 - t), 2) * t * C1PointArr[i - 2].y
							+ 3 * (1 - t) * pow(t, 2) * ptList[i - 1].y
							+ pow(t, 3) * ptList[i].y;

						//calculating tangent points and angle
						tanPt.x = 3 * pow((1 - t), 2) * (C1PointArr[i - 2].x - ptList[i - 3].x)
							+ 6 * (1 - t) * t * (ptList[i - 1].x - C1PointArr[i - 2].x)
							+ 3 * pow(t, 2) * (ptList[i].x - ptList[i - 1].x);

						tanPt.y = 3 * pow((1 - t), 2) * (C1PointArr[i - 2].y - ptList[i - 3].y)
							+ 6 * (1 - t) * t * (ptList[i - 1].y - C1PointArr[i -2].y)
							+ 3 * pow(t, 2) * (ptList[i].y - ptList[i - 1].y);

						angle = atan2(tanPt.y, tanPt.x) * (180.0 / M_PI);

						//plotting arrows
						glPushMatrix();
						glTranslatef(curvePt.x, curvePt.y, 0);
						glRotatef(angle, 0, 0, 1);
						drawRightArrow();
						glPopMatrix();
					}
					
				}
			}
		}
	}

	if (displayObjects) {
		for (int i = 1; i < nPt; i++)
		{
			if (i % 3 == 0) {//Draw the curve at every 4th point 
				if (!C1Continuity) {
					//No C1 continuity
					for (int j = 0; j < NOBJECTONCURVE; j++) {
						//spliting parameter into N line segments
						t = (1.0 / (float)NOBJECTONCURVE) * j;

						//calculating bezier curve points
						curvePt.x = pow((1 - t), 3) * ptList[i - 3].x
							+ 3 * pow((1 - t), 2) * t * ptList[i - 2].x
							+ 3 * (1 - t) * pow(t, 2) * ptList[i - 1].x
							+ pow(t, 3) * ptList[i].x;

						curvePt.y = pow((1 - t), 3) * ptList[i - 3].y
							+ 3 * pow((1 - t), 2) * t * ptList[i - 2].y
							+ 3 * (1 - t) * pow(t, 2) * ptList[i - 1].y
							+ pow(t, 3) * ptList[i].y;

						//calculating tangent points and angle
						tanPt.x = 3 * pow((1 - t), 2) * (ptList[i - 2].x - ptList[i - 3].x)
							+ 6 * (1 - t) * t * (ptList[i - 1].x - ptList[i - 2].x)
							+ 3 * pow(t, 2) * (ptList[i].x - ptList[i - 1].x);

						tanPt.y = 3 * pow((1 - t), 2) * (ptList[i - 2].y - ptList[i - 3].y)
							+ 6 * (1 - t) * t * (ptList[i - 1].y - ptList[i - 2].y)
							+ 3 * pow(t, 2) * (ptList[i].y - ptList[i - 1].y);

						angle = atan2(tanPt.y, tanPt.x) * (180.0 / M_PI);

						//plotting arrows
						glPushMatrix();
						glTranslatef(curvePt.x, curvePt.y, 0);
						glRotatef(angle, 0, 0, 1);
						glScalef(10, 10, 0);
						drawSpirit();
						glPopMatrix();
					}
				}
				else {
					//Have C1 continuity
					for (int j = 0; j < NOBJECTONCURVE; j++) {
						//spliting parameter into N line segments
						t = (1.0 / (float)NOBJECTONCURVE) * j;

						//calculating bezier curve points
						curvePt.x = pow((1 - t), 3) * ptList[i - 3].x
							+ 3 * pow((1 - t), 2) * t * C1PointArr[i -2].x
							+ 3 * (1 - t) * pow(t, 2) * ptList[i - 1].x
							+ pow(t, 3) * ptList[i].x;

						curvePt.y = pow((1 - t), 3) * ptList[i - 3].y
							+ 3 * pow((1 - t), 2) * t * C1PointArr[i -2].y
							+ 3 * (1 - t) * pow(t, 2) * ptList[i - 1].y
							+ pow(t, 3) * ptList[i].y;

						//calculating tangent points and angle
						tanPt.x = 3 * pow((1 - t), 2) * (C1PointArr[i -2].x - ptList[i - 3].x)
							+ 6 * (1 - t) * t * (ptList[i - 1].x - C1PointArr[i -2].x)
							+ 3 * pow(t, 2) * (ptList[i].x - ptList[i - 1].x);

						tanPt.y = 3 * pow((1 - t), 2) * (C1PointArr[i -2].y - ptList[i - 3].y)
							+ 6 * (1 - t) * t * (ptList[i - 1].y - C1PointArr[i - 2].y)
							+ 3 * pow(t, 2) * (ptList[i].y - ptList[i - 1].y);

						angle = atan2(tanPt.y, tanPt.x) * (180.0 / M_PI);

						//plotting arrows
						glPushMatrix();
						glTranslatef(curvePt.x, curvePt.y, 0);
						glRotatef(angle, 0, 0, 1);
						glScalef(10, 10, 0);
						drawSpirit();
						glPopMatrix();
					}
					
				}
			}
		}
	}

	
	//Draw Bezier Curve
	if (animate) 
		glTranslatef(10 * sin(threshold), 10 * sin(threshold), 0);
	//drawBezierCurve();
	for (int i = 0; i < nPt; i++) {
		if (i % nthDegree == 0 && i != 0) {
			drawNthBezierCurve(false, nthDegree, i - nthDegree);
		}
	}
	
	glPopMatrix();
	glutSwapBuffers ();
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,w,h,0);  
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
}

void Timer(int value) {
	
	threshold += 1;
	if (threshold > 1000)
		threshold = 1;

	if (animate2) {
		if (nthDegree > 1)
			nthDegree--;
	}


	glutTimerFunc(refreshMill, Timer, 0);
	glutPostRedisplay();
}

void init(void)
{
	glClearColor (1.0,1.0,1.0, 1.0);
}

void readFile()
{

	std::ifstream file;
    file.open("savefile.txt");
	file >> nPt;

	if(nPt>MAXPTNO)
	{
		cout << "Error: File contains more than the maximum number of points." << endl;
		nPt = MAXPTNO;
	}

	for(int i=0;i<nPt;i++)
	{
		file >> ptList[i].x;
		file >> ptList[i].y;
	}
    file.close();// is not necessary because the destructor closes the open file by default
}

void writeFile()
{
	std::ofstream file;
    file.open("savefile.txt");
    file << nPt << endl;

	for(int i=0;i<nPt;i++)
	{
		file << ptList[i].x << " ";
		file << ptList[i].y << endl;
	}
    file.close();// is not necessary because the destructor closes the open file by default
}

void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
		case 'r':
		case 'R':
			readFile();
			nthDegree = 3;
			displayControlPoints = false;
		break;

		case 'w':
		case 'W':
			writeFile();
		break;

		case 'T':
		case 't':
			displayTangentVectors = !displayTangentVectors;
		break;

		case 'o':
		case 'O':
			displayObjects = !displayObjects;
		break;

		case 'p':
		case 'P':
			displayControlPoints = !displayControlPoints;
		break;

		case 'L':
		case 'l':
			displayControlLines = !displayControlLines;
		break;

		case 'C':
		case 'c':
			C1Continuity = !C1Continuity;
			if (C1Continuity) {
				for (int i = 0; i < nPt; i++) {
					C1PointArr[i].x = ptList[i].x;
					C1PointArr[i].y = ptList[i].y;
				}
				C1Continue();
			}
		break;

		case 'e':
		case 'E':
			// Do something to erase all the control points added
			nPt = 0;
		break;

		case 'u':
		case 'U':
			if(nPt > 1) {
				nPt--;
			}
			break;

		case 'a':
		case 'A':
			displayControlLines = true;
			animate = !animate;
			break;

		case 'n':
		case 'N':
			nthDegree = 50;
			animate2 = !animate2;
			displayControlPoints = false;
			displayControlLines = false;
			break;

		case 'b':
			if(nthDegree > 1)
				nthDegree--;
			break;
		case 'B':
			if(nthDegree < nPt)
				nthDegree++;
			break;
		case 'q':
		case 'Q':
			displayControlPoints = true;
			displayControlLines = true;
			displayTangentVectors = false;
			displayObjects = false;
			C1Continuity = false;
			animate = false;
			animate2 = false;
			nthDegree = 3;
			break;

		case 27:
			exit(0);
		break;

		default:
		break;
	}

	glutPostRedisplay();
}



void mouse(int button, int state, int x, int y)
{
	/*button: GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON */
	/*state: GLUT_UP or GLUT_DOWN */
	enum
	{
		MOUSE_LEFT_BUTTON = 0,
		MOUSE_MIDDLE_BUTTON = 1,
		MOUSE_RIGHT_BUTTON = 2,
		MOUSE_SCROLL_UP = 3,
		MOUSE_SCROLL_DOWN = 4
	};
	if((button == MOUSE_LEFT_BUTTON)&&(state == GLUT_UP))
	{
		if(nPt==MAXPTNO)
		{
			cout << "Error: Exceeded the maximum number of points." << endl;
			return;
		}
		ptList[nPt].x=x;
		ptList[nPt].y=y;
		nPt++;
	}
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	cout<<"CS3241 Lab 4"<< endl<< endl;
	cout << "Left mouse click: Add a control point"<<endl;
	cout << "ESC: Quit" <<endl;
	cout << "P: Toggle displaying control points" <<endl;
	cout << "L: Toggle displaying control lines" <<endl;
	cout << "E: Erase all points (Clear)" << endl;
	cout << "C: Toggle C1 continuity" <<endl;	
	cout << "T: Toggle displaying tangent vectors" <<endl;
	cout << "O: Toggle displaying objects" <<endl;
	cout << "R: Read in control points from \"savefile.txt\"" <<endl;
	cout << "W: Write control points to \"savefile.txt\"" <<endl;
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (600, 600);
	glutInitWindowPosition (50, 50);
	glutCreateWindow ("CS3241 Assignment 4");
	init ();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(0, Timer, 0);
	glutMainLoop();

	return 0;
}
