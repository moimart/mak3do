#pragma once
#include <mak3do/scenegraph/all>
#include <mak3do/game/all>
#include <mak3do/io/all>
#include <mak3do/ui/all>

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

NodePtr hanging_node = nullptr;

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
           //node->action(MoveBy::make(240,Vec3(0,0,100)));
           hanging_node = node;
       } else if (node->name() == "box") {
           node->action(RepeatForever::make(SpinBy::make(2,720)));
       }
    }

    scene->add_node(camera);
    scene->camera("camera2");
    
    camera->action(SpinBy::make(5,Vec3(0,360,0)));
    
    using namespace io;
    
    auto gcm = GameControllerManager::get();
    auto controller_added = std::make_shared<ControllerFoundCallback>();
    
    controller_added->lambda = [&](GameControllerPtr controller) {
        auto left_stick = std::make_shared<AnalogCallback>();
        left_stick->lambda = [=](float x, float y) {
            std::cout << "value " << y << std::endl;
           
            auto pos = hanging_node->position();
            
            pos.z -= y;
            hanging_node->position(pos);
        };
    
        controller->left_stick_changed(left_stick);
    };
    
    gcm->controller_added(controller_added);
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

class TestObject : public GameObject
{
public:
    TestObject() : GameObject("test-object") {
        auto node = std::make_shared<Node>();
        auto box = std::make_shared<Sphere>();
        box->color(color::RGBA::ORANGE_01);
        
        node->geometry(box);
        
        add_node("main", node);
        
        auto element = ElementFactory::make_basic_element(ElementFactory::ElementDef3D(0.075f));
        add_physics_element("main-element", element);
    }
    
};

WorldPtr test_basic_game_api()
{
    auto scene = std::make_shared<Scene>();
    auto director = Director::get();
    auto camera = std::make_shared<Camera>();
    auto light = std::make_shared<Light>(Light::LightType::Directional);
    
    camera->name("main_camera");
    camera->position(Vec3(0,1.2f,4));
   
    light->position(Vec3(0,4,4));
    light->color(color::RGB::WHITE_01);
    light->shadows(true);
    
    director->scene(scene);
    
    scene->add_node(light);
    scene->camera("main_camera");
    scene->add_node(light);
    
    auto physics_world = std::make_shared<PhysicsWorld>(PhysicsWorld::Type::_3D);
    auto world = std::make_shared<World>(physics_world,scene);
    
    world->physics_world()->gravity(Vec3(0,-9.8f,0));
    
    auto object = std::make_shared<TestObject>();
    world->add_object(object);
    object->position(Vec3::ZERO);
    
    world->start();
    
    return world;
}

void test_gltf_loader()
{
    auto director = Director::get();
    auto scene = Scene::load("DamagedHelmet.glb");
    auto camera = std::make_shared<Camera>();
    auto light = std::make_shared<Light>(Light::LightType::Omni);
    auto node = std::make_shared<Node>();
    
    node->geometry(std::make_shared<Surface>());
    node->geometry()->color(color::RGBA::WHITE_01);
    
    std::string modifier =
    "_geometry.position +="
    "    float4(_geometry.normal *"
    "    (1.5*_geometry.position.y*_geometry.position.x) *"
    "    sin(1.0 * scn_frame.time),1.0);";
    
    node->pitch(-90);
    node->position(Vec3(0,-.5f,-3));
    
    light->position(Vec3(0,4,4));
    light->color(color::RGB::WHITE_01);
    light->shadows(true);
    light->look_at(Vec3::ZERO);

    if (scene == nullptr) {
        constructed_scene();
        return;
    }

    director->scene(scene);
    
    scene->add_node(light);
    scene->add_node(camera);
    scene->add_node(node);

    camera->name("camera2");
    camera->position(Vec3(0,1.0f,3));
    camera->look_at(Vec3::ZERO);

    for (auto node : scene->nodes()) {
        std::cout << "node name: " << node->name() << std::endl;
        
        if (node->name() == "node_damagedHelmet_-6514") {
            node->position(Vec3(0,0,0));
            node->action(RepeatForever::make(SpinBy::make(8,720)));
            
            std::function<void(std::vector<NodePtr>)> f = [&](std::vector<NodePtr> children) {
                for (auto& node : children) {
                    if (node->geometry() != nullptr) {
                        //node->geometry()->modify_shader_geometry(modifier);
                        std::cout << "geometry" << std::endl;
                    }
                    
                    f(node->nodes());
                    std::cout << "Node " << node->name() << std::endl;
                }
            };
            
            f(node->nodes());
        }
    }

    scene->add_node(camera);
    scene->camera("camera2");
    
    //camera->action(MoveBy::make(40,Vec3(0,0,-10)));
}

