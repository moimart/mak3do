#include "LightImpl.h"
#include "NodeImpl.h"
#include "../Light.h"
#import <SceneKit/SceneKit.h>


namespace mak3do {

LightImpl::LightImpl(Light* parent)
: m_abstract(parent)
{
    SCNNode* node = (__bridge SCNNode*)parent->m_pimpl->m_native;
    
    SCNLight* light = [SCNLight light];
    [light setType:SCNLightTypeOmni];
    
    node.light = light;
    
    m_native = (void*)CFBridgingRetain(light);
}

LightImpl::LightImpl(const Light::LightType& type, Light* parent)
: m_abstract(parent)
{
    SCNNode* node = (__bridge SCNNode*)parent->m_pimpl->m_native;
    
    SCNLight* light = [SCNLight light];
    m_native = (void*)CFBridgingRetain(light);
    this->type(type);
    
    node.light = light;
}

LightImpl::~LightImpl()
{
    SCNLight* light = (SCNLight*)CFBridgingRelease(m_native);
    
    light = nil;
    m_native = nullptr;
}

void LightImpl::type(const Light::LightType& type)
{
    SCNLight* light = (__bridge SCNLight*)m_native;
    
    switch (type) {
        case Light::LightType::Ambient:
            [light setType:SCNLightTypeAmbient];
            break;
        case Light::LightType::Omni:
            [light setType:SCNLightTypeOmni];
            break;
        case Light::LightType::Spot:
            [light setType:SCNLightTypeSpot];
            break;
        case Light::LightType::Directional:
            [light setType:SCNLightTypeDirectional];
            break;
        default:
            break;
    }
}

Light::LightType LightImpl::type() const
{
    SCNLight* light = (__bridge SCNLight*)m_native;
    SCNLightType __type = [light type];
    
    if ([__type isEqualToString:SCNLightTypeAmbient]) {
        return Light::LightType::Directional;
    } else if ([__type isEqualToString:SCNLightTypeDirectional]) {
        return Light::LightType::Omni;
    } else if ([__type isEqualToString:SCNLightTypeSpot]) {
        return Light::LightType::Omni;
    }
    
    return Light::LightType::Omni;
}

void LightImpl::color(const color::RGB& color)
{
    UIColor* __color = [UIColor colorWithRed:color.r green:color.g blue:color.b alpha:1.0];
    SCNLight* light = (__bridge SCNLight*)m_native;
    
    [light setColor:__color];
}

color::RGB LightImpl::color() const
{
    SCNLight* light = (__bridge SCNLight*)m_native;
    id __color = light.color;
    
    return color::RGB([__color red],[__color green], [__color blue]);
}

}
