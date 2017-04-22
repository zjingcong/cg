
// gcc -I/usr/include -I/usr/X11R6/include -L/usr/lib -L/usr/X11R6/lib -O2 teapot.c -lX11 -lGL -lGLU -lglut -lm -lXmu -lXi -o teapot

# include <iostream>
# include <stdio.h>
# include <cmath>
# include <GL/gl.h>
# include <GL/glu.h>
# include <GL/glut.h>
# include <GL/glx.h>
# include <GL/glext.h>
# include <glm/glm.hpp>	// glm core
# include <glm/vec3.hpp> // glm::vec3
# include <glm/gtx/random.hpp>	// glm random
# include <vector>
# include <string>
# include <fstream>

# include "box.h"

# define WIN_X 1280
# define WIN_Y 1024


int ray_num;
int reflect_num;


using namespace std;


void set_viewvolume()
{
	glm::vec3 eye(0.5, 0.5, 1.96);
	glm::vec3 view(0.5, 0.5, 0.0);
	glm::vec3 up(0.0, 1.0, 0.0);

	// specify size and shape of view volume 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, float(WIN_X) / WIN_Y, 0.1, 20.0);
	// specify position for view volume 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye.x,eye.y,eye.z,view.x,view.y,view.z,up.x,up.y,up.z);
}


string load_shader_file(const char *filePath)
{
	string content;
	ifstream fileStream(filePath, std::ios::in);

	if(!fileStream.is_open())
	{
		cerr << "Could not read file " << filePath << ". File does not exist." << endl;
		exit(0);
	}
	string line = "";
	while(!fileStream.eof())
	{
		getline(fileStream, line);
		content.append(line + "\n");
	}

	return content;
}


unsigned int set_shaders(int id)
{
	string vs_str, fs_str;
	GLuint v, f, p;
	int result = -1;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	switch (id)
	{
		case 0:
		{
			vs_str = load_shader_file("box.vert");
      fs_str = load_shader_file("box.frag");
			break;
		}
		case 1:
		{
			vs_str = load_shader_file("light.vert");
      fs_str = load_shader_file("light.frag");
			break;
		}
	}

	const char *vs = vs_str.c_str();
	const char *fs = fs_str.c_str();
	glShaderSource(v, 1, &vs, NULL);
	glShaderSource(f, 1, &fs, NULL);

	glCompileShader(v);
	glCompileShader(f);
	glGetShaderiv(f,GL_COMPILE_STATUS, &result);

	p = glCreateProgram();
	glAttachShader(p,f);
	glAttachShader(p,v);
	glLinkProgram(p);
	glUseProgram(p);

	return p;
}


void set_lights()
{
	float light0_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
	float light0_diffuse[] = { 1.0, 1.0, 1.0, 0.0 }; 
	float light0_specular[] = { 1.0, 1.0, 1.0, 0.0 }; 
	float light0_position[] = { 0.5, LENGTH - 0.02f, 0.5, 1.0 };
	float light0_direction[] = { 0.0, -1.0, 0.0, 1.0};

	// set scene default ambient 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,light0_ambient);

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,1); 
	glLightfv(GL_LIGHT0,GL_AMBIENT,light0_ambient); 
	glLightfv(GL_LIGHT0,GL_DIFFUSE,light0_diffuse); 
	glLightfv(GL_LIGHT0,GL_SPECULAR,light0_specular); 
	glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,0.1); 
	glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,180.0); 
	glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,1.0); 
	glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,0.2); 
	glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,0.01); 
	glLightfv(GL_LIGHT0,GL_POSITION,light0_position);
	glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,light0_direction);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}


void set_material()
{
	float mat_shininess[] = {2};
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}


void display_scene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	// draw box
	set_shaders(0);
	glVertexPointer(3, GL_FLOAT, 3 * sizeof(GLfloat), box_vertices);
	glNormalPointer(GL_FLOAT, 3 * sizeof(GLfloat), box_normals);
	glColorPointer(3, GL_FLOAT, 3 * sizeof(GLfloat), box_colors);
	glDrawElements(GL_QUADS, 3 * 20, GL_UNSIGNED_INT, box_faces);
	// draw light
	set_shaders(1);
	glVertexPointer(3, GL_FLOAT, 3 * sizeof(GLfloat), light_vertices);
	glNormalPointer(GL_FLOAT, 3 * sizeof(GLfloat), light_normals);
	glColorPointer(3, GL_FLOAT, 3 * sizeof(GLfloat), light_colors);
	glDrawElements(GL_QUADS, 3 * 4, GL_UNSIGNED_INT, light_faces);
	glFlush();
}


void render_scene()
{
	set_viewvolume();
	set_lights();
	set_material();
	display_scene();
	glFlush();
}


void initGL(int argc, char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_ACCUM);
	glutInitWindowSize(WIN_X, WIN_Y);
	glutInitWindowPosition(100, 50);
	glutCreateWindow("teapot_cornell_box");
	glClearColor(0.35, 0.35, 0.35, 0.0);
	glClearAccum(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE_ARB);
}


void pre_load()
{
	// load models
	load_box();	// load box
	load_light();	// load light in cornell box
}


int main(int argc, char **argv)
{
	// usage: ./teapot ray_num reflect_num
	ray_num = atoi(argv[1]);
	reflect_num = atoi(argv[2]);
	initGL(argc, argv);
	pre_load();
	glutDisplayFunc(render_scene);
	glutMainLoop();

	return 0;
}


