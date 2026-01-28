// Font.cpp
//
#include "Font.h"

#include "Math/Math.h"

#include "OpenGL/VertexBuffer.h"
#include "Renderer/RenderState.h"
#include "Renderer/Renderer.h"

#include "Common/StringTools.h"

#include <fstream>
#include <sstream>

using namespace s2;

// Global fonts collection
FontsLibrary::FontCollection FontsLibrary::_fonts;

// ------------------------------------------------------------------------------------------------
template <typename T>
bool parse( const std::string &str,const std::string &cmd, T &t )
{
	if( str.empty() || cmd.empty() )
		return false;
	
	const std::string before = cmd.substr( 0,cmd.find_first_of('%') );
	
	std::string after  = cmd.substr( cmd.find_last_of('%')+1 );
	if( after.empty() )
		after = " ";

	std::string v = str.substr( str.find(before)+before.length() );
	v = v.substr( 0, v.find_first_of(after) );
	
	std::stringstream ss(v);
	ss >> t;

	return true;
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
FontPtr FontLoader::load( const std::wstring &filename )
{
	std::ifstream f;
	f.open( filename );
	if( !f.is_open() )	return nullptr;

	std::string line;
	
	// 1] info
	// "info face=%s size=%d", //bold=%d italic=%d charset=%s unicode=%d stretchH=%d smooth=%d aa=%d padding=%d,%d,%d,%d spacing=%d,%d outline=%d\n"
	std::getline( f, line );
	std::string face;  parse( line, "face=\"%\"", face);
	int pxlSize;	   parse( line, "size=%",     pxlSize);
	
	// 2] common
	// "common lineHeight=%d base=%d scaleW=%d scaleH=%d pages=%d\n"
	std::getline( f, line );
	int lineH;     parse( line, "lineHeight=%", lineH );
	int base;	   parse( line, "base=%",       base  );
	int W;	       parse( line, "scaleW=%",     W  );
	int H;	       parse( line, "scaleH=%",     H  );
	int pages;     parse( line, "pages=%",      pages  );

	// 3] page
	// "page id=%d file=%s\n"
	std::getline( f, line );
	int pag;           parse( line, "id=%",   pag  );
	std::string name;  parse( line, "file=\"%\"", name );

	// 4] chars
	// "chars count=%d\n"
	std::getline( f, line );
	int chCount; parse( line, "count=%", chCount );

	if( chCount <= 0 )
	{
		f.close();
		return false;
	}


	// 5] char entries
	// "char id=%d%[ ]x=%d%[ ]y=%d%[ ]width=%d%[ ]height=%d%[ ]xoffset=%d%[ ]yoffset=%d%[ ]xadvance=%d%[ ]page=%d%[ ] chnl=%d\n"
	FontPtr fnt       = std::make_shared<Font>(face);
	fnt->_texFilename = name;

	int i       = 0;
	while(i < chCount && !f.eof() )
	{
		Font::Char x;
		std::getline( f, line );
		parse( line, "id=%",       x.id      );
		parse( line, "x=%",        x.x       );
		parse( line, "y=%",        x.y       );
		parse( line, "width=%",    x.w       );
		parse( line, "height=%",   x.h       );
		parse( line, "xoffset=%",  x.xOff    );
		parse( line, "yoffset=%",  x.yOff    );
		parse( line, "xadvance=%", x.xAdv    );
		parse( line, "page=%",     x.page    );
		parse( line, "chnl=%",     x.channel );

		fnt->_glyphs[x.id] = x;
		++i;
	}

	// 6] kernings
	// "kernings count=%d\n"
	std::getline( f, line );
	int kCount; parse( line, "count=%", kCount );

	// 7] kernings entries
	// "kerning first=%d%[]second=%d%[]amount=%d\n"
	int k=0;
	while( k<kCount && !f.eof()  )
	{
		std::getline( f, line );

		int first;  parse( line, "first=%",  first  );
		int second; parse( line, "second=%", second );
		int amount; parse( line, "amount=%", amount );

		if( fnt->_glyphs.find(first) != fnt->_glyphs.end() )
		{
			fnt->_glyphs[first].kernings.push_back( second );
			fnt->_glyphs[first].kernings.push_back( amount );
		}
		++k;
	}

	f.close();

	//if( chCount != fnt->_glyphs.size() )
	//	return nullptr;

	//std::string tmp(name);
	//std::string path        = filename.substr( 0, filename.find_last_of("/\\")+1 );
	//std::string texFileName = tmp.substr(tmp.find("\"")+1,tmp.find_last_of("\"")-1 );	// remove both " from the filename

	//  if( !loadTexture(path+texFileName) )
	//  {
	//	chars.clear();
	//		return false;
	//	}

	//	build(pxlSize);

	return fnt;
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
bool FontsLibrary::init( const std::wstring &path )
{
	std::wstring defaultFontFile(L"arial_small.fnt");

	FontLoader fntLoader;

	// @TODO: for each file in path
	FontPtr defaultFont = fntLoader.load( path + L"/" + defaultFontFile );


	if( defaultFont )
	{
		_fonts[defaultFont->name()] = defaultFont;
		//  if( !loadTexture(path+texFileName) )
		//  {
		//	chars.clear();
		//		return false;
		//	}
	}





	return true;
}

// -------------------------------------------------------------------------------------------------
FontPtr FontsLibrary::font( const std::string &fontName, int size )
{
	FontCollection::iterator it = _fonts.find(fontName);
	if( it == _fonts.end() )
		return nullptr;
	
	return it->second;
}

// -------------------------------------------------------------------------------------------------
bool FontsLibrary::contains( const std::string &fontName )
{
	return _fonts.find(fontName) != _fonts.end();
}


// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
Font::Font( const std::string &fontName, int fontSize )
: _name(fontName)
, _size(fontSize)
{
	if( FontsLibrary::contains(fontName) )
	{
		FontPtr fnt = FontsLibrary::font(fontName,fontSize);

		_texture = fnt->_texture;
		_glyphs  = fnt->_glyphs;
	}
}

// -------------------------------------------------------------------------------------------------
Font::~Font()
{}

// ------------------------------------------------------------------------------------------------
Font::TextSize    Font::textSize( const std::wstring &str ) const
{
	float w = 0.f;

	for( size_t j=0; j<str.size(); ++j )
	{
		Char ch = _glyphs.at( str.at(j) );

		w += ch.xAdv-ch.xOff;

		// handle kernings
		//if( j+1<str.size() )
		//{
		//	std::map<int,Char>::const_iterator jt = chars.find( str.at(j+1) );

		//	if( jt == chars.end() )
		//		continue; // unknown char ?

		//	w += adjustForKerningPairs( it->second, jt->second );
		//}
	}

	TextSize sz;
	sz.height = (_size * 1.0f);
	sz.width  =  w * _size;

	return sz;
}

// -------------------------------------------------------------------------------------------------
float Font::adjustForKerningPairs( const Char &ch1, const Char &ch2 ) const
{
	int amount = 0;
	for( size_t i=0; i<ch1.kernings.size(); i+=2 )
		if( ch1.kernings[i] == ch2.id )
		{
			amount = ch1.kernings[i+1];
			break;
		}

	return (float)amount;
}

// ------------------------------------------------------------------------------------------------
void Font::draw( const std::wstring &text )
{
	std::vector<Math::vec2> uv;
	std::vector<Math::dvec3> xy;

	int xOffset = 0;
	int yOffset = 0;

	OpenGL::VertexBuffer vb;
	for( size_t i=0; i<text.size(); ++i )
	{
		const unsigned int t =  text[i];
		const Char ch        = _glyphs[t];

		if( ch.w<=0 || ch.h<=0 )
			continue;

		int u[4],v[4];
		int x[4],y[4];

		u[0] = (ch.x + 0   );
		u[1] = (ch.x + ch.w);
		u[2] = (ch.x + ch.w);
		u[3] = (ch.x + 0   );

		v[0] = (ch.y + ch.h);
		v[1] = (ch.y + ch.h);
		v[2] = (ch.y + 0   );
		v[3] = (ch.y + 0   );
		uv.push_back( Math::vec2(u[0],v[0]) );
		uv.push_back( Math::vec2(u[1],v[1]) );
		uv.push_back( Math::vec2(u[2],v[2]) );
		uv.push_back( Math::vec2(u[3],v[3]) );

		x[0] = xOffset + 0;
		x[1] = xOffset + ch.w;
		x[2] = xOffset + ch.w;
		x[3] = xOffset + 0;
		
		y[0] = yOffset + ch.yOff + ch.h;
		y[1] = yOffset + ch.yOff + ch.h;
		y[2] = yOffset + ch.yOff + 0;
		y[3] = yOffset + ch.yOff + 0;
		xy.push_back( Math::dvec3(x[0],y[0],0.0) );
		xy.push_back( Math::dvec3(x[1],y[1],0.0) );
		xy.push_back( Math::dvec3(x[2],y[2],0.0) );
		xy.push_back( Math::dvec3(x[3],y[3],0.0) );

		vb << OpenGL::Vertex(xy[0],Math::vec3(),uv[0]) 
		   << OpenGL::Vertex(xy[1],Math::vec3(),uv[1]) 
		   << OpenGL::Vertex(xy[2],Math::vec3(),uv[2]) 
		   << OpenGL::Vertex(xy[3],Math::vec3(),uv[3]);

		xOffset = (ch.xAdv - ch.xOff);
		yOffset = 0;

		//glTexCoord2d(u,1.0-v);
		//glVertex2d(x,-y);

		//glTexCoord2d(u,1.0-v);
		//glVertex2d(x,-y);

		//glTexCoord2d(u,1.0-v);
		//glVertex2d(x,-y);

		//glTexCoord2d(u,1.0-v);
		//glVertex2d(x,-y);
	}

	//RenderState rs;

	//r.draw( OpenGL::Quads, rs );
}

#if 0
//-------------------------------------------------------------------------------------------------
// Font
//-------------------------------------------------------------------------------------------------
void   Font::setSize(double pixels){_size = pixels;}
double Font::getSize() const	{return _size;}
double Font::getHeight() const	{return _size;}
void   Font::pushSize()	{_stackSize.push(_size);}
void   Font::popSize()	{_size = _stackSize.top(); _stackSize.pop();}

//-------------------------------------------------------------------------------------------------
// FontTexture
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
FontTexture::FontTexture( const std::string &fontName )
: Font(fontName)
{
	fontDL	            = -1;
	_size               = 1.f;
	internalScaleFactor = 1.f;
	loaded              = false;
}

//-------------------------------------------------------------------------------------------------
FontTexture::~FontTexture()
{
	destroy();
}

//-------------------------------------------------------------------------------------------------
void FontTexture::destroy()
{
	texture.destroy();
	glDeleteLists( fontDL, chars.size() );

	chars.clear();
	loaded = false;
	fontDL = -1;
}

//-------------------------------------------------------------------------------------------------
void FontTexture::setSize(double pixels)
{
	_size = ((double)pixels * internalScaleFactor);
}

//-------------------------------------------------------------------------------------------------
bool FontTexture::loadTexture(const std::string &filename)
{
	Image img;
	if( !ImageLoader::load( filename, img ) )
		return false;

	int numCh = img.numChannels();

	if(numCh != 3 && numCh != 4)
		return false;

	GLenum fmt = (numCh == 3) ? GL_RGB : GL_RGBA;
	GLenum t   = GL_UNSIGNED_BYTE;

	//texture.createMipMap(numCh,fmt,t,img.width(), img.height(),img.pixels());
	texture.create(numCh,fmt,t,false,false,img.width(),img.height(),img.pixels());
	//texture.setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//texture.setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//texture.setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	texture.setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	texture.setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;
}



//-------------------------------------------------------------------------------------------------
void FontTexture::build(int pixelSize)
{
	const double invW    = 1.0 / texture.width();
	const double invH    = 1.0 / texture.height();
	internalScaleFactor = fabs(1.0 / (double) pixelSize);

	fontDL   = glGenLists(chars.size());

	for( std::map<int,Char>::iterator it=chars.begin();
		 it != chars.end();
		 ++it )
	{
		double x,y,u,v;
		glNewList(/*fontDL+*/it->first, GL_COMPILE);

		const Char &ch = it->second;

		if( ch.w )
		{
			glBegin(GL_TRIANGLE_STRIP); //inv N
			// 0]
			u = (ch.x + 0) * invW;
			v = (ch.y + ch.h) * invH;
			x = 0;			//x = ch.xOff + 0;
			/*y = ch.h;*/ y =ch.yOff + ch.h;
			glTexCoord2d(u,1.0-v);
			glVertex2d(x,-y);

			// 3]
			u = (ch.x + 0) * invW;
			v = (ch.y + 0) * invH;
			x = 0;	//x = ch.xOff + 0;
			/*y = 0;*/	y = ch.yOff + 0;
			glTexCoord2d(u,1.0-v);
			glVertex2d(x,-y);

			// 1]
			u = (ch.x + ch.w) * invW;
			v = (ch.y + ch.h) * invH;
			x = ch.w; //x = ch.xOff + ch.w;
			/*y = ch.h;*/ y = ch.yOff + ch.h;
			glTexCoord2d(u,1.0-v);
			glVertex2d(x,-y);

			// 2]
			u = (ch.x + ch.w) * invW;
			v = (ch.y + 0) * invH;
			x = ch.w;	//x = ch.xOff + ch.w;
			/*y = 0;*/		y = ch.yOff + 0;
			glTexCoord2d(u,1.0-v);
			glVertex2d(x,-y);

			glEnd();
			glTranslatef((ch.xAdv - ch.xOff), 0,0);
		}
		else
			glTranslated( 1.0/internalScaleFactor, 0,0);

		glEndList();
	}
}


//-------------------------------------------------------------------------------------------------
void FontTexture::bind()
{
	glPushAttrib( GL_ENABLE_BIT );
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	texture.bind();
}

//-------------------------------------------------------------------------------------------------
void FontTexture::unbind()
{
	glPopAttrib();
	texture.unbind();
}

// -------------------------------------------------------------------------------------------------
void FontTexture::drawText(const std::string &str)
{
	//switch(overlayMode)
	//{
	//case OVERLAY_MASK:			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	break;
	//case OVERLAY_BLEND:			glBlendFunc(GL_SRC_ALPHA, GL_ONE);					break;
	//}
	glPushAttrib( GL_LIST_BIT );

	glPushMatrix();
		glScaled(_size,_size,1.0);
		glCallLists((GLsizei)str.size(),GL_UNSIGNED_BYTE,(const unsigned char *)str.c_str());
	glPopMatrix();

	glPopAttrib();
}


// -------------------------------------------------------------------------------------------------
double FontTexture::getTextWidth( const std::string &str, double size, bool monospaced ) const
{
	double w = 0.f;

	if( monospaced )
		w = getHeight()*str.size()*0.5f;
	else
	{
		for( size_t j=0; j<str.size(); ++j )
		{
			std::map<int,Char>::const_iterator it = chars.find( str.at(j) );

			if( it == chars.end() )
				continue; // unknown char ?

			const Char &ch = it->second;

			w += ch.xAdv-ch.xOff;

			// handle kernings
			//if( j+1<str.size() )
			//{
			//	std::map<int,Char>::const_iterator jt = chars.find( str.at(j+1) );

			//	if( jt == chars.end() )
			//		continue; // unknown char ?

			//	w += adjustForKerningPairs( it->second, jt->second );
			//}
		}
	}

	return w * size * internalScaleFactor;
}

//-------------------------------------------------------------------------------------------------
double FontTexture::getHeight() const
{
	return (_size * 1.0f) / internalScaleFactor;
}
#endif