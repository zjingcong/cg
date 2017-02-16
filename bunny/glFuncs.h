# include <iostream>
# include <math.h>

# include "Vector.h"
# include "Camera.h"

using namespace std;

# define GLUT_WHEEL_UP 3
# define GLUT_WHEEL_DOWN 4
# define ZOOM	0.08


GLfloat *vertices;
GLuint *faces;
int vertex_count, face_count;
Camera myCamera;

static int mouse_button = 0; // 0: no button, 1: left button, 2: right button
static Vector left_origin;	// mouse controlled rotation
static Vector right_origin;	// mouse controlled track


/*
.ply file parser to load ply model
*/
void load_ply(const char *file_name)
{
	cout << "Load model " << file_name << "..." << endl;

	FILE *fptr;
	char buf[512], *parse;

	fptr = fopen(file_name, "r");

	// parse header to get vertex_count and face_count
	// char * fgets ( char * str, int num, FILE * stream );
	fgets(buf, 512, fptr);	// strip off PLY header
	fgets(buf, 512, fptr);	// strip off format line
	fgets(buf, 512, fptr);	// strip off comment
	fgets(buf, 512, fptr);	// get line with vertex count
	parse = strtok(buf, " \t");
	parse = strtok(NULL, " \t");
	parse = strtok(NULL, " \t\n");
	vertex_count = atoi(parse);
	fgets(buf, 512, fptr);	// strip off property x
	fgets(buf, 512, fptr);	// strip off property y
	fgets(buf, 512, fptr);	// strip off property z
	fgets(buf, 512, fptr);	// strip off property nx
	fgets(buf, 512, fptr);	// strip off property ny
	fgets(buf, 512, fptr);	// strip off property nz
	fgets(buf, 512, fptr);	// get line with face count
	parse = strtok(buf, " \t");
	parse = strtok(NULL, " \t");
	parse = strtok(NULL, " \t\n");
	face_count = atoi(parse);
	fgets(buf, 512, fptr);	// strip off property list
	fgets(buf, 512, fptr);	// strip off property end header

	// allocate memory for vertices and faces array
	cout << "vertex_count: " << vertex_count << endl;
	cout << "face_count: " << face_count << endl;
	vertices = (GLfloat *)calloc(vertex_count, 6 * sizeof(GLfloat));
	faces = (GLuint *)calloc(face_count, 3 * sizeof(GLuint));

	// load vertices
	for (int i = 0; i < vertex_count; ++i)
	{
		fgets(buf, 512, fptr);	// get line of vertex
		parse = strtok(buf,  " \t\n");
		vertices[i * 6] = atof(parse);
		for (int j = 1; j < 6; ++j)
		{
			parse = strtok(NULL,  " \t\n");
			vertices[i * 6 + j] = atof(parse);
		}
	}
	// load faces
	for (int i = 0; i < face_count; ++i)
	{
		fgets(buf, 512, fptr);	// get line of face
		parse = strtok(buf,  " \t\n");
		for (int j = 1; j < 4; ++j)
		{
			parse = strtok(NULL,  " \t\n");
			faces[i * 3 + (j - 1)] = atof(parse);
		}
	}
	cout << "Load model success." << endl;
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
		case 'K':
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
	    double deltaAngle = delta_v.magnitude() * 0.01f;
      // get rotation axis
      Vector my_view_dir = myCamera.getViewDir();
			Vector right = myCamera.getRight();
			Vector axis = -delta_v.Y() * myCamera.getRight() - delta_v.X() * myCamera.getUp();
	    // update camera's direction
	    myCamera.rotation(deltaAngle, axis);
			glutPostRedisplay();

      break;
    }

		// mouse right click
		case 2:
		{
			double delta_x = (x - right_origin.X()) * 0.0001f;
			double delta_y = (y - right_origin.Y()) * 0.0001f;
			Vector trans = - delta_x * myCamera.getRight() + delta_y * myCamera.getUp();
			myCamera.track(trans);
			glutPostRedisplay();

			break;
		}

    default:
      return;
  }
}

