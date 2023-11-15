#include <math.h>
#include <memory.h>
#include "Matrix4x4.h"

Matrix4x4::Matrix4x4()
{
	SetIdentity();
}

Matrix4x4::Matrix4x4(const Matrix4x4& mat)
{
	memcpy( m_Mat, mat.m_Mat, 16*sizeof(float) );
}

Matrix4x4& Matrix4x4::operator=(const Matrix4x4& mat)
{
	memcpy( m_Mat, mat.m_Mat, 16*sizeof(float) );
	return *this;
}

Matrix4x4::~Matrix4x4()
{
}


void Matrix4x4::SetIdentity(void)
{
	m_Mat[0][0]= 1;
	m_Mat[1][0]= 0;
	m_Mat[2][0]= 0;
	m_Mat[3][0]= 0;
	m_Mat[0][1]= 0;
	m_Mat[1][1]= 1;
	m_Mat[2][1]= 0;
	m_Mat[3][1]= 0;
	m_Mat[0][2]= 0;
	m_Mat[1][2]= 0;
	m_Mat[2][2]= 1;
	m_Mat[3][2]= 0;
	m_Mat[0][3]= 0;
	m_Mat[1][3]= 0;
	m_Mat[2][3]= 0;
	m_Mat[3][3]= 1;
}

void Matrix4x4::SetPerspective(float ncp, float fcp, float fov, float aspect)
{
	float c,s,Q;
	
	c= (float) cos( 0.5f*RS_DEGTORAD(fov) );
	s= (float) sin( 0.5f*RS_DEGTORAD(fov) );
	
	Q= s/(1.0f-ncp/fcp);
	
	m_Mat[0][0]= c/(aspect*Q*ncp);
	m_Mat[1][0]= 0;
	m_Mat[2][0]= 0;
	m_Mat[3][0]= 0;
	m_Mat[0][1]= 0;
	m_Mat[1][1]= c/(Q*ncp);
	m_Mat[2][1]= 0;
	m_Mat[3][1]= 0;
	m_Mat[0][2]= 0;
	m_Mat[1][2]= 0;
	m_Mat[2][2]= -1/ncp;
	m_Mat[3][2]= -s/(Q*ncp);
	m_Mat[0][3]= 0;
	m_Mat[1][3]= 0;
	m_Mat[2][3]= -1;
	m_Mat[3][3]= 0;
}


void Matrix4x4::SetRotation(float angle, float x, float y, float z)
{
	float length;
	float c,s,t;
	float theta = RS_DEGTORAD(angle);
	
	// normalize
	//--
	length = sqrtf(x*x + y*y + z*z);
	
	// too close to 0, can't make a normalized vector
	//--
	if (length < 0.000001f)
		return;
	
	x /= length;
	y /= length;
	z /= length;
	
	// do the trig
	//--
	c = cosf(theta);
	s = sinf(theta);
	t = 1-c;   
	
	// build the rotation matrix
	//--
	m_Mat[0][0] = t*x*x + c;
	m_Mat[0][1] = t*x*y - s*z;
	m_Mat[0][2] = t*x*z + s*y;
	m_Mat[0][3] = 0;
	
	m_Mat[1][0] = t*x*y + s*z;
	m_Mat[1][1] = t*y*y + c;
	m_Mat[1][2] = t*y*z - s*x;
	m_Mat[1][3] = 0;
	
	m_Mat[2][0] = t*x*z - s*y;
	m_Mat[2][1] = t*y*z + s*x;
	m_Mat[2][2] = t*z*z + c;
	m_Mat[2][3] = 0;
	
	m_Mat[3][0] = 0;
	m_Mat[3][1] = 0;
	m_Mat[3][2] = 0;
	m_Mat[3][3] = 1;
}


void Matrix4x4::SetTranslation(float x, float y, float z)
{
	m_Mat[0][3] = x;
	m_Mat[1][3] = y;
	m_Mat[2][3] = z;
}


void Matrix4x4::SetScale(float x, float y, float z)
{
	m_Mat[0][0] = x;
	m_Mat[1][1] = y;
	m_Mat[2][2] = z;
}

void Matrix4x4::Multiply(const Matrix4x4& mat)
{
	Matrix4x4 res;
	int i,j,k;
	
	for (i=0; i<4; i++) 
	{
		for (j=0; j<4; j++) 
		{
			res.m_Mat[i][j] = 0.0;
			for (k=0; k<4; k++) 
			{
				res.m_Mat[i][j] += m_Mat[i][k]*mat.m_Mat[k][j];
			}
		}
	}

	*this = res;
}

Matrix4x4 Matrix4x4::GetTranspose(void) const
{
	Matrix4x4 rv;
	int i,j;
	
	for (i=0; i<4; i++) 
	{
		for (j=0; j<4; j++) 
		{
			rv.m_Mat[i][j]= m_Mat[j][i];
		}
	}

	return rv;
}
