//
// This is an introduction to vertex arrays.
//
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glut.h>

struct point {
	float x, y, z;
	};

void setup_the_viewvolume()
{
struct point eye, view, up;
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluPerspective(45.0,1.0,0.1,20.0);

glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
eye.x = 0.0; eye.y = 2.0; eye.z = 2.0;
view.x = 0.0; view.y = 0.0; view.z = 0.0;
up.x = 0.0; up.y = 1.0; up.z = 0.0;
gluLookAt(eye.x,eye.y,eye.z,view.x,view.y,view.z,up.x,up.y,up.z);
}

void draw_stuff()
{
int i;
GLfloat colored_vertex[] = {
0.0,0.0,0.0, 1.0,0.0,0.0,
0.0,1.0,0.0, 0.0,1.0,0.0,
1.0,1.0,0.0, 0.0,0.0,1.0,
1.0,0.0,0.0, 1.0,0.0,0.0,
0.0,0.0,1.0, 0.0,1.0,0.0,
0.0,1.0,1.0, 0.0,0.0,1.0,
1.0,1.0,1.0, 1.0,1.0,0.0,
1.0,0.0,1.0, 0.8,0.8,0.8
};

GLuint face[] = {
0, 4, 3, 
0, 3, 1, 
0, 1, 4, 
4, 3, 1
};

glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_COLOR_ARRAY);
glVertexPointer(3,GL_FLOAT,6*sizeof(GLfloat),colored_vertex);
glColorPointer(3,GL_FLOAT,6*sizeof(GLfloat),&colored_vertex[3]);

glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
glDrawElements(GL_QUADS,12,GL_UNSIGNED_INT,face);
glFlush();
}

void getout(unsigned char key, int x, int y)
{
switch(key) {
        case 'q':
                exit(1);
        default:
                break;
    }
}

int main(int argc, char **argv)
{
glutInit(&argc,argv);
glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH);
glutInitWindowSize(768,768);
glutInitWindowPosition(100,50);
glutCreateWindow("my_cool_cube");
glClearColor(0.35,0.35,0.35,0.0);
glEnable(GL_DEPTH_TEST);
setup_the_viewvolume();
glutDisplayFunc(draw_stuff);
glutKeyboardFunc(getout);
glutMainLoop();
return 0;
}

