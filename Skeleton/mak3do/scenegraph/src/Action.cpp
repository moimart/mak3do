#include <mak3do/scenegraph/actions/Action.h>
//#include "ActionInterval.h"
#include <mak3do/scenegraph/Director.h>
#include <mak3do/scenegraph/Node.h>

using namespace mak3do;
//
// Action Base Class
//

Action::Action()
    : m_pOriginalTarget(NULL)
    , m_target(NULL)
    , m_nTag(kActionTagInvalid)
{
}

Action::~Action()
{
}

ActionPtr Action::make()
{
    return std::make_shared<Action>();
}

std::string Action::description() const
{
    return std::string("<Action | Tag = " + std::to_string(m_nTag) + ">");
}

void Action::start(NodePtr aTarget)
{
    m_pOriginalTarget = m_target = aTarget;
}

void Action::stop()
{
    m_target = NULL;
}

bool Action::done()
{
    return true;
}

void Action::step(float dt)
{
  //logger::print("[Action step]. override me");
}

void Action::update(float time)
{
    //logger::print("[Action update]. override me");
}

//
// FiniteTimeAction
//

ActionPtr FiniteTimeAction::reverse()
{
    //logger::print("cocos2d: FiniteTimeAction#reverse: Implement me");
    return nullptr;
}

