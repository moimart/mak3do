#pragma once

#define M3D_PLAT_APPLE 1
//#define M3D_PLAT_ANDROID

#include <sys/time.h>

#include <memory>
#include <vector>
#include <iostream>
#include <string>
#include "math/all"

#ifndef MIN
#define MIN(x, y) (((x) > (y)) ? (y) : (x))
#endif // MIN

#ifndef MAX
#define MAX(x, y) (((x) < (y)) ? (y) : (x))
#endif

namespace mak3do {

struct _timeval {
#ifdef __native_client__
    time_t tv_sec; // seconds
#else
    long tv_sec; // seconds
#endif
    int tv_usec; // microSeconds
};

class Director;
class Scene;
class Node;
class Light;
class Camera;
class Action;
class ActionRunner;

class DirectorImpl;
class SceneImpl;
class NodeImpl;
class LightImpl;
class CameraImpl;

typedef std::shared_ptr<Scene> ScenePtr;
typedef std::shared_ptr<Node> NodePtr;
typedef std::shared_ptr<Camera> CameraPtr;
typedef std::shared_ptr<Light> LightPtr;
typedef std::shared_ptr<Action> ActionPtr;
typedef std::shared_ptr<ActionRunner> ActionRunnerPtr;

typedef std::shared_ptr<DirectorImpl> DirectorImplPtr;
typedef std::shared_ptr<SceneImpl> SceneImplPtr;
typedef std::shared_ptr<NodeImpl> NodeImplPtr;
typedef std::shared_ptr<LightImpl> LightImplPtr;
typedef std::shared_ptr<CameraImpl> CameraImplPtr;
}
