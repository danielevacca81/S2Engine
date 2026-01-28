// CadToolEdit.cpp
//
#include "CadToolEdit.h"

#include "CadEngine.h"
#include "VObjects/VObject.h"

//-------------------------------------------------------------------------------------------------
CadToolEdit::CadToolEdit()
: CadTool("Edit",0)
{}

//-------------------------------------------------------------------------------------------------
CadToolEdit::~CadToolEdit()
{}

//-------------------------------------------------------------------------------------------------
void CadToolEdit::execute()
{
	std::vector<unsigned int> selectedObjects = _manager->getSelectedObjects();
	if( selectedObjects.size() != 1) 
		return;

	CadEngine::Command c;
	c.command   = CadEngine::StartEdit;
	c.primitive = CadEngine::None;
	c.target    = selectedObjects[0];

	_manager->issueCommand(c);
}

//-------------------------------------------------------------------------------------------------
void CadToolEdit::draw( OpenGL::Renderer *r )
{}