// Camera.h
//
#ifndef CAMERA_H
#define CAMERA_H
#pragma once

#include "IDSReal3D.h"
#include "IDSMatrix3D.h"

namespace IDS{
namespace OpenGL{

class Camera
{
protected:
	double viewDistance;

	IDSReal3D eye;
	IDSReal3D up;
	IDSReal3D viewDir;
	IDSReal3D center;
	IDSReal3D crossDir;


	void adjust()
	{
		const IDSReal3D upY(0.0f, 1.0f, 0.0f);

		viewDistance = viewDir.mod();
		viewDir      = viewDir / viewDistance; // basterebbe dividere per viewDistance
		
		crossDir = (viewDir ^ upY).vers();
		up       = (crossDir ^ viewDir).vers();

		//wxString s;
		//s.Printf("\nEYE   [%g,%g,%g]\n",eye[0],eye[1],eye[2]); OutputDebugStr(s.c_str());
		//s.Printf("CTR   [%g,%g,%g]\n",center[0],center[1],center[2]); OutputDebugStr(s.c_str());
		//s.Printf("CROSS [%g,%g,%g]\n",crossDir[0],crossDir[1],crossDir[2]); OutputDebugStr(s.c_str());
	}


public:
	Camera(){}
	~Camera(){}

	void setVectors(IDSReal3D &eyePos, IDSReal3D &upVector, IDSReal3D &dir)
	{
		eye     = eyePos;
		up      = upVector;
		viewDir = dir;

		adjust();

		center = eye + (viewDir * viewDistance);
	}

	void lookAt(IDSReal3D &eyePos, IDSReal3D &centerPos, IDSReal3D &upVector)
	{
		eye      = eyePos;
		viewDir  = centerPos - eye;
		up       = upVector;

		adjust();

		center = eye + (viewDir * viewDistance);
	}

	void lookAt(IDSReal3D &centerPos)
	{
	//	eye      = eyePos;
		viewDir  = centerPos - eye;
	//	up       = upVector;

		adjust();

		center = eye + (viewDir * viewDistance);
	}
	

	void moveEye(float tx, float ty, float tz)
	{
		IDSReal3D moveDir[3];

		moveDir[0] = crossDir * tx;
		moveDir[1] = up       * ty;
		moveDir[2] = viewDir  * tz;

		eye += moveDir[0] + moveDir[1] + moveDir[2];
		center = eye + (viewDir * viewDistance);
	}

	void moveCenter(IDSReal3D &dir)
	{
		IDSReal3D moveDir[3];

		moveDir[0] = crossDir * dir[0];
		moveDir[1] = up       * dir[1];
		moveDir[2] = viewDir  * dir[2];

		center += moveDir[0] + moveDir[1] + moveDir[2];
		center = eye + (viewDir * viewDistance);
	}

	// FPS Rotation
	void rotateAtEyePosition(float ax, float ay, float az)
	{
		IDSUnitQuaternion q;
	
		q.setRotation(ax, crossDir[0],crossDir[1],crossDir[2]);	viewDir = q.rotate(viewDir); 
		q.setRotation(ay, up[0],up[1],up[2]);					viewDir = q.rotate(viewDir); 
		q.setRotation(az, 0,1,0);								viewDir = q.rotate(viewDir); 

		adjust();

		center = eye + (viewDir * viewDistance);
	}

	// Rotation
	void rotateAtCenterPosition(float ax, float ay, float az)
	{
		double dist = viewDistance;
	
		IDSUnitQuaternion q;

		////adjust();
		//IDSUnitQuaternion rotX, rotZ;
		//rotX.setRotation(ax, 1,0,0);
		//rotZ.setRotation(az, 0,1,0);
		//q = rotZ*rotX;
		//viewDir  = q.rotate(viewDir); 

		q.setRotation(ax, crossDir[0],crossDir[1],crossDir[2]);
		viewDir  = q.rotate(viewDir); 
		
		up       = (crossDir ^ viewDir).vers();
		
		q.setRotation(az, up[0],up[1],up[2]);
		viewDir = q.rotate(viewDir); 


		crossDir = (viewDir ^ up).vers();

		eye = center - (viewDir * dist);
	}

	void getGLMatrix(float * m) const
	{
		// Equivalente alla gluLookAt
		IDSReal3D x,y,z;

		z = -viewDir;
		y = up;
		x = crossDir; // right vector = y cross z


		m[ 0] = x[0];
		m[ 1] = y[0];
		m[ 2] = z[0];
		m[ 3] = 0.0f;
		
		m[ 4] = x[1];
		m[ 5] = y[1];
		m[ 6] = z[1];
		m[ 7] = 0.0f;

		m[ 8] = x[2];
		m[ 9] = y[2];
		m[10] = z[2];
		m[11] = 0.0f;

		m[12] = x * (-eye);
		m[13] = y * (-eye);
		m[14] = z * (-eye);
		m[15] = 1.0f;
	}

	IDSReal3D const &getEyePos()  const {return eye;}
	IDSReal3D const &getUp()      const {return up;}
	IDSReal3D const &getViewDir() const {return viewDir;}
	IDSReal3D const &getV()       const {return crossDir;}
	IDSReal3D const &getCenter()  const {return center;}
};

}}

#endif // CAMERA_H
