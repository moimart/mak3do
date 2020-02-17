#include <mak3do/scenegraph/Scheduler.h>

namespace mak3do {

class Timer
{
public:
    Timer(float duration, ScheduleUpdateCallbackPtr callback)
    : m_callback(callback)
    , m_duration(duration)
    {
    }
    
    inline void update(float dt)
    {
        m_at += dt;
    }
    
    inline ScheduleUpdateCallbackPtr callback() const
    {
        return m_callback;
    }
    
    inline bool done()
    {
        bool ret = m_at >= m_duration;

        if (ret) {
            m_callback->lambda(m_at);
            
            if (m_callback->repeat) {
                m_at = 0;
                ret = false;
            }
        }
        
        return ret;
    }
private:
    float m_duration { -1 };
    float m_at { 0 };
    ScheduleUpdateCallbackPtr m_callback;
};

void Scheduler::schedule(ScheduleUpdateCallbackPtr callback)
{
    m_callbacks.push_back(callback);
}

void Scheduler::schedule(float time, ScheduleUpdateCallbackPtr callback)
{
    auto timer = std::make_shared<Timer>(time,callback);
    m_timers.push_back(timer);
}

bool Scheduler::unschedule(ScheduleUpdateCallbackPtr callback)
{
    auto iterator = std::find(m_callbacks.begin(), m_callbacks.end(), callback);
    
    if (iterator != m_callbacks.end()) {
        m_unscheduled_callbacks.push_back(callback);
        return true;
    } else {
        for (auto& timer : m_timers) {
            if (timer->callback() == callback) {
                m_unscheduled_timers.push_back(timer);
                return true;
            }
        }
    }
    
    return false;
}

void Scheduler::update(float dt)
{
    for (auto& callback : m_callbacks) {
        if (!callback->paused) {
            callback->lambda(dt);
        }
    }
    
    m_timers.erase(std::remove_if(m_timers.begin(),m_timers.end(),[&dt](TimerPtr& timer) -> bool {
        timer->update(dt);
        
        return timer->done();
    }),m_timers.end());
}

void Scheduler::cleanup()
{
    for (auto& callback : m_unscheduled_callbacks) {
        auto iterator = std::find(m_callbacks.begin(), m_callbacks.end(), callback);
        
        if (iterator != m_callbacks.end()) {
            m_callbacks.erase(iterator);
        }
    }
    
    for (auto& timer : m_unscheduled_timers) {
        auto iterator = std::find(m_timers.begin(), m_timers.end(), timer);
        
        if (iterator != m_timers.end()) {
            m_timers.erase(iterator);
        }
    }
}

}
