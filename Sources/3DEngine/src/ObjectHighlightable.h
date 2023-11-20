// ObjectHighlightable.h
//
#ifndef SCENE_OBJECTHIGHLIGHTABLE_H
#define SCENE_OBJECTHIGHLIGHTABLE_H

#include "s2Scene_API.h"

#include "ObjectInteraction.h"

namespace Scene 
{

class ObjectHighlightable;
typedef std::shared_ptr<ObjectHighlightable> ObjectHighlightablePtr;

class S2SCENE_API ObjectHighlightable : public virtual ObjectInteraction
{
public:
	enum class HighlightPolicy { None = 0, Mouseover = 1, ByCode = 2, Any = 3 };

	struct HighlightedSignal { bool enabled; };

public:
	ObjectHighlightable();
	~ObjectHighlightable() { callDestroyedSignal(); };

	void            setVisible( bool visible ) override; 	    
	void            setEnabled( bool enabled ) override; 	    

	void			setPolicy( const HighlightPolicy policy );
	HighlightPolicy policy() const { return _highlightPolicy; }

	void            enterEvent( SceneEvent *event )  override;
	void            leaveEvent( SceneEvent *event )  override;
				    
	virtual void    setHighlighted( bool hilighted );
	virtual bool    isHighlighted() const { return _highlightStatus; }
				    
private: 		    
	void            updateHighlightSatus( bool emitIfChanged );
				    
protected:
	HighlightPolicy _highlightPolicy;
	bool            _highlightByCode;
	bool            _highlightByMouse;

	bool            _highlightStatus;
};

}

#endif


