
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

# include "shadow.h"
# include "scene.h"
# include "radiosity.h"

# define WIN_X 1280
# define WIN_Y 1024
# define JITTER 0.001


int ray_num;
int reflect_num;
float reflect_att = 0.7;	// attenuation for each reflection
GLuint box_shader, light_shader, teapot_shader;

glm::vec3 eye(0.0, 2.5, 7.3);
glm::vec3 dir(0.0, 0.0, -1.0);



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
        case 2:
        {
            vs_str = load_shader_file("teapot.vert");
            fs_str = load_shader_file("teapot.frag");
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

	return p;
}


void set_viewvolume(glm::vec3 eye, glm::vec3 dir, GLfloat fov)
{
    glm::vec3 view, up, vdir, utemp, vtemp;

    view = eye + dir;
    view.x += JITTER*(drand48()*2-1); view.y += JITTER*(drand48()*2-1); view.z += JITTER*(drand48()*2-1);
    up.x = 0.0; up.y = 1.0; up.z = 0.0;
    vdir.x = view.x - eye.x; vdir.y = view.y - eye.y; vdir.z = view.z - eye.z;
    // Calculate correct up vector as orthogonal to vdir and in the plane of
    // vdir and (0,1,0).
    vtemp = glm::cross(vdir,up);
    utemp = glm::cross(vtemp,vdir);
    up = glm::normalize(utemp);

	// specify size and shape of view volume
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, (float)(WIN_X) / (float)(WIN_Y), 0.1, 20.0);
	// specify position for view volume
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye.x,eye.y,eye.z,view.x,view.y,view.z,up.x,up.y,up.z);
}


void set_scene_ambient()
{
	// turn off the scene ambient
	float scene_ambient[] = {0.0, 0.0, 0.0, 0.0};
	// set scene default ambient
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, scene_ambient);

		// fill light
	GLuint gl_light_id = GL_LIGHT7;
    glm::vec3 pos(0.0, -20, 0.0);
    glm::vec3 dir(0.0, 1.0, 0.0);
    float exp = 0.1;
    float cutoff = 180.0;

	float light_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
	float light_diffuse[] = { 0.47, 0.47, 0.47, 0.0 };
	float light_specular[] = { 0.0, 0.0, 0.0, 0.0 };
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


void set_lights(unsigned int gl_light_id, glm::vec3 pos, glm::vec3 color, glm::vec3 dir, float exp, float cutoff)
{
	float light_ambient[] = { 0.005, 0.005, 0.005, 0.005 };
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
			float mat_shininess[] = {10};
			glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
			break;
		}
		// teapot meterial
		case 1:
		{
			float mat_ambient[] = {0.105882, 0.058824, 0.113725, 1.000000};
			float mat_diffuse[] = {0.427451, 0.470588, 0.541176, 1.000000};
			float mat_specular[] = {0.666666, 0.666666, 0.921569, 1.000000};
			float mat_shininess[] = {29.846150};

			glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,mat_ambient);
			glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_diffuse);
			glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_specular);
			glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,mat_shininess);
			break;
		}
	}
}


void draw_box()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	set_material(0);
	glVertexPointer(3, GL_FLOAT, 3 * sizeof(GLfloat), box_vertices);
	glNormalPointer(GL_FLOAT, 3 * sizeof(GLfloat), box_normals);
	glColorPointer(3, GL_FLOAT, 3 * sizeof(GLfloat), box_colors);
	glDrawElements(GL_QUADS, 5 * 4, GL_UNSIGNED_INT, box_faces);	// five face(no front face)
}


void draw_teapot(){
	// draw teapot
	set_material(1);
    glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 2 * sizeof(GLfloat), teapot_texcoords);
    glVertexPointer(3, GL_FLOAT, 3 * sizeof(GLfloat), teapot_vertices);
    glNormalPointer(GL_FLOAT, 3 * sizeof(GLfloat), teapot_normals);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,2);
    glPushMatrix();
    glRotatef(-120, 0.0, 1.0, 0.0);
    glDrawElements(GL_QUADS, 4 * teapot_face_count, GL_UNSIGNED_INT, teapot_faces);
    glPopMatrix();
    }


void draw_floor()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  set_material(0);
  glBegin(GL_QUADS);
  glNormal3f(0.0,2.0,0.0);
	glVertex3f(2.0,0.0,0.0);
	glVertex3f(2.0,0.0,2.0);
	glVertex3f(0.0,0.0,2.0);
	glVertex3f(0.0,0.0,0.0);
	glEnd();
}


