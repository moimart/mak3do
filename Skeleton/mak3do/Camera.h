#pragma once

#include "types.h"
#include "Node.h"

namespace mak3do {
class Camera : public Node {
public:
    Camera();
    ~Camera();
private:
    CameraImplPtr m_camera_pimpl;
    
    friend class CameraImpl;
    friend class SceneImpl;
};
}
