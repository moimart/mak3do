#pragma once

#include <mak3do/scenegraph/types.h>

namespace mak3do {
class BoxImpl {
public:
    BoxImpl(Box* parent);
private:
    Box* m_abstract;
    void* m_native_sphere;
};
}

