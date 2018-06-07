// KeyboardStatus.h
//
#ifndef QTBRIDGE_KEYBOARDSTATUS_H
#define QTBRIDGE_KEYBOARDSTATUS_H

#include "s2Qt_API.h"

#include <QKeySequence>

#include <string>

class QKeyEvent;

namespace s2Qt {

class S2QT_API KeyboardStatus
{
public:
	KeyboardStatus();
	~KeyboardStatus();

	void reset() ;
	void update( QKeyEvent *e, bool pressed );

	unsigned int getCode() const;

private:
    //
    enum Modifier
    {
        //Qt::NoModifier           0x00000000  No modifier key is pressed.
        //Qt::ShiftModifier        0x02000000  A Shift key on the keyboard is pressed.
        //Qt::ControlModifier      0x04000000  A Ctrl key on the keyboard is pressed.
        //Qt::AltModifier          0x08000000  An Alt key on the keyboard is pressed.
        //Qt::MetaModifier         0x10000000  A Meta key on the keyboard is pressed.
        //Qt::KeypadModifier       0x20000000  A keypad button is pressed.
        //Qt::GroupSwitchModifier  0x40000000  X11 only.A Mode_switch key on the keyboard is pressed.

        ModifierNone  = 0x0000, // No modifier
        ModifierShift = 0x0100, // Shift key pressed
        ModifierCtrl  = 0x0200, // Ctrl key pressed
        ModifierAlt   = 0x0400, // Alt key pressed
        ModifierMeta  = 0x0800, // Meta key pressed
        ModifierAny   = 0x0F00, // Mask to get modifiers only
    };

private:
    int          modifiers;
    QKeySequence keySequence;
};


}

#endif