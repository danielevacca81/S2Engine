// OpenGLObject.h
//
#ifndef S2_RENDERCORE_OPENGLOBJECT_H
#define S2_RENDERCORE_OPENGLOBJECT_H

#include "s2Engine_API.h"

#include <string>

namespace s2 {
namespace RenderCore {

class Context;

class S2ENGINE_API OpenGLObject
{
public:
	OpenGLObject() = default;	
	virtual ~OpenGLObject() = default;

	virtual void create();
	virtual void destroy();
	virtual void bind()   const = 0;
	virtual void unbind() const = 0;

	void         setLabel( const std::string &label );
	std::string  label() const;
	
	// -----------------------------------------------------------------------------------------------
	inline bool isCreated() const { return _created; }
	inline bool isValid()   const { return isCreated() && _objectID != 0; }
	// @todo:isSupported?
	
	// -----------------------------------------------------------------------------------------------
	inline unsigned int id()        const { return _objectID; }


	// -----------------------------------------------------------------------------------------------
	inline bool operator!=( const OpenGLObject &other ) const { return _objectID != other._objectID; }
	inline bool operator==( const OpenGLObject &other ) const { return _objectID == other._objectID; }
protected:
	virtual void reset() 
	{
		_created  = false;
		_objectID = 0;	
		_context  = nullptr;
		_label.clear();
	}

	virtual int objectLabelIdentifier() const { return 0; }

protected:
	Context*     _context  = nullptr;
	bool         _created  = false;
	unsigned int _objectID = 0;
	std::string  _label;
};

} // namespace RenderCore
} // namespace s2
#endif //! S2_RENDERCORE_OPENGLOBJECT_H