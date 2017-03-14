// 
// Render a teapot inside translucent sphere.
//

#include <stdio.h>
#include <GL/gl.h>
#include <GL/glut.h>

struct point {
	float x;
	float y;
	float z;
	};

void setup_da_viewvol()
{
struct point eye = {2.0,2.0,2.0};
struct point view = {0.0,0.0,0.0};
struct point up = {0.0,1.0,0.0};


glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluPerspective(45.0,1.0,0.1,20.0);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
gluLookAt(eye.x,eye.y,eye.z,view.x,view.y,view.z,up.x,up.y,up.z);
}

void do_material(int i)
{
float mat_ambient[] = {0.0,0.0,0.0,1.0};
// Note alpha values here.
float mat_diffuse[2][4] = {
	0.9,0.0,0.0,0.0,
	0.0,0.9,0.9,0.5
	};
float mat_specular[] = {1.0,1.0,1.0,1.0};
float mat_shininess[2][1] = {10.0,100.0};

glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse[i]);
glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess[i]);
}

void draw_stuff()
{
glClearColor(0.45,0.45,0.45,1.0);
glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
glEnable(GL_MULTISAMPLE_ARB);
glEnable(GL_DEPTH_TEST);

// Draw opaque objects first with blending off.

do_material(0);
glutSolidTeapot(0.6);

// Enable blending, draw translucent objects.
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
// There's a bug in glutSolidSphere that causes a polar artifact; 
// rendering only half of it (culling back faces) fixes that.
glCullFace(GL_BACK);
do_material(1);
glutSolidSphere(0.75,128,128);

// Note:
// We could draw the opaque object second, but the portion
// inside the sphere would be culled by the z-buffer, unless
// we were to set the z-buffer to read only using

// glDepthMask(GL_FALSE);

// while we drew the translucent sphere.  The effects would be 
// different from drawing the opaque object first, though, since 
// the opaque teapot would be written over the top (blended) with 
// the background color, (assuming teapot alpha = 0.5) and the blend 
// isn't the same.

glFlush();
}

void do_lights()
{
float light0_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
float light0_diffuse[] = { 0.4, 0.4, 0.4, 0.0 };
float light0_specular[] = { 0.8, 0.8, 0.8, 0.0 };
float light0_position[] = { 1.5, 2.0, 2.0, 1.0 };
float light0_direction[] = { -1.5, -2.0, -2.0, 1.0};
float light1_position[] = { -1.0, 3.0, 6.0, 1.0 };
float light1_direction[] = { 1.0, -3.0, -6.0, 1.0};

glLightModelfv(GL_LIGHT_MODEL_AMBIENT,light0_ambient);
glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,1);
glLightfv(GL_LIGHT0,GL_AMBIENT,light0_ambient);
glLightfv(GL_LIGHT0,GL_DIFFUSE,light0_diffuse);
glLightfv(GL_LIGHT0,GL_SPECULAR,light0_specular);
glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,1.0);
glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,180.0);
glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,1.0);
glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,0.1);
glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,0.01);
glLightfv(GL_LIGHT0,GL_POSITION,light0_position);
glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,light0_direction);

glLightfv(GL_LIGHT1,GL_AMBIENT,light0_ambient);
glLightfv(GL_LIGHT1,GL_DIFFUSE,light0_diffuse);
glLightfv(GL_LIGHT1,GL_SPECULAR,light0_specular);
glLightf(GL_LIGHT1,GL_SPOT_EXPONENT,1.0);
glLightf(GL_LIGHT1,GL_SPOT_CUTOFF,180.0);
glLightf(GL_LIGHT1,GL_CONSTANT_ATTENUATION,1.0);
glLightf(GL_LIGHT1,GL_LINEAR_ATTENUATION,0.1);
glLightf(GL_LIGHT1,GL_QUADRATIC_ATTENUATION,0.01);
glLightfv(GL_LIGHT1,GL_POSITION,light1_position);
glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,light1_direction);

glEnable(GL_LIGHTING);
glEnable(GL_LIGHT0);
glEnable(GL_LIGHT1);
}

void do_stuff()
{
setup_da_viewvol();
do_lights();
draw_stuff();
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
glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_MULTISAMPLE);
glutInitWindowSize(768,768);
glutInitWindowPosition(100,50);
glutCreateWindow("my_cool_cube");
glutKeyboardFunc(getout);
glutDisplayFunc(do_stuff);
glutMainLoop();
return 0;
}
