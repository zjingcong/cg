
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


using namespace std;


struct Ray
{
	glm::vec3 pos;
	glm::vec3 dir;
};


struct Triangle
{
	glm::vec3 V0;
	glm::vec3 V1;
	glm::vec3 V2;
};


vector<Triangle> triangle_list;


void get_triangles()
{
	// cornell box triangles
	
}


void set_viewvolume()
{
	glm::vec3 eye(0.5, 0.5, 2.2);
	glm::vec3 view(0.5, 0.5, 0.0);
	glm::vec3 up(0.0, 1.0, 0.0);

	// specify size and shape of view volume 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0,1.0,0.1,20.0);
	// specify position for view volume 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye.x,eye.y,eye.z,view.x,view.y,view.z,up.x,up.y,up.z);
}


// set material for cornell box
void set_cornell_material(int mat_id)
{
	float mat_ambient[] = {0.0,0.0,0.0,1.0}; 
	float mat_specular[] = {1.0,1.0,1.0,1.0};
	float mat_shininess[] = {2.0};
	// set material diffuse color
	switch (mat_id)
	{
		// red
		case 0:
		{
			float mat_diffuse[] = {1.0, 0.0, 0.0, 1.0};
			glMaterialfv(GL_FRONT,GL_DIFFUSE, mat_diffuse);
			break;
		}
		// green
		case 1:
		{
			float mat_diffuse[] = {0.0, 1.0, 0.0, 1.0}; 
			glMaterialfv(GL_FRONT,GL_DIFFUSE, mat_diffuse);
			break;
		}
		// grey
		case 2:
		{
			float mat_diffuse[] = {0.5, 0.5, 0.5, 1.0}; 
			glMaterialfv(GL_FRONT,GL_DIFFUSE, mat_diffuse);
			break;
		}
		// white
		case 3:
		{
			float mat_diffuse[] = {1.0, 1.0, 1.0, 1.0}; 
			glMaterialfv(GL_FRONT,GL_DIFFUSE, mat_diffuse);
			break;
		}
		// dark
		case 4:
		{
			float mat_diffuse[] = {0.1, 0.1, 0.1, 1.0}; 
			glMaterialfv(GL_FRONT,GL_DIFFUSE, mat_diffuse);
			break;
		}
	}
	glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
}


void draw_cornell_box()
{
	int i;

	glm::vec3 back[4]   = {glm::vec3(0.0,0.0,0.0), glm::vec3(0.0,1.0,0.0), 
                         glm::vec3(1.0,1.0,0.0), glm::vec3(1.0,0.0,0.0)};
	glm::vec3 left[4]   = {glm::vec3(0.0,0.0,0.0), glm::vec3(0.0,0.0,1.0), 
                         glm::vec3(0.0,1.0,1.0), glm::vec3(0.0,1.0,0.0)};
	glm::vec3 right[4]  = {glm::vec3(1.0,0.0,0.0), glm::vec3(1.0,1.0,0.0), 
                         glm::vec3(1.0,1.0,1.0), glm::vec3(1.0,0.0,1.0)};
	glm::vec3 top[4]    = {glm::vec3(0.0,1.0,0.0), glm::vec3(0.0,1.0,1.0), 
                         glm::vec3(1.0,1.0,1.0), glm::vec3(1.0,1.0,0.0)};
	glm::vec3 bottom[4] = {glm::vec3(0.0,0.0,0.0), glm::vec3(0.0,0.0,1.0), 
                         glm::vec3(1.0,0.0,1.0), glm::vec3(1.0,0.0,0.0)};
	glm::vec3 light[4]	= {glm::vec3(0.3,0.99,0.3), glm::vec3(0.3,0.99,0.7), 
                         glm::vec3(0.7,0.99,0.7), glm::vec3(0.7,0.99,0.3)};

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glBegin(GL_QUADS);

	// top: dark
	set_cornell_material(4);
	glNormal3f(0.0,-1.0,0.0);
	for(i=0;i<4;i++) glVertex3f(top[i].x,top[i].y,top[i].z);
	// back, bottom: white
	set_cornell_material(2);
	glNormal3f(0.0,0.0,1.0);
	for(i=0;i<4;i++) glVertex3f(back[i].x,back[i].y,back[i].z);
	glNormal3f(0.0,1.0,0.0);
	for(i=0;i<4;i++) glVertex3f(bottom[i].x,bottom[i].y,bottom[i].z);
	// left: red
	set_cornell_material(0);
	glNormal3f(1.0,0.0,0.0);
	for(i=0;i<4;i++) glVertex3f(left[i].x,left[i].y,left[i].z);
	// right: green
	set_cornell_material(1);
	glNormal3f(-1.0,0.0,0.0);
	for(i=0;i<4;i++) glVertex3f(right[i].x,right[i].y,right[i].z);
	// light: white
	set_cornell_material(3);
	glNormal3f(0.0,-1.0,0.0);
	for(i=0;i<4;i++) glVertex3f(light[i].x,light[i].y,light[i].z);

	glEnd();
	glFlush();
}


