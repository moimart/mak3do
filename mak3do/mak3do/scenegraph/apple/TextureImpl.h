#pragma once

#include <mak3do/scenegraph/types.h>

namespace mak3do {
class TextureImpl {
public:
    TextureImpl(Texture* parent);
private:
    Texture* m_abstract;
    std::string m_native_texture;
};
}



