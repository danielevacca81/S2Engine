// SceneEvent.h
//
#ifndef SCENEEVENT_H
#define SCENEEVENT_H

#include "RenderPipeline_API.h"

#include "RenderCore/PickerConstants.h"

#include "Math/Math.h"

namespace SceneManager 
{

// ------------------------------------------------------------------------------------------------
struct RENDERPIPELINE_API SceneEvent
{
	SceneEvent();
	SceneEvent( const uint64_t time ) : timeMs( time ) {}

	uint64_t     timeMs;
	mutable bool accepted = false; 
};

// ------------------------------------------------------------------------------------------------
struct RENDERPIPELINE_API SceneMouseEvent : public SceneEvent
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


	Action                      action           = ActionNone;
	Button					    button           = ButtonNone;
	Button					    pressedButtons   = ButtonNone;
	KeyboardModifier            modifiers        = ModifierNone;
	Math::ivec2                 sceneViewportPos = {-1, -1};
	Math::dvec3                 scenePos         = {0.0, 0.0, 0.0};
	RenderCore::PickerConstants::Value pickValue        = RenderCore::PickerConstants::kInvalidValueBegin;
	int                         pickPart         = 0;
};

}

#endif
