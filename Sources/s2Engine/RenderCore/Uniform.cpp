// Uniform.cpp
// 
#include "Uniform.h"

#include "OpenGL.h"
#include "OpenGLCheck.h"

#include "Math/Math.h"


using namespace s2::RenderCore;
// with variant we could rely on std::visit here ?
										    
void UniformBool::set()          { if(!_changed)return; _changed = false; glUniform1i( _location, _value ); glCheck; }
void UniformFloat::set()         { if(!_changed)return; _changed = false; glUniform1f( _location, _value ); glCheck; }
void UniformDouble::set()        { if(!_changed)return; _changed = false; glUniform1d( _location, _value ); glCheck; }
void UniformInt::set()           { if(!_changed)return; _changed = false; glUniform1i( _location, _value ); glCheck; }
void UniformSampler::set()       { if(!_changed)return; _changed = false; glUniform1i( _location, _value ); glCheck; }

void UniformFloatVector2::set()  { if(!_changed)return; _changed = false; glUniform2fv( _location, 1, Math::value_ptr( _value ) ); glCheck; }
void UniformFloatVector3::set()  { if(!_changed)return; _changed = false; glUniform3fv( _location, 1, Math::value_ptr( _value ) ); glCheck; }
void UniformFloatVector4::set()  { if(!_changed)return; _changed = false; glUniform4fv( _location, 1, Math::value_ptr( _value ) ); glCheck; }

void UniformDoubleVector2::set() { if( !_changed )return; _changed = false; glUniform2dv( _location, 1, Math::value_ptr( _value ) ); glCheck; }
void UniformDoubleVector3::set() { if( !_changed )return; _changed = false; glUniform3dv( _location, 1, Math::value_ptr( _value ) ); glCheck; }
void UniformDoubleVector4::set() { if( !_changed )return; _changed = false; glUniform4dv( _location, 1, Math::value_ptr( _value ) ); glCheck; }



void UniformFloatMatrix22::set() { if(!_changed)return; _changed = false; glUniformMatrix2fv( _location, 1, false, Math::value_ptr( _value ) ); glCheck; }
void UniformFloatMatrix33::set() { if(!_changed)return; _changed = false; glUniformMatrix3fv( _location, 1, false, Math::value_ptr( _value ) ); glCheck; }
void UniformFloatMatrix44::set() { if(!_changed)return; _changed = false; glUniformMatrix4fv( _location, 1, false, Math::value_ptr( _value ) ); glCheck; }


void UniformDoubleMatrix22::set() { if( !_changed )return; _changed = false; glUniformMatrix2dv( _location, 1, false, Math::value_ptr( _value ) ); glCheck; }
void UniformDoubleMatrix33::set() { if( !_changed )return; _changed = false; glUniformMatrix3dv( _location, 1, false, Math::value_ptr( _value ) ); glCheck; }
void UniformDoubleMatrix44::set() { if( !_changed )return; _changed = false; glUniformMatrix4dv( _location, 1, false, Math::value_ptr( _value ) ); glCheck; }