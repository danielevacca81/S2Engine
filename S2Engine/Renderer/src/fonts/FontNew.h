// FontNew.h
//
#if 0
#ifndef FONTNEW_ONCE
#define FONTNEW_ONCE

#include "Renderer_API.h"

#include <string>
#include <vector>
#include <map>
#include <memory>

class Font;
typedef std::shared_ptr<Font> FontPtr;

/************************************************************************/
/*                         Fonts Library class                          */
/************************************************************************/
class RENDERER_API FontsLibrary
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
class RENDERER_API Font : public std::enable_shared_from_this<Font>
{
private:
	struct Glyph
	{
		unsigned int texID;
		
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

	std::vector<Glyph> _glyphs;


	std::string _name;

	void   *_face;
	//void *_font;
	//void *_atlas;

	friend class FontLoader;
	friend class FontsLibrary;

	//bool load();

protected:
	//void  draw( const std::wstring &text );
	void buildGlyphs();

public:

	Font( const std::string &fontName, unsigned int fontSize=0 );
	virtual ~Font();

	std::vector<Glyph> rasterizeText( const std::wstring &text );
	//unsigned int atlas() const;
	
	std::string  name() const { return _name; }

};


#endif

#endif
