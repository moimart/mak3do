#pragma once

#include "../types.h"
#include "../Light.h"

namespace mak3do {
class LightImpl {
public:
    LightImpl(Light* parent);
    LightImpl(const Light::LightType& type, Light* parent);
    ~LightImpl();
    
    void type(const Light::LightType& type);
    Light::LightType type() const;
private:
    Light* m_abstract;
    void* m_native;
};
}
