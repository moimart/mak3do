#pragma once

#include "types.h"
#include "Color.h"

namespace mak3do {

struct MaterialProperty
{
    color::RGB color;
    TexturePtr texture;
};

class Material : public std::enable_shared_from_this<Material>
{
public:
    Material();
    
    void diffuse(MaterialPropertyPtr property);
    MaterialPropertyPtr diffuse() const;
    
    void specular(MaterialPropertyPtr property);
    MaterialPropertyPtr specular() const;
    
    void normal(MaterialPropertyPtr property);
    MaterialPropertyPtr normal() const;
    
private:
    MaterialImplPtr m_pimpl;
    
    friend class SceneImpl;
    friend class NodeImpl;
};

}


