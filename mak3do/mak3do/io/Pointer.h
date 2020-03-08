#pragma once

#include "types.h"

namespace mak3do {
namespace io {

class PointerManager
{
public:
    static PointerManager* get();
    
    void touched(PointerEventCallbackPtr callback);
    void released(PointerEventCallbackPtr callback);
    void moved(PointerEventCallbackPtr callback);
    void cancelled(PointerEventCallbackPtr callback);
    void scrolled(ScrollEventCallbackPtr callback);
    
    void stop(PointerEventCallbackPtr callback);
    void stop(ScrollEventCallbackPtr callback);
    
    void simulate_touches(const std::vector<PointerEventPtr>& touches, bool release = false );
    
    void stop_all();
    
    PointerManagerImplPtr pimpl() const;
protected:
    PointerManager();
private:
    PointerManagerImplPtr m_pimpl;
};
}
}
