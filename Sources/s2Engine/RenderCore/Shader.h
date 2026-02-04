// Shader.h
//
#ifndef SHADER_H
#define SHADER_H

#include "s2Engine_API.h"

#include "OpenGLObject.h"

#include <string>
#include <memory>

namespace RenderCore {

enum class ShaderType
{
    Vertex,
    Fragment,
    Geometry,
    Compute,
    TessellationControl,
    TessellationEvaluation
};


class Shader;
typedef std::shared_ptr<Shader> ShaderPtr;

// ------------------------------------------------------------------------------------------------
class S2ENGINE_API Shader : public OpenGLObject
{
public:
	static ShaderPtr New( const ShaderType& type );

public:
    Shader( const ShaderType& type );
	~Shader();

	ShaderType type() const { return _type; }

private:
    void create()  override;
    void destroy() override;
    void bind()    const override {}
    void unbind()  const override {}

	int objectLabelIdentifier() const override;

private:
	ShaderType _type;


	friend class ShaderCompiler;
};

}

#endif // !SHADER_H