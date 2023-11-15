// Box.h
//
#ifndef MATH_BOX_ONCE
#define MATH_BOX_ONCE

#include "Math.h"

namespace glm {
namespace detail {

template<typename T, precision P>
class tbox3
{
public:
	// -----------------------------------------------------------------------------------------------
	tbox3() = default;
	tbox3( const tvec3<T,P>& minP, const tvec3<T,P>& maxP ) : _minPoint( minP ), _maxPoint( maxP ) {}

	// -----------------------------------------------------------------------------------------------
	void setMinPoint( const tvec3<T,P>& p ) { _minPoint = p; }

	// -----------------------------------------------------------------------------------------------
	void setMaxPoint( const tvec3<T,P>& p ) { _maxPoint = p; }

	// -----------------------------------------------------------------------------------------------
	tvec3<T,P> minPoint() const { return _minPoint; }
	tvec3<T,P> maxPoint() const { return _maxPoint; }

	// -----------------------------------------------------------------------------------------------
	bool isNull() const
	{
		return _minPoint.x > _maxPoint.x ||
			_minPoint.y > _maxPoint.y ||
			_minPoint.z > _maxPoint.z;
	}

	// -----------------------------------------------------------------------------------------------
	bool isEmpty() const { return _minPoint == _maxPoint; }

	// -----------------------------------------------------------------------------------------------
	tvec3<T,P> center() const { return isNull() ? tvec3<T,P>( 0 ) : ( ( _minPoint + _maxPoint ) * 0.5 ); }

	// -----------------------------------------------------------------------------------------------
	T diag() const { return isNull() ? 0 : distance( _maxPoint, _minPoint ); }

	// -----------------------------------------------------------------------------------------------
	tbox3<T,P>& extend( const tvec3<T,P>& point )
	{
		if( isNull() )
			_minPoint = _maxPoint = point;
		else
		{
			for( int i = 0; i < 3; i++ )
			{
				if( _minPoint[i] > point[i] ) _minPoint[i] = point[i];
				if( _maxPoint[i] < point[i] ) _maxPoint[i] = point[i];
			}
		}
		return *this;
	}

	// -----------------------------------------------------------------------------------------------
	tbox3<T,P>& extend( const tbox3<T, P>& box )
	{
		// leave this unchanged
		if( box.isNull() )
			return *this;

		if( isNull() )
		{
			_minPoint = box._minPoint;
			_maxPoint = box._maxPoint;
		}
		else
		{
			for( int i = 0; i < 3; i++ )
			{
				if( _minPoint[i] > box._minPoint[i] ) _minPoint[i] = box._minPoint[i];
				if( _maxPoint[i] < box._maxPoint[i] ) _maxPoint[i] = box._maxPoint[i];
			}
		}
		return *this;
	}

	// -----------------------------------------------------------------------------------------------
	void transform( const tmat4x4<T>& m )
	{
		if( isNull() )
			return;

		tvec4<T> minP( _minPoint, 1 ); minP = m * minP;
		tvec4<T> maxP( _maxPoint, 1 ); maxP = m * maxP;

		_minPoint = tvec3<T,P>( minP );
		_maxPoint = tvec3<T,P>( maxP );
	}

	// -----------------------------------------------------------------------------------------------
	tbox3<T, P> transformed( const tmat4x4<T>& m ) const
	{
		if( isNull() )
			return *this;

		tvec4<T> minP( _minPoint, 1.0 ); minP = m * minP;
		tvec4<T> maxP( _maxPoint, 1.0 ); maxP = m * maxP;

		return tbox3<T, P>( tvec3<T,P>( minP ), tvec3<T,P>( maxP ) );
	}

	// -----------------------------------------------------------------------------------------------
	void translate( const tvec3<T,P>& t )
	{
		if( isNull() )
			return;

		_minPoint += t;	_maxPoint += t;
	}

	// -----------------------------------------------------------------------------------------------
	tbox3<T, P> translated( const tvec3<T,P>& t ) const
	{
		if( isNull() )
			return *this;

		return tbox3<T>( _minPoint + t, _maxPoint + t );
	}

	// -----------------------------------------------------------------------------------------------
	tbox3<T, P> rotated( double angle, double nx, double ny, double nz ) const
	{
		if( isNull() )
			return *this;

		const tmat4x4<T> r = glm::rotate( tmat4x4<T,P>( 1 ), angle, tvec3<T,P>( nx, ny, nz ) );

		tvec4<T> p1( _minPoint, 1 );
		tvec4<T> p2( _maxPoint, 1 );

		p1 = r * p1; p2 = r * p2;

		return tbox3<T>( tvec3<T,P>( p1 ), tvec3<T,P>( p2 ) );
	}

