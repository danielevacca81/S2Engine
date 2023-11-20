// SceneEvent.h
//
#ifndef SCENE_SCENEEVENT_H
#define SCENE_SCENEEVENT_H

#include "s2Scene_API.h"

#include "s2Renderer/Picker.h"

#include "Core/Math.h"

namespace Scene 
{

// ------------------------------------------------------------------------------------------------
struct S2SCENE_API SceneEvent
{
	SceneEvent();
	SceneEvent( const uint64_t time ) : timeMs( time ) {}

	uint64_t     timeMs;
	mutable bool accepted = false; 
};

// ------------------------------------------------------------------------------------------------
struct S2SCENE_API SceneMouseEvent : public SceneEvent
{
	enum Action : int
	{
		ActionNone    = 0x0000, 
		MouseMove     = 0x1000, 
		ButtonPress   = 0x4000, 
		ButtonRelease = 0x8000
	};

	enum Button : int
	{
		ButtonNone    = 0x0000, // No button or key pressed.
		ButtonLeft    = 0x0001, // Left mouse button pressed.
		ButtonMiddle  = 0x0002, // Middle mouse button pressed.
		ButtonRight   = 0x0004, // Right mouse button pressed.
		ButtonAny     = 0x00FF, // Mask to get buttons only
	};

	enum KeyboardModifier : int
	{
		ModifierNone  = 0x0000, // No modifier
		ModifierShift = 0x0100, // Shift key pressed
		ModifierCtrl  = 0x0200, // Ctrl key pressed
		ModifierAlt   = 0x0400, // Alt key pressed
		ModifierMeta  = 0x0800, // Meta key pressed
		ModifierAny   = 0x0F00, // Mask to get modifiers only
	};


	Action                   action           = ActionNone;
	Button					 button           = ButtonNone;
	Button					 pressedButtons   = ButtonNone;
	KeyboardModifier         modifiers        = ModifierNone;
	Math::ivec2              sceneViewportPos = {-1, -1};
	Math::dvec3              scenePos         = {0.0, 0.0, 0.0};
	Renderer::Picker::Value  pickValue        = Renderer::Picker::kInvalidValueBegin;
	int                      pickPart         = 0;
};

}

#endif
