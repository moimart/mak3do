#include "Game.h"
#include "types.h"
#include "Director.h"
#include "Scene.h"
#include "Node.h"
#include "Camera.h"
#include "actions/ActionInterval.h"

namespace mak3do {
void Game::startGame()
{
    auto director = Director::get();
    auto scene = std::make_shared<Scene>();
    auto node = std::make_shared<Node>();
    auto camera = std::make_shared<Camera>();
    
    camera->name("main_camera");
    camera->position(Vec3(0,1.2,4));
    
    scene->addNode(node);
    scene->addNode(camera);
    director->setScene(scene);
    
    scene->mainCamera("main_camera");
    
    node->runAction(RepeatForever::make(RotateBy::make(2, 360)));
}
}
