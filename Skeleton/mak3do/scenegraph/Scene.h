#pragma once

#include "types.h"
#include "Node.h"

namespace mak3do {
class Scene : public Node {
public:
    Scene();
    
    void add_node(NodePtr node);
    void camera(const std::string& name);
    CameraPtr camera() const;
    
    std::vector<NodePtr> nodes() const;
    
    static ScenePtr load(const std::string& filename);
private:
    SceneImplPtr m_pimpl;
    
    friend class SceneImpl;
    friend class DirectorImpl;
};

}
