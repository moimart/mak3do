#pragma once

#include "../types.h"

namespace mak3do {
class SceneImpl {
public:
    SceneImpl(Scene* parent);
    ~SceneImpl();
    
    void addNode(NodePtr node);
    void mainCamera(const std::string& name);
private:
    void* m_native;
    
    std::vector<NodePtr> m_nodes;
    
    friend class DirectorImpl;
};
}

