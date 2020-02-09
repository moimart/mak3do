#include "ActionRunner.h"
#include "Node.h"
#include "actions/Action.h"

namespace mak3do {

void ActionRunner::add_action(ActionPtr action, NodePtr target)
{
    m_actions.push_back(action);
    action->start(target);
}

void ActionRunner::remove_target(NodePtr target)
{
    std::remove_if(m_actions.begin(), m_actions.end(), [&target](ActionPtr& action) {
        return action->target() == target;
    });
}

void ActionRunner::update(float dt)
{
    std::vector<std::vector<ActionPtr>::iterator> to_remove;
    
    for (auto iter = m_actions.begin(); iter != m_actions.end(); iter++) {
        auto action = *iter;
        if (action->done()) {
            to_remove.push_back(iter);
            continue;
        }
        
        action->step(dt);
    }
    
    for (auto& iter : to_remove) {
        m_actions.erase(iter);
    }
}

}
