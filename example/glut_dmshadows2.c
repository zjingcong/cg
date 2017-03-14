// 
// Depth map shadows - These can be done with ordinary OpenGL, i.e., no shader
// access is needed.  Nevertheless, there are drawbacks, such as the special
// OpenGL calls required and solid black shadows, which can be avoided with 
// shader use.  This version shows high degree of meshing is unnecessary.
//

// gcc glut_dmshadows2.c -o glut_dmshadows2 -lX11 -lGL -lGLU -lGLEW -lglut -lm -lXmu -lXi -DGL_GLEXT_PROTOTYPE

#include <stdio.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glut.h>

#define XRES 768
#define YRES 768

struct point {
	double x;
	double y;
	double z;
	};

struct vertex {
	double v[4];
	};

struct vertex floor_geom[4] = {
{-1.0, 0.0, -1.0, 1.0},
{-1.0, 0.0,  1.0, 1.0},
{ 1.0, 0.0,  1.0, 1.0},
{ 1.0, 0.0, -1.0, 1.0},
};

struct vertex table_geom[4] = {
{ 0.0, 0.3, -0.5, 1.0},
{-0.5, 0.3,  0.0, 1.0},
{ 0.0, 0.3,  0.5, 1.0},
{ 0.5, 0.3,  0.0, 1.0},
};

struct quad {
	struct vertex *vptr[4];
	};

struct quad floor_quads;
struct quad table_quads;

void build_geometry()
{
int i;
for(i=0;i<4;i++){
	floor_quads.vptr[i] = &floor_geom[i];
	table_quads.vptr[i] = &table_geom[i];
	}
}

setvvolume()
{
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluPerspective(45.0,((float)(XRES))/((float)(YRES)),0.1,20.0);
}

void setview(struct point eyepoint, struct point viewpoint)
{
glEnable(GL_DEPTH_TEST);
glClearColor(0.8,0.6,0.62,1.0);
glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

glMatrixMode(GL_MODELVIEW); 
glLoadIdentity();
gluLookAt(eyepoint.x,eyepoint.y,eyepoint.z,viewpoint.x,viewpoint.y,viewpoint.z,
	0.0,1.0,0.0);
}

GLfloat light0_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat light0_diffuse[] = { 1.0, 1.0, 1.0, 0.0 };
GLfloat light0_specular[] = { 0.9, 0.9, 0.9, 0.0 };
GLfloat light0_position[] = { -0.5, 2.0, -0.5, 1.0 };
GLfloat light0_direction[] = { 0.5, -2.0, 0.5, 1.0};

void lights()
{
glLightModelfv(GL_LIGHT_MODEL_AMBIENT,light0_ambient);

glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,1);
glLightfv(GL_LIGHT0,GL_AMBIENT,light0_ambient);
glLightfv(GL_LIGHT0,GL_DIFFUSE,light0_diffuse);
glLightfv(GL_LIGHT0,GL_SPECULAR,light0_specular);
glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,1.0);
glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,180.0);
glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,light0_direction);
glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,1.0);
glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,0.1);
glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,0.01);
glLightfv(GL_LIGHT0,GL_POSITION,light0_position);

glEnable(GL_LIGHT0);
glEnable(GL_LIGHTING);
}

GLfloat mat0_ambient[] = {0.0,0.0,0.0,1.0};
GLfloat mat0_diffuse[] = {0.9,0.6,0.2,1.0};
GLfloat mat0_specular[] = {1.0,1.0,1.0,1.0};
GLfloat mat0_shininess[] = {2.0};

GLfloat mat1_ambient[] = {0.0,0.0,0.0,1.0};
GLfloat mat1_diffuse[] = {0.0,0.4,0.15,1.0};
GLfloat mat1_specular[] = {1.0,1.0,1.0,1.0};
GLfloat mat1_shininess[] = {2.0};

void mat_floor_vis()
{
glMaterialfv(GL_FRONT,GL_AMBIENT,mat0_ambient);
glMaterialfv(GL_FRONT,GL_DIFFUSE,mat0_diffuse);
glMaterialfv(GL_FRONT,GL_SPECULAR,mat0_specular);
glMaterialfv(GL_FRONT,GL_SHININESS,mat0_shininess);
}

