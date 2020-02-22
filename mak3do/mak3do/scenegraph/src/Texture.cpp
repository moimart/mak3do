#include <mak3do/scenegraph/Texture.h>
#ifdef M3D_PLAT_APPLE
#include <mak3do/scenegraph/apple/TextureImpl.h>
#elif M3D_PLAT_ANDROID
#include <mak3do/scenegraph/android/TextureImpl.h>
#endif

namespace mak3do {

Texture::Texture(const std::string& filename)
{
    m_pimpl = std::make_shared<TextureImpl>(this,filename);
    m_base_pimpl = std::dynamic_pointer_cast<Pimpl>(m_pimpl);
}
}


