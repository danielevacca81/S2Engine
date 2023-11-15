// Math_API.h
//
#ifndef MATH_API_H
#define MATH_API_H

#ifdef _WIN32
#define MATH_EXPORT __declspec(dllexport)
#define MATH_IMPORT __declspec(dllimport)

#ifdef MATH_EXPORTS
#define MATH_API MATH_EXPORT
#else
#define MATH_API MATH_IMPORT
#endif

#else
#define MATH_API
#define MATH_EXPORT
#define MATH_IMPORT
#endif

#endif

#pragma warning (disable: 4251)
#pragma warning (disable: 4275)
