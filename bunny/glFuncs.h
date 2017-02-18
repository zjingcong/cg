# include <iostream>
# include <math.h>

# include "Vector.h"
# include "Camera.h"

using namespace std;

# define GLUT_WHEEL_UP 3
# define GLUT_WHEEL_DOWN 4
# define ZOOM	0.1


Camera myCamera;

static int mouse_button = 0; // 0: no button, 1: left button, 2: right button
static Vector left_origin;	// mouse controlled rotation
static Vector right_origin;	// mouse controlled track


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
    else {mouse_button = 1; left_origin = Vector(x, y, 0);}
  }
	// mouse right click to track
	if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_UP)	{mouse_button = 0;}
		else	{mouse_button = 2; right_origin = Vector(x, y, 0);}
	}
	// mouse scrollwheel zoom in/out
	if (state == GLUT_UP && button == GLUT_WHEEL_UP)
	{
		myCamera.zoom(ZOOM);
		glutPostRedisplay();
	}
	else if (state == GLUT_UP && button == GLUT_WHEEL_DOWN)
	{
		myCamera.zoom(-ZOOM);
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

      break;
    }

		// mouse right click
		case 2:
		{
			double delta_x = (x - right_origin.X()) * 0.0005f;
			double delta_y = (y - right_origin.Y()) * 0.0005f;
			Vector trans = - delta_x * myCamera.getRight() + delta_y * myCamera.getUp();
			myCamera.track(trans);
			right_origin = Vector(x, y, 0);
			glutPostRedisplay();

			break;
		}

    default:
      return;
  }
}

