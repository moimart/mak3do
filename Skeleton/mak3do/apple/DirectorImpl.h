#pragma once

#include "../types.h"

namespace mak3do {

class DirectorImpl {
public:
    DirectorImpl(mak3do::Director* parent);
    
    void loop();
    void setScene(ScenePtr scene);
private:
    Director* m_parent;
    
    ScenePtr m_current_scene;
};

}


