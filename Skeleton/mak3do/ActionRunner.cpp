#include "ActionRunner.h"
#include "Node.h"
#include "actions/Action.h"

namespace mak3do {

void ActionRunner::addAction(ActionPtr action, NodePtr target)
{
    m_actions.push_back(action);
    action->startWithTarget(target);
}

void ActionRunner::update(float dt)
{
    std::vector<std::vector<ActionPtr>::iterator> to_remove;
    
    for (auto iter = m_actions.begin(); iter != m_actions.end(); iter++) {
        auto action = *iter;
        if (action->isDone()) {
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
