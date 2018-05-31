// Box.h
//
#ifndef BOX_ONCE
#define BOX_ONCE

#include "Math.h"

namespace glm{

	class box3
	{
	private:
		dvec3 _minPoint;
		dvec3 _maxPoint;

	public:
		// -----------------------------------------------------------------------------------------------
		box3()									   : _minPoint(1.0),_maxPoint(-1.0)  {}
		box3(const dvec3 &minP, const dvec3 &maxP) : _minPoint(minP),_maxPoint(maxP) {}
		~box3(){};

		// -----------------------------------------------------------------------------------------------
		void reset() { _minPoint = Math::dvec3(1.0); _maxPoint = Math::dvec3(-1.0);}

		// -----------------------------------------------------------------------------------------------
		void setMinPoint( const dvec3 &p ) { _minPoint = p; }

		// -----------------------------------------------------------------------------------------------
		void setMaxPoint( const dvec3 &p ) { _maxPoint = p; }

		// -----------------------------------------------------------------------------------------------
		dvec3 minPoint() const { return _minPoint; }
		dvec3 maxPoint() const { return _maxPoint; }

		// -----------------------------------------------------------------------------------------------
		bool isNull() const
		{
			return _minPoint.x > _maxPoint.x || 
				   _minPoint.y > _maxPoint.y ||
				   _minPoint.z > _maxPoint.z;
		}

		// -----------------------------------------------------------------------------------------------
		bool isEmpty() const 
		{
			return _minPoint == _maxPoint;

			//const bool nan = any( isnan(_minPoint) ) | any( isnan(_maxPoint) );
			//const bool inf = any( isinf(_minPoint) ) | any( isinf(_maxPoint) );
			//const bool notValid  = (_minPoint.x > _maxPoint.x && _minPoint.y > _maxPoint.y && _minPoint.z > _maxPoint.z);	
			//const bool tooSmall  = false; // length(_maxPoint-_minPoint) <= tolerance;
			//return notValid | tooSmall | nan | inf;
		}

		// -----------------------------------------------------------------------------------------------
		dvec3 center() const {	return isNull() ? dvec3(0.0) : ((_minPoint+_maxPoint) * 0.5);	}

		// -----------------------------------------------------------------------------------------------
		double diag() const { return isNull() ? 0.0 : distance(_maxPoint,_minPoint); }

		// -----------------------------------------------------------------------------------------------
		box3 &extend(const dvec3 &point)
		{
			if( isNull() )
				_minPoint = _maxPoint = point;
			else
			{
				for (int i = 0; i < 3; i++)
				{
					if (_minPoint[i] > point[i]) _minPoint[i] = point[i];
					if (_maxPoint[i] < point[i]) _maxPoint[i] = point[i];
				}
			}
			return *this;
		}

