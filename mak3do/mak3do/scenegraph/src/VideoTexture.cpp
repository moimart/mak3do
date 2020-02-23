#include <mak3do/scenegraph/VideoTexture.h>
#ifdef M3D_PLAT_APPLE
#include <mak3do/scenegraph/apple/VideoTextureImpl.h>
#elif M3D_PLAT_ANDROID
#include <mak3do/scenegraph/android/VideoTextureImpl.h>
#endif

namespace mak3do {

VideoTexture::VideoTexture(const std::string& filename)
: Texture(filename)
{
    m_video_pimpl = std::make_shared<VideoTextureImpl>(this,filename);
    m_base_pimpl = std::dynamic_pointer_cast<Pimpl>(m_video_pimpl);
}

void VideoTexture::play()
{
    m_video_pimpl->play();
}

void VideoTexture::pause()
{
    
}

void VideoTexture::resume()
{
    
}
}



