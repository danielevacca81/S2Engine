// CadToolErase.cpp
//
#include "CadToolErase.h"

#include "CadEngine.h"

#include "OpenGL/OpenGL.h"
#include "OpenGL/Renderer.h"

//-------------------------------------------------------------------------------------------------
CadToolErase::CadToolErase()
: CadTool("Erase",0)
{}

//-------------------------------------------------------------------------------------------------
CadToolErase::~CadToolErase()
{}

//-------------------------------------------------------------------------------------------------
void CadToolErase::execute()
{
	std::vector<unsigned int> selectedObjects = _manager->getSelectedObjects();

	for( size_t i=0; i<selectedObjects.size(); ++i )
	{
		CadEngine::Command c;
		c.command   = CadEngine::Destroy;
		c.primitive = CadEngine::None;
		c.target    = selectedObjects[i];
		
		_manager->issueCommand(c);
	}
}

// --------------------------------------------------------------------------------------------
void CadToolErase::draw( OpenGL::Renderer *r )
{}