#ifndef __CAMERA_H__
#define __CAMERA_H__

# include <Vector.h>


class Camera
{
	public:
		Camera();
		~Camera()	{}

		void setEyeViewUp(const Vector& position, const Vector& center, const Vector& up_vector);
		const Vector& eye() const  {return eye;}
    const Vector& view() const {return view;}
    const Vector& up() const   {return up;}

		void rotation(float theta, Vector& axis);
		void zoom()	{}
		void track()	{}

	private:
		Vector eye;	// position of camera
		Vector view;	// center
		Vector up;	// up

		// theta in degree
		// axis should be unit vector
		Vector vecRotation(Vector v, Vector axis, float a)
		{
			float theta = PI * a / 180.0;
			return v * cos(theta) + axis * (axis * v) * ( 1 - cos(theta)) + (axis ^ v) * sin(theta);
		}
};


Camera::Camera()
{
	setEyeViewUp(Vector(1.0, 1.0, 1.0), Vector(0.0, 0.0, 0.0), Vector(0.0, 1.0, 0.0));
}

void Camera::(const Vector& position, const Vector& center, const Vector& up_vector)
{
	eye = position;
	view = center;
	up = up_vector;
}

// theta in degree
void Camera::rotation(float theta, Vector& axis)
{
	Vector rotation_axis = axis.unitvector();
	Vector eye_new = vecRotation(eye, rotation_axis, theta);
	setEyeViewUp(eye_new, view, up);
}

#endif

