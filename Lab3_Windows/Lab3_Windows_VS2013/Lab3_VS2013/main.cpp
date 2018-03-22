// CS3241Lab3.cpp : Defines the entry point for the console application.
//#include <cmath>
#include "math.h"
#include <iostream>
#include <fstream>

#ifdef _WIN32
#include <Windows.h>
#include "glut.h"
#define M_PI 3.141592654
#elif __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#endif

// global variable

bool m_Smooth = false;
bool m_Highlight = false;
GLfloat angle = 0;   /* in degrees */
GLfloat angle2 = 0;   /* in degrees */
GLfloat zoom = 1.0;
GLfloat field_of_view = 40.0;
GLfloat x_translation = 0.0;
GLfloat eye_x = 1;
GLfloat eye_y = 2;
GLfloat eye_z = 1;
GLdouble fovy = 40.0f;
GLdouble nearPlane = 1.0f;
GLdouble farPlane = 80.0f;
float lightpos = 10.0f;

float switchColour = true;

//For animation
bool animate = false;
float jump = 0;
bool enableCamera = false;

float theta = 0;
int refreshMill = 30;

int mouseButton = 0;
int moving, startx, starty;

#define NO_OBJECT 4;
int current_object = 0;

using namespace std;

void setupLighting()
{
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);

	// Lights, material properties
	GLfloat	ambientProperties[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat	diffuseProperties[] = { 0.1f, 0.5f, 0.8f, 1.0f };
	GLfloat	specularProperties[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat lightPosition[] = { -100.0f,  100.0f, 100.0f, 1.0f };

	glClearDepth(1.0);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientProperties);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseProperties);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularProperties);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);

	// Default : lighting
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

}

