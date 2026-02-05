// ViewerController3DAdaptToBox.h
//
#ifndef RENDERENGINE_VIEWERCONTROLLER3DADAPTTOBOX_H
#define RENDERENGINE_VIEWERCONTROLLER3DADAPTTOBOX_H

#include "RenderPipeline_API.h"

#include "ViewerController.h"

namespace RenderPipeline {

// ------------------------------------------------------------------------------------------------
class RENDERPIPELINE_API ViewerController3DAdaptToBox : public ViewerController3D
{
public:
	bool    start( const double fov, const Viewer3D::FovId fovId,
				   const Math::dbox3 &b, const Viewer3D::BoxAdaptationPolicy adaptPolicy );

	bool    doResize( int w, int h ) override;


protected:
	double			              _fov;
	Viewer3D::FovId			      _fovId;
	Math::dbox3                   _box;
	Viewer3D::BoxAdaptationPolicy _boxAdaptPolicy;
};


}

#endif // RENDERENGINE_VIEWERCONTROLLER3DADAPTTOBOX_H



