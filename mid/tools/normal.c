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

struct point unit_length(struct point u){
   double length;
   struct point v;
   length = sqrt(u.x*u.x + u.y*u.y + u.z*u.z);
   v.x = u.x/length;
   v.y = u.y/length;
   v.z = u.z/length;
   return v;
}

struct point calc_normal(struct point v0, struct point v1, struct point v2){
   return unit_length(cross(diff(v1,v0), diff(v2,v0)));
}

int main(int argc, char **argv)
{
   struct point v0, v1, v2;
   struct point r;

   v0.x = 7; v0.y = 8; v0.z = 10;
   v1.x = 4; v1.y = 10; v1.z = 9;
   v2.x = 14; v2.y = 8; v2.z = 13;

   r = cross(diff(v1,v0), diff(v2,v0));

   printf("the result is (%f, %f, %f)\n", r.x, r.y, r.z);

//   float x, y, z;
//   x = (-1)/M_SQRT2;
//   y = 1/M_SQRT2;
//   z = 0;
//
//   printf("%lf, %lf\n", x/(2*sqrt(2*(z+1))) + 0.5, y/(2*sqrt(2*(z+1))) + 0.5);
//      
   return 0;
}
