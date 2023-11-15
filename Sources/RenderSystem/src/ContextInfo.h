// ContextInfo.h
//
#ifndef CONTEXTINFO_ONCE
#define CONTEXTINFO_ONCE

#include "RenderSystem_API.h"

#include <string>
#include <set>

namespace RenderSystem {

class RENDERSYSTEM_API ContextInfo
{
public:
	int32_t openGLMajorVersion() const { return _versionMajor; }
	int32_t openGLMinorVersion() const { return _versionMinor; }

	bool    isContextProfileCore() const;
	bool    isContextProfileCompatibility() const;
	bool    isContextProfileForwardCompatible() const;
	bool    isContextProfileDebug() const;
	bool    isContextProfileRobust() const;
	bool    isContextProfileNoError() const;

	const std::set<std::string>& extensions() const;
	
	std::string toString() const;
private:
	void init();

private:
	int32_t _versionMajor { 0 };
	int32_t _versionMinor { 0 };
	int32_t _openGLContextProfile { 0 };
	int32_t _openGLContextFlags { 0 };
	
	std::string _vendor;
	std::string _version;
	std::string _renderer;
	std::string _shaderVersion;


	mutable std::set<std::string> _extensions;


	friend class Context;
};

}
#endif