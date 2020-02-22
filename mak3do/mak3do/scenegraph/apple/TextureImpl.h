#pragma once

#include <mak3do/scenegraph/types.h>

namespace mak3do {
class TextureImpl : public Pimpl {
public:
    TextureImpl(Texture* parent, const std::string& filename);
    
    std::string name() const;
private:
    Texture* m_abstract;
    std::string m_native_texture;
    
    friend class MaterialImpl;
};
}



