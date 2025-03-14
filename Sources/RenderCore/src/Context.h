// Context.h
//
#ifndef CONTEXT_H
#define CONTEXT_H

#include "RenderCore_API.h"

#include "ContextInfo.h"
#include "StateManager.h"

namespace RenderCore {

class RENDERCORE_API Context
{
public:
	static Context *current();
	//static Context *defaultContext();

public:	
	~Context();
	
	const ContextInfo& info() const { return _info; }

	bool operator==( const Context& o ) const { return _nativeHandle == o._nativeHandle; }
	bool operator!=( const Context& o ) const { return _nativeHandle != o._nativeHandle; }

	virtual void beginRendering();
	virtual void endRendering() ;

	//virtual void makeCurrent() { /*todo*/}
	//virtual void doneCurrent() { /*todo*/}

	virtual int32_t defaultFrameBufferObject() const { return _defaultFBO; }

protected:
	Context();

protected:
	//std::string  _name;
	uint64_t  _nativeHandle { 0 };
	int32_t   _defaultFBO { -1 };

	ContextInfo  _info;
	StateManager _stateManager;
	
	friend class Renderer;
};


}
#endif