		// -----------------------------------------------------------------------------------------------
		box3 &extend(const box3 &box)
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
				for (int i = 0; i < 3; i++)
				{
					if (_minPoint[i] > box._minPoint[i]) _minPoint[i] = box._minPoint[i];
					if (_maxPoint[i] < box._maxPoint[i]) _maxPoint[i] = box._maxPoint[i];
				}
			}
			return *this;
		}

		// -----------------------------------------------------------------------------------------------
		void transform( const dmat4 &m )
		{
			if( isNull() )
				return;

			dvec4 minP(_minPoint,1.0); minP = m*minP;
			dvec4 maxP(_maxPoint,1.0); maxP = m*maxP;

			_minPoint = dvec3(minP);
			_maxPoint = dvec3(maxP);
		}

		// -----------------------------------------------------------------------------------------------
		box3 transformed( const dmat4 &m ) const
		{
			if( isNull() )
				return *this;

			dvec4 minP(_minPoint,1.0); minP = m*minP;
			dvec4 maxP(_maxPoint,1.0); maxP = m*maxP;

			return box3( dvec3(minP),dvec3(maxP) );
		}

		// -----------------------------------------------------------------------------------------------
		void translate( const dvec3 &t )
		{
			if( isNull() )
				return;

			_minPoint += t;	_maxPoint += t;		
		}

		// -----------------------------------------------------------------------------------------------
		box3 translated( const dvec3 &t ) const
		{
			if( isNull() )
				return *this;

			return box3(_minPoint+t,_maxPoint+ t);
		}

		// -----------------------------------------------------------------------------------------------
		box3 rotated( double angle, double nx, double ny, double nz ) const
		{
			if( isNull() )
				return *this;
		
			const dmat4 r  = glm::rotate( dmat4(1.0), angle, dvec3(nx,ny,nz) );

			dvec4 p1(_minPoint,1.0);
			dvec4 p2(_maxPoint,1.0);

			p1 = r*p1; p2 = r*p2;

			return box3( dvec3(p1), dvec3(p2) );	
		}

		// -----------------------------------------------------------------------------------------------
		void rotate( double angle, double nx, double ny, double nz )
		{
			if( isNull() )
				return;

			dmat4 r  = glm::rotate( dmat4(1.0), angle, dvec3(nx,ny,nz) );

			dvec4 p1(_minPoint,1.0);
			dvec4 p2(_maxPoint,1.0);

			p1 = r*p1; p2 = r*p2;

			_minPoint = dvec3(p1);
			_maxPoint = dvec3(p2);
		}

		// -----------------------------------------------------------------------------------------------
		box3 scaled( double s ) const
		{
			if( isNull() )
				return *this;

			const dvec3 ss(s);
			const dvec3 c( center() );
			const dvec3 p0( (_minPoint-c)*s );
			const dvec3 p1( (_maxPoint-c)*s );

			return box3( p0+c,p1+c);
		}

		// -----------------------------------------------------------------------------------------------
		void scale( double s )
		{
			if( isNull() )
				return;

			const dvec3 c( center() );
			const dvec3 p0( (_minPoint-c)*s );
			const dvec3 p1( (_maxPoint-c)*s );

			_minPoint = p0+c;
			_maxPoint = p1+c;
		}

		// -----------------------------------------------------------------------------------------------
		void scale( double xs, double ys, double zs )
		{
			if( isNull() )
				return;

			const dvec3 s(xs,ys,zs);
			const dvec3 c( center() );
			const dvec3 p0( (_minPoint-c)*s );
			const dvec3 p1( (_maxPoint-c)*s );

			_minPoint = p0+c;
			_maxPoint = p1+c;
		}

		// -----------------------------------------------------------------------------------------------
		void scaleToFit( const box3 &target )
		{
			if( isNull() )
				return;

			const double w  =  sizes().x;
			const double h  =  sizes().y;
			const double tw =  target.sizes().x;
			const double th =  target.sizes().y;

			const double ar  = w/h;
			const double tAr = tw/th;

			// scale this box according to the aspect ratio
			if( tAr > ar ) scale(th/h);
			else           scale(tw/w);
		}


		// -----------------------------------------------------------------------------------------------
		static box3 createAABB( const box3 &b )
		{
			return createAABB( b._minPoint,b._maxPoint );
		}

		// -----------------------------------------------------------------------------------------------
		static box3 createAABB( const dvec3 &p0, const dvec3 &p1 )
		{
			box3 ret;

			ret._minPoint = p0;
			ret._maxPoint = p0;

			for( int i=0; i<3; ++i )
			{
				if( p0[i] < ret._minPoint[i] ) ret._minPoint[i] = p0[i];
				if( p1[i] < ret._minPoint[i] ) ret._minPoint[i] = p1[i];

				if( p0[i] > ret._maxPoint[i] ) ret._maxPoint[i] = p0[i];
				if( p1[i] > ret._maxPoint[i] ) ret._maxPoint[i] = p1[i];
			}

			return ret;
		}

		// -----------------------------------------------------------------------------------------------
		void setSizes( const dvec3 sizes )
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
		bool contains(const box3 & box) const
		{
			return contains(box.minPoint()) && contains(box.maxPoint());
		}

		// -----------------------------------------------------------------------------------------------
		bool contains2D(const box3 & box) const
		{
			return contains(dvec2(box.minPoint())) && contains(dvec2(box.maxPoint()));
		}


		// -----------------------------------------------------------------------------------------------
		bool contains(const dvec3 &point) const
		{
			if( isNull() )             return false;
			if (_minPoint.x > point.x) return false;
			if (_minPoint.y > point.y) return false;
			if (_minPoint.z > point.z) return false;
			if (_maxPoint.x < point.x) return false;
			if (_maxPoint.y < point.y) return false;
			if (_maxPoint.z < point.z) return false;
			return true;
		}

		// -----------------------------------------------------------------------------------------------
		bool contains(const dvec2 &point) const
		{
			if( isNull() )             return false;
			if (_minPoint.x > point.x) return false;
			if (_minPoint.y > point.y) return false;
			if (_maxPoint.x < point.x) return false;
			if (_maxPoint.y < point.y) return false;
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
		bool overlaps(const box3 &box, double tolerance = 1e-8) const
		{
			if( isNull() ) return false;

			for(int i = 0; i < 3; ++i)
			{
				if( _minPoint[i] - 2.0*tolerance > box._maxPoint[i] ) return false;
				if( _maxPoint[i] + 2.0*tolerance < box._minPoint[i] ) return false;
			}
			return true;
		}

		// -----------------------------------------------------------------------------------------------
		// Tests if the boxes (projected on plane z=0) overlap within the given tolerance.
		bool overlaps2D(const box3 &box, double tolerance = 1e-8) const
		{
			if( isNull() ) return false;

			for(int i = 0; i < 2; ++i)
			{
				if( _minPoint[i] - 2.0*tolerance > box._maxPoint[i] ) return false;
				if( _maxPoint[i] + 2.0*tolerance < box._minPoint[i] ) return false;
			}
			return true;
		}

		// -----------------------------------------------------------------------------------------------
		// Returns global coords of a local point expressed in [0..1]^3
		dvec3 localToGlobal( const Math::dvec3 &p ) const
		{
			if( isNull() )
				return p;

			return dvec3( 
				_minPoint.x + p.x * (_maxPoint.x-_minPoint.x), 
				_minPoint.y + p.y * (_maxPoint.y-_minPoint.y),
				_minPoint.z + p.z * (_maxPoint.z-_minPoint.z));
		}									 

		// -----------------------------------------------------------------------------------------------
		// Returns local coords expressed in [0..1]^3 of a point in 3D
		dvec3 globalToLocal( const dvec3 &p ) const
		{
			if( isNull() )
				return p;

			return dvec3( 
				(p.x-_minPoint.x)/(_maxPoint.x-_minPoint.x), 
				(p.y-_minPoint.y)/(_maxPoint.y-_minPoint.y), 
				(p.z-_minPoint.z)/(_maxPoint.z-_minPoint.z));
		}

		// -----------------------------------------------------------------------------------------------
		double volume() const
		{
			if( isNull() )
				return 0.0;

			return (_maxPoint.x-_minPoint.x) * 
				(_maxPoint.y-_minPoint.y) *
				(_maxPoint.z-_minPoint.z) ;
		}

		// -----------------------------------------------------------------------------------------------
		inline dvec3 sizes()  const	{ return _maxPoint-_minPoint; }
		inline double sizeX() const { return _maxPoint.x-_minPoint.x; }
		inline double sizeY() const { return _maxPoint.y-_minPoint.y; }
		inline double sizeZ() const { return _maxPoint.z-_minPoint.z; }

		// -----------------------------------------------------------------------------------------------
		inline int sizeMax()  const
		{
			if( isNull() )
				return -1;

			const dvec3 diag = _maxPoint-_minPoint;
			const int i      = diag.x>diag.y ? 0 : 1;
			return          (diag[i]>diag.z) ? i : 2;
		}

		// -----------------------------------------------------------------------------------------------
		inline int sizeMin()  const
		{
			if( isNull() )
				return -1;

			const dvec3 diag = _maxPoint-_minPoint;
			const int i      = diag.x<diag.y ? 0 : 1;
			return          (diag[i]<diag.z) ? i : 2;
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
		dvec3 corner( const int &i ) const
		{
			return dvec3(
				_minPoint.x + (i%2)       * sizeX(),
				_minPoint.y + ((i / 2)%2) * sizeY(),
				_minPoint.z + (i>3)       * sizeZ());
		}

	};

}

#endif