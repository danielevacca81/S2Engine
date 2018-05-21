// KeyboardStatus.h
//
#ifndef S2QT_KEYBOARDSTATUS_ONCE
#define S2QT_KEYBOARDSTATUS_ONCE

#include "S2QtAPI.h"

#include <QKeySequence>

#include <string>

class QKeyEvent;

namespace s2Qt {

class S2QT_API KeyboardStatus
{
private:
	int          _modifiers;
	QKeySequence _keySequence;

	//
	enum Modifier
	{
		ModifierNone  = 0x0000, // No modifier
		ModifierShift = 0x0100, // Shift key pressed
		ModifierCtrl  = 0x0200, // Ctrl key pressed
		ModifierAlt   = 0x0400, // Alt key pressed
		ModifierMeta  = 0x0800, // Meta key pressed
		ModifierAny   = 0x0F00, // Mask to get modifiers only
	};

public:
	KeyboardStatus();
	~KeyboardStatus();

	// -----------------------------------------------------------------------------------------------
	void reset() ;
	void update( QKeyEvent *e, bool pressed );

	unsigned int code() const;
};



}


#endif