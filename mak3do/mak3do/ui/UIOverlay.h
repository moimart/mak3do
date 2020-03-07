#pragma once

#include "types.h"

namespace mak3do {

class UIOverlay {
public:
    static UIOverlay* get();
    
    void fonts(const std::vector<std::string>& font_resources);
    
    bool load(const std::string& resource, const std::string& package = "");
protected:
    UIOverlay();
    
    UIOverlayImplPtr m_pimpl;
};
}
