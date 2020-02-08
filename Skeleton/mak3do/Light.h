#pragma once

#include "types.h"
#include "Node.h"

class Light;

namespace mak3do {
class Light : public Node {
public:
    Light();
protected:
    LightImplPtr m_light_pimpl;
    
    friend class LightImpl;
};
}
