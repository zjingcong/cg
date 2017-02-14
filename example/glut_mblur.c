//
// Motion blur using an accumulation buffer 
//
#include <stdio.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glut.h>

#define VPASSES 150
#define EMOVE 0.015
#define ADDEACH 0.02
#define FADE 0.98 

struct point {
	float x, y, z;
	};

void viewvolume()
{
struct point eye, view, up, vdir, utemp, vtemp;
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluPerspective(45.0,1.0,0.1,20.0);

eye.x = 4.0; eye.y = 4.0; eye.z = 4.0;
view.x = view.y = view.z = 0.0;
up.x = 0.0; up.y = 1.0; up.z = 0.0;

glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
gluLookAt(eye.x,eye.y,eye.z,view.x,view.y,view.z,up.x,up.y,up.z);
}

void draw_stuff()
{
int i;
struct point front[4]={{0.0,0.0,1.0},{1.0,0.0,1.0},{1.0,1.0,1.0},{0.0,1.0,1.0}};
struct point back[4]={{0.0,0.0,0.0},{0.0,1.0,0.0},{1.0,1.0,0.0},{1.0,0.0,0.0}};
struct point left[4]={{0.0,0.0,0.0},{0.0,0.0,1.0},{0.0,1.0,1.0},{0.0,1.0,0.0}};
struct point right[4]={{1.0,0.0,0.0},{1.0,1.0,0.0},{1.0,1.0,1.0},{1.0,0.0,1.0}};
struct point top[4]={{0.0,1.0,0.0},{0.0,1.0,1.0},{1.0,1.0,1.0},{1.0,1.0,0.0}};
struct point bottom[4]={{0.0,0.0,0.0},{0.0,0.0,1.0},{1.0,0.0,1.0},{1.0,0.0,0.0}};

glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
glBegin(GL_QUADS); 
glNormal3f(0.0,0.0,1.0);
for(i=0;i<4;i++) glVertex3f(front[i].x,front[i].y,front[i].z);
glNormal3f(0.0,0.0,-1.0);
for(i=0;i<4;i++) glVertex3f(back[i].x,back[i].y,back[i].z);
glNormal3f(-1.0,0.0,0.0);
for(i=0;i<4;i++) glVertex3f(left[i].x,left[i].y,left[i].z);
glNormal3f(1.0,0.0,0.0);
for(i=0;i<4;i++) glVertex3f(right[i].x,right[i].y,right[i].z);
glNormal3f(0.0,1.0,0.0);
for(i=0;i<4;i++) glVertex3f(top[i].x,top[i].y,top[i].z);
glNormal3f(0.0,-1.0,0.0);
for(i=0;i<4;i++) glVertex3f(bottom[i].x,bottom[i].y,bottom[i].z);
glEnd();
glFlush();
}

void do_lights()
{
float light0_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
float light0_diffuse[] = { 2.0, 2.0, 2.0, 0.0 }; 
float light0_specular[] = { 2.25, 2.25, 2.25, 0.0 }; 
float light0_position[] = { 4.0, 3.0, 2.0, 1.0 };
float light0_direction[] = { -4.0, -3.0, -2.0, 1.0};

glLightModelfv(GL_LIGHT_MODEL_AMBIENT,light0_ambient); 

glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,1); 
glLightfv(GL_LIGHT0,GL_AMBIENT,light0_ambient); 
glLightfv(GL_LIGHT0,GL_DIFFUSE,light0_diffuse); 
glLightfv(GL_LIGHT0,GL_SPECULAR,light0_specular); 
glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,1.0); 
glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,180.0); 
glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,1.0); 
glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,0.2); 
glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,0.01); 
glLightfv(GL_LIGHT0,GL_POSITION,light0_position);
glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,light0_direction);
glEnable(GL_LIGHTING);
glEnable(GL_LIGHT0);
}

void do_material()
{
float mat_ambient[] = {0.0,0.0,0.0,1.0}; 
float mat_diffuse[] = {0.9,0.9,0.1,1.0}; 
float mat_specular[] = {1.0,1.0,1.0,1.0};
float mat_shininess[] = {2.0}; 

glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
}

void go()
{
int view_pass;
glClear(GL_ACCUM_BUFFER_BIT);
glTranslatef(-2.0,0.0,2.0);
draw_stuff();
glFlush();
glAccum(GL_ACCUM,ADDEACH);
for(view_pass=0;view_pass<VPASSES;view_pass++){
	glAccum(GL_MULT,FADE);
	glTranslatef(EMOVE,0.0,-EMOVE);
	draw_stuff();
	glFlush();
	glAccum(GL_ACCUM,ADDEACH);
	}
draw_stuff();
glAccum(GL_ACCUM,20.0*ADDEACH);
glAccum(GL_RETURN,1.0);
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
srandom(123456789);
glutInit(&argc,argv);
glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_ACCUM);
glutInitWindowSize(768,768);
glutInitWindowPosition(100,50);
glutCreateWindow("my_cool_cube");
glClearColor(0.35,0.35,0.35,0.0);
glClearAccum(0.0,0.0,0.0,0.0);
glEnable(GL_DEPTH_TEST);
viewvolume();
do_lights();
do_material();
glutDisplayFunc(go);
glutKeyboardFunc(getout);
glutMainLoop();
return 0;
}

