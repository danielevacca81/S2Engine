// SceneDebugger.cpp
// 
#include "SceneDebugger.h"

using namespace SceneManager;

// ------------------------------------------------------------------------------------------------
void SceneDebugger::logEvent( std::shared_ptr<ObjectInteraction> &o, const SceneEvent &ev, const char *str )
{
#if defined( DEBUG_LOGEVENT )
	++gDebugLogCount;

	std::cout << gDebugLogCount << " obj:";
	if( !o )
		std::cout << "nullptr";
	else if( o->name().empty() )
		std::cout << o.get();
	else
		std::cout << o->name();
	std::cout << "   ";

	std::cout << str;

	std::cout << "\n";
#endif
}

// ------------------------------------------------------------------------------------------------
void SceneDebugger::logEvent( std::shared_ptr<ObjectInteraction> &o, const MouseEventObj &evObj, const char *str )
{
#if defined( DEBUG_LOGEVENT )
	++gDebugLogCount;
	static constexpr Renderer::SurfacePicker::Value _kRangePickValueBegin = Renderer::SurfacePicker::kInvalidValueBegin / 3;

	std::cout << gDebugLogCount << " obj:";
	if( !o )
		std::cout << "nullptr";
	else if( o->name().empty() )
		std::cout << o.get();
	else
		std::cout << o->name();
	std::cout << "   ";

	std::cout << str << " ";

	std::cout	  << "ev[ " 
				     "pickObj:";

	const auto obj = evObj.object.lock();
	if( !obj )
		std::cout << "nullptr";
	else if( obj->name().empty() )
		std::cout << obj.get();
	else
		std::cout << obj->name();

	const auto &ev = *evObj.mouseEvent;
	std::cout << " pickValue:";
	if( ev.pickValue == Renderer::SurfacePicker::kClearValue )
		std::cout << "kClearValue";
	else if( ev.pickValue >= Renderer::SurfacePicker::kInvalidValueBegin )
		std::cout << "kInvalidValueBegin+" << ev.pickValue-Renderer::SurfacePicker::kInvalidValueBegin;
	else if( ev.pickValue >= _kRangePickValueBegin )
		std::cout << "_kRangePickValueBegin+" << ev.pickValue-_kRangePickValueBegin;
	else 
		std::cout << ev.pickValue;
	
	std::cout     << " "
		             "pickPart:"  << ev.pickPart  << " "

				  << " "
		             "scenePos:"  << ev.scenePos.x  << "," << ev.scenePos.y  << "," << ev.scenePos.z  << " "
		             "sceneViewportPos:"  << ev.sceneViewportPos.x  << "," << ev.sceneViewportPos.y   << " "
		            "]";

	std::cout << "\n";
#endif
}