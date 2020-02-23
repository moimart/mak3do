#pragma once

#include "types.h"

namespace mak3do {

class Texture : public Base
{
public:
    Texture(const std::string& filename);
    virtual ~Texture() = default;
    
protected:
    TextureImplPtr m_pimpl;
    
    friend class MaterialImpl;
};

}