void set_lights()
{
	float light0_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
	float light0_diffuse[] = { 1.0, 1.0, 1.0, 0.0 }; 
	float light0_specular[] = { 2.25, 2.25, 2.25, 0.0 }; 
	float light0_position[] = { 0.5, 0.95, 0.5, 1.0 };
	float light0_direction[] = { -1.5, -2.0, -2.0, 1.0};

	// set scene default ambient 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,light0_ambient); 

	// make specular correct for spots 
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


// pick a point at random on an area light
// ***** modify later *****
Ray pick_ray()
{
	// pick a point at random on the area light
	glm::vec2 rand_2d = glm::linearRand(glm::vec2(0.3), glm::vec2(0.7));
	glm::vec3 light_pos = glm::vec3(rand_2d.x, 0.99, rand_2d.y);
	// shoot a ray from the point in random direction 
	// (modify later)
	glm::vec3 light_dir = glm::sphericalRand(1.0f);

	Ray ray;
	ray.pos = light_pos;
	ray.dir = light_dir;

	return ray;
}


// ray/triangle intersection - moller-trumbore intersection algorithm
// triangle vertices: V0, V1, V2
// ray position: origin
// ray direction: dir
// intersection point: P
// P = origin + t * dir = (1 - u - v) * V0 + u * V1 + v * V2 (the same)
bool MTintersection(glm::vec3 V0, glm::vec3 V1, glm::vec3 V2, 
                      glm::vec3 origin, glm::vec3 dir, glm::vec3 &P)
{
	float kEpsilon = 0.000001f;

	glm::vec3 e1, e2;	// edge 1, edge 2
	glm::vec3 pvec, tvec, qvec;
	float det, inv_det;
	float t, u, v;

	e1 = V1 - V0;
	e2 = V2 - V0;
	pvec = glm::cross(dir, e2);
	det = glm::dot(e1, pvec);

	// back-facing triangles or ray misses the triangle
	if (det < kEpsilon)	{return false;}
	// ray and triangle are parallel
	if (fabs(det) < kEpsilon)	{return false;}

	inv_det = 1.0f / det;
	tvec = origin - V0;
	u = glm::dot(tvec, pvec) * inv_det;
	if (u < 0 || u > 1)	{return false;}
	qvec = glm::cross(tvec, e1);
	v = glm::dot(dir, qvec) * inv_det;
	if (v < 0 || u + v > 1)	{return false;}
	t = glm::dot(e2, qvec) * inv_det;

	P = origin + t * dir;

	return true;
}


void do_instant_radiosity()
{
	Ray ray = pick_ray();
	cout << "ray pos: " << ray.pos.x << " " << ray.pos.y << " " << ray.pos.z << endl;
	cout << "ray dir: " << ray.dir.x << " " << ray.dir.y << " " << ray.dir.z << endl;
	// find intersection, loop over all the triangles
	

	// test ray_intersection
	glm::vec3 V0(0, 0, 0);
	glm::vec3 V1(0, 1, 0);
	glm::vec3 V2(0, 0, 1);
	glm::vec3 P;
	glm::vec3 origin(0.3, 0.3, 0.3);
	glm::vec3 dir(-1, 0, 0);

	if (ray_intersection(V0, V1, V2, origin, dir, P))
	{
		
	}

}


void initGL(int argc, char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_ACCUM);
	glutInitWindowSize(1280, 1024);
	glutInitWindowPosition(100, 50);
	glutCreateWindow("teapot_cornell_box");
	glClearColor(0.35, 0.35, 0.35, 0.0);
	glClearAccum(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE_ARB);
}


int main(int argc, char **argv)
{
	initGL(argc, argv);
	set_viewvolume();
	set_lights();
	do_instant_radiosity();
	glutDisplayFunc(draw_cornell_box);
	glutMainLoop();

	return 0;
}

