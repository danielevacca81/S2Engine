// Uniform.cpp
// 
#include "Uniform.h"

#include "OpenGL.h"
#include "OpenGLCheck.h"

#include "Math/Math.h"

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



using namespace RenderSystem;
										    
void UniformBool::set()          { if(!_changed)return; _changed = false; glUniform1i( _location, _value ); glCheck; }
void UniformFloat::set()         { if(!_changed)return; _changed = false; glUniform1f( _location, _value ); glCheck; }
void UniformInt::set()           { if(!_changed)return; _changed = false; glUniform1i( _location, _value ); glCheck; }
void UniformSampler::set()       { if(!_changed)return; _changed = false; glUniform1i( _location, _value ); glCheck; }

void UniformFloatVector2::set()  { if(!_changed)return; _changed = false; glUniform2fv( _location, 1, Math::value_ptr( _value ) ); glCheck; }
void UniformFloatVector3::set()  { if(!_changed)return; _changed = false; glUniform3fv( _location, 1, Math::value_ptr( _value ) ); glCheck; }
void UniformFloatVector4::set()  { if(!_changed)return; _changed = false; glUniform4fv( _location, 1, Math::value_ptr( _value ) ); glCheck; }

void UniformFloatMatrix22::set() { if(!_changed)return; _changed = false; glUniformMatrix2fv( _location, 1, false, Math::value_ptr( _value ) ); glCheck; }
void UniformFloatMatrix33::set() { if(!_changed)return; _changed = false; glUniformMatrix3fv( _location, 1, false, Math::value_ptr( _value ) ); glCheck; }
void UniformFloatMatrix44::set() { if(!_changed)return; _changed = false; glUniformMatrix4fv( _location, 1, false, Math::value_ptr( _value ) ); glCheck; }