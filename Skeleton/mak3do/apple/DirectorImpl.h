#pragma once

#include <mak3do/scenegraph/types.h>

namespace mak3do {

class DirectorImpl {
public:
    DirectorImpl(mak3do::Director* parent);
    
    void loop(float dt);
    void setScene(ScenePtr scene);
private:
    Director* m_parent;
    
    ScenePtr m_current_scene;
};

}


