#include "VideoTextureImpl.h"
#include <mak3do/scenegraph/Texture.h>
#import <SceneKit/SceneKit.h>
#import <AVFoundation/AVFoundation.h>

namespace mak3do {

VideoTextureImpl::VideoTextureImpl(VideoTexture* parent, const std::string& filename)
: m_abstract(parent)
{
    NSString* __filename = [NSString stringWithUTF8String:filename.c_str()];
    NSURL* url = [[NSBundle mainBundle] URLForResource:__filename withExtension:@""];
    AVQueuePlayer* player = [AVQueuePlayer playerWithURL:url];
    
    m_player = (void*)CFBridgingRetain(player);
}

VideoTextureImpl::~VideoTextureImpl()
{
    AVPlayer* player = (AVPlayer*)CFBridgingRelease(m_player);
    m_player = nullptr;
    player = nil;
}

void VideoTextureImpl::play()
{
    AVPlayer* player = (__bridge AVPlayer*)m_player;
    
    [player play];
}

}

