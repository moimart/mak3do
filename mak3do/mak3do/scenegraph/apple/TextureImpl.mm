#include "TextureImpl.h"
#include <mak3do/scenegraph/Texture.h>
#import <SceneKit/SceneKit.h>

namespace mak3do {

TextureImpl::TextureImpl(Texture* parent, const std::string& filename)
: m_abstract(parent)
, m_native_texture(filename)
{
}

std::string TextureImpl::name() const
{
    return m_native_texture;
}

}
