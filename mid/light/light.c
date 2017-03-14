
# include <string.h>
# include <iostream>
# include "math.h"

# include "Vector.h"
# include "Color.h"


using namespace std;
using namespace lux;

# define getMax(x, y) (x > y ? x : y)
# define getMin(x, y) (x < y ? x : y)

Vector vpos(1, 0, 0);
Vector vnormal(0, 1, 0);
Vector eye(2, M_SQRT2, 1);
Vector litPos(0, M_SQRT2, 1);
Vector litDir(0, -1, 0);
float kc = 0;
float kq = 0;
float kl = 1;
Vector matAmbient(0, 0, 0);
Vector matSpec(0.6, 0, 0.6);
Vector matDiff(0.6 * M_SQRT2, 0, 0);
Vector litSpec(1, 1, 1);
Vector litDiff(1, 1, 1);
float shininess = 2.0;
float falloff = 2.0;


int main()
{
	Vector P, N, L, V, H;
	P = vpos;
	N = vnormal.unitvector();
	V = (eye - vpos).unitvector();
	L = (litPos - P).unitvector();
	H = (L + V).unitvector();
	
	float diff = getMax(N * L, 0.0);
    // multiple (shininess + 2.0) / (8.0 * M_PI) - normalized blinn-phong
	float spec = /*((shininess + 2.0) / (8.0 * M_PI)) * */pow(getMax(H * N, 0.0), shininess);
	float d = (litPos - P).magnitude();
	float attenuation = 1.0f / (kc + kl * d + kq * d * d);
	float spot = pow(getMax((-L) * litDir.unitvector(), 0.0), falloff);

	// ambientColor = litAmbient * matAmbient;
	Vector diffColor(litDiff.X() * matDiff.X(), litDiff.Y() * matDiff.Y(), litDiff.Z() * matDiff.Z());
	Vector specColor(litSpec.X() * matSpec.X(), litSpec.Y() * matSpec.Y(), litSpec.Z() * matSpec.Z());
	diffColor *= (attenuation * spot * diff);
	specColor *= (attenuation * spot * spec);

	Vector color;
	color = /*ambientColor + */diffColor + specColor;

	cout << "color: " << color.X() << ", " << color.Y() << ", " << color.Z() << endl;

	return 0;
}

