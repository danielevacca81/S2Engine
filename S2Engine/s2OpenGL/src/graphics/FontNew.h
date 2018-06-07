// FontNew.h
//
#ifndef FONTNEW_ONCE
#define FONTNEW_ONCE

#include "S2RendererAPI.h"

#include "Renderer/Renderer.h"

// TODO: usare FTGL + freetype255
//http://ftgl.sourceforge.net/docs/html/ftgl-tutorial.html

class FTFont;


namespace s2 {

//class Font;
//typedef std::shared_ptr<Font> FontPtr;

//#include <map>
//#include <string>
//
//using namespace std;

//typedef map<string, FTFont*> FontList;
//typedef FontList::const_iterator FontIter;

class S2RENDERER_API FTGLFontManager
{
    public:
        static FTGLFontManager& Instance();
        ~FTGLFontManager();

        FTFont* GetFont(const char *filename, int size);


    private:
        // Hide these 'cause this is a singleton.
        FTGLFontManager(){}
        FTGLFontManager(const FTGLFontManager&){};
        FTGLFontManager& operator = (const FTGLFontManager&){ return *this; };

        // container for fonts
        //FontList fonts;
};

}

#if 0
/************************************************************************/
/*                         Fonts Library class                          */
/************************************************************************/
class S2RENDERER_API FontsLibrary
{
public:
	typedef std::map<std::string,FontPtr> FontCollection;

private:	
	static FontCollection _fonts;
	static FontsLibrary  *_library;
	static void          *_fontManager;
	
	FontsLibrary();
	~FontsLibrary();

	void makeFont( void * );

public:
	static bool    init( const std::wstring &path );
	static FontPtr font( const std::string &fontName );
	static bool    contains( const std::string &fontName );
};

/************************************************************************/
/*                              Font class                              */
/************************************************************************/
class S2RENDERER_API Font : public std::enable_shared_from_this<Font>
{
private:
	void*       _font;
	std::string _name;


	friend class FontLoader;
	friend class FontsLibrary;

protected:

public:	
	//Font( const std::string &fontName = std::string(""), int fontSize=0 );
	Font();
	~Font();

	void  draw( const std::wstring &text,  OpenGL::Renderer *r );

	std::string name() const { return _name; }
};

}

#endif
#endif