#pragma once

#include "../types.h"

namespace mak3do {
namespace io {

class PointerManagerImpl
{
public:
    PointerManagerImpl();
    
    void inject_pointers_begin(const std::vector<PointerEventPtr>& touches);
    void inject_pointers_ended(const std::vector<PointerEventPtr>& touches);
    void inject_pointers_moved(const std::vector<PointerEventPtr>& touches);
    void inject_pointers_cancelled(const std::vector<PointerEventPtr>& touches);
    void inject_scrolled(float dx, float dy);
    
    void touched(PointerEventCallbackPtr callback);
    void released(PointerEventCallbackPtr callback);
    void moved(PointerEventCallbackPtr callback);
    void cancelled(PointerEventCallbackPtr callback);
    void scrolled(ScrollEventCallbackPtr callback);
    
    void stop(PointerEventCallbackPtr callback);
    void stop(ScrollEventCallbackPtr callback);
    
    void simulate_touches(const std::vector<PointerEventPtr>& touches, bool release);
    
    void stop_all();
private:
    std::vector<PointerEventCallbackPtr> m_moved;
    std::vector<PointerEventCallbackPtr> m_ended;
    std::vector<PointerEventCallbackPtr> m_cancelled;
    std::vector<PointerEventCallbackPtr> m_touched;
    std::vector<ScrollEventCallbackPtr> m_scrolled;
};
}
}
