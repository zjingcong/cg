# include <stdio.h>
# include <stdlib.h>
# include <iostream>
# include <math.h>
# include <fstream>

# define WINDOW_SIZE 768

using namespace std;


GLfloat *vertices;
GLuint *faces;
int vertex_count, face_count;
GLfloat light0_position[] = { 2.0, 2.0, 0.0, 1.0 };
GLfloat light0_direction[] = { -2.0, -2.0, 0.0, 1.0};


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

    glBindTexture(GL_TEXTURE_2D,1);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,im_width,im_height,0,GL_RGB,GL_UNSIGNED_BYTE,texture_bytes);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    cfree(texture_bytes);
}
/*
.ply file parser to load ply model
*/
void load_ply(const char *file_name)
{
	cout << "Load model " << file_name << "..." << endl;

	FILE *fptr;
	char buf[512], *parse;

	fptr = fopen(file_name, "r");

	// parse header to get vertex_count and face_count
	// char * fgets ( char * str, int num, FILE * stream );
	fgets(buf, 512, fptr);	// strip off PLY header
	fgets(buf, 512, fptr);	// strip off format line
	fgets(buf, 512, fptr);	// strip off comment
	fgets(buf, 512, fptr);	// get line with vertex count
	parse = strtok(buf, " \t");
	parse = strtok(NULL, " \t");
	parse = strtok(NULL, " \t\n");
	vertex_count = atoi(parse);
	fgets(buf, 512, fptr);	// strip off property x
	fgets(buf, 512, fptr);	// strip off property y
	fgets(buf, 512, fptr);	// strip off property z
	fgets(buf, 512, fptr);	// strip off property nx
	fgets(buf, 512, fptr);	// strip off property ny
	fgets(buf, 512, fptr);	// strip off property nz
	fgets(buf, 512, fptr);	// get line with face count
	parse = strtok(buf, " \t");
	parse = strtok(NULL, " \t");
	parse = strtok(NULL, " \t\n");
	face_count = atoi(parse);
	fgets(buf, 512, fptr);	// strip off property list
	fgets(buf, 512, fptr);	// strip off property end header

	// allocate memory for vertices and faces array
	cout << "vertex_count: " << vertex_count << endl;
	cout << "face_count: " << face_count << endl;
	vertices = (GLfloat *)calloc(vertex_count, 6 * sizeof(GLfloat));
	faces = (GLuint *)calloc(face_count, 3 * sizeof(GLuint));

	// load vertices
	for (int i = 0; i < vertex_count; ++i)
	{
		fgets(buf, 512, fptr);	// get line of vertex
		parse = strtok(buf,  " \t\n");
		vertices[i * 6] = atof(parse);
		for (int j = 1; j < 6; ++j)
		{
			parse = strtok(NULL,  " \t\n");
			vertices[i * 6 + j] = atof(parse);
		}
	}
	// load faces
	for (int i = 0; i < face_count; ++i)
	{
		fgets(buf, 512, fptr);	// get line of face
		parse = strtok(buf,  " \t\n");
		for (int j = 1; j < 4; ++j)
		{
			parse = strtok(NULL,  " \t\n");
			faces[i * 3 + (j - 1)] = atof(parse);
		}
	}
	cout << "Load model success." << endl;
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



void set_uniform(int p)
{
    int location;
    location = glGetUniformLocation(p,"mytexture");
    glUniform1i(location,0);
}

void set_shadowuniform(int p)
{
    int location;
    location = glGetUniformLocation(p,"myshadow");
    glUniform1i(location,6);
}

void build_shadowmap()
{
    // Set properties of texture id #2.
    glBindTexture(GL_TEXTURE_2D,2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
    // Declare size and type of texture; it has no data initially (last arg 0).
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WINDOW_SIZE, WINDOW_SIZE, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
    // Back to default.
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindFramebufferEXT(GL_FRAMEBUFFER,1);
    glDrawBuffer(GL_NONE); // No color buffers will be written.
    // Attach this framebuffer (id #2 above) to texture (id #2 is penultimate arg),
    // so that we can perform an offscreen render-to-texture.
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,2,0);
    // Back to default.
    glBindFramebufferEXT(GL_FRAMEBUFFER,0);
}



// Hollywood lighting
void set_lights()
{
	// scene ambient
	float scene_ambient[] = {0, 0, 0, 0};

	// key light
	float light0_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
	float light0_diffuse[] = { 1.5, 1.5, 1.5, 0.0 };
	float light0_specular[] = { 2.25, 2.25, 2.25, 0.0 };

	// fill light
	float light1_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
	float light1_diffuse[] = { 1.0, 1.0, 1.0, 0.0 };
	float light1_specular[] = { 2.25, 2.25, 2.25, 0.0 };
	float light1_position[] = { 0.0, 2.0, 2.0, 1.0 };
	float light1_direction[] = { 0.0, -2.0, -2.0, 1.0};

	// back light
	float light2_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
	float light2_diffuse[] = { 1.0, 1.0, 1.0, 0.0 };
	float light2_specular[] = { 2.25, 2.25, 2.25, 0.0 };
	float light2_position[] = { -2.0, 2.0, -2.0, 1.0 };
	float light2_direction[] = { 2.0, -2.0, 2.0, 1.0};

	// set scene default ambient
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, scene_ambient);

	// set hollywood light
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
	// key light
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 128);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0);	// cutoff range [0, 90], 180
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.2);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.01);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light0_direction);

	// fill light
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0.1);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 10.0);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.2);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.01);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1_direction);

	// back light
	glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 0.1);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 10.0);
	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.2);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.01);
	glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light2_direction);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
}


void set_material(int i)
{
    if(i==1){
        float mat_ambient[] = {0.021500, 0.174500, 0.021500, 1.000000};
        float mat_diffuse[] = {0.075680, 0.614240, 0.075680, 1.000000};
        float mat_specular[] = {0.633000, 0.727811, 0.633000, 1.000000};
        float mat_shininess[] = {76.800003};

        glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
        glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
        glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
        glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
	}
	if(i==2){
        float mat_ambient[] = {0.105882, 0.058824, 0.113725, 0.500000};
        float mat_diffuse[] = {0.427451, 0.470588, 0.541176, 0.500000};
        float mat_specular[] = {0.333333, 0.333333, 0.521569, 0.500000};
        float mat_shininess[] = {9.846150};

        glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
        glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
        glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
        glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
	}
}


unsigned int set_shaders(int i)
{
	GLint vertCompiled, fragCompiled;
	string vs_str, fs_str;
	GLuint v, f, p;
	int result = -1;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);
	if(i==1){
        vs_str = load_shader_file("bunny.vert");
        fs_str = load_shader_file("bunny.frag");
	}
	if(i==2){
        vs_str = load_shader_file("floor.vert");
        fs_str = load_shader_file("floor.frag");
	}
	const char *vs = vs_str.c_str();
	const char *fs = fs_str.c_str();
	glShaderSource(v, 1, &vs, NULL);
	glShaderSource(f, 1, &fs, NULL);

	glCompileShader(v);
	glCompileShader(f);
	glGetShaderiv(f,GL_COMPILE_STATUS, &result);
	// cerr << result << endl;

	p = glCreateProgram();
	glAttachShader(p,f);
	glAttachShader(p,v);
	glLinkProgram(p);
	glUseProgram(p);

	return p;
}

