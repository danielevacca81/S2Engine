// ViewOrthographic.cpp
// 
#include "ViewOrthographic.h"

using namespace s2;
using namespace s2::SceneGraph;

// ------------------------------------------------------------------------------------------------
ViewOrthographic::ViewOrthographic()
: _nearPlane  ( 0.0  )
, _farPlane   ( 1.0  )
, _leftPlane  ( 0.0  )
, _rightPlane ( 1.0  )
, _bottomPlane( 0.0  )
, _topPlane   ( 1.0  )
{}

// ------------------------------------------------------------------------------------------------
ViewOrthographic::~ViewOrthographic()
{}

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