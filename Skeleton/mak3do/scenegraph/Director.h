#pragma once

#include <stdio.h>
#include "types.h"

namespace mak3do {

class Director {
public:
    static Director* get();
    void loop();
    
    void scene(ScenePtr scene);
    
    ActionRunnerPtr defaultActionRunner() const;
    
protected:
    Director();
    
    void deltaTime();
    
private:
    DirectorImplPtr m_pimpl;
    
    ActionRunnerPtr m_action_runner;
    
    struct _timeval m_last_update;
    float m_dt { 0 };
    bool m_next_dt { false };
};
}

