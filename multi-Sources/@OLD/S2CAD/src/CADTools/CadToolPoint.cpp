// CadToolPoint.cpp
//
#include "CadToolPoint.h"

#include "CadEngine.h"

#include "OpenGL/OpenGL.h"
#include "OpenGL/Renderer.h"

//-------------------------------------------------------------------------------------------------
CadToolPoint::CadToolPoint()
: CadTool("Point",1)
{
	_creationColor = OpenGL::Color::yellow();
	setAttribute( "Name", _name );
}

//-------------------------------------------------------------------------------------------------
CadToolPoint::~CadToolPoint()
{}

//-------------------------------------------------------------------------------------------------
void CadToolPoint::execute()
{
	CadEngine::Command c;

	c.target     = _target;
	c.command    = CadEngine::Create;
	c.primitive  = CadEngine::Point;
	c.color      = _manager->settings().pointStyle();
	c.attributes = attributes();
	c.points.insert( c.points.end(),_points.begin(),_points.end() );
	
	//if( _editTarget >= 0 )
	//{
	//	c.command   = CadEngine::Edit;
	//	c.target    = _editTarget;
	//}
	//else
	//{
	//	c.command   = CadEngine::Create;
	//}

	//c.primitive  = CadEngine::Point;
	//c.color      = _creationColor;
	//c.attributes = attributes();
	//c.points.push_back( _points[0] );

	_manager->issueCommand(c);
}

// --------------------------------------------------------------------------------------------
void CadToolPoint::setCreationColor( const OpenGL::Color &c ) { _creationColor = c; }

// --------------------------------------------------------------------------------------------
OpenGL::Color CadToolPoint::creationColor() const { return _creationColor; }

// --------------------------------------------------------------------------------------------
void CadToolPoint::draw( OpenGL::Renderer *r )
{
	if(_nPoints == 0)
		return;

	glPushAttrib( GL_ENABLE_BIT|GL_POINT_BIT );

	applyStyle();

	drawControlPoints(r);

	//glPointSize(9.0);
	//glColor3ub(0x00, 0x00, 0x00);
	//glBegin(GL_POINTS);
	//	r->glVertex3( _points[0] );
	//glEnd();

	//glPointSize(7.0);
	//glColor4fv( _manager->settings().previewColor() );
	//glBegin(GL_POINTS);
	//	r->glVertex3( _points[0] );
	//glEnd();

	glPopAttrib();

}