void drawEllipsoid(double r1, double r2, double r3, float r, float g, float b)
{

	float no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float mat_ambient[] = { r, g, b, 1.0f };
	float mat_diffuse[] = { 0.7f, 0.5f, 0.8f, 1.0f };
	float mat_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	float mat_emission[] = { 0.3f, 0.2f, 0.2f, 0.0f };
	float no_shininess = 0.0f;
	float shininess = 100.0;

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	if (m_Highlight)
	{
		// your codes for highlight here
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);
		//glMaterialfv(GL_FRONT, GL_EMISSION, mat_specular);//for light producing materials
	}
	else {
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialf(GL_FRONT, GL_SHININESS, no_shininess);
	}


	int i, j;
	int n = 20;
	for (i = 0; i<n; i++)
		for (j = 0; j<2 * n; j++)
			if (m_Smooth)
			{
				glBegin(GL_POLYGON);

				// the normal of each vertex is actaully its own coordinates normalized for a sphere

				// your normal here
				glNormal3d(sin(i*M_PI / n)*cos(j*M_PI / n),
					cos(i*M_PI / n)*cos(j*M_PI / n),
					sin(j*M_PI / n));

				glVertex3d(r1*sin(i*M_PI / n)*cos(j*M_PI / n), r2*cos(i*M_PI / n)*cos(j*M_PI / n), r3*sin(j*M_PI / n));
				// your normal here
				glNormal3d(sin((i + 1)*M_PI / n)*cos(j*M_PI / n),
					cos((i + 1)*M_PI / n)*cos(j*M_PI / n),
					sin(j*M_PI / n));

				glVertex3d(r1*sin((i + 1)*M_PI / n)*cos(j*M_PI / n), r2*cos((i + 1)*M_PI / n)*cos(j*M_PI / n), r3*sin(j*M_PI / n));
				// your normal here
				glNormal3d(sin((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n),
					cos((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n),
					sin((j + 1)*M_PI / n));

				glVertex3d(r1*sin((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r2*cos((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r3*sin((j + 1)*M_PI / n));
				// your normal here
				glNormal3d(sin(i*M_PI / n)*cos((j + 1)*M_PI / n),
					cos(i*M_PI / n)*cos((j + 1)*M_PI / n),
					sin((j + 1)*M_PI / n));

				glVertex3d(r1*sin(i*M_PI / n)*cos((j + 1)*M_PI / n), r2*cos(i*M_PI / n)*cos((j + 1)*M_PI / n), r3*sin((j + 1)*M_PI / n));
				glEnd();
			}
			else {
				glBegin(GL_POLYGON);
				// Explanation: the normal of the whole polygon is the coordinate of the center of the polygon for a sphere
				glNormal3d(sin((i + 0.5)*M_PI / n)*cos((j + 0.5)*M_PI / n),
					cos((i + 0.5)*M_PI / n)*cos((j + 0.5)*M_PI / n),
					sin((j + 0.5)*M_PI / n));

				glNormal3d(sin((i + 0.5)*M_PI / n)*cos((j + 0.5)*M_PI / n), cos((i + 0.5)*M_PI / n)*cos((j + 0.5)*M_PI / n), sin((j + 0.5)*M_PI / n));
				glVertex3d(r1*sin(i*M_PI / n)*cos(j*M_PI / n), r2*cos(i*M_PI / n)*cos(j*M_PI / n), r3*sin(j*M_PI / n));
				glVertex3d(r1*sin((i + 1)*M_PI / n)*cos(j*M_PI / n), r2*cos((i + 1)*M_PI / n)*cos(j*M_PI / n), r3*sin(j*M_PI / n));
				glVertex3d(r1*sin((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r2*cos((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r3*sin((j + 1)*M_PI / n));
				glVertex3d(r1*sin(i*M_PI / n)*cos((j + 1)*M_PI / n), r2*cos(i*M_PI / n)*cos((j + 1)*M_PI / n), r3*sin((j + 1)*M_PI / n));
				glEnd();
			}

}

void drawTorus(float R, float r, int N, int n, float rd, float g, float b) {
	int maxn = 1000; // max precision
	n = fmin(n, maxn - 1);
	N = fmin(N, maxn - 1);
	float rr = 1.5f*r;
	double dv = 2 * M_PI / n;
	double dw = 2 * M_PI / N;
	double v = 0.0f;
	double w = 0.0f;

	float no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	//float mat_ambient[] = { 1.0f, 0.7f, 0.0f, 1.0f };
	float mat_ambient[] = { rd, g, b, 1.0f };
	float mat_diffuse[] = { 0.5f, 0.5f, 0.8f, 1.0f };
	float mat_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	float mat_emission[] = { 0.3f, 0.2f, 0.2f, 0.0f };
	float no_shininess = 0.0f;
	float shininess = 100.0;

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	if (m_Highlight)
	{
		// your codes for highlight here
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);
		//glMaterialfv(GL_FRONT, GL_EMISSION, mat_specular);//for light producing materials
	}
	else {
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialf(GL_FRONT, GL_SHININESS, no_shininess);
	}

	if (m_Smooth) {
		// outer loop
		while (w < 2 * M_PI + dw)
		{
			v = 0.0f;
			glBegin(GL_TRIANGLE_STRIP);
			// inner loop
			while (v < 2 * M_PI + dv)
			{
				glNormal3d(
					(R + rr*cos(v))*cos(w) - (R + r*cos(v))*cos(w),
					(R + rr*cos(v))*sin(w) - (R + r*cos(v))*sin(w),
					(rr*sin(v) - r*sin(v)));
				glVertex3d((R + r*cos(v))*cos(w),
					(R + r*cos(v))*sin(w),
					r*sin(v));
				glNormal3d(
					(R + rr*cos(v + dv))*cos(w + dw) - (R + r*cos(v + dv))*cos(w + dw),
					(R + rr*cos(v + dv))*sin(w + dw) - (R + r*cos(v + dv))*sin(w + dw),
					rr*sin(v + dv) - r*sin(v + dv));
				glVertex3d((R + r*cos(v + dv))*cos(w + dw),
					(R + r*cos(v + dv))*sin(w + dw),
					r*sin(v + dv));
				v += dv;
			} // inner loop
			glEnd();
			w += dw;
		} //outer loop
	}
	else {
		// outer loop
		while (w < 2 * M_PI + dw)
		{
			v = 0.0f;
			glBegin(GL_TRIANGLE_STRIP);
			// inner loop
			while (v < 2 * M_PI + dv)
			{
				glNormal3d(
					(R + rr*cos(v))*cos(w) - (R + r*cos(v))*cos(w),
					(R + rr*cos(v))*sin(w) - (R + r*cos(v))*sin(w),
					(rr*sin(v) - r*sin(v)));

				glVertex3d((R + r*cos(v))*cos(w),
					(R + r*cos(v))*sin(w),
					r*sin(v));

				glVertex3d((R + r*cos(v + dv))*cos(w + dw),
					(R + r*cos(v + dv))*sin(w + dw),
					r*sin(v + dv));
				v += dv;
			} // inner loop
			glEnd();
			w += dw;
		} //outer loop
	}
}

void drawSphere(double r, float rd, float g, float b, BOOLEAN light)
{

	float no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float mat_ambient[] = { rd, g, b, 1.0f };
	float mat_diffuse[] = { 0.1f, 0.5f, 0.8f, 1.0f };
	float mat_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	float mat_emission[] = { 0.3f, 0.3f, 0.3f, 0.0f };
	float no_shininess = 0.0f;
	float shininess = 100.0;

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	if (light)
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	else
		glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	if (m_Highlight)
	{
		// your codes for highlight here
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess);
		//glMaterialfv(GL_FRONT, GL_EMISSION, mat_specular);//for light producing materials
	}
	else {
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialf(GL_FRONT, GL_SHININESS, no_shininess);
	}


	int i, j;
	int n = 20;
	for (i = 0; i<n; i++)
		for (j = 0; j<2 * n; j++)
			if (m_Smooth)
			{
				glBegin(GL_POLYGON);

				// the normal of each vertex is actaully its own coordinates normalized for a sphere

				// your normal here
				glNormal3d(sin(i*M_PI / n)*cos(j*M_PI / n),
					cos(i*M_PI / n)*cos(j*M_PI / n),
					sin(j*M_PI / n));

				glVertex3d(r*sin(i*M_PI / n)*cos(j*M_PI / n), r*cos(i*M_PI / n)*cos(j*M_PI / n), r*sin(j*M_PI / n));
				// your normal here
				glNormal3d(sin((i + 1)*M_PI / n)*cos(j*M_PI / n),
					cos((i + 1)*M_PI / n)*cos(j*M_PI / n),
					sin(j*M_PI / n));

				glVertex3d(r*sin((i + 1)*M_PI / n)*cos(j*M_PI / n), r*cos((i + 1)*M_PI / n)*cos(j*M_PI / n), r*sin(j*M_PI / n));
				// your normal here
				glNormal3d(sin((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n),
					cos((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n),
					sin((j + 1)*M_PI / n));

				glVertex3d(r*sin((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r*cos((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n));
				// your normal here
				glNormal3d(sin(i*M_PI / n)*cos((j + 1)*M_PI / n),
					cos(i*M_PI / n)*cos((j + 1)*M_PI / n),
					sin((j + 1)*M_PI / n));

				glVertex3d(r*sin(i*M_PI / n)*cos((j + 1)*M_PI / n), r*cos(i*M_PI / n)*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n));
				glEnd();
			}
			else {
				glBegin(GL_POLYGON);
				// Explanation: the normal of the whole polygon is the coordinate of the center of the polygon for a sphere
				glNormal3d(sin((i + 0.5)*M_PI / n)*cos((j + 0.5)*M_PI / n),
					cos((i + 0.5)*M_PI / n)*cos((j + 0.5)*M_PI / n),
					sin((j + 0.5)*M_PI / n));

				glNormal3d(sin((i + 0.5)*M_PI / n)*cos((j + 0.5)*M_PI / n), cos((i + 0.5)*M_PI / n)*cos((j + 0.5)*M_PI / n), sin((j + 0.5)*M_PI / n));
				glVertex3d(r*sin(i*M_PI / n)*cos(j*M_PI / n), r*cos(i*M_PI / n)*cos(j*M_PI / n), r*sin(j*M_PI / n));
				glVertex3d(r*sin((i + 1)*M_PI / n)*cos(j*M_PI / n), r*cos((i + 1)*M_PI / n)*cos(j*M_PI / n), r*sin(j*M_PI / n));
				glVertex3d(r*sin((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r*cos((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n));
				glVertex3d(r*sin(i*M_PI / n)*cos((j + 1)*M_PI / n), r*cos(i*M_PI / n)*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n));
				glEnd();
			}

}

void drawMushmom() {
	//body
	glPushMatrix();
	glPushMatrix();
	drawEllipsoid(1.0f, 1.5f, 1.0f, 0.9f, 0.85f, 0.8f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.55, 0);
	drawSphere(1.2, 0.9f, 0.85f, 0.8f, false);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.79, 0);
	drawSphere(1, 0.8f, 0.75f, 0.6f, false);
	glPopMatrix();

	//cap
	glPushMatrix();
	glTranslatef(0, 0.8, 0);
	if (switchColour)
		drawSphere(1, 1.0f, 0.7f, 0.0f, false);
	else
		drawSphere(1, 0.3f, 0.8f, 0.95f, false);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.7, 0);
	glRotatef(90, 1, 0, 0);
	if (switchColour)
		drawTorus(1.2f, 0.2f, 50, 50, 1.0f, 0.7f, 0.0f);
	else
		drawTorus(1.2f, 0.2f, 50, 50, 0.3f, 0.8f, 0.95f);
	glPopMatrix();

	//eyes
	glPushMatrix();
	glTranslatef(-0.35, 0.1, 0.87);
	drawSphere(0.1, 0, 0, 0, false);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.35, 0.1, 0.87);
	drawSphere(0.1, 0, 0, 0, false);
	glPopMatrix();

	//spots
	glPushMatrix();
	glTranslatef(-0.35, 1, 0.85);
	if (switchColour)
		drawSphere(0.1, 1, 0.9, 0, false);
	else
		drawSphere(0.1, 0.9, 0.9, 0.9, false);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.35, 1, 0.5);
	if (switchColour)
		drawSphere(0.4, 1, 0.9, 0, false);
	else
		drawSphere(0.4, 0.9, 0.9, 0.9, false);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.35, 1, -0.5);
	if (switchColour)
		drawSphere(0.4, 1, 0.9, 0, false);
	else
		drawSphere(0.4, 0.9, 0.9, 0.9, false);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 1.35, 0);
	if (switchColour)
		drawSphere(0.5, 1, 0.9, 0, false);
	else
		drawSphere(0.5, 0.9, 0.9, 0.9, false);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.4, 1.15, 0);
	if (switchColour)
		drawSphere(0.5, 1, 0.9, 0, false);
	else
		drawSphere(0.5, 0.9, 0.9, 0.9, false);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.35, 1, -0.7);
	if (switchColour)
		drawSphere(0.2, 1, 0.9, 0, false);
	else
		drawSphere(0.2, 0.9, 0.9, 0.9, false);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, 1, 0);
	if (switchColour)
		drawSphere(0.5, 1, 0.9, 0, false);
	else
		drawSphere(0.5, 0.9, 0.9, 0.9, false);
	glPopMatrix();

	//mouth
	glPushMatrix();
	glTranslatef(0.05, -0.1, 0.57);
	drawSphere(0.5, 0.9f, 0.85f, 0.8f, false);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.05, -0.1, 0.57);
	drawSphere(0.5, 0.9f, 0.85f, 0.8f, false);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.2, 0.62);
	drawSphere(0.5, 0.9f, 0.7f, 0.7f, false);
	glPopMatrix();

	glPopMatrix();

}

void drawSlime() {
	glPushMatrix();

	//slime bubble
	glPushMatrix();
	glTranslatef(1.5, 0, -1.1);
	if (switchColour)
		drawSphere(0.3, 0.4, 1, 0.4, true);
	else
		drawSphere(0.3, 1, 0.9, 0.0, false);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.85, 0.55, -0.7);
	glRotatef(100, 0, 1, 1);
	for (int i = 0; i < 30; i++) {
		glPushMatrix();
		glTranslatef(sin(i * M_PI / 30.0), cos(i * M_PI / 30.0), 0);
		drawSphere(0.05, 0, 0, 0, false);
		glPopMatrix();
	}
	glPopMatrix();

	//body
	glPushMatrix();
	glRotatef(90, 0, 0, 1);
	if (switchColour)
		drawEllipsoid(1.0f, 1.2f, 1.0f, 0.4, 1, 0.4);
	else
		drawEllipsoid(1.0f, 1.2f, 1.0f, 1, 0.9, 0.0);
	glPopMatrix();

	glPushMatrix();
	glRotatef(-10, 0, 0, 1);
	glTranslatef(0, 0.55, 0);
	if (switchColour)
		drawEllipsoid(0.5f, 0.6f, 0.5f, 0.4, 1, 0.4);
	else
		drawEllipsoid(0.5f, 0.6f, 0.5f, 1, 0.9, 0.0);
	glPopMatrix();

	//eyes
	glPushMatrix();
	glTranslatef(0.4, 0, 0.8);
	drawSphere(0.2, 0, 0, 0, false);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.25, 0, 0.04);
	glRotatef(10, 0, 1, 0);
	glScalef(0.2, 0.2, 1);
	drawEllipsoid(0.5f, 1.2f, 1.0f, 1, 0.9, 0);
	glRotatef(90, 0, 0, 1);
	drawEllipsoid(0.5f, 1.2f, 1.0f, 1, 0.9, 0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.4, 0, 0.8);
	drawSphere(0.2, 0, 0, 0, false);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.25, 0, 0.04);
	glRotatef(-10, 0, 1, 0);
	glScalef(0.2, 0.2, 1);
	drawEllipsoid(0.5f, 1.2f, 1.0f, 1, 0.9, 0);
	glRotatef(90, 0, 0, 1);
	drawEllipsoid(0.5f, 1.2f, 1.0f, 1, 0.9, 0);
	glPopMatrix();

	//mouth
	glPushMatrix();
	glTranslatef(0.05, -0.1, 0.94);
	if (switchColour)
		drawSphere(0.1, 0.4, 1, 0.4, false);
	else
		drawSphere(0.1, 1, 0.9, 0.0, false);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.05, -0.1, 0.94);
	if (switchColour)
		drawSphere(0.1, 0.4, 1, 0.4, false);
	else
		drawSphere(0.1, 1, 0.9, 0.0, false);
	glPopMatrix();

	glPopMatrix();
}

