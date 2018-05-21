// Font.h
//
#ifndef FONT_ONCE
#define FONT_ONCE

#include "CoreAPI.h"

#include "OpenGL.h"
#include "Texture.h"

#include <string>
#include <stack>
#include <map>

namespace OpenGL {


//---------------------------------------------------------------------------------------------
/** 
	* Abstract font base class
	*/
class S2CORE_API Font
{
protected:
	double _size;
	std::stack<double> _stackSize;

	std::string _name;

public:
	Font( const std::string &fontName )	: _name(fontName) {}
	virtual ~Font()	{}

	virtual void    bind(){};
	virtual void    unbind(){};
	virtual void	pushSize();
	virtual void	popSize();
	virtual void	setSize(double pixels);
	virtual double   getSize() const;
	virtual double   getHeight() const;

	//virtual void	printXY(double x, double y, const std::string &str) = 0;
	//virtual void	draw(const std::string &str) = 0;
	virtual void	drawText(const std::string &str) = 0;
	virtual double	getTextWidth(const std::string &str, double size, bool monospaced) const = 0;
	
	std::string     name() const { return _name; }		
};

//---------------------------------------------------------------------------------------------
/**
	* Textured font class
	*/
class S2CORE_API FontTexture : public Font
{
public:
	enum FontOverlayMode{OVERLAY_BLEND, OVERLAY_MASK};
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

protected:
	GLuint	fontDL;

	bool	loaded;

	Texture2D     texture;

	double internalScaleFactor;

	std::map<int,Char> chars;

	bool  loadTexture(const std::string &texName);

	bool  loadINI(const std::string &filename);
	void  build(int xCount,int yCount, int align);
													
	bool  loadFNT(const std::string &filename);
	void  build( int pixelSize );

	double adjustForKerningPairs( const Char &ch1, const Char &ch2 ) const;

public:
	FontTexture( const std::string &fontName );
	~FontTexture();

	void bind();
	void unbind();
	
	bool load(const std::string &filename);
	void destroy();

	void setSize(double pixels);
	//void setOverlayMode(FontOverlayMode m);

	//void  draw(const std::string &str);
	void  drawText(const std::string &str);
	//void  printXY(double x, double y, const std::string &str);
	double getTextWidth(const std::string &str, double size, bool monospaced ) const;
	double getHeight() const;

	bool  isLoaded() const {return loaded;}
};

}

#endif