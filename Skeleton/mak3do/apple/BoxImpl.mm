#include "BoxImpl.h"
#include "GeometryImpl.h"
#include "../geometries/Box.h"
#import <SceneKit/SceneKit.h>

namespace mak3do {

BoxImpl::BoxImpl(Box* parent)
: m_abstract(parent)
{
    SCNGeometry* box = [SCNBox boxWithWidth:1 height:1 length:1 chamferRadius:0];
    box.materials.firstObject.diffuse.contents = [UIColor whiteColor];
    
    parent->pimpl()->replace_geometry((__bridge void*)box);
}

}