void test_scheduler()
{
    auto world = test_basic_game_api();
    
    auto sched = Director::get()->scheduler();
    auto task = Scheduler::task();
    task->lambda = [=](float dt) {
       auto object = std::make_shared<TestObject>();
       object->position(Vec3(0,1,0));
       world->add_object(object);
    };
    task->repeat = true;
    
    sched->schedule(5.f,task);

    auto task2 = Scheduler::task();
    task2->lambda = [=](float dt) {
       sched->unschedule(task);
    };
    sched->schedule(8.f,task2);
    
    auto task3 = Scheduler::task();
    task3->repeat = true;
    task3->lambda = [=](float dt) {
        std::cout << "HELLO! " << dt << std::endl;
    };
    
    sched->schedule(2,task3);
}
WorldPtr world = nullptr;
void test_controller_api()
{
    using namespace io;
    world = test_basic_game_api();
    
    world->physics_world()->gravity(Vec3(0,0,0));
    
    auto gcm = GameControllerManager::get();
    auto controller_added = std::make_shared<ControllerFoundCallback>();
    
    controller_added->lambda = [&](GameControllerPtr controller) {
        auto button_pressed = std::make_shared<ButtonCallback>();
        button_pressed->lambda = [=](controller::Button button, float value) {
            std::cout << "value " << value << std::endl;
            
            auto objects = world->objects("test-object");
            
            if (objects.size() == 1) {
                objects[0]->physics_element("main-element")->impulse(Vec3(0,value,0),Vec3::ZERO);
            }
        };
    
        controller->button_pressed(button_pressed);
    };
    
    gcm->controller_added(controller_added);
}

void test_materials()
{
    auto director = Director::get();
    auto scene = std::make_shared<Scene>();
    auto node = std::make_shared<Node>();
    auto camera = std::make_shared<Camera>();
    auto light = std::make_shared<Light>(Light::LightType::Directional);
    auto geometry = std::make_shared<Sphere>();
    
    geometry->color(color::RGBA(color::RGBA(.92f,.12f,0.65f)));
    
    node->geometry(geometry);
    
    camera->name("main_camera");
    camera->position(Vec3(0,0,4));
    
    light->position(Vec3(-15,6,20));
    light->look_at(Vec3::ZERO);
    light->color(color::RGB::WHITE_01);
    light->shadows(true);
    
    scene->add_node(node);
    scene->add_node(camera);
    scene->add_node(light);
    
    director->scene(scene);
    
    scene->camera("main_camera");
    
    auto material = std::make_shared<Material>();
    auto diffuse = std::make_shared<MaterialProperty>("earth_diffuse.png");
    auto normal = std::make_shared<MaterialProperty>("earth_normal.png");
    auto specular = std::make_shared<MaterialProperty>("earth_specular.png");
    
    material->diffuse(diffuse);
    material->normal(normal);
    material->specular(specular);
    
    geometry->material(material);
    
    node->action(RepeatForever::make(SpinBy::make(8, 360)));
    
    auto node2 = std::make_shared<Node>();
    auto geom2 = std::make_shared<Box>();
    
    geom2->material(material);
    node2->geometry(geom2);
    node2->position(Vec3(3,0,0));
    
    node->add_node(node2);
    node2->action(RepeatForever::make(SpinBy::make(4, Vec3(0,360,0))));
    node->scale(Vec3::ONE*1.5f);
    
    std::string modifier =
    "#pragma arguments\n"
    "float amplitude;\n"
    "#pragma body\n"
    "_geometry.position +="
    "    float4(_geometry.normal *"
    "    (amplitude*_geometry.position.y*_geometry.position.x) *"
    "    sin(1.0 * scn_frame.time),1.0);";
    
    geometry->modify_shader_geometry(modifier);
    geometry->value("amplitude", .5f);
    
}

void test_ui() {
    test_materials();
    
    auto overlay = UIOverlay::get();
    
    overlay->fonts({"league.ttf"});
    overlay->load("demo.rml");
    
    auto sched = Director::get()->scheduler();
    auto task = Scheduler::task();
    
    task->lambda = [=](float dt) {
        //overlay->load(); //No UI overlay
    };
    
    sched->schedule(5,task);
}

void test_touch()
{
    test_ui();
    
    auto touch = io::PointerManager::get();
    auto callback = std::make_shared<io::PointerEventCallback>();
    
    callback->lambda = [=](const std::vector<io::PointerEventPtr>& touches) -> bool {
        for (auto& touch : touches) {
            std::cout << "Touch at x: " << touch->location.x << " y: " << touch->location.y << std::endl;
        }
        
        return true;
    };
    
    touch->touched(callback);
}
}
}
