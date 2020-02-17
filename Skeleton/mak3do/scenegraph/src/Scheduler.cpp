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

void Scheduler::update(float dt)
{
    for (auto& callback : m_callbacks) {
        if (!callback->paused) {
            callback->lambda(dt);
        }
    }
    
    m_timers.erase(std::remove_if(m_timers.begin(),m_timers.end(),[dt](TimerPtr& timer) -> bool {
        timer->update(dt);
        
        return timer->done();
    }),m_timers.end());
}

}
