#include <mak3do/scenegraph/Scheduler.h>

namespace mak3do {


void Scheduler::schedule(ScheduleUpdateCallbackPtr callback)
{
    m_callbacks.push_back(callback);
}

void Scheduler::update(float dt)
{
    for (auto& callbacks : m_callbacks) {
        callbacks->lambda(dt);
    }
}

}
