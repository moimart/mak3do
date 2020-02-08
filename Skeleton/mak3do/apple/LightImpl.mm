#include "LightImpl.h"
#include "NodeImpl.h"
#include "../Light.h"
#import <SceneKit/SceneKit.h>


namespace mak3do {

LightImpl::LightImpl(Light* parent)
: m_abstract(parent)
{
    SCNNode* node = (__bridge SCNNode*)parent->m_pimpl->m_native;
    
    
}

}
