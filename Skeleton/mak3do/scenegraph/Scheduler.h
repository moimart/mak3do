#pragma once

#include "types.h"

namespace mak3do {

class Scheduler : public std::enable_shared_from_this<Scheduler>
{
public:
    void schedule(ScheduleUpdateCallbackPtr callback);
    void schedule(float time, ScheduleUpdateCallbackPtr callback);
    
protected:
    void update(float dt);
    
    friend class Director;
private:
    std::vector<ScheduleUpdateCallbackPtr> m_callbacks;
    std::vector<TimerPtr> m_timers;
};

}
