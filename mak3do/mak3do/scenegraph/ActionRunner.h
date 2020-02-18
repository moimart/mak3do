#pragma once

#include "types.h"
#include "actions/Action.h"

namespace mak3do {
class ActionRunner {
public:
    void add_action(ActionPtr action, NodePtr target);
    void remove_target(NodePtr target);
    void update(float dt);
private:
    std::vector<ActionPtr> m_actions;
};
}
