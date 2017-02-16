#ifndef __CAMERA_H__
#define __CAMERA_H__

# include "math.h"
# include <cmath>
# include <iostream>

# include "Vector.h"

using namespace std;


class Camera
{
	public:
		Camera();
		~Camera()	{}

		void setEyeViewUp(const Vector& position, const Vector& center, const Vector& up_vector);
		Vector& getEye()  {return eye;}
    Vector& getView() {return view;}
    Vector& getUp()   {return up;}
		Vector& getViewDir()	{return view_dir;}
		Vector& getRight()	{return right;}

		void rotation(double theta, Vector axis);
		void zoom()	{}
		void track()	{}

	private:
		Vector eye;	// position of camera
		Vector view;	// center
		Vector up;	// up
		Vector right;	// right vector
		Vector view_dir;	// view direction to center

    Vector camera_up; // default (0, 1, 0)
		// theta in degree
		// axis should be unit vector
		Vector vecRotation(Vector vv, Vector axis, float a)
		{
			float theta = M_PI * a / 180.0;
      Vector v = vv - view; // get view direction
			Vector vr = v * cos(theta) + axis * (axis * v) * ( 1 - cos(theta)) + (axis ^ v) * sin(theta);
      return vr + view;
		}
};


Camera::Camera()
{
  camera_up = Vector(0.0, 1.0, 0.0);
  eye = Vector(1.0, 1.0, 1.0);
  view = Vector(0.0, 0.0, 0.0);
  view_dir = (view - eye).unitvector();
	right = (view_dir ^ camera_up).unitvector();
	cout << right.X() << " " << right.Y() << " " << right.Z() << endl;
  up = (right ^ view_dir).unitvector();
	cout << up.X() << " " << up.Y() << " " << up.Z() << endl;
}

void Camera::setEyeViewUp(const Vector& position, const Vector& center, const Vector& up_vector)
{
	eye = position;
	view = center;
	up = up_vector;
	view_dir = (view - eye).unitvector();
	right = (view_dir ^ up).unitvector();
}

// theta in degree
void Camera::rotation(double theta, Vector axis)
{
	Vector rotation_axis = axis.unitvector();
	Vector eye_new = vecRotation(eye, rotation_axis, theta);
  Vector up_new = vecRotation(up, rotation_axis, theta);
	setEyeViewUp(eye_new, view, up_new);
}

#endif

