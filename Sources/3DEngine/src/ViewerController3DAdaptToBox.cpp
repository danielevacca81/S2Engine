// ViewerController3DAdaptToBox.cpp
// 
#include "ViewerController3DAdaptToBox.h"

using namespace Scene;

// ------------------------------------------------------------------------------------------------
bool ViewerController3DAdaptToBox::start( const double fov, const Viewer3D::FovId fovId,
										  const Math::box3 &b, const Viewer3D::BoxAdaptationPolicy adaptPolicy )
{
	if( !canStart() )  return false;

	_fov            = fov;
	_fovId          = fovId;
	_box            = b;
	_boxAdaptPolicy = adaptPolicy;

	viewer()->setCameraTopDownAdaptToBox( _box, _boxAdaptPolicy );

	doStart();  return true;
}

// ------------------------------------------------------------------------------------------------
bool ViewerController3DAdaptToBox::doResize( int w, int h ) 
{
	viewer()->setViewportAndFov( w, h, _fovId, _fov );
	viewer()->setCameraTopDownAdaptToBox( _box, _boxAdaptPolicy );
	return true;
}






				     