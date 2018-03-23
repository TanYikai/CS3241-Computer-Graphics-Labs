// CS3241Lab2.cpp : Defines the entry point for the console application.
#include <cmath>
#include <iostream>
#include <time.h>
#pragma warning(disable:4996)

/* Include header files depending on platform */
#ifdef _WIN32
#include "glut.h"
#define M_PI 3.14159
#elif __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#endif

using namespace std;

#define numStars 100
#define numPlanets 9

class planet
{
public:
	float distFromRef;
	float angularSpeed;
	GLfloat color[4];
	float size;
	float angle;

	planet()
	{
		distFromRef = 0;
		angularSpeed = 0;
		color[0] = color[1] = color[2] = 0;
		size = 0;
		angle = 0;
	}

	planet(float dist, float aSpeed, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha, float size, float angle) {
		distFromRef = dist;
		angularSpeed = aSpeed;
		color[0] = red;
		color[1] = green;
		color[2] = blue;
		color[3] = alpha;
		this->size = size;
		this->angle = angle;
	}

	void drawPlanets() {
		int i;
		glColor4f(color[0], color[1], color[2], color[3]);
		glBegin(GL_POLYGON);
		for (i = 0; i < 100; i++)
			glVertex2f(size * sin(2.0*M_PI*i / 100.0), size * cos(2.0*M_PI*i / 100.0));
		glEnd();
	}

	float setAngle(float angle){
		this->angle = angle;
	}

	float setDistFromRef(float dist){
		this->distFromRef = dist;
	}



};

float alpha = 0.0, k = 1;
float tx = 0.0, ty = 0.0;
bool clockBool = true;
bool timerBool = false;

float rotPos = 0.0;
float rotDelta = 30;
float rotPos2 = 0.0;
float rotDelta2 = 40;
float rotPos6 = 0.0;
float blink = 0.0;
//for clock
float rotPos3 = 0.0;
float rotDelta3 = 40;
float rotPos4 = 0.0;
float rotDelta4 = 20;
float rotPos5 = 0.0;
float rotDelta5 = 100;

int refreshMill = 30;
planet planetList[numPlanets];

void drawSomething() {
	glPushMatrix();
	glRotated(0, 0, 0, 1);
	glBegin(GL_POLYGON);
	glColor3f(1, 0.4, 0);
	glVertex2f(0, 2);
	glVertex2f(-0.2, 0);
	glVertex2f(0.2, 0);
	glEnd();
	glPopMatrix();
}

void draw1ShurikenEdge() {
	glPushMatrix();
	glBegin(GL_POLYGON);
	glColor4f(1, 1, 1, 0.3);
	glVertex2f(-2, -1);
	glVertex2f(-4, 3);
	glVertex2f(-2, 5);
	glVertex2f(-2, 3);
	glVertex2f(1, 1);	
	glEnd();
	glPopMatrix();
}

void drawShuriken() {
	glPushMatrix();
	draw1ShurikenEdge();
	glPopMatrix();
	
	glRotatef(90, 0, 0, 1);
	glPushMatrix();
	draw1ShurikenEdge();
	glPopMatrix();
	
	glRotatef(180, 0, 0, 1);
	glPushMatrix();
	draw1ShurikenEdge();
	glPopMatrix();
	
	glRotatef(270, 0, 0, 1);
	glPushMatrix();
	draw1ShurikenEdge();
	glPopMatrix();
}

void drawFilledCircle(float x, float y, float red, float green, float blue, float ellipseIndexX, float ellipseIndexY, double radius) {
	int i;

	glColor4f(red, green, blue, 0.55);
	double twicePi = 2.0 * 3.142;

	glBegin(GL_LINE_STRIP); //BEGIN CIRCLE
	glVertex2f(x, y); // center of circle
	for (i = 0; i <= 20; i++) {
		glVertex2f(
			(x + (ellipseIndexX * radius * cos(i * twicePi / 20))), (y + (ellipseIndexY * radius * sin(i * twicePi / 20)))
		);
	}
	glEnd(); //END
}

