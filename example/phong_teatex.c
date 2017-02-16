//
// Phong shading, with lighting done in eye coordinates, rather than
// world coordinates, and texture access within shader.
//
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glx.h>
#include <GL/glext.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

float eye[] = {3.0,3.0,3.0};
float viewpt[] = {0.0,0.0,0.0};
float up[] = {0.0,1.0,0.0};
float light0_position[] = {3.0,3.0,3.0,1.0};

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

void set_light()
{
glLightfv(GL_LIGHT0,GL_POSITION,light0_position); 
} 

void set_material()
{
float mat_diffuse[] = {0.3,0.4,0.5,1.0};
float mat_specular[] = {0.5,0.5,0.5,1.0};
float mat_shininess[] = {20.0};

glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
}

void view_volume()
{
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluPerspective(45.0,1.0,1.0,20.0);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
gluLookAt(eye[0],eye[1],eye[2],viewpt[0],viewpt[1],viewpt[2],up[0],up[1],up[2]);
}

void renderScene(void)
{
glClearColor(0.5,0.4,0.3,1.0);
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D,1);
glRotatef(-30.0,1.0,0.0,0.0);
glutSolidTeapot(1.0);
glutSwapBuffers();
}

unsigned int set_shaders()
{
GLint vertCompiled, fragCompiled;
char *vs, *fs;
GLuint v, f, p;

v = glCreateShader(GL_VERTEX_SHADER);
f = glCreateShader(GL_FRAGMENT_SHADER);
vs = read_shader_program("phong_teatex.vert");
fs = read_shader_program("phong_teatex.frag");
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
glUseProgram(p);
return(p);
}

void load_texture(char *filename)
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

glBindTexture(GL_TEXTURE_2D,1);
glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,im_width,im_height,0,GL_RGB, 
	GL_UNSIGNED_BYTE,texture_bytes);
glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
cfree(texture_bytes);
}

void set_uniform(int p)
{
int location;
location = glGetUniformLocation(p,"mytexture");
glUniform1i(location,0);
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
int p;
glutInit(&argc,argv);
glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA| GLUT_MULTISAMPLE);
glutInitWindowPosition(10, 10);
glutInitWindowSize(700, 700);
glutCreateWindow("phong-shaded teapot");
load_texture(argv[1]);
glEnable(GL_DEPTH_TEST);
glEnable(GL_MULTISAMPLE_ARB);
view_volume();
set_light();
set_material();
p=set_shaders();
set_uniform(p);
glutDisplayFunc(renderScene);
glutKeyboardFunc(getout);
glutMainLoop();
return 0;
}

// ..............................................
// phong_teatex.vert

varying vec3 ec_vnormal, ec_vposition;

void main()
{	
ec_vnormal = gl_NormalMatrix*gl_Normal;
ec_vposition = gl_ModelViewMatrix*gl_Vertex;
gl_Position = gl_ProjectionMatrix*gl_ModelViewMatrix*gl_Vertex;
gl_TexCoord[0] = gl_MultiTexCoord0;
}

// ...............................................
// phong_teatex.frag

// Phong lighting in eye coordinates with texture access.

// These are set by the .vert code, interpolated.
varying vec3 ec_vnormal, ec_vposition;

// This is set by the .c code.
uniform sampler2D mytexture;

void main()
{
vec3 P, N, L, V, H; 
vec4 tcolor;
vec4 diffuse_color = gl_FrontMaterial.diffuse; 
vec4 specular_color = gl_FrontMaterial.specular; 
float shininess = gl_FrontMaterial.shininess;

P = ec_vposition;
N = normalize(ec_vnormal);
L = normalize(gl_LightSource[0].position - P);
V = normalize(-P);				
H = normalize(L+V);
		
tcolor = texture2D(mytexture,gl_TexCoord[0].st);
diffuse_color = 0.1*diffuse_color + 0.9*tcolor;
diffuse_color *= max(dot(N,L),0.0);
specular_color *= pow(max(dot(H,N),0.0),shininess);
gl_FragColor = diffuse_color + specular_color;
}

