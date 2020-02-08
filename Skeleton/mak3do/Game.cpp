#include "Game.h"
#include "all"

namespace mak3do {
void Game::startGame()
{
    auto director = Director::get();
    auto scene = std::make_shared<Scene>();
    auto node = std::make_shared<Node>();
    auto camera = std::make_shared<Camera>();
    auto light = std::make_shared<Light>();
    
    camera->name("main_camera");
    camera->position(Vec3(0,1.2,4));
    
    light->position(Vec3(-.2f,2,2));
    
    scene->addNode(node);
    scene->addNode(camera);
    scene->addNode(light);
    director->setScene(scene);
    
    scene->mainCamera("main_camera");
    
    node->runAction(RepeatForever::make(RotateBy::make(2, 360)));
    camera->runAction(MoveBy::make(120,Vec3(0,0,100)));
}
}
