#include "Touch.h"
#include "apple/TouchManagerImpl.h"

#define TOUCH_IMPL(function,type) \
void TouchManager::function(type callback) \
{ \
    m_pimpl->function(callback); \
}

namespace mak3do {
namespace io {

TouchManager* TouchManager::get()
{
    static TouchManager* manager = nullptr;
    
    if (manager == nullptr) {
        manager = new TouchManager();
    }
    
    return manager;
}

TouchManager::TouchManager()
: m_pimpl(std::make_shared<TouchManagerImpl>())
{
}

TOUCH_IMPL(touched,TouchCallbackPtr);
TOUCH_IMPL(moved,TouchCallbackPtr);
TOUCH_IMPL(released,TouchCallbackPtr);
TOUCH_IMPL(cancelled,TouchCallbackPtr);
TOUCH_IMPL(stop,TouchCallbackPtr);

void TouchManager::simulate_touches(const std::vector<TouchPtr>& touches, bool released)
{
    m_pimpl->simulate_touches(touches,released);
}

void TouchManager::stop_all()
{
    m_pimpl->stop_all();
}

TouchManagerImplPtr TouchManager::pimpl() const
{
    return m_pimpl;
}

}
}
