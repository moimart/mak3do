#pragma once

#include "../types.h"

namespace mak3do {
class SceneImpl {
public:
    SceneImpl(Scene* parent);
    
    void addNode(NodePtr node);
private:
    void* m_native;
    
    std::vector<NodePtr> m_nodes;
    
    friend class DirectorImpl;
};
}

