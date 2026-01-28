// FontNew.h
//
#ifndef FONTNEW_ONCE
#define FONTNEW_ONCE

#include "S2FontLibraryAPI.h"

#include <string>
#include <vector>
#include <map>
#include <memory>

namespace s2 {

class Font;
typedef std::shared_ptr<Font> FontPtr;

/************************************************************************/
/*                         Fonts Library class                          */
/************************************************************************/
class S2FONTLIBRARY_API FontsLibrary
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
class S2FONTLIBRARY_API Font : public std::enable_shared_from_this<Font>
{
private:
	std::string _name;

	void *_font;
	void *_atlas;

	friend class FontLoader;
	friend class FontsLibrary;

	//bool load();

protected:
	//void  draw( const std::wstring &text );

public:
	struct Glyph
	{
		int id;

		int   offset_x;
		int   offset_y;
		int   width;
		int   height;
		float s0;
		float t0;
		float s1;
		float t1;	
	};

	Font( const std::string &fontName, unsigned int fontSize=0 );
	virtual ~Font();

	std::vector<Glyph> rasterizeText( const std::wstring &text );

	unsigned int atlas() const;      
	std::string  name() const { return _name; }

};

}

#endif
