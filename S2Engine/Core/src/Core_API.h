// Core_API.h
//
#ifndef CORE_API_H
#define CORE_API_H

#ifdef _WIN32
#define CORE_EXPORT __declspec(dllexport)
#define CORE_IMPORT __declspec(dllimport)

#ifdef CORE_EXPORTS
#define CORE_API CORE_EXPORT
#else
#define CORE_API CORE_IMPORT
#endif

#else
#define CORE_API
#define CORE_EXPORT
#define CORE_IMPORT
#endif

#endif

#pragma warning (disable: 4251)
#pragma warning (disable: 4275)
