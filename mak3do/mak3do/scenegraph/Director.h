#pragma once

#include <stdio.h>
#include "types.h"

namespace mak3do {

class Director : public Base {
public:
    static Director* get();
    void loop();
    
    void scene(ScenePtr scene);
    
    ActionRunnerPtr action_runner() const;
    SchedulerPtr scheduler() const;
    
    float delta() const;
    
protected:
    Director();
    
    void dt();
    
private:
    DirectorImplPtr m_pimpl;
    ActionRunnerPtr m_action_runner;
    SchedulerPtr m_scheduler;
    
    struct _timeval m_last_update;
    float m_dt { 0 };
    bool m_next_dt { false };
};
}

