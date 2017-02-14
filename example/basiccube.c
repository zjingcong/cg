// 
// basiccube.c, a wireframe, 3D unit cube with one vertex at the origin
//
// compile: 
// gcc -L/usr/lib64 -O2 basiccube.c -lX11 -lGL -lGLU -lglut -lm -lXmu 
// 	-o basiccube
//                                                                              
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

// specify size and shape of view volume 
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluPerspective(45.0,1.0,0.1,20.0);

// specify position for view volume 
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();

eye.x = 2.5; eye.y = 1.8; eye.z = 2.0;
view.x = 0.0; view.y = 0.0; view.z = 0.0;
up.x = 0.0; up.y = 1.0; up.z = 0.0;

gluLookAt(eye.x,eye.y,eye.z,view.x,view.y,view.z,up.x,up.y,up.z);
}

void draw_stuff()
{
int i;
struct point front[4] = {
	{0.0,0.0,1.0}, {0.0,1.0,1.0}, {1.0,1.0,1.0}, {1.0,0.0,1.0} };
struct point back[4] = {
	{1.0,0.0,0.0}, {1.0,1.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,0.0} };
struct point left[4] = {
	{0.0,0.0,0.0}, {0.0,1.0,0.0}, {0.0,1.0,1.0}, {0.0,0.0,1.0} };
struct point right[4] = {
	{1.0,0.0,1.0}, {1.0,1.0,1.0}, {1.0,1.0,0.0}, {1.0,0.0,0.0} };
struct point top[4] = {
	{0.0,1.0,1.0}, {0.0,1.0,0.0}, {1.0,1.0,0.0}, {1.0,1.0,1.0} };
struct point bottom[4] = {
	{0.0,0.0,0.0}, {0.0,0.0,1.0}, {1.0,0.0,1.0}, {1.0,0.0,0.0} };

// anti-aliasing
glEnable(GL_MULTISAMPLE_ARB);

// gray background 
glClearColor(0.35,0.35,0.35,0.0);
glClear(GL_COLOR_BUFFER_BIT);

// call for wireframe 
glPolygonMode(GL_FRONT,GL_LINE); 
glPolygonMode(GL_BACK,GL_LINE); 

// fat yellow lines 
glColor3f(1.0,1.0,0.0);
glLineWidth(2.0);

glBegin(GL_QUADS); 
for(i=0;i<4;i++) glVertex3f(front[i].x,front[i].y,front[i].z);
for(i=0;i<4;i++) glVertex3f(back[i].x,back[i].y,back[i].z);
for(i=0;i<4;i++) glVertex3f(left[i].x,left[i].y,left[i].z);
for(i=0;i<4;i++) glVertex3f(right[i].x,right[i].y,right[i].z);
for(i=0;i<4;i++) glVertex3f(top[i].x,top[i].y,top[i].z);
for(i=0;i<4;i++) glVertex3f(bottom[i].x,bottom[i].y,bottom[i].z);
glEnd();
glFlush();
}

int main(int argc, char** argv)
{
glutInit(&argc,argv);
glutInitDisplayMode(GLUT_RGBA|GLUT_MULTISAMPLE);
glutInitWindowSize(768,768);
glutInitWindowPosition(100,50);
glutCreateWindow("my_cool_cube");
setup_the_viewvolume();
glutDisplayFunc(draw_stuff);
glutMainLoop();
return 0;
}

