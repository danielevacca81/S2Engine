// CadToolSelect.cpp
//
#include "CadToolSelect.h"

#include "CadEngine.h"

#include "Math/Box.h"
#include "OpenGL/OpenGL.h"
#include "OpenGL/Renderer.h"

//-------------------------------------------------------------------------------------------------
CadToolSelect::CadToolSelect()
: CadTool("Selection",2)
{}

//-------------------------------------------------------------------------------------------------
CadToolSelect::~CadToolSelect()
{}

//-------------------------------------------------------------------------------------------------
void CadToolSelect::execute()
{
	CadEngine::Command c;

	c.command   = CadEngine::Select;
	c.primitive = CadEngine::Quad;
	c.points.push_back( _points[0] );
	c.points.push_back( _points[1] );
	c.attributes += "Name:SelectArea;";

	_manager->issueCommand(c);
}

//-------------------------------------------------------------------------------------------------
//void CadToolSelect::reset()
//{
//	_totPoints = 2;
//	CadTool::reset();
//}

//-------------------------------------------------------------------------------------------------
void CadToolSelect::draw( OpenGL::Renderer *r )
{
	if(_nPoints < 2) return;

	Math::box3 box;
	box.extend( Math::dvec3(_points[0].x,_points[0].y,0) );
	box.extend( Math::dvec3(_points[1].x,_points[1].y,0) );

	std::vector<Math::dvec3> pts;
	pts.push_back( Math::dvec3(box.minPoint().x, box.maxPoint().y,0) );
	pts.push_back( Math::dvec3(box.minPoint().x, box.minPoint().y,0) );
	pts.push_back( Math::dvec3(box.maxPoint().x, box.minPoint().y,0) );
	pts.push_back( Math::dvec3(box.maxPoint().x, box.maxPoint().y,0) );

	r->beginRendering();
	r->setVertex3DArray( pts );

	glColor4ub(0xFF, 0xFF, 0xFF,0x50);
	glDrawArrays( GL_QUADS,0, 4);
	
	glLineWidth(.5f);	
	glColor4ub(0x00, 0x00, 0x00,0xFF);
	glDrawArrays( GL_LINE_LOOP,0, 4);

	r->endRendering();
}