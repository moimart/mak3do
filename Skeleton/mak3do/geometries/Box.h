#pragma once

#include "types.h"
#include "../Geometry.h"

namespace mak3do {

class Box : public Geometry {
public:
    Box();
    ~Box();
private:
    BoxImplPtr m_box_pimpl;
};
}


