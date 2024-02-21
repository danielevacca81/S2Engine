// Program.h
//
#ifndef PROGRAM_ONCE
#define PROGRAM_ONCE

#include "RenderSystem_API.h"

#include "OpenGLObject.h"
#include "Uniform.h"

#include <map>
#include <string>
#include <memory>
#include <vector>

namespace RenderSystem {

/************************************************************************************************/
/*                                           Program                                            */
/************************************************************************************************/
class Program;
typedef std::shared_ptr<Program>   ProgramPtr;

class RENDERSYSTEM_API Program : public OpenGLObject
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

	void create()  override;
	void destroy() override;
	void bind()    const override;
	void unbind()  const override;
	void applyUniforms() const;

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
			return unusedUniform<T>();

		return dynamic_cast<UniformValue<T> *>( it->second );
	}




private:
	void reset() override;
	void findUniforms();
	static Uniform *createUniform( const std::string &name, unsigned int loc, unsigned int type );
	static void prepareUnusedUniforms();
	int  objectLabelIdentifier() const override;

	template< typename T >
	inline UniformValue<T> *unusedUniform() const
	{
		for( auto &u : _unusedUniforms )
		{
			auto unused = dynamic_cast<UniformValue<T> *>( u );
			if( unused )
				return unused;
		}
		
		return nullptr;
	}

private:
	unsigned int _vshd;
	unsigned int _gshd;
	unsigned int _fshd;
	bool         _linked;
	std::string  _name;

	std::map< std::string, unsigned int >  _attributes;
	std::map< std::string, Uniform*>       _uniforms;
	static std::vector< Uniform *>         _unusedUniforms; // workaround to prevent crashes when setting missing uniforms :(
};

}
#endif