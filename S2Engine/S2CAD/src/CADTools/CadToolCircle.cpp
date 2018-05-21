// CadToolCircle.cpp
//
#include "CadToolCircle.h"

#include "CadEngine.h"

#include "OpenGL/OpenGL.h"
#include "OpenGL/Renderer.h"
#include "Geometry/Geometry.h"


#include "VObjects/VObject.h"

//-------------------------------------------------------------------------------------------------
CadToolCircle::CadToolCircle()
: CadTool("Circle",2),
 _useRadius(false)
{}

//-------------------------------------------------------------------------------------------------
CadToolCircle::~CadToolCircle()
{}

// --------------------------------------------------------------------------------------------
void CadToolCircle::setCreationColor( const OpenGL::Color &c ) { _creationColor = c; }

// --------------------------------------------------------------------------------------------
OpenGL::Color CadToolCircle::creationColor() const { return _creationColor; }

//-------------------------------------------------------------------------------------------------
void CadToolCircle::execute()
{
	if( Math::distance(_points[0],_points[1]) < _manager->pixelSize() )
		return;

	CadEngine::Command c;

	c.target     = _target;
	c.command    = CadEngine::Create;
	c.primitive  = CadEngine::Circle;
	c.color      = _manager->settings().shapeStyle();
	c.attributes = attributes();
	c.points.insert(c.points.end(),_points.begin(),_points.end() );
	
	//if( _editTarget >= 0 )
	//{
	//	c.command   = CadEngine::Edit;
	//	c.target    = _editTarget;
	//}
	//else
	//{
	//	c.command   = CadEngine::Create;
	//}


	_manager->issueCommand(c);
}

//-------------------------------------------------------------------------------------------------
void CadToolCircle::draw( OpenGL::Renderer *r )
{

	glPushAttrib( GL_LINE_BIT|GL_POINT_BIT|GL_ENABLE_BIT );
	applyStyle();

	if( _nPoints > 1 )
	{
		const Math::dvec3 p0 = _points[0];
		const Math::dvec3 p1 = _points[1];
		
		std::vector< Math::dvec3 > pts = Geometry::circle( p0, Math::distance(p0,p1) );
		
		r->beginRendering();
		r->setVertex3DArray( pts );

		glDrawArrays( GL_LINE_STRIP,1,pts.size()-1 );

		r->setVertex3DArray( _points );

		glDrawArrays( GL_LINES, 0, 2);

		r->endRendering();
	}

	drawControlPoints(r);

	glPopAttrib();
}