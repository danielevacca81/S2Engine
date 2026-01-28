// ViewerController3DPan.h
//
#ifndef RENDERENGINE_VIEWERCONTROLLER3DPAN_H
#define RENDERENGINE_VIEWERCONTROLLER3DPAN_H

#include "RenderPipeline_API.h"

#include "ViewerController.h"

namespace RenderPipeline {

// ------------------------------------------------------------------------------------------------
class RENDERPIPELINE_API ViewerController3DPan : public ViewerController3D
{
public:
	bool    start(); 

	bool    doDragBegin ( const Math::ivec2 &screenPos )  override;
	bool    doDragUpdate( const Math::ivec2 &screenPos )  override;
	bool    doDragEnd   ( const Math::ivec2 &screenPos )  override;

protected:
	bool    updatePos( const Math::ivec2 &screenPos );

protected:
	bool         _dragInProgress = false;
	Math::ivec2  _screenPickPos;
	Math::dvec3  _pickWorldPos;
};


}

#endif // RENDERENGINE_VIEWERCONTROLLER3DPAN_H



