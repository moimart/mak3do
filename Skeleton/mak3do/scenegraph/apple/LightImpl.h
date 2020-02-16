#pragma once

#include <mak3do/scenegraph/types.h>
#include <mak3do/scenegraph/Light.h>
#include <mak3do/scenegraph/Color.h>

namespace mak3do {
class LightImpl {
public:
    LightImpl(Light* parent);
    LightImpl(const Light::LightType& type, Light* parent);
    ~LightImpl();
    
    void type(const Light::LightType& type);
    Light::LightType type() const;
    
    void color(const color::RGB& color);
    color::RGB color() const;
    
    void shadows(bool onoff);
    bool shadows() const;
private:
    Light* m_abstract;
    void* m_native;
    
    friend class SceneImpl;
};
}
