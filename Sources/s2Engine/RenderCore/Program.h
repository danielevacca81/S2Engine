// Program.h
//
#ifndef S2_RENDERCORE_PROGRAM_H
#define S2_RENDERCORE_PROGRAM_H

#include "s2Engine_API.h"

#include "OpenGLObject.h"
#include "Uniform.h"
#include "Shader.h"

#include <map>
#include <string>
#include <memory>
#include <vector>

namespace s2 {
namespace RenderCore {

/************************************************************************************************/
/*                                           Program                                            */
/************************************************************************************************/
class Program;
typedef std::shared_ptr<Program>   ProgramPtr;

class S2ENGINE_API Program : public OpenGLObject // rename in shader?
{
public:
	static ProgramPtr New();

public:
	Program();
	~Program();

	bool attachVertexShader( const ShaderPtr &shader );
	bool attachFragmentShader( const ShaderPtr &shader );
	bool attachGeometryShader( const ShaderPtr &shader );
	bool attachComputeShader( const ShaderPtr& shader );
	bool attachTessellationControlShader( const ShaderPtr& shader );
	bool attachTessellationEvaluationShader( const ShaderPtr& shader );

	bool        isLinked()    const;
	std::string name() const;

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
	void create()  override;
	void destroy() override;
	int  objectLabelIdentifier() const override;
	void reset() override;

	void findUniforms();
private:
	ShaderPtr _vshd;
	ShaderPtr _fshd;
	ShaderPtr _gshd;
	
	ShaderPtr _cshd; // compute shader
	ShaderPtr _tshd; // tessellation shader
	ShaderPtr _teshd;// tessellation evaluation shader


	bool         _linked { false };
	std::string  _name;

	std::map< std::string, unsigned int >  _attributes;
	std::map< std::string, Uniform*>       _uniforms;


	friend class ShaderCompiler;
};

} // namespace RenderCore
} // namespace s2
#endif // !S2_RENDERCORE_PROGRAM_H