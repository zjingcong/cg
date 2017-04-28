# ifndef _SCENE_H_
# define _SCENE_H_

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

# define LENGTH 2.5

using namespace std;

GLfloat *box_vertices;
GLfloat *box_normals;
GLuint *box_faces;
GLfloat *box_colors;
int box_face_num = 6;

GLfloat *light_vertices;
GLfloat *light_normals;
GLuint *light_faces;
GLfloat *light_colors;

GLfloat *teapot_vertices;
GLfloat *teapot_normals;
GLfloat *teapot_tangents;
GLfloat *teapot_bitangents;
GLfloat *teapot_texcoords;
GLuint *teapot_faces;
int teapot_vertex_count = 0;
int teapot_face_count = 0;
int teapot_tex_count = 0;


void load_obj(const char *file_name){
    FILE *fptr;
    char buf[512];
    GLfloat *temp_normals;
    GLfloat *temp_texcoords;
    GLuint *normalindices;
    GLuint *texindices;
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
        else if(strcmp(buf, "vt" ) == 0 )
            teapot_tex_count++;
    }

    // allocate memory for vertices and faces array
    teapot_vertices = (GLfloat *)calloc(teapot_vertex_count, 3 * sizeof(GLfloat));
    teapot_normals = (GLfloat *)calloc(teapot_vertex_count, 3 * sizeof(GLfloat));
    teapot_tangents = (GLfloat *)calloc(teapot_vertex_count+1, 3 * sizeof(GLfloat));
    teapot_bitangents = (GLfloat *)calloc(teapot_vertex_count+1, 3 * sizeof(GLfloat));
    teapot_texcoords = (GLfloat *)calloc(teapot_vertex_count, 2 * sizeof(GLfloat));
    teapot_faces = (GLuint *)calloc(teapot_face_count, 4 * sizeof(GLuint));
    normalindices = (GLuint *)calloc(teapot_face_count, 4 * sizeof(GLuint));
    texindices = (GLuint *)calloc(teapot_face_count, 4 * sizeof(GLuint));
    temp_normals = (GLfloat *)calloc(teapot_vertex_count, 3 * sizeof(GLfloat));
    temp_texcoords = (GLfloat *)calloc(teapot_tex_count, 2 * sizeof(GLfloat));
    fclose(fptr);
    fptr = fopen(file_name, "r");
    int vertnum = 0;
    int facenum = 0;
    int normalnum = 0;
    int tangentnum = 0;
    int bitangentnum = 0;
    int texcoordnum = 0;

    while(1){
        // read the first word of the line
        int res = fscanf(fptr, "%s", buf);
        if(res == EOF)
            break; // EOF = End Of File. Quit the loop.
        if(strcmp( buf, "v" ) == 0 ){
            fscanf(fptr,"%f %f %fn", &teapot_vertices[vertnum*3], &teapot_vertices[vertnum*3+1], &teapot_vertices[vertnum*3+2]);
            vertnum++;
        }
        else if(strcmp(buf, "vn" ) == 0 ){
            fscanf(fptr,"%f %f %fn", &temp_normals[normalnum*3], &temp_normals[normalnum*3+1], &temp_normals[normalnum*3+2]);
            normalnum++;
        }
        else if(strcmp(buf, "vx" ) == 0 ){
            fscanf(fptr,"%f %f %fn", &teapot_tangents[tangentnum*3], &teapot_tangents[tangentnum*3+1], &teapot_tangents[tangentnum*3+2]);
            tangentnum++;
        }
        else if(strcmp(buf, "vy" ) == 0 ){
            fscanf(fptr,"%f %f %fn", &teapot_bitangents[bitangentnum*3], &teapot_bitangents[bitangentnum*3+1], &teapot_bitangents[bitangentnum*3+2]);
            bitangentnum++;
        }
        else if(strcmp(buf, "vt" ) == 0 ){
            fscanf(fptr,"%f %fn", &temp_texcoords[texcoordnum*2], &temp_texcoords[texcoordnum*2+1]);
            texcoordnum++;
        }
        else if ( strcmp(buf, "f" ) == 0 ){
            GLuint v1,v2,v3,v4,t1,t2,t3,t4,n1,n2,n3,n4;
            fscanf(fptr, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%dn",&v1,&t1,&n1,&v2,&t2,&n2,&v3,&t3,&n3,&v4,&t4,&n4);
            teapot_faces[facenum*4] = v1-1;   normalindices[facenum*4] = n1-1;   texindices[facenum*4] = t1-1;
            teapot_faces[facenum*4+1] = v2-1; normalindices[facenum*4+1] = n2-1; texindices[facenum*4+1] = t2-1;
            teapot_faces[facenum*4+2] = v3-1; normalindices[facenum*4+2] = n3-1; texindices[facenum*4+2] = t3-1;
            teapot_faces[facenum*4+3] = v4-1; normalindices[facenum*4+3] = n4-1; texindices[facenum*4+3] = t4-1;
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

            teapot_texcoords[(v1-1)*2] = temp_texcoords[(t1-1)*2];
            teapot_texcoords[(v1-1)*2+1] = temp_texcoords[(t1-1)*2+1];
            teapot_texcoords[(v2-1)*2] = temp_texcoords[(t2-1)*2];
            teapot_texcoords[(v2-1)*2+1] = temp_texcoords[(t2-1)*2+1];
            teapot_texcoords[(v3-1)*2] = temp_texcoords[(t3-1)*2];
            teapot_texcoords[(v3-1)*2+1] = temp_texcoords[(t3-1)*2+1];
            teapot_texcoords[(v4-1)*2] = temp_texcoords[(t4-1)*2];
            teapot_texcoords[(v4-1)*2+1] = temp_texcoords[(t4-1)*2+1];
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
		-LENGTH, 2.0f * LENGTH, -LENGTH,
		-LENGTH, 2.0f * LENGTH, LENGTH,
		-LENGTH, 0.0, LENGTH,
		-LENGTH, 0.0, -LENGTH,
		// right
		LENGTH, 2.0f * LENGTH, LENGTH,
		LENGTH, 2.0f * LENGTH, -LENGTH,
		LENGTH, 0.0, -LENGTH,
		LENGTH, 0.0, LENGTH,
		// top
		-LENGTH, 2.0f * LENGTH, LENGTH,
		-LENGTH, 2.0f * LENGTH, -LENGTH,
		LENGTH, 2.0f * LENGTH, -LENGTH,
		LENGTH, 2.0f * LENGTH, LENGTH,
		// bottom
		LENGTH, 0.0, -LENGTH,
		-LENGTH, 0.0, -LENGTH,
		-LENGTH, 0.0, LENGTH,
		LENGTH, 0.0, LENGTH,
		// back
		LENGTH, 2.0f * LENGTH, -LENGTH,
		-LENGTH, 2.0f * LENGTH, -LENGTH,
		-LENGTH, 0.0, -LENGTH,
		LENGTH, 0.0, -LENGTH,
		// front
		-LENGTH, 2.0f * LENGTH, LENGTH,
		LENGTH, 2.0f * LENGTH, LENGTH,
		LENGTH, 0.0, LENGTH,
		-LENGTH, 0.0, LENGTH

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

	float dx = 1.2;
	GLfloat colors[] =
	{
		// left: red
		dx, 0.0, 0.0,
		dx, 0.0, 0.0,
		dx, 0.0, 0.0,
		dx, 0.0, 0.0,
		// right: green
		0.0, dx, 0.0,
		0.0, dx, 0.0,
		0.0, dx, 0.0,
		0.0, dx, 0.0,
		// others: white
		// top
		dx, dx, dx,
		dx, dx, dx,
		dx, dx, dx,
		dx, dx, dx,
		// bottom
		dx, dx, dx,
		dx, dx, dx,
		dx, dx, dx,
		dx, dx, dx,
		// back
		dx, dx, dx,
		dx, dx, dx,
		dx, dx, dx,
		dx, dx, dx,
		// front
		dx, dx, dx,
		dx, dx, dx,
		dx, dx, dx,
		dx, dx, dx,
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
	GLfloat y = 2.0f * LENGTH - 0.01f;
	float dx = 0.8;
	GLfloat vertices[] =
	{
		dx, y, dx,
		-dx, y, dx,
		-dx, y, -dx,
		dx, y, -dx
	};

	GLfloat normals[] =
	{
		0.0, -1.0, 0.0,
		0.0, -1.0, 0.0,
		0.0, -1.0, 0.0,
		0.0, -1.0, 0.0
	};

	float dc = 10.0;
	GLfloat colors[] =
	{
		// white
		dc, dc, dc,
		dc, dc, dc,
		dc, dc, dc,
		dc, dc, dc,
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

void load_texture(char *filename)
{
    FILE *fptr;
    char buf[512];
    int im_size, im_width, im_height, max_color;
    char *texture_bytes, *parse;

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
    texture_bytes = (char *)calloc(3,im_size);
    fread(texture_bytes,3,im_size,fptr);
    fclose(fptr);

    glBindTexture(GL_TEXTURE_2D,2);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,im_width,im_height,0,GL_RGB,GL_UNSIGNED_BYTE,texture_bytes);
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


# endif