void drawMonsters() {
	glPushMatrix();

	//Lone Slime
	glPushMatrix();
	glTranslated(0, abs(cos(theta)), 0);
	drawSlime();
	glPopMatrix();

	//MushMOMS!!!
	glPushMatrix();
	glTranslatef(2, abs(sin(theta)), -3);
	drawMushmom();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2, abs(sin(theta)), -3);
	drawMushmom();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-4, abs(cos(theta)), -6);
	drawMushmom();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, abs(sin(theta)), -6);
	drawMushmom();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(4, abs(cos(theta)), -6);
	drawMushmom();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-6, abs(sin(theta)), -9);
	drawMushmom();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2, abs(cos(theta)), -9);
	drawMushmom();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2, abs(cos(theta)), -9);
	drawMushmom();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(6, abs(sin(theta)), -9);
	drawMushmom();
	glPopMatrix();

	glPopMatrix();
}

void Timer(int value) {
	if (enableCamera) {
		eye_z++;
		eye_y++;
		if (eye_y > 360) {
			eye_y = 0;
		}
		if (eye_z > 360) {
			eye_z = 0;
		}
	}

	if (animate) {
		theta++;
		if (theta > 360)
			theta = 0;

		GLfloat lightPosition[] = { 200 * cos(lightpos),  200.0f, 200 * sin(lightpos), 1.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

		lightpos += 1.0;
		if (lightpos >= 360) {
			lightpos = 0;
		}

	}
	refreshMill = 10;

	glutTimerFunc(refreshMill, Timer, 0);
	glutPostRedisplay();
}

void display(void)
{//Add Projection tool and Camera Movement somewhere here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Set up projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective( /* field of view in degree */ fovy,
		/* aspect ratio */ 1.0, nearPlane, farPlane);

	//Set up camera view
	glMatrixMode(GL_MODELVIEW);
	int upVector = 1;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(0, 0, -6);
	glRotatef(angle2, 1.0, 0.0, 0.0);
	glRotatef(angle, 0.0, 1.0, 0.0);

	glScalef(zoom, zoom, zoom);

	switch (current_object) {
	case 0:
		glTranslatef(-1, 0, 0);
		drawSphere(1, 1.0f, 0.7f, 0.0f, false);
		glTranslatef(2, 0, 0);
		drawEllipsoid(1.0f, 1.2f, 1.0f, 0.9f, 0.85f, 0.8f);
		break;
	case 1:
		// draw your second primitive object here
		drawTorus(1.0f, 0.5f, 50, 50, 1.0f, 0.7f, 0.0f);
		break;
	case 2:
		// draw your first composite object here
		drawMushmom();
		break;
	case 3:
		// draw your second composite object here
		if (enableCamera)
			gluLookAt(eye_x, cos(eye_y), sin(eye_z), 1, 1, -1, 0, 1, 0);
		else
			gluLookAt(1, 2, 1, 1, 1, -1, 0, 1, 0);
		glTranslatef(1, 0, 0);
		drawMonsters();
		break;
	default:
		break;
	};
	glPopMatrix();
	glutSwapBuffers();
}


