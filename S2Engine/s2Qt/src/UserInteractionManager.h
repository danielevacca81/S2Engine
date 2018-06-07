// UserInteractionManager.h
//
#ifndef QTBRIDGE_USERINTERACTIONMANAGER_ONCE
#define QTBRIDGE_USERINTERACTIONMANAGER_ONCE

#include "s2Qt_API.h"

#include "MouseStatus.h"
#include "KeyboardStatus.h"
#include "GestureStatus.h"
#include "UIMCommand.h"

#include <QString>

#include <map>

namespace s2Qt {

class S2QT_API UserInteractionManager
{
public:
    enum ZoomMode { ZoomOnMouse, ZoomOnCenter };    
    struct Preferences
    {        
        MouseStatus::Status primaryButton;
        MouseStatus::Status secondaryButton;
        ZoomMode            zoomMode;
        bool                gesturePanEnabled;
        bool                gestureSwipeEnabled;
        bool                gesturePinchEnabled;
        bool				gestureTapEnabled;
        bool				gestureTapHoldEnabled;

        Preferences()
        : primaryButton  ( MouseStatus::ButtonLeft )
        , secondaryButton( MouseStatus::ButtonRight )
        , zoomMode ( ZoomOnMouse )
        , gesturePanEnabled( false )
        , gestureSwipeEnabled( false )
        , gesturePinchEnabled( false )
        , gestureTapEnabled( false )
        , gestureTapHoldEnabled( false )
        {}
    };

public:
	UserInteractionManager();
	~UserInteractionManager();

	void setPreferences( const Preferences &p );    
	Preferences preferences() const;

	// ------------------------------------------------------------------------------------------------
	bool registerMouseCommand( const UIMCommand &c );
	bool registerKeyboardCommand( const UIMCommand &c );
	bool registerGestureCommand( const UIMCommand &c );

	bool unregisterMouseCommand( const QString &activator );
	bool unregisterKeyboardCommand( const QString &activator );
	bool unregisterGestureCommand( const QString &activator );

	void clear();
	
	void updateMouse( QGraphicsSceneMouseEvent *e, bool updateButtons );
	void updateMouse( QGraphicsSceneWheelEvent *e, bool updateButtons );
	void updateKeyboard( QKeyEvent *e, bool keyPressed );
	void updateGesture( QGestureEvent *e );

    MouseStatus    &mouseStatus()    { return _ms;  }
	GestureStatus  &gestureStatus()  { return _gst; }
	KeyboardStatus &keyboardStatus() { return _kb;  }

private:
    static std::map<QString, unsigned int> mouseCommandCodes;
    static std::map<QString, unsigned int> keyboardCommandCodes;
	static std::map<QString, unsigned int> gestureCommandCodes;
    static unsigned int decodeMouseCommnand( const QString &command );
    static unsigned int decodeKeyboardCommnand( const QString &command );	
	static unsigned int decodeGestureCommand( const QString &command );

private:
	void executeMouseCommand( unsigned int commandID );
	void executeKeyboardCommand( unsigned int commandID );
	void executeGestureCommand( unsigned int commandID );

private:
    typedef std::map<unsigned int, UIMCommand> Bindings;

    Bindings        _keyBindings;
    Bindings        _mouseBindings;
	Bindings        _gestureBindings;

    Preferences     _preferences;
    MouseStatus		_ms;
    KeyboardStatus  _kb;
	GestureStatus  _gst;
};

}

#endif
