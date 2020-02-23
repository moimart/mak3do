#include "BoxImpl.h"
#include "GeometryImpl.h"
#include <mak3do/scenegraph/geometries/Box.h>
#import <SceneKit/SceneKit.h>

namespace mak3do {

BoxImpl::BoxImpl(Box* parent)
: m_abstract(parent)
{
    SCNGeometry* box = [SCNBox boxWithWidth:1 height:1 length:1 chamferRadius:0];
#if TARGET_OS_MAC
    box.materials.firstObject.diffuse.contents = [NSColor whiteColor];
#else
    box.materials.firstObject.diffuse.contents = [UIColor whiteColor];
#endif
    
    auto pimpl = std::dynamic_pointer_cast<GeometryImpl>(parent->pimpl());
    pimpl->replace_geometry((__bridge void*)box);
}

}