void resetCamera() {
	zoom = 1.0;
	angle = 0;
	angle2 = 0;
	zoom = 1.0;
	field_of_view = 40.0;
	x_translation = 0.0;
	eye_x = 1;
	eye_y = 2;
	eye_z = 1;

	fovy = 40.0f;
	nearPlane = 1.0f;
	farPlane = 80.0f;

	return;
}

void setCameraBestAngle() {
	//fill in values below

	angle = 0.0;
	angle2 = 3.0;
	zoom = 0.52;
	field_of_view = 40.0;
	x_translation = 0.0;

	return;
}

void keyboard(unsigned char key, int x, int y)
{//add additional commands here to change Field of View and movement
	switch (key) {
	case 'p':
	case 'P':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 'w':
	case 'W':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'v':
	case 'V':
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
	case 's':
	case 'S':
		m_Smooth = !m_Smooth;
		break;
	case 'h':
	case 'H':
		m_Highlight = !m_Highlight;
		break;

	case 'd':
		animate = !animate;
		break;

	case 'D':
		enableCamera = !enableCamera;
		break;
	case 'c':
		switchColour = !switchColour;
		break;

	case 'n':
		// -Insert code to adjust Near-
		nearPlane--;
		break;

	case 'N':
		// -Insert code to adjust Near-
		nearPlane++;
		break;

	case 'f':
		// -Insert code to adjust Far-
		farPlane--;
		break;

	case 'F':
		// -Insert code to adjust Far-
		farPlane++;
		break;

	case 'o':
		// -Insert code to adjust Fovy-
		fovy--;
		break;

	case 'O':
		// -Insert code to adjust Fovy-
		fovy++;
		break;

	case 'r':
		resetCamera();
		break;

	case 'R':
		setCameraBestAngle();
		break;

	case '1':
	case '2':
	case '3':
	case '4':
		current_object = key - '1';
		break;

	case 27:
		exit(0);
		break;

	default:
		break;
	}

	glutPostRedisplay();
}


