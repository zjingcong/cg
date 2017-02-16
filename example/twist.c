// 
// GLSL example 2, twist.c, a twisting box
//
#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glx.h>
#include <GL/glext.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

char *read_shader_program(char *filename) 
{
char *content = NULL;
int fd, count;
fd = open(filename,O_RDONLY);
count = lseek(fd,0,SEEK_END);
content = (char *)calloc(1,(count+1));
lseek(fd,0,SEEK_SET); 
count = read(fd,content,count*sizeof(char));
content[count] = '\0';
close(fd);
return content;
}

#define BOX 1

void graphics_init()
{
int i,j;
// Clear color is dark red. 
glClearColor(0.7,0.0,0.0,0.0);

// Build a compiled list to draw a highly-meshed, 2D box. 
glNewList(BOX,GL_COMPILE);
for(i=0;i<100;i++){
	glBegin(GL_QUAD_STRIP);
	for(j=0;j<110;j++){
		glColor3f(0.0,0.01*j,0.01*j);
		glVertex2f(-0.5+j*0.01,-0.5+i*0.01);
		glVertex2f(-0.5+j*0.01,-0.5+(i+1)*0.01);
		}
	glEnd();
	}
glEndList();
}

int p;

void twister()
{
float twist_value;
int location;

// Find the address (slot) of the shader uniform variable named "twist".
location = glGetUniformLocation(p,"twist");

for(twist_value=0.0; twist_value<=3.14;twist_value += 0.01){
	// Set the new twist value in the shader.
	glUniform1f(location,twist_value);
	glClear(GL_COLOR_BUFFER_BIT);
	glCallList(BOX);
	glutSwapBuffers();
	}

for(twist_value=3.14; twist_value>=0.0;twist_value -= 0.01){
	glUniform1f(location,twist_value);
	glClear(GL_COLOR_BUFFER_BIT);
	glCallList(BOX);
	glutSwapBuffers();
	}
}


void set_shaders()
{
GLint vertCompiled, fragCompiled;
char *vs, *fs;
GLuint v, f; 

v = glCreateShader(GL_VERTEX_SHADER);
f = glCreateShader(GL_FRAGMENT_SHADER);
vs = read_shader_program("twist.vert");
fs = read_shader_program("twist.frag");
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
return;
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
glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);
glutInitWindowSize(768,768);
glutInitWindowPosition(100,50);
glutCreateWindow("hurricane");
set_shaders();
graphics_init();
glutDisplayFunc(twister);
glutKeyboardFunc(getout);
glutIdleFunc(glutPostRedisplay);
glutMainLoop();
return 0;
}

/*
// ............................................................
// twist.vert

uniform float twist;

void main()
{
float angle, c, s;
mat2 rotation;
vec2 new_position;

angle = twist*length(gl_Vertex.xy);
s = sin(angle);
c = cos(angle);
rotation = mat2(c,-s,s,c);
new_position = rotation*gl_Vertex.xy;
gl_Position = vec4(new_position,0.0,1.0);
gl_FrontColor = gl_Color;
}

// ............................................................
// twist.frag

void main()
{
gl_FragColor = gl_Color;
}
*/

