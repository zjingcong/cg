# ifndef _BOX_H_
# define _BOX_H_

# include <iostream>
# include <stdio.h>
# include <cmath>
# include <GL/gl.h>
# include <GL/glu.h>
# include <GL/glut.h>
# include <GL/glx.h>
# include <GL/glext.h>
# include <vector>
# include <cstring>

# define LENGTH 2.0

using namespace std;

GLfloat *box_vertices;
GLfloat *box_normals;
GLuint *box_faces;
GLfloat *box_colors;

GLfloat *light_vertices;
GLfloat *light_normals;
GLuint *light_faces;
GLfloat *light_colors;

GLfloat *teapot_vertices;
GLfloat *teapot_normals;
GLuint *teapot_faces;
int teapot_vertex_count=0;
int teapot_face_count=0;

int box_face_num = 5;

void load_obj(const char *file_name){
    FILE *fptr;
    char buf[512];
    GLfloat *temp_normals;
    GLuint *normalindices;
    fptr = fopen(file_name, "r");

    if( fptr == NULL ){
        printf("Impossible to open the file !n");
    }

    while(1){
        // read the first word of the line
        int res = fscanf(fptr, "%s", buf);
        if(res == EOF)
            break; // EOF = End Of File. Quit the loop.
        if(strcmp( buf, "v" ) == 0 )
            teapot_vertex_count++;
        else if(strcmp(buf, "f" ) == 0 )
            teapot_face_count++;
    }

    // allocate memory for vertices and faces array
    cout << "vertex_count: " << teapot_vertex_count << endl;
    cout << "face_count: " << teapot_face_count << endl;
    teapot_vertices = (GLfloat *)calloc(teapot_vertex_count, 3 * sizeof(GLfloat));
    teapot_normals = (GLfloat *)calloc(teapot_vertex_count, 3 * sizeof(GLfloat));
    teapot_faces = (GLuint *)calloc(teapot_face_count, 4 * sizeof(GLuint));
    normalindices = (GLuint *)calloc(teapot_face_count, 4 * sizeof(GLuint));
    temp_normals = (GLfloat *)calloc(teapot_vertex_count, 3 * sizeof(GLfloat));
    fclose(fptr);
    fptr = fopen(file_name, "r");
    int vertnum = 0;
    int facenum = 0;
    int normalnum = 0;

    while(1){
        // read the first word of the line
        int res = fscanf(fptr, "%s", buf);
        if(res == EOF)
            break; // EOF = End Of File. Quit the loop.
        if(strcmp( buf, "v" ) == 0 ){
            GLfloat vx,vy,vz;
            fscanf(fptr,"%f %f %fn", &vx, &vy, &vz);
            teapot_vertices[vertnum*3] = vx;
            teapot_vertices[vertnum*3+1] = vy;
            teapot_vertices[vertnum*3+2] = vz;
            vertnum++;
        }
        else if(strcmp(buf, "vn" ) == 0 ){
            GLfloat nx,ny,nz;
            fscanf(fptr,"%f %f %fn", &nx, &ny, &nz);
            temp_normals[normalnum*3] = nx;
            temp_normals[normalnum*3+1] = ny;
            temp_normals[normalnum*3+2] = nz;
            normalnum++;
        }
        else if ( strcmp(buf, "f" ) == 0 ){
            GLuint v1,v2,v3,v4,t1,t2,t3,t4,n1,n2,n3,n4;
            fscanf(fptr, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%dn",&v1,&t1,&n1,&v2,&t2,&n2,&v3,&t3,&n3,&v4,&t4,&n4);
            teapot_faces[facenum*4] = v1-1;   normalindices[facenum*4] = n1-1;
            teapot_faces[facenum*4+1] = v2-1; normalindices[facenum*4+1] = n2-1;
            teapot_faces[facenum*4+2] = v3-1; normalindices[facenum*4+2] = n3-1;
            teapot_faces[facenum*4+3] = v4-1; normalindices[facenum*4+3] = n4-1;
            teapot_normals[(v1-1)*3] = temp_normals[(n1-1)*3];
            teapot_normals[(v1-1)*3+1] = temp_normals[(n1-1)*3+1];
            teapot_normals[(v1-1)*3+2] = temp_normals[(n1-1)*3+2];
            teapot_normals[(v2-1)*3] = temp_normals[(n2-1)*3];
            teapot_normals[(v2-1)*3+1] = temp_normals[(n2-1)*3+1];
            teapot_normals[(v2-1)*3+2] = temp_normals[(n2-1)*3+2];
            teapot_normals[(v3-1)*3] = temp_normals[(n3-1)*3];
            teapot_normals[(v3-1)*3+1] = temp_normals[(n3-1)*3+1];
            teapot_normals[(v3-1)*3+2] = temp_normals[(n3-1)*3+2];
            teapot_normals[(v4-1)*3] = temp_normals[(n4-1)*3];
            teapot_normals[(v4-1)*3+1] = temp_normals[(n4-1)*3+1];
            teapot_normals[(v4-1)*3+2] = temp_normals[(n4-1)*3+2];
            facenum++;
        }
    }
    cout << "Load model success." <<endl;
}

void load_box()
{
	GLfloat vertices[] =
	{
		// left
		0.0, 0.0, 0.0,
		0.0, LENGTH, 0.0,
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
		// front: for test
		0.0, LENGTH, LENGTH,
		LENGTH, LENGTH, LENGTH,
		LENGTH, 0.0, LENGTH,
		0.0, 0.0, LENGTH
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
		// front
		0.0, 0.0, -1.0,
		0.0, 0.0, -1.0,
		0.0, 0.0, -1.0,
		0.0, 0.0, -1.0,
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
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		// bottom
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		// back
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		// front
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
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
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0
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

