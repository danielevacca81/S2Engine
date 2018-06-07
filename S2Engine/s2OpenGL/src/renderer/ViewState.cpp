// ViewState.cpp
// 
#include "ViewState.h"

using namespace s2;
using namespace s2::OpenGL;

// -------------------------------------------------------------------------------------
double ViewState::pixelSize( const Math::dvec3 &p) const
{
	return 0.0;
	//switch (type())
	//{
	//case Camera::PERSPECTIVE :
	//	return 2.0 * fabs((frame()->coordinatesOf(position)).z) * tan(fieldOfView()/2.0) / screenHeight();
	//case Camera::ORTHOGRAPHIC :
	//	{
	//		GLdouble w, h;
	//		getOrthoWidthHeight(w,h);
	//		return 2.0 * h / screenHeight();
	//	}
	//}

}

// -------------------------------------------------------------------------------------
Math::dray  ViewState::rayAt( int pxlX, int pxlY ) const
{
	const Math::dvec3 tn(pxlX,pxlY,0);
	const Math::dvec3 tf(pxlX,pxlY,1.0);

	const Math::ivec4 vp( viewport.left(),viewport.bottom(),viewport.width(),viewport.height() );

	const Math::dvec3 n = Math::unProject( tn, modelViewMatrix(), projectionMatrix(), vp );
	const Math::dvec3 f = Math::unProject( tf, modelViewMatrix(), projectionMatrix(), vp );

	return Math::dray( view.eye(), Math::normalize(f-n) );
}

// -------------------------------------------------------------------------------------
Math::dvec3 ViewState::worldPoint( int pxlX, int pxlY )  const
{
	Math::dvec3 t(pxlX,pxlY,0);

	const Math::ivec4 vp( viewport.left(),viewport.bottom(),viewport.width(),viewport.height() );

	Math::dvec3 out = Math::unProject( t, modelViewMatrix(), projectionMatrix(), vp );

	return out;
}
