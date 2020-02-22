#include <mak3do/scenegraph/Mesh.h>
#ifdef M3D_PLAT_APPLE
#include <mak3do/scenegraph/apple/MeshImpl.h>
#elif M3D_PLAT_ANDROID
#include <mak3do/scenegraph/android/MeshImpl.h>
#endif

namespace mak3do {

Mesh::Mesh(const std::vector<std::vector<Vec3> >& vertices)
: m_pimpl(std::make_shared<MeshImpl>(this))
{
    
}

std::vector<Vec3> Mesh::vertices(int index) const
{
    return std::vector<Vec3>();
}
}

