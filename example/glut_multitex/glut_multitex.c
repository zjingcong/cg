//
// This is an example of multi-texturing without using a shader.
//

// gcc glut_multitex.c -o glut_multitex -lX11 -lGL -lGLU -lGLEW -lglut -lm -lXmu -lXi -DGL_GLEXT_PROTOTYPE

#include <stdio.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <string.h>

// The "alpha" function computes an alpha value for a pixel. 
// Its argument is a pointer to the 3 (RGB) char values for that pixel.

// This is the null alpha function.
#define NULL_A (unsigned char(*)(unsigned char *))0

// This is a non-null alpha function for the "Wow!" texture.
unsigned char wowalpha(unsigned char *aarg)
{
// For the "wow.ppm" texture, red == 219 indicates a pixel we want drawn. 
if(*aarg==219) return(255);
return(0);
}

struct texture_control {
	char *name;
	int texid;
	GLint format;
	unsigned char (*alpha)(unsigned char *);
	GLenum texunit;
	GLint combine;
	};

struct texture_control tc[] = {
	"ppm_files/Woodgrain.ppm",1,GL_RGB,NULL_A,GL_TEXTURE0,GL_MODULATE,
	"ppm_files/scuff.ppm",     2,GL_RGB,NULL_A,GL_TEXTURE1,GL_MODULATE,
	"ppm_files/wow.ppm",3,GL_RGBA,wowalpha,GL_TEXTURE2,GL_DECAL,
	0
	};

void load_textures()
{
FILE *fopen(), *fptr;
char buf[512];
int im_size, im_width, im_height, max_color;
unsigned char *texture_bytes; 
char *parse;
unsigned char *tb_alpha, *tb_src, *tb_dst, *tb_final, *aarg;
struct texture_control *tcp;
int i;

// Build all the textures first.
tcp = tc;
while(tcp->name){
	// load ppm file 
	fptr=fopen(tcp->name,"r");
	fgets(buf,512,fptr);
	do{
		fgets(buf,512,fptr);
		} while(buf[0]=='#');
	parse = strtok(buf," ");
	im_width = atoi(parse);
	parse = strtok(NULL,"\n");
	im_height = atoi(parse);
	fgets(buf,512,fptr);
	parse = strtok(buf," ");
	max_color = atoi(parse);
	im_size = im_width*im_height;
	texture_bytes = (unsigned char *)calloc(3,im_size);
	fread(texture_bytes,1,3*im_size,fptr);
	fclose(fptr);
	if(tcp->format==GL_RGBA){
		// Expand this texture to a new one that includes alpha values.
		tb_alpha = (unsigned char *)calloc(4,im_size);
		tb_src = texture_bytes;
		tb_dst = tb_alpha;
		for(i=0;i<im_size;i++){
			// Here use the per-pixel alpha function to load
			// new alpha components.
			aarg = tb_src;		// pointer to RGB
			*tb_dst++ = *tb_src++; 	// R 
			*tb_dst++ = *tb_src++; 	// G 
			*tb_dst++ = *tb_src++; 	// B 
			*tb_dst++ = (tcp->alpha)(aarg);
			}
		free(texture_bytes);
		tb_final = tb_alpha;
		}
	else tb_final = texture_bytes;
	glBindTexture(GL_TEXTURE_2D,tcp->texid);
	glTexImage2D(GL_TEXTURE_2D,0,tcp->format,im_width,im_height,0,
			tcp->format,GL_UNSIGNED_BYTE,tb_final);
	free(tb_final);
	tcp++;
	}

}

struct point {
	float x, y, z;
	};

void viewvolume()
{
struct point eye, view, up;

glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluPerspective(45.0,1.0,0.1,20.0);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();

eye.x = 2.0; eye.y = 2.0; eye.z = 2.0;
view.x = 0.0; view.y = 0.0; view.z = 0.0;
up.x = 0.0; up.y = 1.0; up.z = 0.0;

gluLookAt(eye.x,eye.y,eye.z,view.x,view.y,view.z,up.x,up.y,up.z);
}

