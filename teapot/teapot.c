
// g++ -I/usr/include -I/usr/X11R6/include -L/usr/lib -L/usr/X11R6/lib -O2 teapot.c -lX11 -lGL -lGLU -lglut -lm -lXmu -lXi -o teapot

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
# include "radiosity.h"

# define WIN_X 1280
# define WIN_Y 1024


int ray_num;
int reflect_num;


using namespace std;


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


void set_viewvolume()
{
	glm::vec3 eye(0.5, 0.5, 1.96);
	// glm::vec3 eye(0.5, 0.5, 0.05);
	// glm::vec3 view(0.0, 0.0, 0.5);
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


void set_scene_ambient()
{
	float scene_ambient[] = {0.0, 0.0, 0.0, 0.0};
	// set scene default ambient 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, scene_ambient);
}


void set_lights(unsigned int gl_light_id, glm::vec3 pos, glm::vec3 color, glm::vec3 dir, float exp, float cutoff)
{
	float light_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
	float light_diffuse[] = { color.x, color.y, color.z, 0.0 }; 
	float light_specular[] = { 1.0, 1.0, 1.0, 0.0 }; 
	float light_position[] = { pos.x, pos.y, pos.z, 1.0 };
	float light_direction[] = { dir.x, dir.y, dir.z, 1.0};

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,1); 
	glLightfv(gl_light_id, GL_AMBIENT, light_ambient); 
	glLightfv(gl_light_id, GL_DIFFUSE, light_diffuse); 
	glLightfv(gl_light_id, GL_SPECULAR, light_specular); 
	glLightf(gl_light_id, GL_SPOT_EXPONENT, exp); 
	glLightf(gl_light_id, GL_SPOT_CUTOFF, cutoff); 
	glLightfv(gl_light_id, GL_POSITION, light_position);
	glLightfv(gl_light_id, GL_SPOT_DIRECTION, light_direction);
	glEnable(GL_LIGHTING);
	glEnable(gl_light_id);
}


void set_material(int id)
{
	switch (id)
	{
		// box material
		case 0:
		{
			float mat_shininess[] = {4};
			glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
			break;
		}
	}
}


void display_scene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	// draw box
	set_shaders(0);
	set_material(0);
	glVertexPointer(3, GL_FLOAT, 3 * sizeof(GLfloat), box_vertices);
	glNormalPointer(GL_FLOAT, 3 * sizeof(GLfloat), box_normals);
	glColorPointer(3, GL_FLOAT, 3 * sizeof(GLfloat), box_colors);
	glDrawElements(GL_QUADS, 3 * box_face_num * 4, GL_UNSIGNED_INT, box_faces);
	// draw light
	set_shaders(1);
	glVertexPointer(3, GL_FLOAT, 3 * sizeof(GLfloat), light_vertices);
	glNormalPointer(GL_FLOAT, 3 * sizeof(GLfloat), light_normals);
	glColorPointer(3, GL_FLOAT, 3 * sizeof(GLfloat), light_colors);
	glDrawElements(GL_QUADS, 3 * 4, GL_UNSIGNED_INT, light_faces);
	glFlush();
}


// add shadow render here
void render_scene()
{
	display_scene();
	glFlush();
}


void do_render()
{
	set_viewvolume();
	set_scene_ambient();

	if (ray_num == 0)
	{
		render_scene();
		return;
	}

	glClear(GL_ACCUM_BUFFER_BIT);
	for (int i = 0; i < ray_num; ++i)
	{
		// select random ray and find intersection, if no intersection: try new random ray
		cout << "ray id: " << i << endl;
		Ray light_ray;
		Ray ray;
		bool find = false;
		do 
		{
			// pick a random light
			cout << "pick ray..." << endl;
			light_ray = pick_ray();
			find = find_intersection(light_ray, ray);
		} while (!find);
		cout << "main light ray: " << endl;
		cout << "\t | pos: " << light_ray.pos.x << " " << light_ray.pos.y << " " << light_ray.pos.z << endl;
		cout << "\t | dir: " << light_ray.dir.x << " " << light_ray.dir.y << " " << light_ray.dir.z << endl;
		cout << "reflect ray: " << endl;
		cout << "\t | pos: " << ray.pos.x << " " << ray.pos.y << " " << ray.pos.z << endl;
		cout << "\t | dir: " << ray.dir.x << " " << ray.dir.y << " " << ray.dir.z << endl;
		cout << "------------------------------------------" << endl;

		// set main light
		set_lights(GL_LIGHT0, light_ray.pos, light_ray.color, light_ray.dir, 0.1, 180.0);
		// set other lights
		set_lights(GL_LIGHT1, ray.pos, ray.color, ray.dir, 0.1, 180.0);

		// render the scene
		render_scene();
		glAccum(GL_ACCUM, 1.0f / ray_num);

		/*
		for (int j = 1; j <= reflect_num; ++j)
		{
				// set vlp
				// set_lights(ray.pos, ray.color, ray.dir);
				render_scene();
				// glAccum(GL_ACCUM, 1.0f / ray_num);
		}
		*/
	}
	glAccum(GL_RETURN, 1.0);
	glutSwapBuffers();
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
	generateBoxTri();	// get triangles for the box
}


int main(int argc, char **argv)
{
	// usage: ./teapot ray_num reflect_num
	ray_num = atoi(argv[1]);
	reflect_num = atoi(argv[2]);
	initGL(argc, argv);
	pre_load();
	glutDisplayFunc(do_render);
	glutMainLoop();

	return 0;
}


