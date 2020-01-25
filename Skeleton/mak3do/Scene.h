#pragma once

#include "types.h"
#include "Node.h"

namespace mak3do {
class Scene : public Node {
public:
    Scene();
private:
    SceneImplPtr m_pimpl;
};

}
