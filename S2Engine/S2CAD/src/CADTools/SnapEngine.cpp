// SnapEngine.cpp
//
#include "SnapEngine.h"

#include "OpenGL/OpenGL.h"
#include "OpenGL/Renderer.h"


// ------------------------------------------------------------------------------------------------
SnapEngine::SnapEngine()
: _validHint(false)
{}

// ------------------------------------------------------------------------------------------------
SnapEngine::~SnapEngine()
{}

// ------------------------------------------------------------------------------------------------
void SnapEngine::addPoints( const std::vector<Math::dvec3> &points, unsigned int objID )
{
	for( size_t i=0; i<points.size(); ++i )
		_points.push_back( SnapPoint(points[i],objID) );
}

// ------------------------------------------------------------------------------------------------
void SnapEngine::addPoint( const Math::dvec3 &p, unsigned int objID )
{
	_points.push_back( SnapPoint(p,objID) );
}

// ------------------------------------------------------------------------------------------------
void SnapEngine::clear()
{
	_points.clear();
	_validHint = false;
}

// ------------------------------------------------------------------------------------------------
void SnapEngine::removePoints( unsigned int objID )
{
	for( SnapPoints::iterator it = _points.begin();
		 it != _points.end();
		 /**/ )
	{
		SnapPoints::iterator jt = it++;
		if( jt->objectID == objID )
			_points.erase(jt);
	}
}

// ------------------------------------------------------------------------------------------------
bool SnapEngine::hasPoints() const
{
	return !_points.empty();
}

// ------------------------------------------------------------------------------------------------
void SnapEngine::updateSnapHint( const Math::dvec3 &p, double threshold )
{
	_validHint = getClosestPoint( p,_hint,threshold );
}

// ------------------------------------------------------------------------------------------------
void SnapEngine::getHint( Math::dvec3 &hint ) const
{
	if(_validHint)
		hint = _hint;
}



// ------------------------------------------------------------------------------------------------
bool SnapEngine::getClosestPoint( const Math::dvec3 &p, Math::dvec3 &closest, double threshold )
{
	SnapPoint closestPoint;

	double minDistance = std::numeric_limits<double>::infinity();

	for( SnapPoints::iterator it = _points.begin();
		 it != _points.end();
		 ++it )
	{
		const double d = Math::distance(p,it->p);
		if( d < minDistance )
		{
			closestPoint = SnapPoint( *it );
			minDistance  = d;
		}
	}

	if( minDistance < threshold )
	{
		closest = closestPoint.p;
		return true;
	}

	closest = p;
	return false;
}

// ------------------------------------------------------------------------------------------------
std::vector<Math::dvec3> SnapEngine::pointsInRange( const Math::dvec3 &p, double range )
{
	std::vector<Math::dvec3> ret;

	for( SnapPoints::iterator it = _points.begin();
		 it != _points.end();
		 ++it )
	{
		const double d = Math::distance(p,it->p);
		if( d < range )
			ret.push_back( it->p );
	}

	return ret;
}

// ------------------------------------------------------------------------------------------------
void SnapEngine::draw( OpenGL::Renderer *r )
{
	if( _validHint )
	{
		// draw a small cross centererd at _hint coordinates
		const Math::dvec3 o = _hint;//-r->origin();
	
		const double crossSize = r->pixelToWorldSize(8); // half size in pixel

		std::vector<Math::dvec3> pts;
		pts.push_back( o + Math::dvec3(-crossSize, crossSize,0) );
		pts.push_back( o + Math::dvec3( crossSize,-crossSize,0) );
		pts.push_back( o + Math::dvec3(-crossSize,-crossSize,0) );
		pts.push_back( o + Math::dvec3( crossSize, crossSize,0) );

		glLineWidth(2);
		glColor4fv(_snapColor );
		r->beginRendering();
		r->setVertex3DArray( pts );
		
		glDrawArrays( GL_LINES, 0, pts.size() );

		r->endRendering();
	}
}

