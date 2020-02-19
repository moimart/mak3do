#include "SurfaceImpl.h"
#include "GeometryImpl.h"
#include <mak3do/scenegraph/geometries/Surface.h>
#import <SceneKit/SceneKit.h>

namespace mak3do {

SurfaceImpl::SurfaceImpl(Surface* parent)
: m_abstract(parent)
{
    SCNGeometry* plane = [SCNPlane planeWithWidth:1.5 height:1.5];
    plane.materials.firstObject.diffuse.contents = [UIColor whiteColor];
    
    parent->pimpl()->replace_geometry((__bridge void*)plane);
}

}



