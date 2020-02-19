#pragma once

#define M3D_PLAT_APPLE 1
//#define M3D_PLAT_ANDROID

#include <sys/time.h>

#include <memory>
#include <mutex>
#include <vector>
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <mak3do/math/all>

#ifndef MIN
#define MIN(x, y) (((x) > (y)) ? (y) : (x))
#endif // MIN

#ifndef MAX
#define MAX(x, y) (((x) < (y)) ? (y) : (x))
#endif

namespace mak3do {

struct _timeval {
#ifdef __native_client__
    time_t tv_sec { 0 }; // seconds
#else
    long tv_sec { 0 }; // seconds
#endif
    int tv_usec { 0 }; // microSeconds
};

struct ScheduleUpdate {
    std::function<void(float)> lambda { [&](float){} };
    std::string cb_id;
    bool paused { false };
    bool repeat { false };
};

typedef std::shared_ptr<ScheduleUpdate> ScheduleUpdateCallbackPtr;

class Director;
class Scheduler;
class Timer;
class Scene;
class Node;
class Light;
class Camera;

class Geometry;
class Sphere;
class Box;
class Surface;

class Action;
class ActionRunner;

class DirectorImpl;
class SceneImpl;
class NodeImpl;
class LightImpl;
class CameraImpl;
class GeometryImpl;
class SphereImpl;
class BoxImpl;
class SurfaceImpl;

typedef std::shared_ptr<Scheduler> SchedulerPtr;
typedef std::shared_ptr<Timer> TimerPtr;
typedef std::shared_ptr<Scene> ScenePtr;
typedef std::shared_ptr<Node> NodePtr;
typedef std::shared_ptr<Camera> CameraPtr;
typedef std::shared_ptr<Light> LightPtr;
typedef std::shared_ptr<Geometry> GeometryPtr;
typedef std::shared_ptr<Sphere> SpherePtr;
typedef std::shared_ptr<Box> BoxPtr;
typedef std::shared_ptr<Surface> SurfacePtr;
typedef std::shared_ptr<Action> ActionPtr;
typedef std::shared_ptr<ActionRunner> ActionRunnerPtr;

typedef std::shared_ptr<DirectorImpl> DirectorImplPtr;
typedef std::shared_ptr<SceneImpl> SceneImplPtr;
typedef std::shared_ptr<NodeImpl> NodeImplPtr;
typedef std::shared_ptr<LightImpl> LightImplPtr;
typedef std::shared_ptr<CameraImpl> CameraImplPtr;
typedef std::shared_ptr<GeometryImpl> GeometryImplPtr;
typedef std::shared_ptr<SphereImpl> SphereImplPtr;
typedef std::shared_ptr<BoxImpl> BoxImplPtr;
typedef std::shared_ptr<SurfaceImpl> SurfaceImplPtr;
}
