#include <mak3do/scenegraph/ActionRunner.h>
#include <mak3do/scenegraph/Node.h>
#include <mak3do/scenegraph/actions/Action.h>

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
    std::remove_if(m_actions.begin(), m_actions.end(), [&](ActionPtr& action) {
        auto done = action->done();
        if (!done) {
            action->step(dt);
        }
        
        return done;
    });
}

}
