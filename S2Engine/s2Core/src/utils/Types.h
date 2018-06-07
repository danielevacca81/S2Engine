// Types.h
// 
// Common s2 types

#ifndef TYPES_ONCE
#define TYPES_ONCE

#include <memory>

// ********************************************************
#define DECLARE_SHARED_PTR_CLASS(X)  \
class X;                             \
typedef std::shared_ptr<X> X ## Ptr; \
// ********************************************************



#endif