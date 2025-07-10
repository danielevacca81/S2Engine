// ViewOrthographic.cpp
// 
#include "ViewOrthographic.h"

using namespace glm;

// ------------------------------------------------------------------------------------------------
ViewOrthographic::ViewOrthographic()
: ViewOrthographic( 0.0, 1.0, 0.0, 1.0, 0.0, 1.0 )
{}

// ------------------------------------------------------------------------------------------------
ViewOrthographic::ViewOrthographic( double left, double right, double bottom, double top, double nearPlane, double farPlane )
{
	set( left, right, bottom, top, nearPlane, farPlane );
}

// ------------------------------------------------------------------------------------------------
ViewOrthographic::ViewOrthographic( int w, int h, double nearPlane, double farPlane, double zoomFactor )
{
	set( w, h, nearPlane, farPlane, zoomFactor );
}

// ------------------------------------------------------------------------------------------------
Math::dmat4 ViewOrthographic::matrix()      const { return _matrix; }
double      ViewOrthographic::nearPlane()   const { return _nearPlane;   }
double      ViewOrthographic::farPlane()    const { return _farPlane;    }
double      ViewOrthographic::leftPlane()   const { return _leftPlane;   }
double      ViewOrthographic::rightPlane()  const { return _rightPlane;  }
double      ViewOrthographic::bottomPlane() const { return _bottomPlane; }
double      ViewOrthographic::topPlane()    const { return _topPlane;    }

// ------------------------------------------------------------------------------------------------
void ViewOrthographic::set( double left, double right, double bottom, double top, double nearPlane, double farPlane ) 
{ 
	_leftPlane   = left;
	_rightPlane  = right;
	_bottomPlane = bottom;
	_topPlane    = top;
	_nearPlane   = nearPlane;
	_farPlane    = farPlane;

	_matrix = Math::ortho( left, right, bottom, top, nearPlane, farPlane );
}

// ------------------------------------------------------------------------------------------------
void ViewOrthographic::set( int w, int h, double nearPlane, double farPlane, double zoomFactor )
{
	assert(zoomFactor != 0.0);

	const double aspect = (h == 0 || w == 0) ? 1.0 : (w / (double)h);
	const double a      = 1.0 / zoomFactor;
	const double b      = ( a / aspect );

	_leftPlane   = -a;
	_rightPlane  = +a;
	_bottomPlane = -b;
	_topPlane    = +b;
	_nearPlane   = nearPlane;
	_farPlane    = farPlane;

	/*                                 *\
	 	  2                      R+L
		-----     0      0    - -----
		 R-L                     R-L

	              2              R+L
		  0     -----    0    - -----
				 T-B    	     R-L

                        -2       F+N
		  0       0    -----  - -----
					    F-N      F-N
			   		
					       
          0       0      0        1
	\*                                 */
	_matrix = Math::ortho( _leftPlane, _rightPlane, _bottomPlane, _topPlane, _nearPlane, _farPlane );
}