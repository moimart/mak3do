#pragma once

#include <mak3do/scenegraph/types.h>

namespace mak3do {
class MaterialImpl {
public:
    MaterialImpl(Material* parent);
private:
    Material* m_abstract;
    void* m_native_material;
};
}