	// -----------------------------------------------------------------------------------------------
	void rotate( double angle, double nx, double ny, double nz )
	{
		if( isNull() )
			return;

		tmat4x4<T> r = glm::rotate( tmat4x4<T,P>( 1.0 ), angle, tvec3<T,P>( nx, ny, nz ) );

		tvec4<T> p1( _minPoint, 1.0 );
		tvec4<T> p2( _maxPoint, 1.0 );

		p1 = r * p1; p2 = r * p2;

		_minPoint = tvec3<T,P>( p1 );
		_maxPoint = tvec3<T,P>( p2 );
	}

	// -----------------------------------------------------------------------------------------------
	tbox3<T, P> scaled( T s ) const
	{
		if( isNull() )
			return *this;

		const tvec3<T,P> ss( s );
		const tvec3<T,P> c( center() );
		const tvec3<T,P> p0( ( _minPoint - c ) * s );
		const tvec3<T,P> p1( ( _maxPoint - c ) * s );

		return tbox3<T,P>( p0 + c, p1 + c );
	}

	// -----------------------------------------------------------------------------------------------
	void scale( double s )
	{
		if( isNull() )
			return;

		const tvec3<T,P> c( center() );
		const tvec3<T,P> p0( ( _minPoint - c ) * s );
		const tvec3<T,P> p1( ( _maxPoint - c ) * s );

		_minPoint = p0 + c;
		_maxPoint = p1 + c;
	}

	// -----------------------------------------------------------------------------------------------
	void scale( double xs, double ys, double zs )
	{
		if( isNull() )
			return;

		const tvec3<T,P> s( xs, ys, zs );
		const tvec3<T,P> c( center() );
		const tvec3<T,P> p0( ( _minPoint - c ) * s );
		const tvec3<T,P> p1( ( _maxPoint - c ) * s );

		_minPoint = p0 + c;
		_maxPoint = p1 + c;
	}

	// -----------------------------------------------------------------------------------------------
	void scaleToFit( const tbox3<T, P>& target )
	{
		if( isNull() )
			return;

		const double w = sizes().x;
		const double h = sizes().y;
		const double tw = target.sizes().x;
		const double th = target.sizes().y;

		const double ar = w / h;
		const double tAr = tw / th;

		// scale this box according to the aspect ratio
		if( tAr > ar ) scale( th / h );
		else           scale( tw / w );
	}


	// -----------------------------------------------------------------------------------------------
	static tbox3<T, P> createAABB( const tbox3<T, P>& b )
	{
		return createAABB( b._minPoint, b._maxPoint );
	}

	// -----------------------------------------------------------------------------------------------
	static tbox3<T, P> createAABB( const tvec3<T,P>& p0, const tvec3<T,P>& p1 )
	{
		tbox3<T, P> ret;

		ret._minPoint = p0;
		ret._maxPoint = p0;

		for( int i = 0; i < 3; ++i )
		{
			if( p0[i] < ret._minPoint[i] ) ret._minPoint[i] = p0[i];
			if( p1[i] < ret._minPoint[i] ) ret._minPoint[i] = p1[i];

			if( p0[i] > ret._maxPoint[i] ) ret._maxPoint[i] = p0[i];
			if( p1[i] > ret._maxPoint[i] ) ret._maxPoint[i] = p1[i];
		}

		return ret;
	}

	// -----------------------------------------------------------------------------------------------
	void setSizes( const tvec3<T,P> sizes )
	{
		_maxPoint = _minPoint + sizes;
	}

	//// -----------------------------------------------------------------------------------------------
	//box3 &scale( const dvec3 &t )
	//{
	//	return *this;
	//}

	//// -----------------------------------------------------------------------------------------------
	//box3 &rotate( double angle, const dvec3 &axis )
	//{
	//	mat4 r  = glm::rotate( mat4(1.0), angle, axis );
	//	
	//	dvec4 p1(_minPoint,1.0);
	//	dvec4 p2(_maxPoint,1.0);

	//	p1 = r*p1; p2 = r*p2;
	//	
	//	_minPoint = dvec3(p1);
	//	_maxPoint = dvec3(p2);
	//	return *this;
	//}

	// -----------------------------------------------------------------------------------------------
	bool contains( const tbox3<T, P>& box ) const
	{
		return contains( box.minPoint() ) && contains( box.maxPoint() );
	}

	// -----------------------------------------------------------------------------------------------
	bool contains2D( const tbox3<T, P>& box ) const
	{
		return contains( tvec2<T, P>( box.minPoint() ) ) && contains( tvec2<T, P>( box.maxPoint() ) );
	}


	// -----------------------------------------------------------------------------------------------
	bool contains( const tvec3<T,P>& point ) const
	{
		if( isNull() )              return false;
		if( _minPoint.x > point.x ) return false;
		if( _minPoint.y > point.y ) return false;
		if( _minPoint.z > point.z ) return false;
		if( _maxPoint.x < point.x ) return false;
		if( _maxPoint.y < point.y ) return false;
		if( _maxPoint.z < point.z ) return false;
		return true;
	}

