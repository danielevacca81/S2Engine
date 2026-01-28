// CadToolRectangle.cpp
//
#include "CadToolRectangle.h"

#include "CadEngine.h"

#include "Math/Box.h"
#include "OpenGL/OpenGL.h"
#include "OpenGL/Renderer.h"

//-------------------------------------------------------------------------------------------------
CadToolRectangle::CadToolRectangle()
: CadTool("Rectangle",2)
{
	_creationColor = OpenGL::Color::blue();
	_square = false;
	_byRectSize = false;
}

//-------------------------------------------------------------------------------------------------
CadToolRectangle::~CadToolRectangle()
{}

//-------------------------------------------------------------------------------------------------
void CadToolRectangle::execute()
{
	if( Math::distance(_points[0],_points[1]) < _manager->pixelSize() )
		return;

	CadEngine::Command c;

	c.target     = _target;
	c.command    = CadEngine::Create;
	c.primitive  = CadEngine::Quad;
	c.color      = _manager->settings().shapeStyle();
	c.attributes = attributes();
	//c.points.insert( c.points.end(),_points.begin(),_points.end() );

	//if( _editTarget >= 0 )
	//{
	//	c.command   = CadEngine::Edit;
	//	c.target    = _editTarget;
	//}
	//else
	//{
	//	c.command   = CadEngine::Create;
	//}

	//c.primitive = CadEngine::Quad;
	//c.color     = _creationColor;
	if( _square )
	{
		const double diag = Math::distance(_points[0],_points[1]);
		const double side = diag/Math::sqrt(2.0);

		Math::dvec3 p;
		p.x = _points[0].x + side * Math::sign( _points[1].x - _points[0].x );
		p.y = _points[0].y + side * Math::sign( _points[1].y - _points[0].y );

		c.points.push_back( _points[0] );
		c.points.push_back( p );
	}
	else
	{
		c.points.push_back( _points[0] );
		c.points.push_back( _points[1] );
	}

	_manager->issueCommand(c);
}

// --------------------------------------------------------------------------------------------
void CadToolRectangle::setCreationColor( const OpenGL::Color &c ) { _creationColor = c; }

// --------------------------------------------------------------------------------------------
OpenGL::Color CadToolRectangle::creationColor() const { return _creationColor; }

//-------------------------------------------------------------------------------------------------
void CadToolRectangle::draw( OpenGL::Renderer *r )
{
	const VStyle style = _manager->settings().previewStyle();
	
	glPushAttrib( GL_LINE_BIT| GL_POINT_BIT|GL_ENABLE_BIT );
	if(_nPoints == 2)
	{
		Math::dvec3 p = _points[1];
		if( _square )
		{
			const double diag = Math::distance(_points[0],_points[1]);
			const double side = diag/Math::sqrt(2.0);

			p.x = _points[0].x + side * Math::sign( _points[1].x - _points[0].x );
			p.y = _points[0].y + side * Math::sign( _points[1].y - _points[0].y );
		}


		Math::box3 box;
		box.extend( Math::dvec3(_points[0].x,_points[0].y,0) );
		box.extend( Math::dvec3(p.x,p.y,0) );

		applyStyle();

		r->beginRendering();
		std::vector<Math::dvec3> pts;
		pts.push_back( Math::dvec3(box.minPoint().x, box.maxPoint().y,_points[0].z) );
		pts.push_back( Math::dvec3(box.minPoint().x, box.minPoint().y,_points[0].z) );
		pts.push_back( Math::dvec3(box.maxPoint().x, box.minPoint().y,_points[0].z) );
		pts.push_back( Math::dvec3(box.maxPoint().x, box.maxPoint().y,_points[0].z) );

		r->setVertex3DArray( pts );

		glDrawArrays( GL_LINE_LOOP,0, 4);

		r->endRendering();
	}

	drawControlPoints(r);

	glPopAttrib();
}