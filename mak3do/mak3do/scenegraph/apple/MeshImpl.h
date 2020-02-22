#pragma once

#include <mak3do/scenegraph/types.h>

namespace mak3do {
class MeshImpl : public Pimpl {
public:
    MeshImpl(Mesh* parent);
private:
    Mesh* m_abstract;
    void* m_native_mesh;
};
}


