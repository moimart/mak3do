/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2011      Zynga Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "Action.h"
//#include "ActionInterval.h"
#include "../Director.h"
#include "../Node.h"

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

void Action::startWithTarget(NodePtr aTarget)
{
    m_pOriginalTarget = m_target = aTarget;
}

void Action::stop()
{
    m_target = NULL;
}

bool Action::isDone()
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

