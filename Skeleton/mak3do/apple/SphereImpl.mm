#include "SphereImpl.h"
#include "GeometryImpl.h"
#include "../geometries/Sphere.h"
#import <SceneKit/SceneKit.h>

namespace mak3do {

SphereImpl::SphereImpl(Sphere* parent)
: m_abstract(parent)
{
    SCNGeometry* sphere = [SCNSphere sphereWithRadius:1];
    sphere.materials.firstObject.diffuse.contents = [UIColor whiteColor];
    
    parent->get_pimpl()->replace_geometry((__bridge void*)sphere);
}

}



