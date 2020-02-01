#pragma once

#include "types.h"

namespace mak3do {
class Node {
public:
    Node();
        
    void addChild(NodePtr node);
    std::vector<NodePtr> getChildren() const;
    
    void setPosition(const Vec3& position);
    Vec3 getPosition() const;
private:
    
    NodeImplPtr m_pimpl;
    
    friend class NodeImpl;
    friend class SceneImpl;
};
}
