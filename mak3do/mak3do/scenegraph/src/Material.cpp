#include <mak3do/scenegraph/Material.h>
#ifdef M3D_PLAT_APPLE
#include <mak3do/scenegraph/apple/MaterialImpl.h>
#elif M3D_PLAT_ANDROID
#include <mak3do/scenegraph/android/MaterialImpl.h>
#endif

namespace mak3do {

Material::Material()
{
    m_pimpl = std::make_shared<MaterialImpl>(this);
    m_base_pimpl = std::dynamic_pointer_cast<Pimpl>(m_pimpl);
}

#define MATERIAL_IMPLEMENTATION(name) \
void Material::name(MaterialPropertyPtr property) \
{ \
    m_pimpl->name(property); \
} \
\
MaterialPropertyPtr Material::name() const \
{ \
    return m_pimpl->name(); \
}

MATERIAL_IMPLEMENTATION(diffuse);
MATERIAL_IMPLEMENTATION(specular);
MATERIAL_IMPLEMENTATION(ambient);
MATERIAL_IMPLEMENTATION(emission);
MATERIAL_IMPLEMENTATION(transparent);
MATERIAL_IMPLEMENTATION(displacement);
MATERIAL_IMPLEMENTATION(normal);

}



