// Font.h
//
#ifndef FONT_ONCE
#define FONT_ONCE

#include "S2RendererAPI.h"

#include "OpenGL/Texture.h"

#include <string>
#include <vector>
#include <map>

namespace s2 {

class Font;
typedef std::shared_ptr<Font> FontPtr;

/************************************************************************/
/*                         Abstract font Loader                         */
/************************************************************************/
class S2RENDERER_API IFontLoader
{
public:
	IFontLoader()	{}
	virtual ~IFontLoader()	{}

	virtual FontPtr  load( const std::wstring &fntFileName ) = 0;
};


/************************************************************************/
/*                              Font Loader                             */
/************************************************************************/
class S2RENDERER_API FontLoader : public IFontLoader
{

public:
	FontLoader()	{}
	virtual ~FontLoader()	{}

	FontPtr  load( const std::wstring &fntFileName );
};

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
	struct Char
	{
		unsigned int channel;
		short id;
		short x,y;
		short w,h;

		short xOff,yOff;
		short xAdv,page;

		std::vector<int> kernings;
	};

	friend class FontLoader;
	friend class FontsLibrary;

protected:
	int                         _size;
	std::string                 _name;
	std::string                 _texFilename;
	std::map<unsigned int,Char> _glyphs;
	OpenGL::Texture2DPtr        _texture;

	float adjustForKerningPairs( const Char &ch1, const Char &ch2 ) const;
	void  draw( const std::wstring &text );

public:
	struct TextSize
	{
		float width;
		float height;
	};
	
	Font( const std::string &fontName = std::string(""), int fontSize=0 );
	virtual ~Font();

	
	TextSize    textSize( const std::wstring &str ) const;
	std::string name() const { return _name; }
};

}

#endif
