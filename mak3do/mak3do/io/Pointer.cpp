#include "Pointer.h"
#include "apple/PointerManagerImpl.h"

#define POINTER_IMPL(function,type) \
void PointerManager::function(type callback) \
{ \
    m_pimpl->function(callback); \
}

namespace mak3do {
namespace io {

PointerManager* PointerManager::get()
{
    static PointerManager* manager = nullptr;
    
    if (manager == nullptr) {
        manager = new PointerManager();
    }
    
    return manager;
}

PointerManager::PointerManager()
: m_pimpl(std::make_shared<PointerManagerImpl>())
{
}

POINTER_IMPL(touched,PointerEventCallbackPtr);
POINTER_IMPL(moved,PointerEventCallbackPtr);
POINTER_IMPL(released,PointerEventCallbackPtr);
POINTER_IMPL(cancelled,PointerEventCallbackPtr);
POINTER_IMPL(scrolled,ScrollEventCallbackPtr);
POINTER_IMPL(stop,PointerEventCallbackPtr);
POINTER_IMPL(stop,ScrollEventCallbackPtr);

void PointerManager::simulate_touches(const std::vector<PointerEventPtr>& touches, bool released)
{
    m_pimpl->simulate_touches(touches,released);
}

void PointerManager::stop_all()
{
    m_pimpl->stop_all();
}

PointerManagerImplPtr PointerManager::pimpl() const
{
    return m_pimpl;
}

}
}
