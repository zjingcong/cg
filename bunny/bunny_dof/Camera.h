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

		double getNear()	{return near;}
		double getFar()	{return far;}
		double getFocal()	{return focal;}
		Vector& getEye()  {return eye;}
    Vector& getView() {return view;}
    Vector& getUp()   {return up;}
		Vector& getViewDir()	{return view_dir;}
		Vector& getRight()	{return right;}

		// camera movement
		void rotation(double theta, Vector axis);
		void zoom(double distance);
		void track(Vector translate);
		void reset();

	private:
		double near;
		double far;
		double focal;

		Vector eye;	// position of camera
		Vector view;	// center
		Vector up;	// up
		Vector right;	// right vector
		Vector view_dir;	// view direction to center, it's a unit vector

    Vector camera_up; // default (0, 1, 0)

		// theta in degree
		// axis should be unit vector
		Vector vecRotation(Vector vv, Vector axis, float a)
		{
			float theta = M_PI * a / 180.0;
      Vector v = view - vv; // get view direction
			Vector vr = v * cos(theta) + axis * (axis * v) * ( 1 - cos(theta)) + (axis ^ v) * sin(theta);
      return view - vr;
		}
};


Camera::Camera()
{
	near = 0.01;
	focal = 6.0;
	far = 20.0;
  reset();
}


void Camera::reset()
{
	camera_up = Vector(0.0, 1.0, 0.0);
  eye = Vector(0.3, 0.3, 0.3);
  view = Vector(0.0, 0.0, 0.0);
  view_dir = (view - eye).unitvector();
	right = (view_dir ^ camera_up).unitvector();
  up = (right ^ view_dir).unitvector();
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


void Camera::zoom(double step_size)
{
	Vector eye_view = view - eye;
	double distance = step_size * eye_view.magnitude();
	Vector eye_view_new = eye_view - distance * view_dir;
	if (eye_view_new.magnitude() > (near + 0.01f))
	{
		Vector eye_new = view - eye_view_new;
		setEyeViewUp(eye_new, view, up);
	}
}


void Camera::track(Vector translate)
{
	Vector eye_new = eye + translate;
	Vector view_new = view + translate;
	setEyeViewUp(eye_new, view_new, up);
}

#endif

