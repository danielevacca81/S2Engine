// Body.h
//
// Wraps a s2Entity into ODE Entity
#pragma once

#include	<ode/ode.h>
#include "../s2Game/entity/Entity.h"



namespace s2Wrap{
namespace	s2Physics{

class Body
{
protected:
	dBodyID body;
	dGeomID	geom;
	float		m[16];

public:
	~Body(){};

	float	*getMatrix()
	{
		//glPushMatrix();
		const dReal *p = dBodyGetPosition(body);
		const dReal *r = dBodyGetRotation(body);
	  	
		
		m[ 0] = r[ 0];m[ 1] = r[ 4];m[ 2] = r[ 8];m[ 3] = 0;
		m[ 4] = r[ 1];m[ 5] = r[ 5];m[ 6] = r[ 9];m[ 7] = 0;
		m[ 8] = r[ 2];m[ 9] = r[ 6];m[10] = r[10];m[11] = 0;
		m[12] = p[ 0];m[13] = p[ 1];m[14] = p[ 2];m[15] = 1;

		//glMultMatrixf(m);
		//draw();
		//glPopMatrix();
		return m;
	}

	void setPosition(float x,float y,float z)
	{
		dBodySetPosition(body,x,y,z);
	}
};


}
}