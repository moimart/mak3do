#pragma once

#include "../types.h"

namespace mak3do {
class SceneImpl {
public:
    SceneImpl(Scene* parent);
    ~SceneImpl();
    
    void add_node(NodePtr node);
    void main_camera(const std::string& name);
    
    static ScenePtr load(const std::string& filename);
    static void add_children(void* children, NodePtr parent);
private:
    void* m_native;

    std::vector<NodePtr> m_nodes;
    
    friend class DirectorImpl;
};
}

