// ViewerController2DPan.cpp
// 
#include "ViewerController2DPan.h"

#include "Math/Math.h"

using namespace RenderPipeline;

// ------------------------------------------------------------------------------------------------
bool ViewerController2DPan::start()
{
	if( !canStart() )  return false;

		_dragInProgress = false;

	doStart();  return true;
}

// ------------------------------------------------------------------------------------------------
bool ViewerController2DPan::doDragBegin( const Math::ivec2 &screenPos )
{
	_dragInProgress = true;
	_prevScreenPos = screenPos;
	return true;
}

// ------------------------------------------------------------------------------------------------
bool ViewerController2DPan::doDragUpdate( const Math::ivec2 &screenPos )
{
	if( !_dragInProgress )
		return false;

	updatePos( screenPos );
	return true;
}

// ------------------------------------------------------------------------------------------------
bool ViewerController2DPan::doDragEnd( const Math::ivec2 &screenPos )
{
	if( !_dragInProgress )
		return false;

	updatePos( screenPos );
	doReleaseViewer( true );
	return true;
}

// ------------------------------------------------------------------------------------------------
inline void ViewerController2DPan::updatePos( const Math::ivec2 &screenPos )
{
	const auto view = viewer()->view();

	const Math::dvec2 curr( view.worldPoint( screenPos ) );
	const Math::dvec2 prev( view.worldPoint( _prevScreenPos ) );
	const Math::dvec3 diff = Math::dvec3( curr - prev, 0.0 );
	viewer()->moveToLookAt( viewer()->camera().target() - diff );

	_prevScreenPos = screenPos;
}	     