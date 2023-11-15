#ifndef _MatrixStack_h_
#define _MatrixStack_h_

#include "Matrix4x4.h"
#include <list>

class MatrixStack
{
private:
	// The stack, represented by a STL container: list
	//--
	std::list< Matrix4x4 > stack;

public:	
	// Constructor
	//--
	MatrixStack() {
		// Empty the stack
		//--
		stack.clear();
	}

	// Destruction
	//--
	~MatrixStack() {
		stack.clear();
	}

	// Push currentMatrix on the stack
	//--
	void push( const Matrix4x4& m ) {
		stack.push_back( m );
	}

	// Remove the top-most matrix from the stack
	//--
	void pop() {
		stack.pop_back();
	}

	// Get top most matrix
	//--
	const Matrix4x4& getCurrent() const {
		return stack.back();
	}
};

#endif





























//////////////////////////////////////////////////////////////////////////////////
#if 0
class Matrix4x4 {
public:
	
	Matrix4x4 ();
	~Matrix();
	Matrix4x4 (Double a11, Double a12, Double a13, Double a14,
				  Double a21, Double a22, Double a23, Double a24,
				  Double a31, Double a32, Double a33, Double a34,
				  Double a41, Double a42, Double a43, Double a44  );
	Matrix4x4 (const Matrix4x4 &m );

	void setValue ( const Matrix4x4 &m );
	void getValue ( Matrix4x4 &m ) const;
	const Matrix4x4& getValue () const;

	void makeIdentity ();
	static Matrix4x4 identity ();

	void setRotate (const RotationEuler &q );
	void setRotate (const Quaternion &q );
	void setScale ( Double s );
	void setScale ( const Vec3f &s );
	void setTranslate ( const Vec3f &t );
	Matrix4x4 inverse (void) const;
	Matrix4x4 transpose (void) const;
	Matrix4x4& multRight ( const Matrix4x4 &m );
	Matrix4x4& multLeft ( const Matrix4x4 &m );
	void multMatrixVec( const Vec3f &src, Vec3f &dst ) const; 
	void multVecMatrix( const Vec3f &src, Vec3f &dst ) const; 
	void multDirMatrix( const Vec3f &src, Vec3f &dst ) const; 
	void multLineMatrix( const Line &src, Line &dst ) const; 

	void print(void) const;

	operator Double*(); 
	operator Matrix4x4&(); 

	Double* operator [] ( int i ); 
	const Double* operator[] ( int i ) const; 

	Matrix4x4& operator = ( const Matrix4x4 &m ); 
	Matrix4x4& operator = ( const Matrix4x4 &m ); 
	Matrix4x4& operator = ( const RotationXYZ &q ); 
	Matrix4x4& operator = ( const RotationQuat &q ); 

	Matrix4x4& operator *= ( const Matrix4x4 &m ); 
	friend Matrix4x4 operator *( const Matrix4x4 &m1, const Matrix4x4 &m2 );
	friend bool operator == ( const Matrix4x4 & m1, const Matrix4x4 &m2 ); 
	friend bool operator != ( const Matrix4x4 & m1, const Matrix4x4 &m2 ); 
	bool equals( const Matrix4x4 &m, Double tolerance ) const; 

	Double _mat[4][4];
};
void Matrix4x4::setRotation(const RotationEuler &euler )
{
	float cosroll,sinroll,cospitch,sinpitch,cosyaw,sinyaw;
	float sinyawcosroll,sinyawsinroll;

	/* Convert to radians */
	float roll,pitch,yaw;
	euler.getRPY(roll,pitch,yaw);
	roll*=_PI_OVER_180;
	pitch*=_PI_OVER_180;
	yaw*=_PI_OVER_180;

	/* Calculate trig functions */
	cosroll=cos(euler.roll());
	sinroll=sin(euler.roll());
	cospitch=cos(euler.pitch());
	sinpitch=sin(euler.pitch());
	cosyaw=cos(euler.yaw());
	sinyaw=sin(euler.yaw());
	sinyawcosroll=sinyaw*cosroll;
	sinyawsinroll=sinyaw*sinroll;

	/* Form the matrix */

	_mat[0][0] = cosyaw*cosroll; //XX
	_mat[0][1] = sinpitch*sinyawcosroll - cospitch*sinroll; //YX
	_mat[0][2] = cospitch*sinyawcosroll + sinpitch*sinroll; //ZX

	_mat[1][0] = cosyaw*sinroll; //XY
	_mat[1][1] = sinpitch*sinyawsinroll + cospitch*cosroll; //YY
	_mat[1][2] = cospitch*sinyawsinroll - sinpitch*cosroll; //ZY

	_mat[2][0] = -sinyaw; //XZ
	_mat[2][1] = cosyaw*sinpitch; //YZ
	_mat[2][2] = cosyaw*cospitch; //ZZ
}
void Matrix4x4::setRotate(const Quaternion &q )
{
	float sqw = q._angle*q._angle;
	float sqx = q._x*q._x;
	float sqy = q._y*q._y;
	float sqz = q._z*q._z;

	float heading = atan(2.0*
		                    ((q._x*q._y + q._z*q._angle)/(sqx-sqy-sqz+sqw)));

	float bank    = atan(2.0*
		                    ((q._y*q._z + q._x*q._angle)/(-sqx-sqy+sqz+sqw)));

	float attitude = asin(-2.0*(q._x*q._z + q._y*q._angle));

	RotationEuler rot(heading,bank,attitude);
	setRotation(rot);
}

#endif
