#pragma once
#include "types.h"
#include <mak3do/tests/tests.hpp>

namespace mak3do {
class Game {
public:
    void start()
    {
        tests::shader_modifier();
    };
};
}