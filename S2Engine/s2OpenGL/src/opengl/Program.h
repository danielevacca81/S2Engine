// Program.h
//
#ifndef PROGRAM_ONCE
#define PROGRAM_ONCE

#include "s2OpenGL_API.h"

#include "Uniform.h"

#include <map>
#include <string>
#include <memory>

namespace s2 {
namespace OpenGL {

// ------------------------------------------------------------------------------------------------
class Program;
typedef std::shared_ptr<Program>   ProgramPtr;

class S2OPENGL_API Program
{
public:
	static ProgramPtr New() { return std::make_shared<Program>(); }
	Program();
	~Program();

	bool attachVertexShader( const std::string &vertexSource );
	bool attachFragmentShader( const std::string &fragmentSource );
	bool attachGeometryShader( const std::string &geometrySource );

	bool link( const std::string &name = std::string( "" ) );

	bool isValid()     const;
	bool isLinked()    const;
	std::string info( bool verbose = false ) const;
	std::string name() const;
	bool operator==( const Program & ) const;
	bool operator!=( const Program & ) const;

	void bind()   const;
	void unbind() const;

	//Uniform *uniform( const std::string &name );

	template< typename T >
	inline UniformValue<T> *uniform( const std::string &name ) const
	{
		auto it = _uniforms.find( name );

		if( it == _uniforms.end() )
			return 0;

		return dynamic_cast<UniformValue<T> *>( it->second );
	}

private:

	bool create();
	void detatch( unsigned int &shd );
	void detatchAll();

	void findUniforms();
	Uniform *createUniform( const std::string &name, unsigned int loc, unsigned int type );


protected:
	unsigned int _vshd;
	unsigned int _gshd;
	unsigned int _fshd;
	unsigned int _progID;
	bool         _linked;
	std::string  _name;
	std::string  _compileMessage;

	std::map< std::string, unsigned int > _attributes;
	std::map< std::string, Uniform*>       _uniforms;
};

}
}
#endif