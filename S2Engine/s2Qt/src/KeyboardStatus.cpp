// KeyboardStatus.cpp
//
#include "KeyboardStatus.h"

#include <QKeyEvent>

using namespace s2Qt;

// -----------------------------------------------------------------------------------------------
KeyboardStatus::KeyboardStatus()
{
	reset();
}

// -----------------------------------------------------------------------------------------------
KeyboardStatus::~KeyboardStatus()
{
}

// -----------------------------------------------------------------------------------------------
void KeyboardStatus::reset() 
{
	//memset(buffer,0,sizeof(bool)*256);
	modifiers = 0;
	keySequence = QKeySequence("");
}

// -----------------------------------------------------------------------------------------------
void KeyboardStatus::update( QKeyEvent *e, bool pressed )
{
#if 0
	unsigned int k = e->nativeVirtualKey();
	
	if( k<256 )
		buffer[k] = pressed;

	Qt::KeyboardModifiers mod = e->modifiers();

	if (modifiers & Qt::ShiftModifier)	 modifiers |= ModifierShift;
	if (modifiers & Qt::ControlModifier) modifiers |= ModifierCtrl;
	if (modifiers & Qt::AltModifier)	 modifiers |= ModifierAlt;
#endif
	modifiers = 0;
	Qt::KeyboardModifiers mod = e->modifiers();

	if (mod & Qt::ShiftModifier)	modifiers |= ModifierShift;
	if (mod & Qt::ControlModifier)	modifiers |= ModifierCtrl;
	if (mod & Qt::AltModifier)		modifiers |= ModifierAlt;
	if (mod & Qt::MetaModifier)		modifiers |= ModifierMeta;

	keySequence = QKeySequence( e->key() );
	e->accept();
	//std::cout<< keySequence.toString().toStdString();
}

// -----------------------------------------------------------------------------------------------
unsigned int KeyboardStatus::getCode() const
{
    unsigned int ks = 0;       //0 means no valid code
    if (keySequence.count() == 1)
    {
        ks = keySequence[0];

        if (modifiers & ModifierShift) ks += Qt::SHIFT;
        if (modifiers & ModifierCtrl)  ks += Qt::CTRL;
        if (modifiers & ModifierAlt)   ks += Qt::ALT;
        if (modifiers & ModifierMeta)  ks += Qt::META;
    }
	return ks;
}