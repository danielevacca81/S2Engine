// CadToolPolygon.cpp
//
#include "CadToolPolygon.h"

#include "CadEngine.h"

#include "Math/Box.h"
#include "OpenGL/OpenGL.h"
#include "OpenGL/Renderer.h"

//-------------------------------------------------------------------------------------------------
CadToolPolygon::CadToolPolygon()
: CadTool("Polygon",1)
{
	//_chained       = true;
	_creationColor = OpenGL::Color::red().lighter();
}

//-------------------------------------------------------------------------------------------------
CadToolPolygon::~CadToolPolygon()
{}

//-------------------------------------------------------------------------------------------------
void CadToolPolygon::execute()
{
	// we need at least three points
	if( collectedPointCount() > 2 )
	{
		CadEngine::Command c;

		c.target     = _target;
		c.command    = CadEngine::Create;
		c.primitive  = CadEngine::Polygon;
		c.color      = _manager->settings().shapeStyle();
		c.attributes = attributes();
		c.points.insert( c.points.end(),_points.begin(),_points.end()-1 );

		//if( _editTarget >= 0 )
		//{
		//	c.command=	CadEngine::Edit;
		//	c.target=	_editTarget;
		//}
		//else
		//{
		//	c.command   = CadEngine::Create;
		//}

		//c.primitive  = CadEngine::Polygon;
		//c.color      = _creationColor;
		//c.attributes = attributes();
		//for( int i   =0; i<_nPoints-1; ++i )
		//	c.points.push_back( _points[i] );

		_manager->issueCommand(c);
	}
}

// --------------------------------------------------------------------------------------------
void CadToolPolygon::reset()
{
	_totPoints = 1;

	CadTool::reset();
	setWaitConfirm(true);

	//_chained = true;
}

// --------------------------------------------------------------------------------------------
void CadToolPolygon::setCreationColor( const OpenGL::Color &c ) { _creationColor = c; }

// --------------------------------------------------------------------------------------------
OpenGL::Color CadToolPolygon::creationColor() const { return _creationColor; }

//-------------------------------------------------------------------------------------------------
void CadToolPolygon::draw( OpenGL::Renderer *r )
{
	glPushAttrib( GL_LINE_BIT|GL_ENABLE_BIT|GL_POINT_BIT );
	applyStyle();

	if( _nPoints > 1 )
	{
		r->beginRendering();
		r->setVertex3DArray( _points );

		glDrawArrays( _nPoints>2 ? GL_LINE_LOOP : GL_LINE_STRIP,0,_nPoints );

		r->endRendering();
	}

	drawControlPoints(r);

	glPopAttrib();
}