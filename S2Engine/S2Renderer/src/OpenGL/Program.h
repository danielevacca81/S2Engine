// Program.h
//
#ifndef PROGRAM_ONCE
#define PROGRAM_ONCE

#include "S2RendererAPI.h"

#include "Uniform.h"

#include <map>
#include <string>

namespace OpenGL {

// ------------------------------------------------------------------------------------------------
class Program;
typedef std::shared_ptr<Program>   ProgramPtr;

class S2RENDERER_API Program : public std::enable_shared_from_this<Program>
{
protected:
	unsigned int _vshd;
	unsigned int _gshd;
	unsigned int _fshd;
	unsigned int _progID;
	bool         _linked;
	std::string  _name;

	std::map< std::string, unsigned int > _attributes;
	std::map< std::string, Uniform*>       _uniforms;
	
	bool create();
	void detatch( unsigned int &shd );
	void detatchAll();

	void findUniforms();
	Uniform *createUniform( const std::string &name, unsigned int loc, unsigned int type );

public:
	static ProgramPtr New() { return std::make_shared<Program>();}
	Program();
	~Program();

	bool attachVertexShader  ( const std::string &vertexSource   );
	bool attachFragmentShader( const std::string &fragmentSource );
	bool attachGeometryShader( const std::string &geometrySource );

	bool compile( const std::string &name = std::string("") );

	bool isValid()     const;
	bool isLinked()    const;
	std::string info() const;
	std::string name() const;
	bool operator==( const Program &) const;
	bool operator!=( const Program &) const;

	void bind()   const;
	void unbind() const;

	//Uniform *uniform( const std::string &name );
	
	template< typename T >
	inline UniformValue<T> *uniform( const std::string &name )
	{
		auto it = _uniforms.find(name);

		if( it == _uniforms.end() )
			return 0;

		return dynamic_cast< UniformValue<T> *>( it->second );
	}
};

}
#endif