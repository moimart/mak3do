#pragma once

#include "types.h"

namespace mak3do {
// sub-class onto GameScriptNative and GameScriptMono or GameScriptJS or whatever...

class GameScript {
public:
    typedef Callback<void(GameObjectPtr)> EnterCallback;
    typedef Callback<void(GameObjectPtr)> ExitCallback;

    virtual void update(float dt) = 0;

    virtual WorldPtr world() const = 0;

    virtual int on_enter(const std::string& id, EnterCallback callback) = 0;
    virtual int on_exit(const std::string& id, ExitCallback callback) = 0;

protected:
    virtual void world(WorldPtr world) = 0;
};
}
