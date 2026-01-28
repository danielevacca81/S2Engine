// ViewerController3DPan.cpp
// 
#include "ViewerController3DPan.h"

#include "Math/Math.h"


using namespace RenderPipeline;

// ------------------------------------------------------------------------------------------------
bool ViewerController3DPan::start()
{
	if( !canStart() )  return false;

	_dragInProgress = false;

	doStart();  return true;
}

// ------------------------------------------------------------------------------------------------
bool ViewerController3DPan::doDragBegin( const Math::ivec2 &screenPos )
{
	_screenPickPos = screenPos;
	_dragInProgress = viewer()->depth0PlaneAtScreenPos( screenPos, _pickWorldPos );

	if( !_dragInProgress )
		doReleaseViewer( false );

	return _dragInProgress;
}

// ------------------------------------------------------------------------------------------------
bool ViewerController3DPan::doDragUpdate( const Math::ivec2 &screenPos )
{
	if( !_dragInProgress )
		return false;

	updatePos( screenPos );
	return true;
}

// ------------------------------------------------------------------------------------------------
bool ViewerController3DPan::doDragEnd( const Math::ivec2 &screenPos )
{
	if( !_dragInProgress )
		return false;

	updatePos( screenPos );
	doReleaseViewer( true );
	return true;
}

// ------------------------------------------------------------------------------------------------
inline bool ViewerController3DPan::updatePos( const Math::ivec2 &screenPos )
{
	assert( viewer() );
	assert( _dragInProgress );

	Math::dvec3 pickWorldPos;
	if( ! viewer()->depth0PlaneAtScreenPos( screenPos, pickWorldPos ) )
		return false;

	const Math::dvec3 diff = pickWorldPos - _pickWorldPos;

	auto camera = viewer()->camera();
	camera.set( camera.position() - diff, camera.orientation() );
	viewer()->setCamera( camera );
	return true;
}