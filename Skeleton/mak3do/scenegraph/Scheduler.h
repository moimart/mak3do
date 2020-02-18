#pragma once

#include "types.h"

namespace mak3do {

class Scheduler : public std::enable_shared_from_this<Scheduler>
{
public:
    static ScheduleUpdateCallbackPtr task();
    
    void schedule(ScheduleUpdateCallbackPtr callback);
    void schedule(float time, ScheduleUpdateCallbackPtr callback);
    bool unschedule(ScheduleUpdateCallbackPtr callback);
    
protected:
    void update(float dt);
    void cleanup();
    
    friend class Director;
private:
    std::vector<ScheduleUpdateCallbackPtr> m_callbacks;
    std::vector<ScheduleUpdateCallbackPtr> m_unscheduled_callbacks;
    std::vector<TimerPtr> m_timers;
    std::vector<TimerPtr> m_unscheduled_timers;;
    std::mutex m_mutex;
};

}
