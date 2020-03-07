#pragma once

#include "types.h"

namespace mak3do {
namespace io {

class TouchManager
{
public:
    static TouchManager* get();
    
    void touched(TouchCallbackPtr callback);
    void released(TouchCallbackPtr callback);
    void moved(TouchCallbackPtr callback);
    void cancelled(TouchCallbackPtr callback);
    
    void stop(TouchCallbackPtr callback);
    
    void simulate_touches(const std::vector<TouchPtr>& touches, bool release = false );
    
    void stop_all();
    
    TouchManagerImplPtr pimpl() const;
protected:
    TouchManager();
private:
    TouchManagerImplPtr m_pimpl;
};
}
}