void
mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		mouseButton = button;
		moving = 1;
		startx = x;
		starty = y;
	}
	if (state == GLUT_UP) {
		mouseButton = button;
		moving = 0;
	}
}

void motion(int x, int y)
{
	if (moving) {
		if (mouseButton == GLUT_LEFT_BUTTON)
		{
			angle = angle + (x - startx);
			angle2 = angle2 + (y - starty);
		}
		else zoom += ((y - starty)*0.01);
		startx = x;
		starty = y;
		glutPostRedisplay();
	}

	//for finding out good camera angle and zoom
	//printf("angle x,y & zoom: %f,%f,%f", angle,angle2,zoom);
}


int main(int argc, char **argv)
{
	cout << "CS3241 Lab 3" << endl << endl;

	cout << "1-4: Draw different objects" << endl;
	cout << "S: Toggle Smooth Shading" << endl;
	cout << "H: Toggle Highlight" << endl;
	cout << "W: Draw Wireframe" << endl;
	cout << "P: Draw Polygon" << endl;
	cout << "V: Draw Vertices" << endl;
	cout << "d: dance party" << endl;
	cout << "D: dynamic leap of faith" << endl;
	cout << "n, N: Reduce or increase the distance of the near plane from the camera" << endl;
	cout << "f, F: Reduce or increase the distance of the far plane from the camera" << endl;
	cout << "o, O: Reduce or increase the distance of the povy plane from the camera" << endl;
	cout << "r: Reset camera to the initial parameters when the program starts" << endl;
	cout << "R: Change camera to another setting that is has the best viewing angle for your object" << endl;
	cout << "ESC: Quit" << endl << endl;


	cout << "Left mouse click and drag: rotate the object" << endl;
	cout << "Right mouse click and drag: zooming" << endl;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("CS3241 Assignment 3");
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glutDisplayFunc(display);
	glMatrixMode(GL_PROJECTION);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	setupLighting();
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glutTimerFunc(0, Timer, 0);//For Animation

	glutMainLoop();

	return 0;
}