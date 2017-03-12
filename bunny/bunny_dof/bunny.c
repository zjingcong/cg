
# include <GL/gl.h>
# include <GL/glu.h>
# include <GL/glut.h>
# include <GL/glx.h>
# include <GL/glext.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <iostream>
# include "math.h"
# include <glm/vec3.hpp> // glm::vec3
# include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
# include "bunnyScene.h"

using namespace std;
using namespace glm;

# define WINDOW_SIZE 768
# define EYEDX 0.02


extern GLfloat *vertices;
extern GLuint *faces;
extern int vertex_count, face_count;

glm::vec3 eye(0.3, 0.3, 0.3);
glm::vec3 view(-0.05, 0.08, 0.0);
glm::vec3 up(0.0, 1.0, 0.0);
glm::vec3 view_dir = glm::normalize(view - eye);
glm::vec3 my_right = glm::normalize(glm::cross(view_dir, up));

float focal = 0.5;
float near = 0.01;
float far = 5.0;


void setup_viewvolume(float xt)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-0.5 * near + xt * (near / focal), 0.5 * near + xt * (near / focal), -0.5 * near, 0.5 * near, near, far);	// left, right, bottom, top, nearVal, farVal

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye.x, eye.y, eye.z, view.x, view.y, view.z, up.x, up.y, up.z);

	 glTranslatef( my_right.x*xt, my_right.y*xt, my_right.z*xt);
}


void display()
{
    int p;

	set_lights();
	set_material(1);
	set_shaders(1);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 6 * sizeof(GLfloat), vertices);
	glNormalPointer(GL_FLOAT, 6 * sizeof(GLfloat), &vertices[3]);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawElements(GL_TRIANGLES, 3 * face_count, GL_UNSIGNED_INT, faces);
	glFlush();

	glPushMatrix();
	glTranslatef(0.0,0.069,0.0);
	glRotatef(180,0.0,1.0,0.0);
	glScalef(-1.0,-1.0,-1.0);
    glDrawElements(GL_TRIANGLES, 3 * face_count, GL_UNSIGNED_INT, faces);
    glPopMatrix();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    set_material(2);
    p=set_shaders(2);
	set_uniform(p);
	glNormal3f(0.0,1.0,0.0);
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,1);
    glTranslatef(0.0,0.035,0.0);
    glBegin(GL_QUADS);
	glTexCoord2d(1.0,1.0);glVertex3f(0.25,0.0,0.25);
	glTexCoord2d(1.0,0.0);glVertex3f(0.25,0.0,-0.25);
	glTexCoord2d(0.0,0.0);glVertex3f(-0.25,0.0,-0.25);
	glTexCoord2d(0.0,1.0);glVertex3f(-0.25,0.0,0.25);
	glEnd();
    glutSwapBuffers();

}


void sceneRenderer()
{
	float xt;
	glClear(GL_ACCUM_BUFFER_BIT);
	for (xt = -EYEDX; xt < EYEDX; xt += EYEDX / 10.0)
	{
		setup_viewvolume(xt);
		display();
		glFlush();
		glAccum(GL_ACCUM, 0.05);
	}
	glAccum(GL_RETURN, 1.0);
	glutSwapBuffers();
}


/*
Keyboard Callback Routine: 'q', 'Q' or ESC quit
This routine is called every time a key is pressed on the keyboard
*/
void handleKey(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'q':		// q - quit
		case 'Q':
		case 27:		// esc - quit
			exit(0);

		default:		// not a valid key -- just ignore it
			return;
  }
}


void initGL(int argc, char *argv[])
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_ACCUM);
	glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
	glutInitWindowPosition(100, 50);
	glutCreateWindow("My Bunny");
	load_texture(argv[1]);
	glClearColor(0.35, 0.35, 0.35, 0.0);
	glClearAccum(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE_ARB);
}


int main(int argc, char *argv[])
{
    cout << "Project 2: Show Me The Bunny!" << endl;
    cout << "=============================" << endl;
    cout << "[Usage]./bunny [texture_file_path]" << endl;
    cout << "Press Q, q or exit to quit program." << endl;
	srandom(123456789);

	// load bunny model
	load_ply("bunnyN.ply");
	initGL(argc, argv);
	// set up the callback routines to be called when glutMainLoop() detects an event
	glutDisplayFunc(sceneRenderer);
	 glutKeyboardFunc(handleKey);	// keyboard callback

	glutMainLoop();

	return 0;
}

