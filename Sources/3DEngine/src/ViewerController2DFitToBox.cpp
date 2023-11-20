// ViewerController2DFitToBox.cpp
// 
#include "ViewerController2DFitToBox.h"

using namespace Scene;

// ------------------------------------------------------------------------------------------------
bool ViewerController2DFitToBox::start( const Math::box3 &b )
{
	if( !canStart() )  return false;

	_box = b;
	viewer()->fitToBox( _box ); 

	doStart();  return true;
}

// ------------------------------------------------------------------------------------------------
bool ViewerController2DFitToBox::updateBox( const Math::box3 &box )
{
	if( status() != Status::InProgress )
		return false;

	if( _box.equals( box ) )
		return true;
	
	_box = box;
	viewer()->fitToBox( _box ); 
	return true;
}

// ------------------------------------------------------------------------------------------------
bool ViewerController2DFitToBox::doResize( int w, int h ) 
{
	if( _box.isEmpty() )
		return false;

	viewer()->setViewport( Math::Rectangle( 0, 0, w, h ) );
	viewer()->fitToBox( _box );
	return true;
}









				     