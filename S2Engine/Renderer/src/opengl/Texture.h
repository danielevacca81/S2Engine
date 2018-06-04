// Texture.h
//
#ifndef TEXTURE_ONCE
#define TEXTURE_ONCE

#include "Renderer_API.h"

#include <memory>

namespace OpenGL {

// ------------------------------------------------------------------------------------------------
class Texture;
class Texture1D;
class Texture2D;

typedef std::shared_ptr<Texture>   TexturePtr;
typedef std::shared_ptr<Texture1D> Texture1DPtr;
typedef std::shared_ptr<Texture2D> Texture2DPtr;

/************************************************************************************************/
/*                                             Texture                                          */
/************************************************************************************************/
class RENDERER_API Texture : public std::enable_shared_from_this<Texture>
{
public:
	// -----------------------------------------------------------------------------------------------
	enum  DataFormat
	{
		InvalidFormat  = -1,
		Red, 
		RG,
		RGB,
		BGR,
		RGBA,
		BGRA,
		RedInteger,
		RGInteger,
		RGBInteger,
		BGRInteger,
		RGBAInteger,
		BGRAInteger,
		StencilIndex,
		DepthComponent,
		DepthStencil,
	};

	// -----------------------------------------------------------------------------------------------
	enum  DataType
	{
		InvalidDataType = -1,
		UByte_8,
		Byte_8,
		UShort_16,
		Short_16,
		UInt_32,
		Int_32,
		Float_32,
		UByte_3_3_2,
		UByte_2_3_3_r,
		UShort_5_6_5,
		UShort_5_6_5_r,
		UShort_4_4_4_4,
		UShort_4_4_4_4_r,
		UShort_5_5_5_1,
		UShort_1_5_5_5_r,
		UInt_8_8_8_8,
		UInt_8_8_8_8_r,
		UInt_10_10_10_2,
		UInt_2_10_10_10_r,
	};

	// -----------------------------------------------------------------------------------------------
	enum  TextureType
	{
		Texture_1D,
		Texture_2D,
	};

	// -----------------------------------------------------------------------------------------------
	enum  TextureMinFilter
	{
		InvalidMinFilter = -1,
		MinFilterNearest,
		MinFilterNearestWithMipmaps,
		MinFilterNearestWithBilinearMipmaps,
		MinFilterBilinear,
		MinFilterBilinearWithMipmaps,
		MinFilterTrilinear,
	};

	// -----------------------------------------------------------------------------------------------
	enum  TextureMagFilter
	{
		InvalidMagFilter = -1,
		MagFilterNearest,
		MagFilterBilinear,
	};

	// -----------------------------------------------------------------------------------------------
	enum  WrapMode
	{
		InvalidWrapMode = -1,
		Clamp,
		Repeat,
		MirroredRepeat,
	};

public:
	Texture();
	virtual ~Texture();

	// Create empty Texture
	virtual void	create();
	virtual void	destroy();

	// Custom methods
	virtual void	setMinFilter( const TextureMinFilter &minFilter );
	virtual void	setMagFilter( const TextureMagFilter &magFilter );
	virtual void	setWrapMode ( const WrapMode &wrap );

	virtual void	bind(unsigned int unit = 0)		const;
	virtual void	unbind()	const;
	virtual bool	isValid()	const;
	virtual bool    isBound()   const;

	virtual unsigned int unit()   const;
	virtual int          width()  const;
	virtual int          height() const;

	unsigned int	 id()	        const;
	DataFormat       dataFormat()   const;
	DataType         dataType()     const;
	TextureMinFilter minFilter()    const;
	TextureMagFilter magFilter()    const;
	WrapMode         wrap()         const;
	unsigned int     channelCount() const;

	virtual TextureType	type() const = 0;

protected:
	virtual void setDefaultFilterParameters()
	{};

protected:
	unsigned int     _textureID;
	DataFormat       _dataFormat;
	DataType         _dataType;
	TextureMinFilter _minFilter;
	TextureMagFilter _magFilter;
	WrapMode         _wrap;
	unsigned int     _channelCount;


	bool             _valid;
	mutable int      _unit;
};


/************************************************************************************************/
/*                                          Texture1D                                           */
/************************************************************************************************/
class RENDERER_API Texture1D : public Texture
{
public:	
	static Texture1DPtr New() { return std::make_shared<Texture1D>(); }
	static Texture1DPtr New(int channelCount, const DataFormat &format, const DataType &type,bool mipmaps,int width,void *pixels)
	{ Texture1DPtr t = std::make_shared<Texture1D>(); t->create(channelCount,format,type,mipmaps,width,pixels); return t;}


	Texture1D();

	TextureType	type()	const;

	void create(
		int channelCount,
		const DataFormat &format,
		const DataType &type,
		bool mipmaps,
		int   width,
		void *pixels);

	void clear();

	virtual int     width()     const { return _textureWidth; }
	virtual int     height()    const { return 1; }

protected:
	int _textureWidth;
};


/************************************************************************************************/
/*                                          Texture2D                                           */
/************************************************************************************************/
class RENDERER_API Texture2D : public Texture
{
public:	
	static Texture2DPtr New() { return std::make_shared<Texture2D>(); }
	static Texture2DPtr New(int channelCount, const DataFormat &format, const DataType &type,bool mipmaps, bool border,int width,int height,void *pixels)
	{ Texture2DPtr t = std::make_shared<Texture2D>(); t->create(channelCount,format,type,mipmaps,width,height,pixels); return t;}

	Texture2D();

	float       u()      const;
	float       v()      const;
	virtual int width()  const;
	virtual int height() const;
	TextureType type()   const;

	// -----------------------------------------------------------------------------------------------
	void create(
		int channelCount,
		const DataFormat &format,
		const DataType &type,
		bool mipmaps,
		int width,
		int height,
		void *pixels);

	// -----------------------------------------------------------------------------------------------
	void createFromFrameBuffer( int width, int height );

	// -----------------------------------------------------------------------------------------------
	void update(int xOffset,
		int yOffset,
		int width,
		int height,
		void *pixels);

	// -----------------------------------------------------------------------------------------------
	void clear();

	// -----------------------------------------------------------------------------------------------
	void setU( int w )   { _u = w/(float)_textureWidth;  }
	void setV( int h )   { _v = h/(float)_textureHeight; }

protected:
	void setDefaultFilterParameters();

protected:
	int _textureWidth;
	int _textureHeight;
	float _u,_v;
};

/* @TODO: add 3D textures, cubemaps and so on..
 *
 **/


}



#endif