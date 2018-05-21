// Settings.h
// 
#ifndef SETTINGS_ONCE
#define SETTINGS_ONCE

#include "S2QtAPI.h"

#include "MouseStatus.h"

namespace s2Qt {

enum MouseButtons
{
	MainButton      = MouseStatus::ButtonLeft,
	SecondaryButton = MouseStatus::ButtonRight,
};

enum ZoomMode	{ ZoomOnMouse, ZoomOnCenter };

}

#endif