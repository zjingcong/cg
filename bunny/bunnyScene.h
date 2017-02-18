# include <stdio.h>
# include <stdlib.h>
# include <iostream>
# include <math.h>
# include <fstream>

using namespace std;


GLfloat *vertices;
GLuint *faces;
int vertex_count, face_count;


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


// Hollywood lighting
void set_lights()
{
	// key light
	float light0_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
	float light0_diffuse[] = { 1.5, 1.5, 1.5, 0.0 }; 
	float light0_specular[] = { 2.25, 2.25, 2.25, 0.0 }; 
	float light0_position[] = { 2.0, 2.0, 0.0, 1.0 };
	float light0_direction[] = { 0.0, 0.0, 0.0, 1.0};

	// fill light
	float light1_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
	float light1_diffuse[] = { 1.0, 1.0, 1.0, 0.0 }; 
	float light1_specular[] = { 2.25, 2.25, 2.25, 0.0 }; 
	float light1_position[] = { 0.0, 2.0, 2.0, 1.0 };
	float light1_direction[] = { 0.0, 0.0, 0.0, 1.0};

	// back light
	float light2_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
	float light2_diffuse[] = { 1.0, 1.0, 1.0, 0.0 }; 
	float light2_specular[] = { 2.25, 2.25, 2.25, 0.0 }; 
	float light2_position[] = { -2.0, 2.0, -2.0, 1.0 };
	float light2_direction[] = { 0.0, 0.0, 0.0, 1.0};

	// set scene default ambient 
	// glLightModelfv(GL_LIGHT_MODEL_AMBIENT,light0_ambient);

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

	// key light
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient); 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse); 
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular); 
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 0.1); 
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0); 
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
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 180.0); 
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
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 180.0); 
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


void set_material()
{
	float mat_ambient[] = {0.0,0.0,0.0,1.0}; 
	float mat_diffuse[] = {0.9,0.9,0.1,1.0}; 
	float mat_specular[] = {1.0,1.0,1.0,1.0};
	float mat_shininess[] = {2.0}; 

	glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
}


unsigned int set_shaders()
{
	GLint vertCompiled, fragCompiled;
	string vs_str, fs_str;
	GLuint v, f, p;
	int result = -1;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);
	vs_str = load_shader_file("bunny.vert");
	fs_str = load_shader_file("bunny.frag");
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

