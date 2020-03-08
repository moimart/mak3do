#include "PointerManagerImpl.h"

namespace mak3do {
namespace io {

PointerManagerImpl::PointerManagerImpl()
{
    
}

void PointerManagerImpl::inject_pointers_begin(const std::vector<PointerEventPtr>& touches)
{
    for (auto& event : m_touched) {
        if (event->lambda != nullptr) {
            event->lambda(touches);
        }
    }
}

void PointerManagerImpl::inject_pointers_ended(const std::vector<PointerEventPtr>& touches)
{
    for (auto& event : m_ended) {
        if (event->lambda != nullptr) {
            event->lambda(touches);
        }
    }
}

void PointerManagerImpl::inject_pointers_moved(const std::vector<PointerEventPtr>& touches)
{
    for (auto& event : m_moved) {
        if (event->lambda != nullptr) {
            event->lambda(touches);
        }
    }
}

void PointerManagerImpl::inject_pointers_cancelled(const std::vector<PointerEventPtr>& touches)
{
    for (auto& event : m_cancelled) {
        if (event->lambda != nullptr) {
            event->lambda(touches);
        }
    }
}

void PointerManagerImpl::inject_scrolled(float dx, float dy)
{
    for (auto& event : m_scrolled) {
        if (event->lambda != nullptr) {
            event->lambda(dx,dy);
        }
    }
}

void PointerManagerImpl::touched(PointerEventCallbackPtr callback)
{
    m_touched.push_back(callback);
}

void PointerManagerImpl::released(PointerEventCallbackPtr callback)
{
    m_ended.push_back(callback);
}

void PointerManagerImpl::moved(PointerEventCallbackPtr callback)
{
    m_moved.push_back(callback);
}

void PointerManagerImpl::cancelled(PointerEventCallbackPtr callback)
{
    m_cancelled.push_back(callback);
}

void PointerManagerImpl::scrolled(ScrollEventCallbackPtr callback)
{
    m_scrolled.push_back(callback);
}

void PointerManagerImpl::stop(PointerEventCallbackPtr callback)
{
    m_touched.erase(std::remove(m_touched.begin(),m_touched.end(),callback),m_touched.end());
    m_moved.erase(std::remove(m_moved.begin(),m_moved.end(),callback),m_moved.end());
    m_ended.erase(std::remove(m_ended.begin(),m_ended.end(),callback),m_ended.end());
    m_cancelled.erase(std::remove(m_cancelled.begin(),
                                  m_cancelled.end(),
                                  callback),
                      m_cancelled.end());
}

void PointerManagerImpl::stop(ScrollEventCallbackPtr callback)
{
    m_scrolled.erase(std::remove(m_scrolled.begin(),
                                 m_scrolled.end(),
                                 callback),
                     m_scrolled.end());
}

void PointerManagerImpl::simulate_touches(const std::vector<PointerEventPtr>& touches, bool release)
{
    for (auto& touched : m_touched) {
        if (touched->lambda != nullptr) {
            touched->lambda(touches);
        }
    }
    
    if (release) {
        for (auto& released : m_ended) {
            if (released->lambda != nullptr) {
                released->lambda(touches);
            }
        }
    }
}

void PointerManagerImpl::stop_all()
{
    m_touched.clear();
    m_moved.clear();
    m_cancelled.clear();
    m_ended.clear();
    m_scrolled.clear();
}

}
}
