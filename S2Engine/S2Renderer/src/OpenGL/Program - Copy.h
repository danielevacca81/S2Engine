// Program.h
//
#ifndef PROGRAM_ONCE
#define PROGRAM_ONCE

#include "S2RendererAPI.h"

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
	bool _linked;
	std::string  _name;

	std::map< std::string, unsigned int > _attributes; 
	
	bool create();
	void detatch( unsigned int &shd );
	void detatchAll();
	int uniformLocation(const std::string &name) const;

public:
	static ProgramPtr New() { return std::make_shared<Program>();}
	Program();
	~Program();

	bool attachVertexShader(const std::string &vertexSource);
	bool attachFragmentShader(const std::string &fragmentSource);
	bool attachGeometryShader(const std::string & geometrySource);

	bool compile( const std::string &name = std::string("") );

	bool isValid()     const;
	bool isLinked()    const;
	std::string info() const;
	std::string name() const;
	bool operator==( const Program &) const;
	bool operator!=( const Program &) const;

	void bind()const;
	void unbind()const;

	//void addVertexAttribute( const std::string &attribName);
	void setVertexAttribute( const std::string &attribName, int size, int type, bool normalized, int stride, const void *ptr);

	// Single valued (int)
	void uniform(const std::string & name, int x) const;
	void uniform(const std::string & name, int x, int y)const;
	void uniform(const std::string & name, int x, int y, int z)const;
	void uniform(const std::string & name, int x, int y, int z, int w)const;

	// Array values (int) 
	void uniform1v(const std::string & name, int size, const int * x)const;
	void uniform2v(const std::string & name, int size, const int * xy)const;
	void uniform3v(const std::string & name, int size, const int * xyz)const;
	void uniform4v(const std::string & name, int size, const int * xyzw)const;

	// Single valued (float)
	void uniform(const std::string & name, float x)const;
	void uniform(const std::string & name, float x, float y)const;
	void uniform(const std::string & name, float x, float y, float z)const;
	void uniform(const std::string & name, float x, float y, float z, float w)const;

	// Array values (float)
	void uniform1v(const std::string & name, int size, const float * x)const;
	void uniform2v(const std::string & name, int size, const float * xy)const;
	void uniform3v(const std::string & name, int size, const float * xyz)const;
	void uniform4v(const std::string & name, int size, const float * xyzw)const;

	// Matrix uniforms
	void uniformMatrix2( const std::string &name, int count, bool transpose, const float *values ) const; 
	void uniformMatrix3( const std::string &name, int count, bool transpose, const float *values ) const; 
	void uniformMatrix4( const std::string &name, int count, bool transpose, const float *values ) const; 
};

}
#endif