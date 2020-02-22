#pragma once

#include "types.h"

namespace mak3do {

class Texture : public std::enable_shared_from_this<Texture>
{
public:
    Texture(const std::string& filename);
    
private:
    TextureImplPtr m_pimpl;
};

}

