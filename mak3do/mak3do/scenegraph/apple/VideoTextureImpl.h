#pragma once

#include <mak3do/scenegraph/types.h>

namespace mak3do {
class VideoTextureImpl : public Pimpl {
public:
    VideoTextureImpl(VideoTexture* parent, const std::string& filename);
    ~VideoTextureImpl();
    
    void play();
private:
    VideoTexture* m_abstract;
    void* m_player;
    
    friend class MaterialImpl;
};
}


