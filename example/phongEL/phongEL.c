//
// Environment lighting using two textures, a diffuse irradiance map and a
// specular irradiance map. Texture access is within the shader.
//

// gcc phongEL.c -o phongEL -lX11 -lGL -lGLU -lGLEW -lglut -lm -lXmu -lXi -DGL_GLEXT_PROTOTYPE

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glx.h>
#include <GL/glext.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

float eye[] = {0.0,0.0,0.0};
float viewpt[] = {0.0,0.0,-1.0};
float up[] = {0.0,1.0,0.0};

char *read_shader_program(char *filename) 
{
FILE *fp;
char *content = NULL;
int fd, count;
fd = open(filename,O_RDONLY);
count = lseek(fd,0,SEEK_END);
close(fd);
content = (char *)calloc(1,(count+1));
fp = fopen(filename,"r");
count = fread(content,sizeof(char),count,fp);
content[count] = '\0';
fclose(fp);
return content;
}

void set_material()
{
float mat_diffuse[] = {0.8,0.4,0.2,1.0};
float mat_specular[] = {1.0,1.0,1.0,1.0};
float mat_shininess[] = {20.0};

glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
}

void view_volume()
{
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluPerspective(45.0,1.0,1.0,50.0);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
gluLookAt(eye[0],eye[1],eye[2],viewpt[0],viewpt[1],viewpt[2],up[0],up[1],up[2]);
}

void set_uniform(int p)
{
int location;
// Set texture units.
location = glGetUniformLocation(p,"diffuse_irr_map");
glUniform1i(location,0);
location = glGetUniformLocation(p,"specular_irr_map");
glUniform1i(location,1);
}

GLuint sprogram;

void renderScene(void)
{
GLUquadric* qptr=gluNewQuadric();

glClearColor(0.8,0.8,0.9,1.0);
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

glUseProgram(sprogram);
set_uniform(sprogram);
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D,1);
glActiveTexture(GL_TEXTURE1);
glBindTexture(GL_TEXTURE_2D,2);

// Position the donut.
glPushMatrix();
glTranslatef(0.0,0.0,-5.0);
glRotatef(-20.0,1.0,0.0,0.0);
glRotatef(-20.0,0.0,1.0,0.0);
glutSolidTorus(0.4,0.8,128,128);
glPopMatrix();

// Return to fixed-function rendering:
glUseProgram(0);
gluQuadricTexture(qptr,1);
gluQuadricOrientation(qptr,GLU_INSIDE);

glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D,3);
glEnable(GL_TEXTURE_2D);
// Rotations to position the hemi-texture must be applied to the 
// irradiance map calculations, except for those, such as here,
// simply used to place it in standard A,E position.
glRotatef(-90.0,0.0,0.0,1.0);
glRotatef(60.0,0.0,1.0,0.0);
glRotatef(90.0,0.0,0.0,1.0);
gluSphere(qptr,40.0,64,64);

glutSwapBuffers();
}

unsigned int set_shaders()
{
GLint vertCompiled, fragCompiled;
char *vs, *fs;
GLuint v, f, p;

v = glCreateShader(GL_VERTEX_SHADER);
f = glCreateShader(GL_FRAGMENT_SHADER);
vs = read_shader_program("phongEL.vert");
fs = read_shader_program("phongEL.frag");
glShaderSource(v,1,(const char **)&vs,NULL);
glShaderSource(f,1,(const char **)&fs,NULL);
free(vs);
free(fs); 
glCompileShader(v);
glCompileShader(f);
p = glCreateProgram();
glAttachShader(p,f);
glAttachShader(p,v);
glLinkProgram(p);
return(p);
}

void load_texture(char *filename,unsigned int tid)
{
FILE *fopen(), *fptr;
char buf[512];
int im_size, im_width, im_height, max_color;
unsigned char *texture_bytes, *parse; 

fptr=fopen(filename,"r");
fgets(buf,512,fptr);
do{
	fgets(buf,512,fptr);
	} while(buf[0]=='#');
parse = strtok(buf," \t");
im_width = atoi(parse);

parse = strtok(NULL," \n");
im_height = atoi(parse);

fgets(buf,512,fptr);
parse = strtok(buf," \n");
max_color = atoi(parse);

im_size = im_width*im_height;
texture_bytes = (unsigned char *)calloc(3,im_size);
fread(texture_bytes,3,im_size,fptr);
fclose(fptr);

glBindTexture(GL_TEXTURE_2D,tid);
glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,im_width,im_height,0,GL_RGB, 
	GL_UNSIGNED_BYTE,texture_bytes);
glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
free(texture_bytes);
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
glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA |GLUT_MULTISAMPLE);
glutInitWindowPosition(100, 100);
glutInitWindowSize(768,768);
glutCreateWindow("Red donut with environment lighting");
load_texture(argv[1],1);
load_texture(argv[2],2);
load_texture(argv[3],3);
glEnable(GL_DEPTH_TEST);
glEnable(GL_MULTISAMPLE_ARB);
view_volume();
set_material();
sprogram=set_shaders();
glutDisplayFunc(renderScene);
glutKeyboardFunc(getout);
glutMainLoop();
return 0;
}

