// CadToolLine.cpp
//
#include "CadToolLine.h"

#include "CadEngine.h"

#include "Math/Box.h"
#include "OpenGL/OpenGL.h"
#include "OpenGL/Renderer.h"

//-------------------------------------------------------------------------------------------------
CadToolLine::CadToolLine()
: CadTool("Line",2)
{
	_creationColor = OpenGL::Color::red();
	_useLen= false;
	_length= 0.;
	_angle= 0.;
}

//-------------------------------------------------------------------------------------------------
CadToolLine::~CadToolLine()
{}

//-------------------------------------------------------------------------------------------------
void CadToolLine::execute()
{
	if( Math::distance(_points[0],_points[1]) < _manager->pixelSize() )
		return;

	CadEngine::Command c;

	c.target     = _target;
	c.command    = CadEngine::Create;
	c.primitive  = CadEngine::Line;
	c.color      = _manager->settings().lineStyle();
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

	//c.primitive  = CadEngine::Line;
	//c.color      = _creationColor;
	//c.attributes = attributes();
	//c.points.push_back( _points[0] );
	//c.points.push_back( _points[1] );

	_manager->issueCommand(c);
}

// --------------------------------------------------------------------------------------------
void CadToolLine::setCreationColor( const OpenGL::Color &c ) { _creationColor = c; }

// --------------------------------------------------------------------------------------------
OpenGL::Color CadToolLine::creationColor() const { return _creationColor; }

//-------------------------------------------------------------------------------------------------
void CadToolLine::draw( OpenGL::Renderer *r )
{
	glPushAttrib( GL_LINE_BIT|GL_POINT_BIT|GL_ENABLE_BIT );
	applyStyle();

	if( _nPoints > 1 )
	{
		r->beginRendering();
		r->setVertex3DArray( _points );

		glDrawArrays( GL_LINE_STRIP,0,_points.size() );

		r->endRendering();
	}

	drawControlPoints(r);
	glPopAttrib();
}

//-------------------------------------------------------------------------------------------------
const double angularThreshold= 5.; // 5 degrees

//-------------------------------------------------------------------------------------------------
void GetPolar( const Math::dvec3 &startPoint, const Math::dvec3 &endPoint, double &len, double &ang )
{
	Math::dvec3 sz= endPoint - startPoint;
	
	len= Math::length( sz );
	ang= Math::degrees( atan2( sz.y, sz.x ) );

	ang= floor( ang / angularThreshold + .5 ) * angularThreshold;
}

//-------------------------------------------------------------------------------------------------
void CadToolLine::validate()
{
	if( _useLen )
	{
		if( _nPoints == 2 )
		{
			// Dragging start point
			if( _selIndex == 0 )
			{
				// len cannot change via mouse (can only be edited)
				_points[1].x= _points[0].x + _length * cos( Math::radians( _angle ) );
				_points[1].y= _points[0].y + _length * sin( Math::radians( _angle ) );
			}
			// Dragging end point
			if( _selIndex == 1 )
			{
				double len;
				GetPolar( _points[0], _points[1], len, _angle );

				// len cannot change via mouse (can only be edited)
				_points[1].x= _points[0].x + _length * cos( Math::radians( _angle ) );
				_points[1].y= _points[0].y + _length * sin( Math::radians( _angle ) );
			}
		}
	}
	else
	{
		GetPolar( _points[0], _points[1], _length, _angle );
	}
}