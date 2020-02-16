#pragma once
#include <mak3do/all>

namespace mak3do {
namespace tests {

void constructed_scene()
{
    auto director = Director::get();
    auto scene = std::make_shared<Scene>();
    auto node = std::make_shared<Node>();
    auto camera = std::make_shared<Camera>();
    auto light = std::make_shared<Light>(Light::LightType::Omni);
    auto geometry = std::make_shared<Box>();
    
    geometry->color(color::RGBA(color::RGBA(.92f,.12f,0.65f)));
    
    node->geometry(geometry);
    
    camera->name("main_camera");
    camera->position(Vec3(0,1.2f,4));
    
    light->position(Vec3(0,4,4));
    light->color(color::RGB::WHITE_01);
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
    
    auto geometry2 = std::make_shared<Sphere>();
    node2->geometry(geometry2);
    
    geometry2->color(color::RGBA(color::RGBA::ORANGE_01));
    
    node->action(RepeatForever::make(SpinBy::make(2, 360)));
    node2->action(RepeatForever::make(SpinBy::make(16, Vec3(0, 0, 360))));
    camera->action(MoveBy::make(120,Vec3(0,0,100)));
}

void imported_scene()
{
    auto director = Director::get();
    auto scene = Scene::load("test_scene.scn");
    auto camera = std::make_shared<Camera>();

    if (scene == nullptr) {
        constructed_scene();
        return;
    }

    director->scene(scene);

    camera->name("main_camera");
    camera->position(Vec3(0,1.2f,4));

    for (auto node : scene->nodes()) {
       auto light = std::dynamic_pointer_cast<Light>(node);
       if (light != nullptr) {
           light->shadows(true);
       }
       
       if (node->name() == "torus") {
           node->action
           (
               RepeatForever::make
               (
                   Sequence::make
                   ({
                       MoveBy::make(3,Vec3(0,5,0)),
                       MoveBy::make(1,Vec3(0,-5,0))
                   })
                )
            );
       } else if (node->name() == "camera2") {
           node->action(MoveBy::make(240,Vec3(0,0,100)));
       } else if (node->name() == "box") {
           node->action(RepeatForever::make(SpinBy::make(2,720)));
       }
    }

    scene->add_node(camera);
    scene->camera("main_camera");
    
    //camera->action(SpinBy::make(5,Vec3(0,360,0)));
}

void shader_modifier()
{
    auto director = Director::get();
    auto scene = std::make_shared<Scene>();
    auto node = std::make_shared<Node>();
    auto camera = std::make_shared<Camera>();
    auto light = std::make_shared<Light>(Light::LightType::Omni);
    auto geometry = std::make_shared<Sphere>();
    
    geometry->color(color::RGBA(color::RGBA(.92f,.12f,0.65f)));
    
    node->geometry(geometry);
    
    camera->name("main_camera");
    camera->position(Vec3(0,1.2f,4));
    
    light->position(Vec3(0,4,4));
    light->color(color::RGB::WHITE_01);
    light->shadows(true);
    
    scene->add_node(node);
    scene->add_node(camera);
    scene->add_node(light);
    
    director->scene(scene);
    
    scene->camera("main_camera");
    
    std::string modifier =
    "_geometry.position +="
    "    float4(_geometry.normal *"
    "    (0.5*_geometry.position.y*_geometry.position.x) *"
    "    sin(1.0 * scn_frame.time),1.0);";
    
    geometry->modify_shader_geometry(modifier);
    
    node->action(RepeatForever::make(SpinBy::make(8, 360)));
}

}
}
