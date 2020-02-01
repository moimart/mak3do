#pragma once

#include "types.h"
#include "Node.h"

namespace mak3do {
class Scene : public Node {
public:
    Scene();
    
    void addNode(NodePtr node);
private:
    SceneImplPtr m_pimpl;
    
    friend class DirectorImpl;
};

}
