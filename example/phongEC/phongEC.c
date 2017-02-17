//
// Phong shading, with lighting done in eye coordinates, rather than
// world coordinates.  NOTE: To get the gl_LightSource[i].position in
// eye coordinates, you have to set the ModelView matrix to the final view
// BEFORE you call glLightfv().  
//

// to compile: g++ -I/usr/include -I/usr/X11R6/include -L/usr/lib -L/usr/X11R6/lib -O2 phongEC.c -lX11 -lGL -lGLU -lGLEW -lglut -lm -lXmu -lXi -DGL_GLEXT_PROTOTYPES -o phongEC

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glx.h>
#include <GL/glext.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>
# include <iostream>
# include <fstream>

using namespace std;

float eye[] = {3.0,3.0,3.0};
float viewpt[] = {0.0,0.0,0.0};
float up[] = {0.0,1.0,0.0};
float light0_position[] = {3.0,3.0,3.0,1.0};

string read_shader_program(const char *filePath)
{
		string content;
    ifstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open()) {
        cerr << "Could not read file " << filePath << ". File does not exist." << endl;
        exit(0);
    }
    string line = "";
    while(!fileStream.eof()) {
        getline(fileStream, line);
        content.append(line + "\n");
    }
		
		return content;
}

void set_light()
{
glLightfv(GL_LIGHT0,GL_POSITION,light0_position); 
} 

void set_material()
{
float mat_diffuse[] = {0.4,0.9,0.0,1.0};
float mat_specular[] = {0.9,0.1,0.0,1.0};
float mat_shininess[] = {10.0};

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
glutSolidTorus(0.4,0.8,128,128);
glutSwapBuffers();
}

unsigned int set_shaders()
{
GLint vertCompiled, fragCompiled;
string vs_str, fs_str;
GLuint v, f, p;
int result = -1;

v = glCreateShader(GL_VERTEX_SHADER);
f = glCreateShader(GL_FRAGMENT_SHADER);
vs_str = read_shader_program("phongEC.vert");
fs_str = read_shader_program("phongEC.frag");
const char *vs = vs_str.c_str();
const char *fs = fs_str.c_str();
glShaderSource(v,1,&vs,NULL);
glShaderSource(f,1,&fs,NULL);

glCompileShader(v);
glCompileShader(f);
glGetShaderiv(f,GL_COMPILE_STATUS,&result);
fprintf(stderr,"%d\n",result);
p = glCreateProgram();
glAttachShader(p,f);
glAttachShader(p,v);
glLinkProgram(p);
glUseProgram(p);
return(p);
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
glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
glutInitWindowPosition(100, 100);
glutInitWindowSize(768,768);
glutCreateWindow("phong-shaded donut");
glEnable(GL_DEPTH_TEST);
glEnable(GL_MULTISAMPLE_ARB);
view_volume();
set_light();
set_material();
set_shaders();
glutDisplayFunc(renderScene);
glutKeyboardFunc(getout);
glutMainLoop();
return 0;
}

