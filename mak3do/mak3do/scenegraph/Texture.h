#pragma once

#include "types.h"

namespace mak3do {

class Texture : public Base
{
public:
    Texture(const std::string& filename);
    
private:
    TextureImplPtr m_pimpl;
    
    friend class MaterialImpl;
};

}