void mat_table_vis()
{
glMaterialfv(GL_FRONT,GL_AMBIENT,mat1_ambient);
glMaterialfv(GL_FRONT,GL_DIFFUSE,mat1_diffuse);
glMaterialfv(GL_FRONT,GL_SPECULAR,mat1_specular);
glMaterialfv(GL_FRONT,GL_SHININESS,mat1_shininess);
}

void drawquads()
{
int i,j,k;
int scount;
int vis_index, blk_index;
int high, low;

glNormal3f(0.0,1.0,0.0);

mat_floor_vis();
glBegin(GL_QUADS); 
for(k=0;k<4;k++) glVertex3dv((floor_quads.vptr[k])->v);
glEnd();

mat_table_vis();
glBegin(GL_QUADS); 
for(k=0;k<4;k++) glVertex3dv((table_quads.vptr[k])->v);
glEnd();
}

void transpose(float x[16])
{
int i;
float y[16];
for(i=0;i<16;i++) y[i]=x[i];
for(i=0;i<16;i++) x[i] = y[4*(i%4)+i/4];
}

void do_stuff()
{
struct point eyepoint, viewpoint;
float tcg_matrix[16];

// Render first from the light source. 
eyepoint.x = light0_position[0]; 
eyepoint.y = light0_position[1]; 
eyepoint.z = light0_position[2]; 

viewpoint.x = light0_direction[0]+light0_position[0];
viewpoint.y = light0_direction[1]+light0_position[1];
viewpoint.z = light0_direction[2]+light0_position[2];

setvvolume();
setview(eyepoint,viewpoint);  
lights(); 
drawquads();
glFlush();

// To see from the light source:
sleep(2);

// Create a depth map texture. 
glCopyTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,0,0,XRES,YRES,0);
// Use PROJECTION matrix to build texture coordinate generator matrix. 
glMatrixMode(GL_PROJECTION);
glPushMatrix();
glLoadIdentity();
// This epsilon, to pull texcoords back toward viewer, is scene-dependent.
glTranslatef(0.0,0.0,-0.005);
glScalef(0.5,0.5,0.5);
glTranslatef(1.0,1.0,1.0);
gluPerspective(45.0,(float)(XRES)/(float)(YRES),0.1,20.0);
gluLookAt(eyepoint.x,eyepoint.y,eyepoint.z,viewpoint.x,viewpoint.y,viewpoint.z,
	0.0,1.0,0.0);
glGetFloatv(GL_PROJECTION_MATRIX,tcg_matrix);
transpose(tcg_matrix);
glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
glTexGeni(GL_R,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
glTexGeni(GL_Q,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
glTexGenfv(GL_S,GL_OBJECT_PLANE,&tcg_matrix[0]);
glTexGenfv(GL_T,GL_OBJECT_PLANE,&tcg_matrix[4]);
glTexGenfv(GL_R,GL_OBJECT_PLANE,&tcg_matrix[8]);
glTexGenfv(GL_Q,GL_OBJECT_PLANE,&tcg_matrix[12]);
glEnable(GL_TEXTURE_GEN_S);
glEnable(GL_TEXTURE_GEN_T);
glEnable(GL_TEXTURE_GEN_R);
glEnable(GL_TEXTURE_GEN_Q);
glPopMatrix();

// Draw scene, complete with shadows. 
eyepoint.x = 1.0; eyepoint.y = 2.0; eyepoint.z = 2.0;
viewpoint.x = 0.0; viewpoint.y = 0.0; viewpoint.z = 0.0;

// We need these in case view from light didn't see everything. 
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

// Magic parameter strings to invoke a compare of depth with current texture: 
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_FUNC,GL_LEQUAL);
glTexParameteri(GL_TEXTURE_2D,GL_DEPTH_STENCIL_TEXTURE_MODE,GL_DEPTH_COMPONENT);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_MODE,GL_COMPARE_R_TO_TEXTURE);

glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
// The texture lookup value is 1 if R<=capture depth and 0 otherwise.  The
// GL_MODULATE then creates the shadow.  Try GL_REPLACE!
glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
glEnable(GL_MULTISAMPLE_ARB);
glEnable(GL_TEXTURE_2D);

setview(eyepoint,viewpoint);
lights();
drawquads();
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
glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_MULTISAMPLE);
glutInitWindowSize(XRES,YRES);
glutInitWindowPosition(300,50);
glutCreateWindow("view from light source, then from eye");
build_geometry();
glutDisplayFunc(do_stuff);
glutKeyboardFunc(getout);
glutMainLoop();
return 0;
}
