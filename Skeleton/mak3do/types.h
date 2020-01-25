#pragma once

#define M3D_PLAT_APPLE 1
//#define M3D_PLAT_ANDROID

#include <memory>
#include <vector>
#include <iostream>

namespace mak3do {
class Director;
class Scene;
class Node;

class DirectorImpl;
class SceneImpl;
class NodeImpl;

typedef std::shared_ptr<Scene> ScenePtr;
typedef std::shared_ptr<Node> NodePtr;

typedef std::shared_ptr<DirectorImpl> DirectorImplPtr;
typedef std::shared_ptr<SceneImpl> SceneImplPtr;
typedef std::shared_ptr<NodeImpl> NodeImplPtr;
}
