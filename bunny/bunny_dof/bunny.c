# include <GL/gl.h>
# include <GL/glu.h>
# include <GL/glut.h>
# include <GL/glx.h>
# include <GL/glext.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <iostream>

# include "glFuncs.h"
# include "Vector.h"
# include "Camera.h"
# include "bunnyScene.h"

using namespace std;

# define WINDOW_SIZE 768
# define EYEDX 0.02

extern GLfloat *vertices;
extern GLuint *faces;
extern int vertex_count, face_count;
extern Camera myCamera;


void setup_viewvolume(Vector& eye, Vector& view, Vector& up, float xt)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// gluPerspective(60.0, 1.0, myCamera.getNear(), myCamera.getFar());	// fov, aspect, near, far
	float focal = myCamera.getFocal();
	float near = myCamera.getNear();
	float far = myCamera.getFar();	
	glFrustum(-0.5 * near + xt * (near / focal), 0.5 * near + xt * (near / focal), -0.5 * near, 0.5 * near, near, far);	// left, right, bottom, top, nearVal, farVal
	// glFrustum(-0.5+xt*(1.0/5.0),0.5+xt*(1.0/5.0),-0.5,0.5,1.0,15.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye.X(),eye.Y(),eye.Z(),view.X(),view.Y(),view.Z(),up.X(),up.Y(),up.Z());
	// glTranslatef(1.0 + xt, 1.0, 1.0);
}


void display()
{
  // setup_viewvolume(myCamera.getEye(), myCamera.getView(), myCamera.getUp(), xt);
	set_lights();
	set_material();
	set_shaders();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 6 * sizeof(GLfloat), vertices);
	glNormalPointer(GL_FLOAT, 6 * sizeof(GLfloat), &vertices[3]);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawElements(GL_TRIANGLES, 3 * face_count, GL_UNSIGNED_INT, faces);
	glFlush();
}


void sceneRenderer()
{
	float xt;
	glClear(GL_ACCUM_BUFFER_BIT);
	for (xt = -EYEDX; xt < EYEDX; xt += EYEDX / 10.0)
	{
		setup_viewvolume(myCamera.getEye(), myCamera.getView(), myCamera.getUp(), xt);
		display();
		glFlush();
		glAccum(GL_ACCUM, 0.05);
	}
	glAccum(GL_RETURN, 1.0);
	// glDrawElements(GL_TRIANGLES, 3 * face_count, GL_UNSIGNED_INT, faces);
	glutSwapBuffers();
}


void initGL(int argc, char *argv[])
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_ACCUM);
	glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
	glutInitWindowPosition(100, 50);
	glutCreateWindow("My Bunny");
	glClearColor(0.35, 0.35, 0.35, 0.0);
	glClearAccum(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE_ARB);
}


int main(int argc, char *argv[])
{
	srandom(123456789);

	// load bunny model
	load_ply("bunnyN.ply");

	initGL(argc, argv);

	// set up the callback routines to be called when glutMainLoop() detects an event
	glutDisplayFunc(sceneRenderer);
	// glutDisplayFunc(display);
	// glutIdleFunc(sceneRenderer);
	// glutKeyboardFunc(handleKey);	// keyboard callback
	// glutMouseFunc(mouseButton); // mouse click and mouse scrollwheel callback
	// glutMotionFunc(mouseMove);  // mouse movement callback

	glutMainLoop();

	return 0;
}

