#pragma once

#include <mak3do/scenegraph/types.h>

namespace mak3do {
class CameraImpl : public Pimpl {
public:
    CameraImpl(Camera* parent);
    ~CameraImpl();
private:
    Camera* m_abstract;
    void* m_native_camera;
    
    friend class SceneImpl;
};
}
