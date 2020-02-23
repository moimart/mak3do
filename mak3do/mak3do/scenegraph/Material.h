#pragma once

#include "types.h"
#include "Color.h"

namespace mak3do {

struct MaterialProperty
{
    color::RGB color;
    TexturePtr texture;
};

class Material : public Base
{
public:
    Material();
    
    void diffuse(MaterialPropertyPtr property);
    MaterialPropertyPtr diffuse() const;
    
    void specular(MaterialPropertyPtr property);
    MaterialPropertyPtr specular() const;
    
    void ambient(MaterialPropertyPtr property);
    MaterialPropertyPtr ambient() const;
    
    void emission(MaterialPropertyPtr property);
    MaterialPropertyPtr emission() const;
    
    void transparent(MaterialPropertyPtr property);
    MaterialPropertyPtr transparent() const;
    
    void displacement(MaterialPropertyPtr property);
    MaterialPropertyPtr displacement() const;
    
    void normal(MaterialPropertyPtr property);
    MaterialPropertyPtr normal() const;
    
    void roughness(MaterialPropertyPtr property);
    MaterialPropertyPtr roughness() const;
    
private:
    MaterialImplPtr m_pimpl;
    
    friend class SceneImpl;
    friend class NodeImpl;
};

}


