#pragma once

#include "types.h"

namespace mak3do {
class Node {
public:
    Node();
        
    void addChild(NodePtr node);
    
    std::vector<NodePtr> getChildren() const;
private:
    std::vector<NodePtr> m_children;
    
    NodeImplPtr m_pimpl;
};
}