void drawInnerSwirl() {
	int i;
	glPushMatrix();
	for (i = 0; i < 60; i++) {
		glRotatef(10 * i, 0, 0, 1);
		drawFilledCircle(0, 0, 0.9, 0.8, 0.7, 0.2, 0.7, 2);
	}
	glPopMatrix();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-10, 10, -10, 10, -10, 10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.3, 1.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	//controls transformation
	glScalef(k, k, k);
	glTranslatef(tx, ty, 0);
	glRotatef(alpha, 0, 0, 1);

	//draw stuff here!
	if (clockBool) {
		planetList[0] = planet(0, 0, 0.55, 0.9, 0.9, 0.5, 1.8, 0);//rasengan
		planetList[1] = planet(0, 0, 0.4, 0.25, 0.12, 0.5, 1, 0);//shukaku
		planetList[2] = planet(0, 0, 0.1, 0.35, 0.7, 0.5, 1, 0);//matatabi
		planetList[3] = planet(0, 0, 0.4, 0.8, 0.7, 0.5, 1, 0);//isogu
		planetList[4] = planet(0, 0, 1, 0.75, 0.0, 0.5, 1, 0);//son goku //0.4,0.1,0.08
		planetList[5] = planet(0, 0, 0.6, 0.65, 0.7, 0.5, 1, 0);//kokuo
		planetList[6] = planet(0, 0, 0.6, 0.3, 0.7, 0.5, 1, 0);//saiken
		planetList[7] = planet(0, 0, 0, 0.3, 0, 0.5, 1, 0);//chomei
		planetList[8] = planet(0, 0, 0.05, 0.1, 0.07, 0.5, 1, 0);//gyuki
	}
	else {
		planetList[0] = planet(0, 0, 0.55, 0.9, 0.9, 0.2, 1.8, 0);//rasengan
		planetList[1] = planet(0, 0, 0.4, 0.25, 0.12, 0.2, 1, 0);//shukaku
		planetList[2] = planet(0, 0, 0.1, 0.35, 0.7, 0.2, 1, 0);//matatabi
		planetList[3] = planet(0, 0, 0.4, 0.8, 0.7, 0.2, 1, 0);//isogu
		planetList[4] = planet(0, 0, 1, 0.75, 0.0, 0, 1, 0);//son goku //0.4,0.1,0.08
		planetList[5] = planet(0, 0, 0.6, 0.65, 0.7, 0.2, 1, 0);//kokuo
		planetList[6] = planet(0, 0, 0.6, 0.3, 0.7, 0, 1, 0);//saiken
		planetList[7] = planet(0, 0, 0, 0.3, 0, 0.2, 1, 0);//chomei
		planetList[8] = planet(0, 0, 0.05, 0.1, 0.07, 0.2, 1, 0);//gyuki
	}

	time_t current_time = time(NULL);
	struct tm timeinfo;
	localtime_s(&timeinfo, &current_time);

	double angleHour = 450 - (timeinfo.tm_hour) % 12 / 12.0f * 360;
	double angleMin = 500 - (float)timeinfo.tm_min / 60  * 360;
	double angleSec = 500 - (float)timeinfo.tm_sec / 60 * 360;

	//spiral stars
	glPushMatrix();
	glRotatef(rotPos6, 0, 0, 1);
	int i;
	for (i = 0; i < 40; i++) {
		glPushMatrix();
		
		glRotatef(30 * i, 0, 0, 1);
		glPushMatrix();
		glTranslatef(0, 0.5 * i, 0);
		planet(0, 0, 1, 1, 1, fabs(cos(blink)), 0.1, 0).drawPlanets();
		glPopMatrix();
		
		glPopMatrix();
	}
	glPopMatrix();

	//9th
	glPushMatrix();
	if (!clockBool) {
		glRotatef(-rotPos6, 0, 0, 1);
		//glRotatef(-angleSec, 0, 0, 1);
	}
	else
		glRotatef(rotPos2, 0, 0, 1);

	drawInnerSwirl();
	drawShuriken();
	planetList[0].drawPlanets();
	planet(0, 0, 0.55, 0.9, 0.9, 0.5, 1.5, 0).drawPlanets();
	planet(0, 0, 0.55, 0.95, 0.95, 0.5, 1, 0).drawPlanets();
	glPopMatrix();
	
	if (!clockBool) {
		;
	}
	else
		glRotatef(-rotPos6, 0, 0, 1);
	glPushMatrix();
	//1st
	glPushMatrix();
	glTranslatef(0, 6, 0);
	if (!clockBool) {
		//glRotatef(rotPos6, 0, 0, 1);
		glRotatef(angleSec, 0, 0, 1);
	}
	else
		 glRotatef(rotPos, 0, 0, 1);

	glScalef(0.5, 0.5, 1);
	drawShuriken();
	drawInnerSwirl();
	glScalef(2, 2, 1);

	planetList[1].drawPlanets();
	glPopMatrix();

	//3rd
	glPushMatrix();
	glTranslatef(6, 0, 0);
	if (!clockBool) {
		glRotatef(angleSec, 0, 0, 1);
		//glRotatef(rotPos6, 0, 0, 1);
	}
	else
		glRotatef(rotPos, 0, 0, 1);

	glScalef(0.5, 0.5, 1);
	drawShuriken();
	drawInnerSwirl();
	glScalef(2, 2, 1);

	planetList[3].drawPlanets();
	glPopMatrix();

	//5th
	glPushMatrix();
	glTranslatef(0, -6, 0);
	if (!clockBool) {
		//glRotatef(rotPos6, 0, 0, 1);
		glRotatef(angleSec, 0, 0, 1);
	}
	else
		glRotatef(rotPos, 0, 0, 1);

	glScalef(0.5, 0.5, 1);
	drawShuriken();
	drawInnerSwirl();
	glScalef(2, 2, 1);

	planetList[5].drawPlanets();
	glPopMatrix();

	//7th
	glPushMatrix();
	glTranslatef(-6, 0, 0);
	if (!clockBool) {
		//glRotatef(rotPos6, 0, 0, 1);
		glRotatef(angleSec, 0, 0, 1);
	}
	else
		glRotatef(rotPos, 0, 0, 1);

	glScalef(0.5, 0.5, 1);
	drawShuriken();
	drawInnerSwirl();
	glScalef(2, 2, 1);

	planetList[7].drawPlanets();
	glPopMatrix();

	glPushMatrix();
	//2nd
	glPushMatrix();
	glTranslatef(-3.8, 4, 0);
	if (!clockBool) {
		//glRotatef(rotPos6, 0, 0, 1);
		glRotatef(angleSec, 0, 0, 1);
	}
	else
		glRotatef(rotPos, 0, 0, 1);

	glScalef(0.5, 0.5, 1);
	drawShuriken();
	drawInnerSwirl();
	glScalef(2, 2, 1);

	planetList[2].drawPlanets();
	glPopMatrix();

	//4th
	glPushMatrix();
	if (!clockBool) {
		glTranslatef(-3.8, -4, 0);
		//glRotatef(rotPos5, 0, 0, 1);
		glRotatef(angleSec, 0, 0, 1);
	}
	else {
		glTranslatef(5 * cos(M_PI * (rotPos / 360)), -8 * sin(M_PI * (rotPos / 360)), 0);//Disable this to see the rotation of 6th about 4th
		glRotatef(rotPos, 0, 0, 1);
	}

	glPushMatrix();
	if (!clockBool) {
		glTranslatef(4, 4, 0);
		glRotatef(rotPos5, 0, 0, 1);
		//glRotatef(angleSec, 0, 0, 1);	
	}
	else {
		glTranslatef(-3 * cos(M_PI * (rotPos / 360)), 3 * sin(M_PI * (rotPos / 360)), 0);
		glRotatef(rotPos, 0, 0, 1);

		glScalef(0.7, 0.7, 1);//scale the whole rasenshuriken
		
		glScalef(0.5, 0.5, 1);
		drawShuriken();
		drawInnerSwirl();
		glScalef(2, 2, 1);

		planetList[6].drawPlanets();
	}

	glPopMatrix();

	glScalef(0.5, 0.5, 1);
	drawShuriken();
	drawInnerSwirl();
	glScalef(2, 2, 1);

	planet(0, 0, 0.1, 0.35, 0.7, 0.2, 0.7, 0).drawPlanets();
	planetList[4].drawPlanets();

	glPopMatrix();

	//6th
	glPushMatrix();
	if (!clockBool) {
		glTranslatef(4, 4, 0);
		//glRotatef(rotPos5, 0, 0, 1);
		glRotatef(angleSec, 0, 0, 1);

		glScalef(0.5, 0.5, 1);
		drawShuriken();
		drawInnerSwirl();
		glScalef(2, 2, 1);

		planetList[6].drawPlanets();
	}
	else {
		glRotatef(0, 0, 0, 1);
		glTranslatef(5 * cos(M_PI * (rotPos / 360)), 8 * sin(M_PI * (rotPos / 360)), 0);
		glRotatef(rotPos, 0, 0, 1);
	}

	glPopMatrix();

	//8th
	glPushMatrix();
	glTranslatef(4, -4, 0);
	if (!clockBool) {
		//glRotatef(rotPos5, 0, 0, 1);
		glRotatef(angleSec, 0, 0, 1);
	}
	else 
		glRotatef(rotPos, 0, 0, 1);
	
	glScalef(0.5, 0.5, 1);
	drawShuriken();
	drawInnerSwirl();
	glScalef(2, 2, 1);
	
	planet(0, 0, 0.6, 0.3, 0.7, 0.5, 0.7, 0).drawPlanets();
	planet(0, 0, 0.6, 0.3, 0.7, 0.5, 0.5, 0).drawPlanets();
	planetList[8].drawPlanets();	
	glPopMatrix();

	glPopMatrix();
	
	
	glPopMatrix();

	//Timer
	if (timerBool) {
		glPushMatrix();
		
		//HourHand
		glPushMatrix();
		glRotatef(angleHour, 0, 0, 1);
		glTranslatef(8, 0, 0);
		
		glRotatef(rotPos5, 0, 0, 1);
		glScalef(0.5, 0.5, 1);
		drawShuriken();
		drawInnerSwirl();
		glScalef(2, 2, 1);

		planet(0, 0, 0, 0.3, 0, 0.5, 1, 0).drawPlanets();
		glPopMatrix();
		glPopMatrix();

		//MinHand
		glPushMatrix();
		glRotatef(angleMin, 0, 0, 1);
		glTranslatef(8, 0, 0);
		glPushMatrix();
		glScalef(0.85, 0.85, 1);
		glTranslatef(-3.8, -4, 0);
		glRotatef(rotPos4, 0, 0, 1);

		glScalef(0.5, 0.5, 1);
		drawShuriken();
		drawInnerSwirl();
		glScalef(2, 2, 1);

		planet(0, 0, 0.1, 0.35, 0.7, 0.5, 1, 0).drawPlanets();
		glPopMatrix();
		glPopMatrix();

		//SecHand
		glPushMatrix();
		glRotatef(angleSec, 0, 0, 1);
		glTranslatef(5, 0, 0);
		glPushMatrix();
		glScalef(0.7, 0.7, 1);
		glTranslatef(-3.8, -4, 0);
		glRotatef(rotPos3, 0, 0, 1);

		glScalef(0.5, 0.5, 1);
		drawShuriken();
		drawInnerSwirl();
		glScalef(2, 2, 1);
		
		planet(0, 0, 0.9, 0.2, 0.2, 0.2, 0.7, 0).drawPlanets();
		planet(0, 0, 1, 0.75, 0.0, 0.5, 1, 0).drawPlanets();
		glPopMatrix();
		
		
		glPopMatrix();
	}

	glPopMatrix();
	glFlush();
}

