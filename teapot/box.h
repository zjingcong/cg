# ifndef _BOX_H_
# define _BOX_H_

# include <iostream>

# define LENGTH 1.0


GLfloat *box_vertices;
GLfloat *box_normals;
GLuint *box_faces;
GLfloat *box_colors;

GLfloat *light_vertices;
GLfloat *light_normals;
GLuint *light_faces;
GLfloat *light_colors;

int box_face_num = 5;


void load_box()
{
	GLfloat vertices[] = 
	{
		// left
		0.0, 0.0, 0.0, 
		0.0, 1.0, 0.0, 
		0.0, LENGTH, LENGTH, 
		0.0, 0.0, LENGTH, 
		// right
		LENGTH, LENGTH, LENGTH, 
		LENGTH, LENGTH, 0.0, 
		LENGTH, 0.0, 0.0, 
		LENGTH, 0.0, LENGTH, 
		// top
		0.0, LENGTH, 0.0, 
		LENGTH, LENGTH, 0.0, 
		LENGTH, LENGTH, LENGTH, 
		0.0, LENGTH, LENGTH, 
		// bottom
		0.0, 0.0, 0.0, 
		0.0, 0.0, LENGTH, 
		LENGTH, 0.0, LENGTH, 
		LENGTH, 0.0, 0.0, 
		// back
		LENGTH, LENGTH, 0.0, 
		0.0, LENGTH, 0.0, 
		0.0, 0.0, 0.0, 
		LENGTH, 0.0, 0.0, 
		/*
		// front: for test
		0.0, LENGTH, LENGTH, 
		LENGTH, LENGTH, LENGTH, 
		LENGTH, 0.0, LENGTH, 
		0.0, 0.0, LENGTH
		*/
	};

	GLfloat normals[] = 
	{
		// left
		1.0, 0.0, 0.0, 
		1.0, 0.0, 0.0, 
		1.0, 0.0, 0.0, 
		1.0, 0.0, 0.0, 
		// right
		-1.0, 0.0, 0.0, 
		-1.0, 0.0, 0.0, 
		-1.0, 0.0, 0.0, 
		-1.0, 0.0, 0.0, 
		// top
		0.0, -1.0, 0.0, 
		0.0, -1.0, 0.0, 
		0.0, -1.0, 0.0, 
		0.0, -1.0, 0.0, 
		// bottom
		0.0, 1.0, 0.0, 
		0.0, 1.0, 0.0, 
		0.0, 1.0, 0.0, 
		0.0, 1.0, 0.0, 
		// back
		0.0, 0.0, 1.0, 
		0.0, 0.0, 1.0, 
		0.0, 0.0, 1.0, 
		0.0, 0.0, 1.0, 
		/*
		// front
		0.0, 0.0, -1.0, 
		0.0, 0.0, -1.0, 
		0.0, 0.0, -1.0, 
		0.0, 0.0, -1.0, */
	};

	GLfloat colors[] = 
	{
		// left: red
		1.0, 0.0, 0.0, 
		1.0, 0.0, 0.0, 
		1.0, 0.0, 0.0, 
		1.0, 0.0, 0.0, 
		// right: green
		0.0, 1.0, 0.0, 
		0.0, 1.0, 0.0, 
		0.0, 1.0, 0.0, 
		0.0, 1.0, 0.0, 
		// others: white
		// top
		0.5, 0.5, 0.5, 
		0.5, 0.5, 0.5, 
		0.5, 0.5, 0.5,  
		0.5, 0.5, 0.5, 
		// bottom
		0.5, 0.5, 0.5, 
		0.5, 0.5, 0.5, 
		0.5, 0.5, 0.5, 
		0.5, 0.5, 0.5, 
		// back
		0.5, 0.5, 0.5, 
		0.5, 0.5, 0.5, 
		0.5, 0.5, 0.5, 
		0.5, 0.5, 0.5, 
		/*
		// front
		1.0, 1.0, 1.0, 
		1.0, 1.0, 1.0, 
		1.0, 1.0, 1.0, 
		1.0, 1.0, 1.0
		*/
	};

	box_vertices =  (GLfloat *)calloc(box_face_num * 4, 3 * sizeof(GLfloat));
	box_normals =  (GLfloat *)calloc(box_face_num * 4, 3 * sizeof(GLfloat));
	box_colors =  (GLfloat *)calloc(box_face_num * 4, 3 * sizeof(GLfloat));
	box_faces =  (GLuint *)calloc(box_face_num * 4, 3 * sizeof(GLuint));
	for (int j = 0; j < box_face_num * 4 * 3; ++j)
	{
		box_faces[j] = j;
		box_vertices[j] = vertices[j];
		box_normals[j] = normals[j];
		box_colors[j] = colors[j];
	}
}


void load_light()
{
	GLfloat y = LENGTH - 0.01f;
	GLfloat vertices[] = 
	{
		0.3f * LENGTH, y, 0.3f * LENGTH, 
		0.7f * LENGTH, y, 0.3f * LENGTH, 
		0.7f * LENGTH, y, 0.7f * LENGTH, 
		0.3f * LENGTH, y, 0.7f * LENGTH
	};

	GLfloat normals[] = 
	{
		0.0, -1.0, 0.0, 
		0.0, -1.0, 0.0, 
		0.0, -1.0, 0.0, 
		0.0, -1.0, 0.0
	};
	
	GLfloat colors[] = 
	{
		// white
		2.0, 2.0, 2.0, 
		2.0, 2.0, 2.0, 
		2.0, 2.0, 2.0, 
		2.0, 2.0, 2.0
	};

	light_vertices =  (GLfloat *)calloc(4, 3 * sizeof(GLfloat));
	light_normals =  (GLfloat *)calloc(4, 3 * sizeof(GLfloat));
	light_colors =  (GLfloat *)calloc(4, 3 * sizeof(GLfloat));
	light_faces =  (GLuint *)calloc(4, 3 * sizeof(GLuint));
	for (int j = 0; j < 12; ++j)
	{
		light_faces[j] = j;
		light_vertices[j] = vertices[j];
		light_normals[j] = normals[j];
		light_colors[j] = colors[j];
	}
}


# endif

