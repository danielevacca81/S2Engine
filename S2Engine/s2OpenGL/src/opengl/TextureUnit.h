// TextureUnit.h
//
#ifndef TEXTUREUNIT_ONCE
#define TEXTUREUNIT_ONCE

#include "s2OpenGL_API.h"

#include "Texture.h"
#include "TextureSampler.h"

namespace s2 {

	namespace OpenGL {
	
		class S2OPENGL_API TextureUnit
		{
		
		public:
			TextureUnit( int index );

			void setTexture( const Texture2DPtr &texture );
			void setTextureSampler( const TextureSamplerPtr &sampler );

			Texture2DPtr      texture() const;
			TextureSamplerPtr sampler() const;

			void use();

		private:
			void validate();

		private:
			enum Changes
			{
				None = 0,
				Texture = 1,
				Sampler = 2,
				Both = Texture | Sampler
			};
		
		private:
			Texture2DPtr      _texture;
			TextureSamplerPtr _textureSampler;

			int  _textureUnitIndex;
			int  _textureUnit;
			Changes _changes;
		};
	

	}

}

#endif // !TEXTUREUNIT_ONCE
