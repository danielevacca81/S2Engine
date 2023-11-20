// ViewerController3DAdaptToBox.h
//
#ifndef VIEWERCONTROLLER3DADAPTTOBOX_ONCE
#define VIEWERCONTROLLER3DADAPTTOBOX_ONCE

#include "s2Scene_API.h"

#include "ViewerController.h"

namespace Scene {

// ------------------------------------------------------------------------------------------------
class S2SCENE_API ViewerController3DAdaptToBox : public ViewerController3D
{
public:
	bool    start( const double fov, const Viewer3D::FovId fovId,
				   const Math::box3 &b, const Viewer3D::BoxAdaptationPolicy adaptPolicy );

	bool    doResize( int w, int h ) override;


protected:
	double			                 _fov;
	Viewer3D::FovId			         _fovId;
	Math::box3			             _box;
	Viewer3D::BoxAdaptationPolicy	 _boxAdaptPolicy;
};


}

#endif // VIEWERCONTROLLER3DADAPTTOBOX_ONCE



