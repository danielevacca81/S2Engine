// Program.h
//
#ifndef PROGRAM_ONCE
#define PROGRAM_ONCE

#include "s2Renderer_API.h"

#include "OpenGLObject.h"
#include "Uniform.h"

#include <map>
#include <string>
#include <memory>

namespace s2 {
namespace Renderer {

/************************************************************************************************/
/*                                           Program                                            */
/************************************************************************************************/
class Program;
typedef std::shared_ptr<Program>   ProgramPtr;

class S2RENDERER_API Program : public OpenGLObject
{
public:
	static ProgramPtr New();

	//OBJECT_DECLARE_MOVEABLE( Program )
	//OBJECT_DISABLE_COPY( Program )

	Program();
	~Program();

	// @todo: return this for concatenation
	bool attachVertexShader( const std::string &vertexSource );
	bool attachFragmentShader( const std::string &fragmentSource );
	bool attachGeometryShader( const std::string &geometrySource );

	bool link( const std::string &name = std::string( "" ) );

	bool isLinked()    const;
	std::string info( bool verbose = false ) const;
	std::string name() const;

	bool create()  override;
	void destroy() override;
	void bind()    const override;
	void unbind()  const override;

	//Uniform *uniform( const std::string &name );


	// operator [] 
	//Uniform& Program::operator[](const std::string& uniformName) throw(std::out_of_range) {
    //    return _uniforms.at(uniformName);
    //}
	//
	//const Uniform& Program::operator[](const std::string& uniformName) const throw(std::out_of_range) {
    //    return _uniforms.at(uniformName);
    //}

	template< typename T >
	inline UniformValue<T> *uniform( const std::string &name ) const
	{
		auto it = _uniforms.find( name );

		if( it == _uniforms.end() )
			return 0;

		return dynamic_cast<UniformValue<T> *>( it->second );
	}


private:
	void reset() override;
	void findUniforms();
	Uniform *createUniform( const std::string &name, unsigned int loc, unsigned int type );

private:
	unsigned int _vshd;
	unsigned int _gshd;
	unsigned int _fshd;
	bool         _linked;
	std::string  _name;

	std::map< std::string, unsigned int > _attributes;
	std::map< std::string, Uniform*>       _uniforms;
};

}
}
#endif