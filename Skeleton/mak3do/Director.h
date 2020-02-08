//
//  Director.hpp
//  Skeleton
//
//  Created by Moises Martinez on 20.01.20.
//  Copyright © 2020 Moises Martinez. All rights reserved.
//

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

