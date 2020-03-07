#include "TouchManagerImpl.h"

namespace mak3do {
namespace io {

TouchManagerImpl::TouchManagerImpl()
{
    
}

void TouchManagerImpl::inject_touches_begin(const std::vector<TouchPtr>& touches)
{
    for (auto& event : m_touched) {
        if (event->lambda != nullptr) {
            event->lambda(touches);
        }
    }
}

void TouchManagerImpl::inject_touches_ended(const std::vector<TouchPtr>& touches)
{
    for (auto& event : m_ended) {
        if (event->lambda != nullptr) {
            event->lambda(touches);
        }
    }
}

void TouchManagerImpl::inject_touches_moved(const std::vector<TouchPtr>& touches)
{
    for (auto& event : m_moved) {
        if (event->lambda != nullptr) {
            event->lambda(touches);
        }
    }
}

void TouchManagerImpl::inject_touches_cancelled(const std::vector<TouchPtr>& touches)
{
    for (auto& event : m_cancelled) {
        if (event->lambda != nullptr) {
            event->lambda(touches);
        }
    }
}

void TouchManagerImpl::touched(TouchCallbackPtr callback)
{
    m_touched.push_back(callback);
}

void TouchManagerImpl::released(TouchCallbackPtr callback)
{
    m_ended.push_back(callback);
}

void TouchManagerImpl::moved(TouchCallbackPtr callback)
{
    m_moved.push_back(callback);
}

void TouchManagerImpl::cancelled(TouchCallbackPtr callback)
{
    m_cancelled.push_back(callback);
}

void TouchManagerImpl::stop(TouchCallbackPtr callback)
{
    m_touched.erase(std::remove(m_touched.begin(),m_touched.end(),callback),m_touched.end());
    m_moved.erase(std::remove(m_moved.begin(),m_moved.end(),callback),m_moved.end());
    m_ended.erase(std::remove(m_ended.begin(),m_ended.end(),callback),m_ended.end());
    m_cancelled.erase(std::remove(m_cancelled.begin(),
                                  m_cancelled.end(),
                                  callback),
                      m_cancelled.end());
}

void TouchManagerImpl::simulate_touches(const std::vector<TouchPtr>& touches, bool release)
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

void TouchManagerImpl::stop_all()
{
    m_touched.clear();
    m_moved.clear();
    m_cancelled.clear();
    m_ended.clear();
}

}
}
