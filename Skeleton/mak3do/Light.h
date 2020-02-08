#pragma once

#include "types.h"
#include "Node.h"
#include "Color.h"

class Light;

namespace mak3do {
class Light : public Node {
public:
    
    enum class LightType {
        Ambient,
        Directional,
        Omni,
        Spot
    };
    
    Light();
    Light(LightType type);
    
    void type(const LightType& type);
    LightType type() const;
    
    void color(const color::RGB& color);
    color::RGB color() const;
    
    void shadows(bool onoff);
    bool shadows() const;
protected:
    LightImplPtr m_light_pimpl;
    
    friend class LightImpl;
};
}
