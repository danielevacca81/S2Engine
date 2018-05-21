// UserInteractionManager.h
//
#ifndef S2QT_USERINTERACTIONMANAGER_ONCE
#define S2QT_USERINTERACTIONMANAGER_ONCE

#include "S2QtAPI.h"

#include <map>
#include <string>
#include <iostream>

#include "MouseStatus.h"
#include "KeyboardStatus.h"

namespace s2Qt{

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
namespace {
	class UIMCallbackBase
	{
	public:
		virtual void invoke() = 0;
	};

	////
	template< typename T >
	class UIMCallback : public UIMCallbackBase
	{
	public:
		typedef void (T::*CallbackType)();
		T				*object;
		CallbackType	callback;

		void invoke() { (object->*callback)(); }
	};
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
class S2QT_API UIMCommand
{
private:
	std::string _name;
	std::string _activator;
	std::shared_ptr<UIMCallbackBase> _callback;

public:
	UIMCommand() {}
	
	template <typename T> 
	UIMCommand( const std::string &name, const std::string &activator, T *dest, void (T::*m)() )
	{
		std::shared_ptr<UIMCallback<T>> f(new UIMCallback<T>);
		f->object   = dest;
		f->callback = m;

		_callback  = f;
		_activator = activator;
		_name      = name;
	}

	std::string name()		const {return _name;}
	std::string activator() const {return _activator;}
	
	void execute() { if(_callback) _callback->invoke(); }	
};

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
class S2QT_API UserInteractionManager
{
private:
	typedef std::map<unsigned int, UIMCommand> Bindings;
	
	Bindings keyBindings;
	Bindings mouseBindings;

	MouseStatus		ms;
	KeyboardStatus  kb;

	// ------------------------------------------------------------------------------------------------
	void executeMouseCommand( unsigned int commandID )
	{
		Bindings::iterator it = mouseBindings.find( commandID );
		if( it != mouseBindings.end() )
			it->second.execute();
	}

	// ------------------------------------------------------------------------------------------------
	void executeKeyboardCommand( unsigned int commandID )
	{
		Bindings::iterator it = keyBindings.find( commandID );
		if( it != keyBindings.end() )
			it->second.execute();
	}

	// ------------------------------------------------------------------------------------------------
	static std::map<std::string, unsigned int> mouseCommandCodes;
	static std::map<std::string, unsigned int> keyboardCommandCodes;
	static unsigned int decodeMouseCommnand( const std::string &command );
	static unsigned int decodeKeyboardCommnand( const std::string &command );
public:

	// ------------------------------------------------------------------------------------------------
	UserInteractionManager()	{ clear(); }
	~UserInteractionManager()	{ clear(); }

	// ------------------------------------------------------------------------------------------------
	bool registerMouseCommand( const UIMCommand &c )
	{
		unsigned int commandCode = decodeMouseCommnand(c.activator());
		if( mouseBindings.find( commandCode ) != mouseBindings.end() )
			return false;	// bind already used! 
		mouseBindings[ commandCode ] = c;
		return true;
	}
	
	// ------------------------------------------------------------------------------------------------
	bool registerKeyboardCommand( const UIMCommand &c )
	{
		unsigned int commandCode = decodeKeyboardCommnand(c.activator());
		if( keyBindings.find( commandCode ) != keyBindings.end() )
			return false;	// bind already used! 

		keyBindings[ commandCode ] = c;
		return true;
	}

	// ------------------------------------------------------------------------------------------------
	bool unregisterMouseCommand( const std::string &activator )
	{
		unsigned int code = decodeMouseCommnand(activator);
		return mouseBindings.erase( code ) == 1;
	}

	// ------------------------------------------------------------------------------------------------
	bool unregisterKeyboardCommand( const std::string &activator )
	{
		unsigned int code = decodeKeyboardCommnand(activator);
		return keyBindings.erase( code ) == 1;
	}

	// ------------------------------------------------------------------------------------------------
	void clear()
	{
		ms.reset();
		kb.reset();

		mouseBindings.clear();
		keyBindings.clear();
	}

	// ------------------------------------------------------------------------------------------------
	void updateMouse( QMouseEvent *e, bool updateButtons )
	{
		ms.update( e, updateButtons );
		executeMouseCommand( ms.code() );
	}

	// ------------------------------------------------------------------------------------------------
	void updateMouse( QWheelEvent *e, bool updateButtons )
	{
		ms.update( e, updateButtons );
		executeMouseCommand( ms.code() );
	}

	
	// ------------------------------------------------------------------------------------------------
	void updateMouse( QGraphicsSceneMouseEvent *e, bool updateButtons )
	{
		ms.update( e, updateButtons );
		executeMouseCommand( ms.code() );
	}

	// ------------------------------------------------------------------------------------------------
	void updateMouse( QGraphicsSceneWheelEvent *e, bool updateButtons )
	{
		ms.update( e, updateButtons );

		executeMouseCommand( ms.code() );
	}	

	// ------------------------------------------------------------------------------------------------
	void updateKeyboard( QKeyEvent *e, bool keyPressed )
	{
		kb.update( e, keyPressed );

		if( keyPressed )
			executeKeyboardCommand( kb.code() );
	}

	// ------------------------------------------------------------------------------------------------
	MouseStatus    &mouseStatus()    { return ms; }
	KeyboardStatus &keyboardStatus() { return kb; }
};

}

#endif

