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

using namespace std;


extern GLfloat *vertices;
extern GLuint *faces;
extern int vertex_count, face_count;
extern Camera myCamera;


void setup_viewvolume(Vector& eye, Vector& view, Vector& up)
{
	// struct point eye, view, up;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 1.0, 0.1, 20.0);	// fov, aspect, near, far

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye.X(),eye.Y(),eye.Z(),view.X(),view.Y(),view.Z(),up.X(),up.Y(),up.Z());
}


void display()
{
  setup_viewvolume(myCamera.getEye(), myCamera.getView(), myCamera.getUp());

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 6 * sizeof(GLfloat), vertices);
	glColorPointer(3, GL_FLOAT, 6 * sizeof(GLfloat), &vertices[3]);

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glDrawElements(GL_TRIANGLES, 3 * face_count, GL_UNSIGNED_INT, faces);
	glFlush();
}


void initGL(int argc, char *argv[])
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH);
	glutInitWindowSize(768, 768);
	glutInitWindowPosition(100, 50);
	glutCreateWindow("My Bunny");
	glClearColor(0.35,0.35,0.35,0.0);
	// OpenGL init
	glEnable(GL_DEPTH_TEST);
}


int main(int argc, char *argv[])
{
	// load bunny model
	load_ply("bunnyN.ply");

	initGL(argc,argv);

	// set up the callback routines to be called when glutMainLoop() detects an event
  glutDisplayFunc(display);
  glutIdleFunc(display);
	glutKeyboardFunc(handleKey);	// keyboard callback
	glutMouseFunc(mouseButton); // mouse click callback
	glutMotionFunc(mouseMove);  // mouse movement callback

	glutMainLoop();

	return 0;
}

