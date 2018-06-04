// FontNew.h
//
#ifndef FONTNEW_ONCE
#define FONTNEW_ONCE

#include "S2RendererAPI.h"

#include "OpenGL/Texture.h"

#include <string>
#include <vector>
#include <map>

namespace s2 {

class Font;
typedef std::shared_ptr<Font> FontPtr;

/************************************************************************/
/*                         Fonts Library class                          */
/************************************************************************/
class S2RENDERER_API FontsLibrary
{
public:
	typedef std::map<std::string,FontPtr> FontCollection;

private:	
	static FontCollection _fonts;

	FontsLibrary() {}
	~FontsLibrary() {}

public:
	static bool    init( const std::wstring &path );
	static FontPtr font( const std::string &fontName, int size );
	static bool    contains( const std::string &fontName );
};

/************************************************************************/
/*                         Textured font class                          */
/************************************************************************/
class S2RENDERER_API Font : public std::enable_shared_from_this<Font>
{
private:
	std::string _name;

	void *_face;

	friend class FontLoader;
	friend class FontsLibrary;

protected:
	//void  draw( const std::wstring &text );

public:
	Font( const std::string &fontName, unsigned int fontSize=0 );
	virtual ~Font();

	std::string name() const { return _name; }
};

}

#endif
