// Settings.h
// 
#ifndef QTBRIDGE_SETTINGS_ONCE
#define QTBRIDGE_SETTINGS_ONCE

#include "QtBridge_API.h"

#include "MouseStatus.h"

namespace QtBridge {

enum MouseButtons
{
	MainButton      = MouseStatus::ButtonLeft,
	SecondaryButton = MouseStatus::ButtonRight,
};

enum ZoomMode	{ ZoomOnMouse, ZoomOnCenter };

}

#endif