#include "SphereImpl.h"
#include "GeometryImpl.h"
#include <mak3do/scenegraph/geometries/Sphere.h>
#import <SceneKit/SceneKit.h>

namespace mak3do {

SphereImpl::SphereImpl(Sphere* parent)
: m_abstract(parent)
{
    SCNGeometry* sphere = [SCNSphere sphereWithRadius:1];
#if TARGET_OS_MAC
    sphere.materials.firstObject.diffuse.contents = [NSColor whiteColor];
#else
    sphere.materials.firstObject.diffuse.contents = [UIColor whiteColor];
#endif
    auto pimpl = std::dynamic_pointer_cast<GeometryImpl>(parent->pimpl());
    pimpl->replace_geometry((__bridge void*)sphere);
}

}



