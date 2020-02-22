#pragma once

#include <mak3do/scenegraph/types.h>

namespace mak3do {
class SceneImpl : public Pimpl {
public:
    SceneImpl(Scene* parent);
    ~SceneImpl();
    
    void add_node(NodePtr node);
    void main_camera(const std::string& name);
    CameraPtr camera() const;
    
    std::vector<NodePtr> nodes() const;
    
    static ScenePtr load(const std::string& filename);
    static void add_children(void* children, NodePtr parent);
    static void create_geometry(void* node, NodePtr real_node);
private:
    void* m_native;

    std::vector<NodePtr> m_nodes;
    
    friend class DirectorImpl;
};
}