void idle()
{
	//update animation here

	//glutPostRedisplay();	//after updating, draw the screen again
}

void Timer(int value) {

	
	rotPos += rotDelta;
	rotPos2 += rotDelta2;
	rotPos3 += rotDelta3;
	rotPos4 += rotDelta4;
	rotPos5 += rotDelta5;
	
	//for background
	rotPos6 += 0.5;
	blink += 0.05;
	
	refreshMill = 30;

	glutTimerFunc(refreshMill, Timer, 0);
	glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y)
{
	//keys to control scaling - k
	//keys to control rotation - alpha
	//keys to control translation - tx, ty
	switch (key) {

	case 'a':
		alpha += 10;
		glutPostRedisplay();
		break;

	case 'd':
		alpha -= 10;
		glutPostRedisplay();
		break;

	case 'q':
		k += 0.1;
		glutPostRedisplay();
		break;

	case 'e':
		if (k>0.1)
			k -= 0.1;
		glutPostRedisplay();
		break;

	case 'z':
		tx -= 0.1;
		glutPostRedisplay();
		break;

	case 'c':
		tx += 0.1;
		glutPostRedisplay();
		break;

	case 's':
		ty -= 0.1;
		glutPostRedisplay();
		break;

	case 'w':
		ty += 0.1;
		glutPostRedisplay();
		break;

	case 't':
		timerBool = !timerBool;
		clockBool = !clockBool;
		break;

	case 27:
		exit(0);
		break;

	default:
		break;
	}
}

int main(int argc, char **argv)
{
	cout << "CS3241 Lab 2\n\n";
	cout << "+++++CONTROL BUTTONS+++++++\n\n";
	cout << "Scale Up/Down: Q/E\n";
	cout << "Rotate Clockwise/Counter-clockwise: A/D\n";
	cout << "Move Up/Down: W/S\n";
	cout << "Move Left/Right: Z/C\n";
	cout << "ESC: Quit\n";

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(50, 50);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	//glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(0, Timer, 0);
	glutMainLoop();

	return 0;
}
