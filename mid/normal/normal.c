
# include <iostream>
# include "Vector.h"

using namespace std;
using namespace lux;

Vector p0(7, 8, 10);
Vector p1(4, 10, 9);
Vector p2(14, 8, 13);

int main()
{
    Vector e1 = p1 - p0;
    Vector e2 = p2 - p0;

    Vector normal = e1 ^ e2;
    Vector norm_normal = normal.unitvector();

    cout << "normal: " << normal.X() << ", " << normal.Y() << ", " << normal.Z() << endl;
    cout << "norm_normal: " << norm_normal.X() << ", " << norm_normal.Y() << ", " << norm_normal.Z() << endl;

    return 0;
}