void draw_stuff()
{
int i;
struct texture_control *tcp;
struct point front[4]={{0.0,0.0,1.0},{1.0,0.0,1.0},{1.0,1.0,1.0},{0.0,1.0,1.0}};
struct point back[4]={{0.0,0.0,0.0},{0.0,1.0,0.0},{1.0,1.0,0.0},{1.0,0.0,0.0}};
struct point left[4]={{0.0,0.0,0.0},{0.0,0.0,1.0},{0.0,1.0,1.0},{0.0,1.0,0.0}};
struct point right[4]={{1.0,0.0,0.0},{1.0,1.0,0.0},{1.0,1.0,1.0},{1.0,0.0,1.0}};
struct point top[4]={{0.0,1.0,0.0},{0.0,1.0,1.0},{1.0,1.0,1.0},{1.0,1.0,0.0}};
struct point bottom[4]={{0.0,0.0,0.0},{0.0,0.0,1.0},{1.0,0.0,1.0},{1.0,0.0,0.0}};
float mytexcoords[4][2] = {{0.0,1.0},{1.0,1.0},{1.0,0.0},{0.0,0.0}};

glEnable(GL_DEPTH_TEST);
glEnable(GL_MULTISAMPLE_ARB);
glClearColor(0.35,0.35,0.35,0.0);
glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

// Now activate all the texture units.
tcp = tc;
while(tcp->name){
	glActiveTexture(tcp->texunit);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,tcp->texid);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,tcp->combine);
	tcp++;
	}

// Attach all textures to the front face. 
glBegin(GL_QUADS); 
glNormal3f(0.0,0.0,1.0);
for(i=0;i<4;i++){
	// Here use the same texcoords in each unit, but we can vary these. 
	// Either way, they must be set for each unit.
	tcp = tc;
	while(tcp->name){
		glMultiTexCoord2fv(tcp->texunit,mytexcoords[i]);
		tcp++;
		}
	glVertex3f(front[i].x,front[i].y,front[i].z);
	}
glEnd();
// Now shut down all the texture units. 
tcp = tc;
while(tcp->name){
	glActiveTexture(tcp->texunit);
	glDisable(GL_TEXTURE_2D);
	tcp++;
	}
// Render the other faces as before. 
glBegin(GL_QUADS); 
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

void lights()
{
float light0_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
float light0_diffuse[] = { 1.0, 1.0, 1.0, 0.0 };
float light0_specular[] = { 1.0, 1.0, 1.0, 0.0 };
float light0_position[] = { 1.5, 2.0, 2.0, 1.0 };
float light0_direction[] = { -1.5, -2.0, -2.0, 1.0};

glLightModelfv(GL_LIGHT_MODEL_AMBIENT,light0_ambient);
glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,1);

glLightfv(GL_LIGHT0,GL_AMBIENT,light0_ambient);
glLightfv(GL_LIGHT0,GL_DIFFUSE,light0_diffuse);
glLightfv(GL_LIGHT0,GL_SPECULAR,light0_specular);
glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,1.0);
glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,180.0);
glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,0.5);
glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,0.1);
glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,0.01);
glLightfv(GL_LIGHT0,GL_POSITION,light0_position);
glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,light0_direction);

glEnable(GL_LIGHTING);
glEnable(GL_LIGHT0);
}

void material()
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
int mtu;
glutInit(&argc,argv);
glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_MULTISAMPLE);
glutInitWindowSize(768,768);
glutInitWindowPosition(100,50);
glutCreateWindow("my_cool_cube");
glGetIntegerv(GL_MAX_TEXTURE_UNITS,&mtu);
fprintf(stderr,"%d\n",mtu);
load_textures();
viewvolume();
lights();
material();
glutDisplayFunc(draw_stuff);
glutKeyboardFunc(getout);
glutMainLoop();
return 0;
}
