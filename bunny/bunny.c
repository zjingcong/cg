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

using namespace std;


struct point
{
	float x, y, z;
};


extern GLfloat *vertices;
extern GLuint *faces;
extern int vertex_count, face_count;


void setup_viewvolume()
{
	struct point eye, view, up;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 1.0, 0.1, 20.0);	// fov, aspect, near, far

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	eye.x = 1.0; eye.y = 1.0; eye.z = 1.0;
	view.x = 0.0; view.y = 0.0; view.z = 0.0;
	up.x = 0.0; up.y = 1.0; up.z = 0.0;
	gluLookAt(eye.x,eye.y,eye.z,view.x,view.y,view.z,up.x,up.y,up.z);
}


void display()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 6 * sizeof(GLfloat), vertices);
	glColorPointer(3, GL_FLOAT, 6 * sizeof(GLfloat), &vertices[3]);

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glDrawElements(GL_TRIANGLES, 3 * face_count, GL_UNSIGNED_INT,&faces[1]);
	glFlush();
}


int main(int argc, char *argv[])
{
	// load bunny model
	load_ply("bunnyN.ply");

	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH);
	glutInitWindowSize(768,768);
	glutInitWindowPosition(100,50);
	glutCreateWindow("My Bunny");
	glClearColor(0.35,0.35,0.35,0.0);
	glEnable(GL_DEPTH_TEST);

	setup_viewvolume();

	// set up the callback routines to be called when glutMainLoop() detects an event
	glutDisplayFunc(display);
	glutKeyboardFunc(handleKey);	// keyboard callback

	glutMainLoop();	

	return 0;
}

