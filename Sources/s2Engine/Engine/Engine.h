// Engine.h
//
#ifndef S2_ENGINE_ENGINE_H
#define S2_ENGINE_ENGINE_H

#include "s2Engine_API.h"

#include "Core/TimePoint.h"

namespace s2{
namespace Engine{

class S2ENGINE_API Engine
{
public:
    // start/stop
    bool  isRunning() const { return true; }
    float deltaTime() const { return _timer.elapsed_msec()*0.001f; }


private:
    HighResTimePoint _timer;

};


}
}


#endif //!S2_ENGINE_ENGINE_H
