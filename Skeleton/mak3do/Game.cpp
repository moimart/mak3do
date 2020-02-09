#include "Game.h"
#include "all"

namespace mak3do {
void Game::start()
{
    /*
    auto director = Director::get();
    auto scene = std::make_shared<Scene>();
    auto node = std::make_shared<Node>();
    auto camera = std::make_shared<Camera>();
    auto light = std::make_shared<Light>(Light::LightType::Omni);
    
    camera->name("main_camera");
    camera->position(Vec3(0,1.2f,4));
    
    light->position(Vec3(0,4,4));
    light->color(color::RGB::ORANGE_01);
    light->shadows(true);
    
    scene->add_node(node);
    scene->add_node(camera);
    scene->add_node(light);
    
    director->scene(scene);
    
    scene->camera("main_camera");
    
    auto node2 = std::make_shared<Node>();
    node2->position(Vec3(-.5f,2,0));
    node2->pitch(45);
    node2->yaw(45);
    scene->add_node(node2);
    
    node->action(RepeatForever::make(SpinBy::make(2, 360)));
    node2->action(RepeatForever::make(SpinBy::make(16, Vec3(0, 0, 360))));
    camera->action(MoveBy::make(120,Vec3(0,0,100)));
     */
    auto director = Director::get();
    auto scene = Scene::load("chair.scn");
    auto camera = std::make_shared<Camera>();
    
    director->scene(scene);
    
    camera->name("main_camera");
    camera->position(Vec3(0,1.2f,4));
    
    for (auto& node : scene->nodes()) {
        if (node) {
            
        }
    }
    
    scene->camera("camera2");
}
}
