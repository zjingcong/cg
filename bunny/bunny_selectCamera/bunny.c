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

// # include "glFuncs.h"
# include "Vector.h"
# include "Camera.h"
# include "bunnyScene.h"

using namespace std;
using namespace glm;

# define WINDOW_SIZE 768
# define GLUT_WHEEL_UP 3
# define GLUT_WHEEL_DOWN 4
# define ZOOM	0.2

extern GLfloat *vertices;
extern GLuint *faces;
extern int vertex_count, face_count;

Camera myCamera;
static int mouse_button = 0; // 0: no button, 1: left button, 2: right button
// static Vector left_origin;	// mouse controlled rotation
glm::vec3 right_origin(0.0, 0.0, 0.0);	// mouse controlled track
glm::vec3 left_origin(0.0, 0.0, 0.0);	// mouse controlled track

glm::vec3 eye(0.3, 0.3, 0.3);
glm::vec3 view(0.0, 0.0, 0.0);
glm::vec3 up(0.0, 1.0, 0.0);
glm::vec3 view_dir = glm::normalize(view - eye);
glm::vec3 my_right = glm::normalize(glm::cross(view_dir, up));
float near = 0.01;
float far = 20.0;

void setup_viewvolume()
{
	// struct point eye, view, up;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 1.0, near, far);	// fov, aspect, near, far

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye.x, eye.y, eye.z, view.x, view.y, view.z, up.x, up.y, up.z);
}


void display()
{
  setup_viewvolume();
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

		// press f to focus object
		case 'f':
		case 'F':
			myCamera.reset();
			glutPostRedisplay();

			break;
						
		default:		// not a valid key -- just ignore it
			return;
  }
}


void mouseButton(int button, int state, int x, int y)
{
	// mouse left click to rotate
	if (button == GLUT_LEFT_BUTTON)
	{
		// mouse left button is released
		if (state == GLUT_UP) {mouse_button = 0;}
		else {mouse_button = 1; left_origin = glm::vec3(x, y, 0.0);}
	}
	// mouse right click to track
	if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_UP)	{mouse_button = 0;}
		else	{mouse_button = 2; right_origin = glm::vec3(x, y, 0.0);}
	}
	// mouse scrollwheel zoom in/out
	if (state == GLUT_UP && button == GLUT_WHEEL_UP)
	{
		vec3 view_eye = glm::normalize(view - eye);
		eye.x -= ZOOM * view_eye.x;
		eye.y -= ZOOM * view_eye.y;
		eye.z -= ZOOM * view_eye.z;
		view.x -= ZOOM * view_eye.x;
		view.y -= ZOOM * view_eye.y;
		view.z -= ZOOM * view_eye.z;
		glutPostRedisplay();
	}
	else if (state == GLUT_UP && button == GLUT_WHEEL_DOWN)
	{
		vec3 view_eye = glm::normalize(view - eye);
		eye.x += ZOOM * view_eye.x;
		eye.y += ZOOM * view_eye.y;
		eye.z += ZOOM * view_eye.z;
		view.x += ZOOM * view_eye.x;
		view.y += ZOOM * view_eye.y;
		view.z += ZOOM * view_eye.z;
		glutPostRedisplay();
	}
}


void mouseMove(int x, int y)
{
  switch (mouse_button)
  {
    case 0:
      break;

		// mouse left click
    case 1:
    {
			/*
      // get deltaAngle
      Vector xy(x, y, 0);
      Vector delta_v = xy - left_origin;
			if (delta_v.magnitude() == 0)	{delta_v = Vector(1.0, 1.0, 0.0);}
	    double deltaAngle = delta_v.magnitude() * 0.1f;
      // get rotation axis
      Vector my_view_dir = myCamera.getViewDir();
			Vector right = myCamera.getRight();
			Vector axis = -delta_v.Y() * myCamera.getRight() - delta_v.X() * myCamera.getUp();
	    // update camera's direction
	    myCamera.rotation(deltaAngle, axis);
			left_origin = Vector(x, y, 0);
			glutPostRedisplay();
			*/
			vec3 view_eye = glm::normalize(view - eye);
			float distance = glm::length(view_eye);

			// (eye.x * sin(theta_x + asin(left_origin.x / distance)) * distance);

			float theta_x = (x - left_origin.x) * M_PI / WINDOW_SIZE;
			float theta_y = (y - left_origin.y) * M_PI / WINDOW_SIZE;
			// eye.x += (sin(theta_x) * distance);
			// eye.z -= ((1 - cos(theta_x)) * distance);
			eye.x = (sin(theta_x + asin(left_origin.x / distance)) * distance);
			eye.z = (cos(theta_x + asin(left_origin.x / distance)) * distance);
			// eye.y += (eye.y * sin(theta_y) * distance);
			// eye.z -= (eye.z * (1 - cos(theta_y)) * distance);
			// left_origin = glm::vec3(x, y, 0);
			glutPostRedisplay();

      break;
    }

		// mouse right click
		case 2:
		{
			double delta_x = (x - right_origin.x) * 0.0005f;
			double delta_y = (y - right_origin.y) * 0.0005f;
			eye = eye + vec3(-delta_x, delta_y, 0.0);
			view = view + vec3(-delta_x, delta_y, 0.0);
			right_origin = glm::vec3(x, y, 0);
			break;
		}

    default:
      return;
  }
}


void initGL(int argc, char *argv[])
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
	glutInitWindowPosition(100, 50);
	glutCreateWindow("My Bunny");
	glClearColor(0.35,0.35,0.35,0.0);
	// OpenGL init
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE_ARB);
}


int main(int argc, char *argv[])
{
	// load bunny model
	load_ply("bunnyN.ply");

	cout << my_right.x << my_right.y << my_right.z << endl;

	initGL(argc, argv);

	// set up the callback routines to be called when glutMainLoop() detects an event
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutKeyboardFunc(handleKey);	// keyboard callback
	glutMouseFunc(mouseButton); // mouse click and mouse scrollwheel callback
	glutMotionFunc(mouseMove);  // mouse movement callback

	glutMainLoop();

	return 0;
}

