#pragma once

#include "../types.h"

namespace mak3do {
namespace io {

class TouchManagerImpl
{
public:
    TouchManagerImpl();
    
    void inject_touches_begin(const std::vector<TouchPtr>& touches);
    void inject_touches_ended(const std::vector<TouchPtr>& touches);
    void inject_touches_moved(const std::vector<TouchPtr>& touches);
    void inject_touches_cancelled(const std::vector<TouchPtr>& touches);
    
    void touched(TouchCallbackPtr callback);
    void released(TouchCallbackPtr callback);
    void moved(TouchCallbackPtr callback);
    void cancelled(TouchCallbackPtr callback);
    
    void stop(TouchCallbackPtr callback);
    
    void simulate_touches(const std::vector<TouchPtr>& touches, bool release);
    
    void stop_all();
private:
    std::vector<TouchCallbackPtr> m_moved;
    std::vector<TouchCallbackPtr> m_ended;
    std::vector<TouchCallbackPtr> m_cancelled;
    std::vector<TouchCallbackPtr> m_touched;
    
};
}
}
