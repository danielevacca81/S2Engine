// CadToolPolyLine.cpp
//
#include "CadToolPolyLine.h"

#include "CadEngine.h"

#include "Math/Box.h"
#include "OpenGL/OpenGL.h"
#include "OpenGL/Renderer.h"

//-------------------------------------------------------------------------------------------------
CadToolPolyLine::CadToolPolyLine()
: CadTool("PolyLine",2)
{
	_creationColor = OpenGL::Color::red().lighter();
}

//-------------------------------------------------------------------------------------------------
CadToolPolyLine::~CadToolPolyLine()
{}

//-------------------------------------------------------------------------------------------------
void CadToolPolyLine::execute()
{
	// we need at least two points
	if( collectedPointCount() > 1 )
	{
		CadEngine::Command c;

		c.target     = _target;
		c.command    = CadEngine::Create;
		c.primitive  = CadEngine::PolyLine;
		c.color      = _manager->settings().lineStyle();
		c.attributes = attributes();
		c.points.insert( c.points.end(),_points.begin(),_points.begin()+collectedPointCount() );

		//if( _editTarget >= 0 )
		//{
		//	c.command   = CadEngine::Edit;
		//	c.target    = _editTarget;
		//}
		//else
		//{
		//	c.command   = CadEngine::Create;
		//		
		//}

		//c.primitive = CadEngine::PolyLine;
		//c.color     = _creationColor;
		//c.attributes = attributes();
		//for( int i=0; i<_nPoints-1; ++i )
		//	c.points.push_back( _points[i] );

		_manager->issueCommand(c);
	}
}

// --------------------------------------------------------------------------------------------
void CadToolPolyLine::reset()
{
	_totPoints = 2;

	CadTool::reset();
	setWaitConfirm(true);
}

// --------------------------------------------------------------------------------------------
void CadToolPolyLine::setCreationColor( const OpenGL::Color &c ) { _creationColor = c; }

// --------------------------------------------------------------------------------------------
OpenGL::Color CadToolPolyLine::creationColor() const { return _creationColor; }

//-------------------------------------------------------------------------------------------------
void CadToolPolyLine::draw( OpenGL::Renderer *r )
{
	const VStyle style = _manager->settings().previewStyle();

	glPushAttrib( GL_LINE_BIT|GL_ENABLE_BIT|GL_POINT_BIT );

	applyStyle();

	if( _nPoints > 1 )
	{
		r->beginRendering();
		r->setVertex3DArray( _points );

		glDrawArrays( GL_LINE_STRIP,0,_nPoints );

		r->endRendering();
	}

	drawControlPoints(r);

	glPopAttrib();
}