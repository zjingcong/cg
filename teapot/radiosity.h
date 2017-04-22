# ifndef _RADIOSITY_H_
# define _RADIOSITY_H_

# include <glm/glm.hpp>	// glm core
# include <glm/vec3.hpp> // glm::vec3
# include <glm/gtx/random.hpp>	// glm random
# include <vector>
# include <iostream>

# include "box.h"


using namespace std;

typedef struct
{
	glm::vec3 pos;
	glm::vec3 dir;	// unit length vector
}	Ray;


typedef struct
{
	glm::vec3 V0;
	glm::vec3 V1;
	glm::vec3 V2;
	glm::vec3 N;	// normal, unit length vector
	glm::vec3 C;	// color
}	Triangle;


vector<Triangle> triangle_list;


void generateBoxTri()
{
	glm::vec3 point_list[box_face_num * 4], normal_list[box_face_num * 4], color_list[box_face_num * 4];
	// get all the point
	for (int i = 0; i < box_face_num * 4; ++i)
	{
		glm::vec3 point, normal, color;
		point.x = box_vertices[i * 3];
		point.y = box_vertices[i * 3 + 1];
		point.z = box_vertices[i * 3 + 2];
		normal.x = box_normals[i * 3];
		normal.y = box_normals[i * 3 + 1];
		normal.z = box_normals[i * 3 + 2];
		color.x = box_colors[i * 3];
		color.y = box_colors[i * 3 + 1];
		color.z = box_colors[i * 3 + 2];
		point_list[i] = point;
		normal_list[i] = glm::normalize(normal);
		color_list[i] = color;
	}
	for (int i = 0; i < box_face_num; ++i)
	{
		Triangle tri1, tri2;

		tri1.V0 = point_list[i * 4];
		tri1.V1 = point_list[i * 4 + 1];
		tri1.V2 = point_list[i * 4 + 2];
		tri1.N = normal_list[i * 4];
		tri1.C = color_list[i * 4];

		tri2.V0 = point_list[i * 4 + 1];
		tri2.V1 = point_list[i * 4 + 2];
		tri2.V2 = point_list[i * 4 + 3];
		tri2.N = normal_list[i * 4];
		tri2.C = color_list[i * 4];

		triangle_list.push_back(tri1);
		triangle_list.push_back(tri2);
	}
}


// pick a point at random on an area light
// ***** modify later *****
Ray pick_ray()
{
	// pick a point at random on the area light
	glm::vec2 rand_2d = glm::linearRand(glm::vec2(0.3f * LENGTH), glm::vec2(0.7f * LENGTH));
	glm::vec3 light_pos = glm::vec3(rand_2d.x, LENGTH - 0.02f, rand_2d.y);
	// shoot a ray from the point in random direction 
	// (modify later)
	glm::vec3 light_dir = glm::sphericalRand(1.0f);
	if (light_dir.y > 0)	{light_dir.y = -light_dir.y;}
	light_dir = glm::normalize(light_dir);

	Ray ray;
	ray.pos = light_pos;
	ray.dir = light_dir;

	return ray;
}


// ray/triangle intersection - moller-trumbore intersection algorithm
// triangle vertices: V0, V1, V2
// ray position: origin
// ray direction: dir
// intersection point: P P = origin + t * dir = (1 - u - v) * V0 + u * V1 + v * V2 (the same)
// triangle unit length normal: N
bool MTintersection(Triangle tri, 
                    glm::vec3 origin, glm::vec3 dir, glm::vec3 &P, glm::vec3 &N)
{
	float kEpsilon = 0.000001f;

	glm::vec3 e1, e2;	// edge 1, edge 2
	glm::vec3 pvec, tvec, qvec;
	float det, inv_det;
	float t, u, v;

	e1 = tri.V1 - tri.V0;
	e2 = tri.V2 - tri.V0;
	glm::vec3 tmpN = glm::normalize(glm::cross(e1, e2));
	N = tri.N;
	if (tmpN != N)
	{
		e1 = -e1;
		e2 = -e2;
		// cout << "inverse" << endl;
	}

	pvec = glm::cross(dir, e2);
	det = glm::dot(e1, pvec);

	// back-facing triangles or ray misses the triangle
	if (det < kEpsilon)	{/*cout << "backface" << endl;*/ return false;}
	// ray and triangle are parallel
	if (fabs(det) < kEpsilon)	{/*cout << "parallel" << endl;*/ return false;}

	inv_det = 1.0f / det;
	tvec = origin - tri.V0;
	u = glm::dot(tvec, pvec) * inv_det;
	if (u < 0 || u > 1)	{/*cout << "u fail" << endl;*/ return false;}
	qvec = glm::cross(tvec, e1);
	v = glm::dot(dir, qvec) * inv_det;
	if (v < 0 || u + v > 1)	{/*cout << "v fail" << endl;*/ return false;}
	t = glm::dot(e2, qvec) * inv_det;

	P = origin + t * dir;

	return true;
}


bool set_vlp(glm::vec3 origin, glm::vec3 dir, int reflect_id)
{
	// find intersection, loop over all the triangles
	glm::vec3 intersection;
	glm::vec3 N;
	glm::vec3 color;
	bool find_intersection = false;
	for (vector<Triangle>::iterator it = triangle_list.begin(); it != triangle_list.end(); ++it)
	{
		Triangle tri = *it;
		if (MTintersection(tri, origin, dir, intersection, N))
		{
			find_intersection = true;
			color = tri.C;
			goto end_loop;
		}
	}
	end_loop: ;
	if (!find_intersection)	{ cout << "No intersection" << endl;	return false;}
	cout << "intersection: " << intersection.x << " " << intersection.y << " " << intersection.z << endl;

	/*
	// get the reflect direction
	glm::vec3 V = -dir;
	glm::vec3 R = -V + 2 * glm::dot(V, N) * N;
	// attach new OpenGL light at ray/surface intersection
	// set light colors(diffuse/specular) to surface color
	float light1_position[] = { intersection.x, intersection.y, intersection.z, 1.0 };
	float light1_direction[] = { R.x, R.y, R.z, 1.0};
	float light1_diffuse[] = {color.x, color.y, color.z, 0.0};
	float light1_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
	float light1_specular[] = { 1.0, 1.0, 1.0, 0.0 }; 
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,1);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,light1_diffuse); 
	glLightfv(GL_LIGHT1,GL_AMBIENT,light1_ambient); 
	glLightfv(GL_LIGHT1,GL_SPECULAR,light1_specular); 
	glLightf(GL_LIGHT1,GL_SPOT_EXPONENT,0.1);		// large falloff if surface is shiny
	glLightf(GL_LIGHT1,GL_SPOT_CUTOFF,180.0);	// spotlight here 
	glLightf(GL_LIGHT1,GL_CONSTANT_ATTENUATION,1.0); 
	glLightf(GL_LIGHT1,GL_LINEAR_ATTENUATION,0.2); 
	glLightf(GL_LIGHT1,GL_QUADRATIC_ATTENUATION,0.01); 
	glLightfv(GL_LIGHT1,GL_POSITION,light1_position);
	glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,light1_direction);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);
	*/

	return true;
}

# endif

