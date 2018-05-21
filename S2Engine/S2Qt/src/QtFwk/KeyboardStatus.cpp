// KeyboardStatus.cpp
//
#include "KeyboardStatus.h"

#include <QKeyEvent>

#include <iostream>

using namespace s2Qt;

// -----------------------------------------------------------------------------------------------
KeyboardStatus::KeyboardStatus()
{
	reset();
}

// -----------------------------------------------------------------------------------------------
KeyboardStatus::~KeyboardStatus()
{}

// -----------------------------------------------------------------------------------------------
void KeyboardStatus::reset() 
{
	_modifiers   = 0;
	_keySequence = QKeySequence("");
}

// -----------------------------------------------------------------------------------------------
void KeyboardStatus::update( QKeyEvent *e, bool pressed )
{
	_modifiers = 0;
	Qt::KeyboardModifiers mod = e->modifiers();

	if( mod & Qt::ShiftModifier   ) _modifiers |= ModifierShift;
	if( mod & Qt::ControlModifier ) _modifiers |= ModifierCtrl;
	if( mod & Qt::AltModifier     ) _modifiers |= ModifierAlt;
	if( mod & Qt::MetaModifier    ) _modifiers |= ModifierMeta;

	_keySequence = QKeySequence( e->key() );
}

// -----------------------------------------------------------------------------------------------
unsigned int KeyboardStatus::code() const
{
	unsigned int ks = (int)(_keySequence);
	
	if( _modifiers & ModifierShift ) ks += Qt::SHIFT;
	if( _modifiers & ModifierCtrl  ) ks += Qt::CTRL;
	if( _modifiers & ModifierAlt   ) ks += Qt::ALT;
	if( _modifiers & ModifierMeta  ) ks += Qt::META;

	return ks;
}