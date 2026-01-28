// Font.cpp
//
#include "Font.h"


#include "Image.h"
#include "ImageLoader.h"

#include <fstream>
#include <vector>

#include "Utils/MemCheck.h"

using namespace OpenGL;

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
	texture.create(0,numCh,fmt,t,false,img.width(),img.height(),img.pixels());
	//texture.setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//texture.setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//texture.setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	texture.setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	texture.setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	return true;
}

//-------------------------------------------------------------------------------------------------
bool FontTexture::loadFNT(const std::string &filename)
{
	if(loaded) destroy();

	FILE *f = fopen(filename.c_str(),"rt");
	if(!f)	return false;


	// 1] 
	char face[64];
	int pxlSize;
	fscanf(f,"info face=%s size=%d", //bold=%d italic=%d charset=%s unicode=%d stretchH=%d smooth=%d aa=%d padding=%d,%d,%d,%d spacing=%d,%d outline=%d\n"
		face,
		&pxlSize);
	while(fgetc(f) != '\n');


	// 2]
	int lineH,base,W,H,pages;
	fscanf(f, "common lineHeight=%d base=%d scaleW=%d scaleH=%d pages=%d", &lineH, &base, &W, &H, &pages);
	while(fgetc(f) != '\n');
	
	// 3]
	int pag;
	char name[256];
	fscanf(f, "page id=%d file=%s\n", &pag, name );
	
	// 4]
	int chCount = 0;
	fscanf(f, "chars count=%d\n", &chCount );

	// 5]
	char spaces[32];	
	bool failed = false;
	int i = 0;
	while(i < chCount && !feof(f) && !failed )
	{
		Char x;
		int bytes = fscanf(f, "char id=%d%[ ]x=%d%[ ]y=%d%[ ]width=%d%[ ]height=%d%[ ]xoffset=%d%[ ]yoffset=%d%[ ]xadvance=%d%[ ]page=%d%[ ] chnl=%d\n",
			&x.id,        spaces,
			&x.x,       spaces,
			&x.y,       spaces,
			&x.w,       spaces,
			&x.h,       spaces,
			&x.xOff,    spaces,
			&x.yOff,    spaces,
			&x.xAdv,    spaces,
			&x.page,    spaces,
			&x.channel/*, spaces*/);
		
		if ( bytes != 19 )
		{
			failed = true;
			break;
		}
		else
		{
			chars[x.id] = x;
			++i;
		}
	}
	
	// 6] kernings?
	int kCount = 0;
	fscanf(f, "kernings count=%d\n", &kCount );
	int k=0;
	while( k<kCount && !feof(f) && !failed )
	{
		int first;
		int second;
		int amount;
		
		fscanf(f, "kerning first=%d%[]second=%d%[]amount=%d\n",
			&first, spaces,
			&second, spaces,
			&amount/*, spaces*/);

		if( first >= 0 && first < 256 )
		{
			chars[first].kernings.push_back( second );
			chars[first].kernings.push_back( amount );
		}
		++k;
	}

	fclose(f);

	if( chCount != chars.size() )
		return false;

	std::string tmp(name);
	std::string path        = filename.substr( 0, filename.find_last_of("/\\")+1 );
	std::string texFileName = tmp.substr(tmp.find("\"")+1,tmp.find_last_of("\"")-1 );	// remove both " from the filename
	
	if( !loadTexture(path+texFileName) )
	{
		chars.clear();
		return false;
	}

	build(pxlSize);
	loaded = true;
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
bool FontTexture::loadINI(const std::string &filename)
{	
	if(loaded) destroy();

	std::ifstream f;
	
	f.open(filename);
	if( !f.is_open() )
		return false;

	std::string attr;
	std::string texName;
	std::string align;
	int numCols,numRows;

	f >> attr;				// Font name
	f >> attr >> texName;	// texture filename
	f >> attr >> numRows;	// char per row
	f >> attr >> numCols;	// char per col
	f >> attr >> align;		// alignment (Center,Left,Right)
	f >> attr;				// Widths

	int numChar    = numCols * numRows;
	//chars.resize(numChar);
	//charWidths = new short[numChar];

	//int i, width;
	while( !f.eof() )
	{
		Char ch;
		int id;
		f >> id >> ch.w;
		
		chars[id] = ch; 
	}
	f.close();

	//std::string ss = filename.substr( 0, filename.find_last_of("/")+1 );

	//if(! loadTexture( ss+texName ) )
	//{
	//	chars.clear();
	//	return false;
	//}
			
	int alignMode = -1;
	if(align.compare("Left")   == 0) alignMode = 0;
	if(align.compare("Center") == 0) alignMode = 1;
	if(align.compare("Right")  == 0) alignMode = 2;

	build(numCols,numRows,alignMode);

	loaded = true;
	//fontDLBase  = 0;
	return true;
}

//-------------------------------------------------------------------------------------------------
void FontTexture::build(int xCount, int yCount,int align)
{	
	// Determina la dimensione dei caratteri
	// sapendo che sono xCount in riga e yCount in colonna
	int chWidth		= texture.width()  / xCount;
	int chHeight	= texture.height() / yCount;

	internalScaleFactor = 1.f / std::max(chHeight,chWidth);

	setSize( (double)std::max(chHeight,chWidth) );

	fontDL   = glGenLists( chars.size() );
	
	// Dimensioni del singolo carattere (sono tutti uguali)
	// Rappresentano le dimensioni del boundingBox che
	// racchiude un carattere
	const double cwx = (1.f/chars.size())*xCount;
	const double cwy	= (1.f/chars.size())*yCount;
	
	for( size_t i=0; i<chars.size(); ++i )
	{
		// Start x position at LEFT
		const double currX = double(i%xCount) * cwx;

		// Start y position at TOP
		const double currY = 1.f - (double(i/yCount) * cwy) - cwy;

		glNewList(fontDL+i, GL_COMPILE);
		{
			// pre-translation needed?
			if(chars[i].w >= 0)
			{
				switch(align)
				{
				case 0:	 break;
				case 1:	glTranslatef(-.5f * (chWidth-chars[i].w),0,0);	break;
				case 2:	glTranslatef(chars[i].w,0,0);					break;
				}			
			}

			glBegin(GL_TRIANGLE_STRIP); // inv N
				glTexCoord2d(currX,     currY+cwy);	glVertex2i(0,chHeight);
				glTexCoord2d(currX,     currY    ); glVertex2i(0,0);
				glTexCoord2d(currX+cwx, currY+cwy);	glVertex2i(chWidth,chHeight);
				glTexCoord2d(currX+cwx, currY    );	glVertex2i(chWidth,0);
			glEnd();

			// Traslazione pari alla larghezza del carattere corrente (se presente)
			if(chars[i].w >= 0)
			{
				switch(align)
				{
				case 0:	glTranslatef(chars[i].w,0,0); break;
				case 1:	glTranslatef(chars[i].w + (.5f * (chWidth-chars[i].w)),0,0);	break;
				case 2:	break;
				}
			}
			else
			{
				// Altrimenti uso larghezza di default
				// Pari a meta' larghezza di una cella
				glTranslatef(chWidth*.5f,0,0);
				chars[i].w = chWidth >> 1;
			}
		}
		glEndList();
	}
}

//-------------------------------------------------------------------------------------------------
bool FontTexture::load(const std::string &fontFileName)
{
	if( fontFileName.find(".fnt") != std::string::npos )		return loadFNT(fontFileName);
	else if( fontFileName.find(".ini") != std::string::npos )	return loadINI(fontFileName);
	else return false;
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

//-------------------------------------------------------------------------------------------------
// Esegue l'output di una stringa
//void FontTexture::printXYf(double x, double y, const std::string &s)
//{
//	printXYd(x,y,s);
//}

//-------------------------------------------------------------------------------------------------
//void FontTexture::printXY(double x, double y, const std::string &s)
//{
//	if(immediate)
//	{
//		glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT | GL_LIST_BIT);
//		glEnable(GL_BLEND);
//		glEnable(GL_TEXTURE_2D);
//		glDisable(GL_DEPTH_TEST);
//		glDisable(GL_LIGHTING);
//
//		texture.bind();	// Seleziona la texture che contiene i caratteri
//	}
//#if	0
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glBegin(GL_QUADS);
//		glTexCoord2f(0,0);	glVertex2f(0,0);
//		glTexCoord2f(1,0);	glVertex2f(300,0);
//		glTexCoord2f(1,1);	glVertex2f(300,300);
//		glTexCoord2f(0,1);	glVertex2f(0,300);
//	glEnd();
//#else
//	switch(overlayMode) 
//	{
//	case OVERLAY_MASK:			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	break;
//	case OVERLAY_BLEND:			glBlendFunc(GL_SRC_ALPHA, GL_ONE);					break;
//	}
//
//	glPushMatrix();
//		glTranslated(x,y,0.0);
//		glScalef(size,size,1.0);
//		glCallLists((GLsizei)s.size(),GL_UNSIGNED_BYTE,(const unsigned char *)s.c_str());
//	glPopMatrix();
//#endif
//
//	if(immediate)
//	{
//		glPopAttrib();
//	}
//}

//-------------------------------------------------------------------------------------------------
//void FontTexture::draw(const std::string &str)
//{
//	glPushAttrib( GL_ENABLE_BIT | GL_LIST_BIT );
//	
//	glEnable(GL_TEXTURE_2D);
//	glEnable(GL_BLEND);
//	texture.bind();
//
//	switch(overlayMode) 
//	{
//	case OVERLAY_MASK:			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	break;
//	case OVERLAY_BLEND:			glBlendFunc(GL_SRC_ALPHA, GL_ONE);					break;
//	}
//	
//	glPushMatrix();
//		glScalef(size,size,1.0);
//		glCallLists((GLsizei)str.size(),GL_UNSIGNED_BYTE,(const unsigned char *)str.c_str());
//	glPopMatrix();	
//	
//	glPopAttrib();
//}

//-------------------------------------------------------------------------------------------------
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


//-------------------------------------------------------------------------------------------------
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
double FontTexture::adjustForKerningPairs(const Char &ch1,const Char &ch2) const 
{
	double amount = 0.0;
	for( size_t i=0; i<ch1.kernings.size(); i+=2 )
		if( ch1.kernings[i] == ch2.id )
		{
			amount = ch1.kernings[i+1];
			break;
		}
	
	return amount;
}

//-------------------------------------------------------------------------------------------------
double FontTexture::getHeight() const
{
	return (_size * 1.0f) / internalScaleFactor;
}