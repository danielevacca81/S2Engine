// CompilerMessage.h
//
#ifndef COMMON_COMPILERMESSAGE_H
#define COMMON_COMPILERMESSAGE_H

#include "utils/String.h"

#ifndef  __GCC
#define COMPILER_MESSAGE(X) __pragma( message( __FILE__ "(" TOSTRING(__LINE__) "): "  TOSTRING(X) ));
#else
// @todo: da rivedere per GCC
#define COMPILER_MESSAGE(msg) 
//#define COMPILER_MESSAGE(msg) (_Pragma( message( __FILE__ "(" TOSTRING(__LINE__) "): "  TOSTRING(X) ));
#endif // ! __GCC

#endif // !COMMON_COMPILERMESSAGE_H
