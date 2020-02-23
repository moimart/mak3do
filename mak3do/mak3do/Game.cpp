#include "Game.h"
#include <mak3do/tests/tests.hpp>

namespace mak3do {

void Game::start()
{
    //tests::shader_modifier();
    //tests::test_basic_game_api();
    tests::test_gltf_loader();
    //tests::test_scheduler();
    //tests::imported_scene();
    //tests::test_controller_api();
    tests::test_materials();
}

}

