// Samplers.cpp
//
#include "Samplers.h"

using namespace RenderSystem;

SamplerPtr Samplers::NearestClamp;
SamplerPtr Samplers::LinearClamp;
SamplerPtr Samplers::NearestRepeat;
SamplerPtr Samplers::LinearRepeat;
bool       Samplers::_initialized { false };

// ------------------------------------------------------------------------------------------------
bool Samplers::init()
{
	if( _initialized )
		return true;

	NearestClamp  = Sampler::New( Sampler::MinificationFilter::Nearest, Sampler::MagnificationFilter::Nearest, Sampler::Wrap::Clamp, Sampler::Wrap::Clamp,  1 );
	LinearClamp   = Sampler::New( Sampler::MinificationFilter::Linear,  Sampler::MagnificationFilter::Linear,  Sampler::Wrap::Clamp, Sampler::Wrap::Clamp,  1 );
	NearestRepeat = Sampler::New( Sampler::MinificationFilter::Nearest, Sampler::MagnificationFilter::Nearest, Sampler::Wrap::Repeat,Sampler::Wrap::Repeat, 1 );
	LinearRepeat  = Sampler::New( Sampler::MinificationFilter::Linear,  Sampler::MagnificationFilter::Linear,  Sampler::Wrap::Repeat,Sampler::Wrap::Repeat, 1 );

	_initialized = true;
	return true;
}

// ------------------------------------------------------------------------------------------------
void Samplers::destroy()
{
	if( !_initialized )
		return;

	NearestClamp  = nullptr;
	LinearClamp   = nullptr;
	NearestRepeat = nullptr;
	LinearRepeat  = nullptr;
}