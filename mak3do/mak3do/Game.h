#pragma once
#include <functional>

namespace mak3do {
class Game {
public:
    void start(std::function<void(void)> function = nullptr);
private:
    void tests();
};
}