	// -----------------------------------------------------------------------------------------------
	bool contains( const tvec2<T>& point ) const
	{
		if( isNull() )              return false;
		if( isNull() )              return false;
		if( _minPoint.x > point.x ) return false;
		if( _minPoint.y > point.y ) return false;
		if( _maxPoint.x < point.x ) return false;
		if( _maxPoint.y < point.y ) return false;
		return true;
	}

	///// Tests if the 2D projection of the box contains the 2D projection of the given point, within the given tolerance.
	//bool contains2D(const IDSReal3D &point, REAL tolerance = 0.) const {
	//	for (int i = 0; i < 2; i++) {
	//		if (minCoord[i] - tolerance > point[i]) return false;
	//		if (maxCoord[i] + tolerance < point[i]) return false;
	//	}
	//	return true;
	//}

	// -----------------------------------------------------------------------------------------------
	bool overlaps( const tbox3<T, P>& box, double tolerance = 1e-8 ) const
	{
		if( isNull() ) return false;

		for( int i = 0; i < 3; ++i )
		{
			if( _minPoint[i] - 2.0 * tolerance > box._maxPoint[i] ) return false;
			if( _maxPoint[i] + 2.0 * tolerance < box._minPoint[i] ) return false;
		}
		return true;
	}

	// -----------------------------------------------------------------------------------------------
	// Tests if the boxes (projected on plane z=0) overlap within the given tolerance.
	bool overlaps2D( const tbox3<T, P>& box, double tolerance = 1e-8 ) const
	{
		if( isNull() ) return false;

		for( int i = 0; i < 2; ++i )
		{
			if( _minPoint[i] - 2.0 * tolerance > box._maxPoint[i] ) return false;
			if( _maxPoint[i] + 2.0 * tolerance < box._minPoint[i] ) return false;
		}
		return true;
	}

	// -----------------------------------------------------------------------------------------------
	// Returns global coords of a local point expressed in [0..1]^3
	tvec3<T,P> localToGlobal( const tvec3<T,P>& p ) const
	{
		if( isNull() )
			return p;

		return tvec3<T,P>(
			_minPoint.x + p.x * ( _maxPoint.x - _minPoint.x ),
			_minPoint.y + p.y * ( _maxPoint.y - _minPoint.y ),
			_minPoint.z + p.z * ( _maxPoint.z - _minPoint.z ) );
	}

	// -----------------------------------------------------------------------------------------------
	// Returns local coords expressed in [0..1]^3 of a point in 3D
	tvec3<T,P> globalToLocal( const tvec3<T,P>& p ) const
	{
		if( isNull() )
			return p;

		return tvec3<T,P>(
			( p.x - _minPoint.x ) / ( _maxPoint.x - _minPoint.x ),
			( p.y - _minPoint.y ) / ( _maxPoint.y - _minPoint.y ),
			( p.z - _minPoint.z ) / ( _maxPoint.z - _minPoint.z ) );
	}

	// -----------------------------------------------------------------------------------------------
	double volume() const
	{
		if( isNull() )
			return 0.0;

		return ( _maxPoint.x - _minPoint.x ) *
			( _maxPoint.y - _minPoint.y ) *
			( _maxPoint.z - _minPoint.z );
	}

	// -----------------------------------------------------------------------------------------------
	inline tvec3<T,P> sizes() const { return _maxPoint - _minPoint; }
	inline T       sizeX()  const { return _maxPoint.x - _minPoint.x; }
	inline T       sizeY()  const { return _maxPoint.y - _minPoint.y; }
	inline T       sizeZ()  const { return _maxPoint.z - _minPoint.z; }

	// -----------------------------------------------------------------------------------------------
	inline int sizeMax()  const
	{
		if( isNull() )
			return -1;

		const tvec3<T,P> diag = _maxPoint - _minPoint;
		const int i = diag.x > diag.y ? 0 : 1;
		return          ( diag[i] > diag.z ) ? i : 2;
	}

	// -----------------------------------------------------------------------------------------------
	inline int sizeMin()  const
	{
		if( isNull() )
			return -1;

		const tvec3<T,P> diag = _maxPoint - _minPoint;
		const int i = diag.x < diag.y ? 0 : 1;
		return          ( diag[i] < diag.z ) ? i : 2;
	}

	// -----------------------------------------------------------------------------------------------
	// gives the ith box vertex in order:
	// (x,y,z)
	// (X,y,z)
	// (x,Y,z)
	// (X,Y,z)
	// (x,y,Z)
	// (X,y,Z)
	// (x,Y,Z)
	// (X,Y,Z)
	tvec3<T,P> corner( const int& i ) const
	{
		return tvec3<T,P>(
			_minPoint.x + ( i % 2 ) * sizeX(),
			_minPoint.y + ( ( i / 2 ) % 2 ) * sizeY(),
			_minPoint.z + ( i > 3 ) * sizeZ() );
	}

private:
	tvec3<T,P> _minPoint;
	tvec3<T,P> _maxPoint;
};
}

typedef detail::tbox3<float, defaultp>  fbox3;
typedef detail::tbox3<double, defaultp> dbox3;


}

#endif