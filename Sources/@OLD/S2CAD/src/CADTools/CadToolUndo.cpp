// CadToolUndo.cpp
//
#include "CadToolUndo.h"

#include "CadEngine.h"

//-------------------------------------------------------------------------------------------------
CadToolUndo::CadToolUndo()
: CadTool("Undo",0)
{}

//-------------------------------------------------------------------------------------------------
CadToolUndo::~CadToolUndo()
{}

//-------------------------------------------------------------------------------------------------
void CadToolUndo::execute()
{
	CadEngine::Command c;
	c.command= CadEngine::Undo;
	_manager->issueCommand(c);
}

//-------------------------------------------------------------------------------------------------
void CadToolUndo::draw( OpenGL::Renderer *r )
{
}