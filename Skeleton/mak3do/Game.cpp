#include "Game.h"
#include "all"

namespace mak3do {
void Game::start()
{
    auto director = Director::get();
    auto scene = std::make_shared<Scene>();
    auto node = std::make_shared<Node>();
    auto camera = std::make_shared<Camera>();
    auto light = std::make_shared<Light>(Light::LightType::Omni);
    
    camera->name("main_camera");
    camera->position(Vec3(0,1.2,4));
    
    light->position(Vec3(-.2f,2,2));
    
    scene->add_node(node);
    scene->add_node(camera);
    scene->add_node(light);
    
    director->scene(scene);
    
    scene->camera("main_camera");
    
    node->run_action(RepeatForever::make(SpinBy::make(2, 360)));
    camera->run_action(MoveBy::make(120,Vec3(0,0,100)));
}
}
