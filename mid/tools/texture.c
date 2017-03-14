#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glx.h>
#include <GL/glext.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <math.h>

float eye[] = {M_SQRT2, 2.0, 2.0};
float viewpt[] = {0.0, 0.0, 0.0};
float up[] = {0.0, 1.0, 0.0};

char *read_shader_program(char *filename){
   FILE *fp;
   char *content = NULL;
   int fd, count;
   fd = open(filename, O_RDONLY);
   count = lseek(fd, 0, SEEK_END);
   close(fd);
   content = (char *)calloc(1, (count+1));
   count = fread(content, sizeof(char), count, fp);
   content[count] = '\0';
   fclose(fp);
   return content;
}

void load_texture(char *filename){
   FILE *fopen(), *fptr;
   char buf[512], *parse;
   int im_size, im_width, im_height, max_color;
   unsigned char *texture_bytes;

   fptr = fopen(filename, "r");
   fgets(buf, 512, fptr);
   do{
      fgets(buf,512,fptr);
   }while(buf[0] == '#');
   parse = (char *)strtok(buf, " \t");
   im_width = atoi(parse);

   parse = (char *)strtok(NULL, " \n");
   im_height = atoi(parse);

   fgets(buf, 512, fptr);
   parse = (char *)strtok(buf, "\n");
   max_color = atoi(parse);

   im_size = im_width*im_height;
   texture_bytes = (unsigned char *)calloc(3,im_size);
   fread(texture_bytes, 3, im_size, fptr);
   fclose(fptr);

   glBindTexture(GL_TEXTURE_2D, 1);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, im_width, im_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_bytes);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   free(texture_bytes);
}

void lights(){
   float light0_ambient[] = {0,0,0,0};
   float light0_diffuse[] = {0.9,0.8,0.8,0.0};
   float light0_specular[] = {1.0,0.9,0.9,0.0};
   float light0_position[] = {M_SQRT2,2.0,2.0,1.0};
   float light0_direction[] = {-M_SQRT2,-2.0,-2.0,1.0};

   glLightModelfv(GL_LIGHT_MODEL_AMBIENT,light0_ambient);
   glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,1);
   glLightfv(GL_LIGHT0,GL_AMBIENT,light0_ambient);
   glLightfv(GL_LIGHT0,GL_DIFFUSE,light0_diffuse);
   glLightfv(GL_LIGHT0,GL_SPECULAR,light0_specular);
   glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,0.0);
   glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,180.0);
   glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,0.0);
   glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,1.0);
   glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,0.0);
   glLightfv(GL_LIGHT0,GL_POSITION,light0_position);
   glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,light0_direction);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
}

void material(){
   float mat_diffuse[] = {1.0,1.0,0.0,1.0};
   float mat_specular[] = {1.0,1.0,1.0,1.0};
   float mat_shininess[] = {2.0};

   glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void view_volume(){
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0,1.0,1.0,40.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(eye[0],eye[1],eye[2],viewpt[0],viewpt[1],viewpt[2],up[0],up[1],up[2]);
}

int sprogram;

void render_scene(){
   int i;
   float front[4][3] = {{0,0,1},{1,0,1},{1,1,1},{0,1,1}};
   float back[4][3] = {{0,0,0},{0,1,0},{1,1,0},{1,0,0}};
   float left[4][3] = {{0,0,0},{0,0,1},{0,1,1},{0,1,0}};
   float right[4][3] = {{1,0,0},{1,1,0},{1,1,1},{1,0,1}};
   float top[4][3] = {{0,1,0},{0,1,1},{1,1,1},{1,1,0}};
   float bottom[4][3] = {{0,0,0},{0,0,1},{1,0,1},{1,0,0}};
   float mytexcoords[4][2] = {{0,1},{1,1},{1,0},{0,0}};

   glClearColor(3.0,4.0,5.0,1.0);
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

   glUseProgram(0);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D,1);
   glEnable(GL_TEXTURE_2D);
   glBegin(GL_QUADS);
   glNormal3f(0.0,0.0,1.0);
   for(i=0;i<4;i++){
      glTexCoord2fv(mytexcoords[i]);
      glVertex3f(front[i][0],front[i][1],front[i][2]);
   }
   glEnd();
   glDisable(GL_TEXTURE_2D);
   glUseProgram(0);
   glBegin(GL_QUADS);
   glNormal3f(0.0,0.0,-1.0);
   for(i=0;i<4;i++) glVertex3f(back[i][0],back[i][1],back[i][2]);
   glNormal3f(-1.0,0.0,0.0);
   for(i=0;i<4;i++) glVertex3f(left[i][0],left[i][1],left[i][2]);
   glNormal3f(1.0,0.0,0.0);
   for(i=0;i<4;i++) glVertex3f(right[i][0],right[i][1],right[i][2]);
   glNormal3f(0.0,1.0,0.0);
   for(i=0;i<4;i++) glVertex3f(top[i][0],top[i][1],top[i][2]);
   glNormal3f(0.0,-1.0,0.0);
   for(i=0;i<4;i++) glVertex3f(bottom[i][0],bottom[i][1],bottom[i][2]);
   glEnd();
   glutSwapBuffers();
}

unsigned int set_shaders(){
   GLint vertCompiled, fragCompiled;
   char *vs, *fs;
   GLuint v, f, p;

   v = glCreateShader(GL_VERTEX_SHADER);
   f = glCreateShader(GL_FRAGMENT_SHADER);
   vs = read_shader_program("phong_texcube.vert");
   fs = read_shader_program("phong_texcube.frag");
   glShaderSource(v,1,(const char **)&vs,NULL);
   glShaderSource(f,1,(const char **)&fs,NULL);
   free(vs);
   free(fs);
   glCompileShader(v);
   glCompileShader(f);
   p = glCreateProgram();
   glAttachShader(p,f);
   glAttachShader(p,v);
   glLinkProgram(p);
   glUseProgram(p);
   return (p);
}

void set_uniform_parameters(unsigned int p){
   int location;
   location = glGetUniformLocation(p, "mytexture");
   glUniform1i(location, 0);
}

int main(int argc, char **argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
   glutInitWindowPosition(100,100);
   glutInitWindowSize(512,512);
   glutCreateWindow("phong-shaded, textured cube");
   glEnable(GL_DEPTH_TEST);
   load_texture(argv[1]);
   view_volume();
   lights();
   material();
   sprogram = set_shaders();
   set_uniform_parameters(sprogram);
   glutDisplayFunc(render_scene);
   glutMainLoop();
   return 0;
}