void draw_light()
{
	// draw light
	glEnableClientState(GL_COLOR_ARRAY);
	glUseProgram(light_shader);
	glVertexPointer(3, GL_FLOAT, 3 * sizeof(GLfloat), light_vertices);
	glNormalPointer(GL_FLOAT, 3 * sizeof(GLfloat), light_normals);
	glColorPointer(3, GL_FLOAT, 3 * sizeof(GLfloat), light_colors);
	glPushMatrix();
	glTranslatef(JITTER*(drand48()*2-1)*50.0, 0.0 ,JITTER*(drand48()*2-1)*50.0);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, light_faces);
	glPopMatrix();
}


void save_matrix(glm::vec3 eye, glm::vec3 view)
{
    glMatrixMode(GL_TEXTURE);
    // This must match the unit used in the vertex shader.
    glActiveTexture(GL_TEXTURE7);
    glLoadIdentity();
    glTranslatef(0.0,0.0,-0.00005);
    glScalef(0.5,0.5,0.5);
    glTranslatef(1.0,1.0,1.0);
    gluPerspective(60.0,(float)(WINDOW_WIDTH)/(float)(WINDOW_HEIGHT),0.1,20.0);
    gluLookAt(eye.x,eye.y,eye.z,view.x,view.y,view.z,0.0,1.0,0.0);
}

// add shadow render here
void do_shadow_map(Ray ray)
{
  float distance = 7.0;
  glm::vec3 c_eye = ray.pos - distance * ray.dir;

  glBindFramebufferEXT(GL_FRAMEBUFFER,1);
  glUseProgram(0);

  set_viewvolume(c_eye,ray.dir,60.0);
	draw_floor();
	draw_teapot();
	glBindFramebufferEXT(GL_FRAMEBUFFER,0);
	save_matrix(c_eye,ray.pos+ray.dir);
  glUseProgram(box_shader);
  set_shadowuniform(box_shader);
  glActiveTexture(GL_TEXTURE6);
  glBindTexture(GL_TEXTURE_2D,1);
  set_viewvolume(eye,dir,45.0);
}

void render_scene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(box_shader);
  draw_box();
  glUseProgram(teapot_shader);
  set_uniform(teapot_shader);
	draw_teapot();
	draw_light();
}


void do_render()
{
	set_scene_ambient();

	glClear(GL_ACCUM_BUFFER_BIT);
	int r = 0;
	for (int i = 0; i < ray_num; ++i)
	{
		// select random ray and find intersection, if no intersection: try new random ray
		Ray light_ray;
		Ray ray;
		bool find = false;
		do
		{
			// pick a random light
			light_ray = pick_ray(r);
			find = find_intersection(light_ray, ray);
			r++;
		} while (!find);

		// set main light
		do_shadow_map(light_ray);
		set_lights(GL_LIGHT0, light_ray.pos, light_ray.color, light_ray.dir, light_ray.exp, light_ray.cutoff);
		// set vlp
		// note: large falloff if surface is shiny, make it spotlight
		Ray current_ray = ray;
		float att = 1.0;
		for (int i = 1; i <= reflect_num; ++i)
		{
			// set light for current_ray
			att *= reflect_att;

			if (!HIT_TEAPOT)	{set_lights(GL_LIGHT1, current_ray.pos, att * current_ray.color, current_ray.dir, current_ray.exp, current_ray.cutoff);}
			// render the scene
			render_scene();
			glAccum(GL_ACCUM, 1.0f / (ray_num));

			// set the next ray
			Ray new_ray;
			bool find_new_ray = find_intersection(current_ray, new_ray);
			if (HIT_TEAPOT)	{goto end_loop;}
			if (!find_new_ray)
			{
				goto end_loop;
			}
			current_ray = new_ray;
		}
		end_loop: ;
	}
	glAccum(GL_RETURN, 1.0);
	glutSwapBuffers();
}


void initGL(int argc, char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_ACCUM);
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
	load_obj("teapot.obj");	// get triangles for the box
	load_texture("texture.ppm");
	// generate triangles from quads for intersection finding
	generateBoxTri();	// generate box triangles
	generateTeapotTri();	// generate teapot triangles
}


int main(int argc, char **argv)
{
	// usage: ./teapot ray_num reflect_num
	ray_num = atoi(argv[1]);
	reflect_num = atoi(argv[2]);
	initGL(argc, argv);
	pre_load();
	build_shadowmap();
	box_shader = set_shaders(0);
	light_shader = set_shaders(1);
	teapot_shader = set_shaders(2);
	glutDisplayFunc(do_render);
	glutMainLoop();

	return 0;
}

