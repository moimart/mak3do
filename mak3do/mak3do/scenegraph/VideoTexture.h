#pragma once

#include "types.h"
#include "Texture.h"

namespace mak3do {

class VideoTexture : public Texture
{
public:
    VideoTexture(const std::string& filename);
    virtual ~VideoTexture() = default;
    
    void play();
    void pause();
    void resume();
    
private:
    VideoTextureImplPtr m_video_pimpl;
    
    friend class MaterialImpl;
};

}

