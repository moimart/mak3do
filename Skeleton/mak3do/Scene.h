#pragma once

#include "types.h"
#include "Node.h"

namespace mak3do {
class Scene : public Node {
public:
    Scene();
    
    void addNode(NodePtr node);
    
    void mainCamera(const std::string& name);
private:
    SceneImplPtr m_pimpl;
    
    friend class DirectorImpl;
};

}
