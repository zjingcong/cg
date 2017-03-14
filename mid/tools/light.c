#include <stdio.h>
#include <math.h>

struct point{
   float x;
   float y;
   float z;
};

struct point cross(struct point u, struct point v){
   struct point w;
   w.x = u.y*v.z - u.z*v.y;
   w.y = -(u.x*v.z - u.z*v.x);
   w.z = u.x*v.y - u.y*v.x;
   return w;
}

struct point diff(struct point u, struct point v){
   struct point w;
   w.x = u.x - v.x;
   w.y = u.y - v.y;
   w.z = u.z - v.z;
   return w;
}

struct point add(struct point u, struct point v){
   struct point w;
   w.x = u.x + v.x;
   w.y = u.y + v.y;
   w.z = u.z + v.z;
   return w;
}

struct point unit_length(struct point u){
   double length;
   struct point v;
   length = sqrt(u.x*u.x + u.y*u.y + u.z*u.z);
   v.x = u.x/length;
   v.y = u.y/length;
   v.z = u.z/length;
   return v;
}

float dot(struct point u, struct point v){
   return (u.x*v.x + u.y*v.y + u.z*v.z);
}

float max(float a, float b){
   return (a>b ? a : b);
}

float length(struct point a, struct point b){
   struct point c;
   c = diff(a,b);
   return sqrt(c.x*c.x + c.y*c.y + c.z*c.z);
}

struct point pfmul(struct point a, float b){
   struct point c;
   c.x = a.x * b;
   c.y = a.y * b;
   c.z = a.z * b;
   return c;
}

struct point ppmul(struct point a, struct point b){
   struct point c;
   c.x = a.x * b.x;
   c.y = a.y * b.y;
   c.z = a.z * b.z;
   return c;
}

int main(int argc, char **argv)
{
   struct point vertex_position;
   struct point vertex_normal;
   struct point eye;
   struct point view_direction;
   struct point light_position;
   struct point light_direction;

   struct point Dm, Sm, Am;
   struct point Ds, Ss;
   float shininess, falloff, kc, kl, kq;

   vertex_position.x = 1; vertex_position.y = 0; vertex_position.z = 0;
   vertex_normal.x = 0; vertex_normal.y = 1; vertex_normal.z = 0;
   eye.x = 2; eye.y = M_SQRT2; eye.z = 1;
   view_direction = diff(vertex_position, eye);
   light_position.x = 0; light_position.y = M_SQRT2; light_position.z = 1;
   light_direction.x = 0; light_direction.y = -1; light_direction.z = 0;
   
   Dm.x = 0.6*M_SQRT2; Dm.y = 0; Dm.z = 0;
   Sm.x = 0.6; Sm.y = 0; Sm.z = 0.6;
   Am.x = 0; Am.y = 0; Am.z = 0;
   Ds.x = 1; Ds.y = 1; Ds.z = 1;
   Ss.x = 1; Ss.y = 1; Ss.z = 1;
   shininess = 2;
   falloff = 2;
   kc = 0; kl = 0; kq = 1;

   struct point P,N,L,_L,V,H,SD;
   float d;
   P = vertex_position;
   N = unit_length(vertex_normal);
   L = unit_length(diff(light_position, P));
   _L = unit_length(diff(P, light_position)); // -L
   V = unit_length(diff(eye, P));
   H = unit_length(add(L, V));
   SD = unit_length(light_direction);
   d = length(P,light_position);
   
   struct point specular_color = pfmul(ppmul(Sm,Ss), pow(max(dot(H,N),0.0), shininess));
   printf("specular factor is %f\n",pow(max(dot(H,N),0.0), shininess));
   struct point diffuse_color = pfmul(ppmul(Dm,Ds), max(dot(L,N),0.0));
   printf("diffuse factor is %f\n", max(dot(L,N),0.0));
   struct point color = add(diffuse_color, specular_color);
   float atten = 1.0/(kc+kl*d+kq*d*d);
   float spoteffect = pow(dot(_L,SD),falloff);
   color = pfmul(color,atten*spoteffect);

   printf("distance is %f\n", d);
   printf("color is (%f, %f, %f)\n", color.x, color.y, color.z);
   return 0;   
}
