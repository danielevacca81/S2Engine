// Program.h
//
#ifndef PROGRAM_H
#define PROGRAM_H

#include "s2Engine_API.h"

#include "OpenGLObject.h"
#include "Uniform.h"

#include <map>
#include <string>
#include <memory>
#include <vector>

namespace RenderCore {

/************************************************************************************************/
/*                                           Program                                            */
/************************************************************************************************/
class Program;
typedef std::shared_ptr<Program>   ProgramPtr;

class S2ENGINE_API Program : public OpenGLObject
{
public:
	static ProgramPtr New();

	//OBJECT_DECLARE_MOVEABLE( Program )
	//OBJECT_DISABLE_COPY( Program )

public:
	Program();
	~Program();

	// @todo: return this for concatenation
	bool attachVertexShader( const std::string &vertexSource );
	bool attachFragmentShader( const std::string &fragmentSource );
	bool attachGeometryShader( const std::string &geometrySource );

	bool link( const std::string &name = std::string( "" ) );

	bool        isLinked()    const;
	std::string info( bool verbose = false ) const;
	std::string name() const;

	void create()  override;
	void destroy() override;
	void bind()    const override;
	void unbind()  const override;
	void applyUniforms() const;

	// shortcut for setting uniforms value by name.
	// it will search for uniform by name and set its value.
	// warning: if uniform is not found, it will do nothing.
	// @todo: throw exception or assert if uniform not found?
	template< typename T >
	inline void setUniformValue( const std::string& uniformName, const T&value )
	{
		auto it = _uniforms.find( uniformName );
		if( it == _uniforms.end() )
			return;

		if( auto u = dynamic_cast<UniformValue<T>*>( it->second ) )
			u->set( value );
	}

	// used to get uniform by name
	// warning: returns nullptr if not found. check before using it!
	template< typename T >
	inline UniformValue<T>* uniform( const std::string& name )
	{
		auto it = _uniforms.find( name );

		if( it == _uniforms.end() )
			return nullptr;

		return dynamic_cast<UniformValue<T> *>( it->second );
	}

private:
	static Uniform *createUniform( const std::string &name, unsigned int loc, unsigned int type );
	
	void findUniforms();
	int  objectLabelIdentifier() const override;
	void reset() override;

private:
	unsigned int _vshd;
	unsigned int _gshd;
	unsigned int _fshd;
	bool         _linked;
	std::string  _name;

	std::map< std::string, unsigned int >  _attributes;
	std::map< std::string, Uniform*>       _uniforms;
};

}
#endif