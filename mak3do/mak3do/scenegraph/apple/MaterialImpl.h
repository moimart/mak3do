#pragma once

#include <mak3do/scenegraph/types.h>

namespace mak3do {
class MaterialImpl : public Pimpl {
public:
    MaterialImpl(Material* parent);
    ~MaterialImpl();
    
    void diffuse(MaterialPropertyPtr property);
    MaterialPropertyPtr diffuse();
    
    void specular(MaterialPropertyPtr property);
    MaterialPropertyPtr specular();
    
    void ambient(MaterialPropertyPtr property);
    MaterialPropertyPtr ambient();
    
    void emission(MaterialPropertyPtr property);
    MaterialPropertyPtr emission();
    
    void transparent(MaterialPropertyPtr property);
    MaterialPropertyPtr transparent();
    
    void displacement(MaterialPropertyPtr property);
    MaterialPropertyPtr displacement();
    
    void normal(MaterialPropertyPtr property);
    MaterialPropertyPtr normal();
    
    void roughness(MaterialPropertyPtr property);
    MaterialPropertyPtr roughness();
    
private:
    Material* m_abstract;
    void* m_native_material;
    
    friend class GeometryImpl;
};
}



