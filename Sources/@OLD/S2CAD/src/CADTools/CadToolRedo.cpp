// CadToolRedo.cpp
//
#include "CadToolRedo.h"

#include "CadEngine.h"

//-------------------------------------------------------------------------------------------------
CadToolRedo::CadToolRedo()
: CadTool("Redo",0)
{}

//-------------------------------------------------------------------------------------------------
CadToolRedo::~CadToolRedo()
{}

//-------------------------------------------------------------------------------------------------
void CadToolRedo::execute()
{
	CadEngine::Command c;
	c.command= CadEngine::Redo;
	_manager->issueCommand(c);
}

//-------------------------------------------------------------------------------------------------
void CadToolRedo::draw( OpenGL::Renderer *r )
{
}