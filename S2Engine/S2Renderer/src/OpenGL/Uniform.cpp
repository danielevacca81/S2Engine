// Uniform.cpp
// 
#include "Uniform.h"

#include "UniformInt.h"
#include "UniformBool.h"
#include "UniformFloat.h"
#include "UniformSampler.h"

#include "UniformFloatVector2.h"
#include "UniformFloatVector3.h"
#include "UniformFloatVector4.h"

#include "UniformFloatMatrix22.h"
#include "UniformFloatMatrix33.h"
#include "UniformFloatMatrix44.h"

#include "OpenGL/OpenGL.h"

using namespace OpenGL;

void UniformBool::set()    { glUniform1i(_location,_value); }
void UniformFloat::set()   { glUniform1f(_location,_value); }
void UniformInt::set()     { glUniform1i(_location,_value); }
void UniformSampler::set() {}

void UniformFloatVector2::set() { glUniform2fv( _location,1, Math::value_ptr(_value) ); }
void UniformFloatVector3::set() { glUniform3fv( _location,1, Math::value_ptr(_value) ); }
void UniformFloatVector4::set() { glUniform4fv( _location,1, Math::value_ptr(_value) ); }

void UniformFloatMatrix22::set() { glUniformMatrix2fv( _location,1,false, Math::value_ptr(_value) ); }
void UniformFloatMatrix33::set() { glUniformMatrix3fv( _location,1,false, Math::value_ptr(_value) ); }
void UniformFloatMatrix44::set() { glUniformMatrix4fv( _location,1,false, Math::value_ptr(_value) ); }