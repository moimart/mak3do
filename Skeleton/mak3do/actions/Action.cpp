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
#include "ActionInterval.h"
#include <mak3do/base/Director.h>
#include <mak3do/base/Log.h>
#include <mak3do/base/Node.h>

using namespace mak3do;
//
// Action Base Class
//

Action::Action()
    : m_pOriginalTarget(NULL)
    , m_pTarget(NULL)
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

void Action::startWithTarget(Node* aTarget)
{
    m_pOriginalTarget = m_pTarget = aTarget;
}

void Action::stop()
{
    m_pTarget = NULL;
}

bool Action::isDone()
{
    return true;
}

void Action::step(float dt)
{
    CC_UNUSED_PARAM(dt);
    logger::print("[Action step]. override me");
}

void Action::update(float time)
{
    CC_UNUSED_PARAM(time);
    logger::print("[Action update]. override me");
}

//
// FiniteTimeAction
//

ActionPtr FiniteTimeAction::reverse()
{
    logger::print("cocos2d: FiniteTimeAction#reverse: Implement me");
    return NULL;
}

//
// Speed
//
SpeedPtr Speed::make(ActionIntervalPtr pAction, float fSpeed)
{
    auto pRet = std::make_shared<Speed>();
    pRet->initWithAction(pAction, fSpeed);

    return pRet;
}

bool Speed::initWithAction(ActionIntervalPtr pAction, float fSpeed)
{
    logger::assertion(pAction != NULL, "");
    m_pInnerAction = pAction;
    m_fSpeed = fSpeed;
    return true;
}

void Speed::startWithTarget(Node* pTarget)
{
    Action::startWithTarget(pTarget);
    m_pInnerAction->startWithTarget(pTarget);
}

void Speed::stop()
{
    m_pInnerAction->stop();
    Action::stop();
}

void Speed::step(float dt)
{
    m_pInnerAction->step(dt * m_fSpeed);
}

bool Speed::isDone()
{
    return m_pInnerAction->isDone();
}

ActionPtr Speed::reverse()
{
    return Speed::make(std::static_pointer_cast<ActionInterval>(m_pInnerAction->reverse()), m_fSpeed);
}

void Speed::setInnerAction(ActionIntervalPtr pAction)
{
    if (m_pInnerAction != pAction) {
        CC_SAFE_RELEASE(m_pInnerAction);
        m_pInnerAction = pAction;
        CC_SAFE_RETAIN(m_pInnerAction);
    }
}

//
// Follow
//
Follow::~Follow()
{
    CC_SAFE_RELEASE(m_pobFollowedNode);
}

Follow* Follow::create(Node* pFollowedNode, const Rect4& rect /* = CCRectZero*/)
{
    Follow* pRet = new Follow();
    if (pRet && pRet->initWithTarget(pFollowedNode, rect)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool Follow::initWithTarget(Node* pFollowedNode, const Rect4& rect /* = CCRectZero*/)
{
    logger::assertion(pFollowedNode != NULL, "");

    pFollowedNode->retain();
    m_pobFollowedNode = pFollowedNode;
    if (rect.equals(CCRectZero)) {
        m_bBoundarySet = false;
    } else {
        m_bBoundarySet = true;
    }

    m_bBoundaryFullyCovered = false;

    Vec2 winSize = mak3do::Director::getInstance()->getWinSize();
    m_obFullScreenSize = Vec2(winSize.width, winSize.height);
    m_obHalfScreenSize = (m_obFullScreenSize * .5f);

    if (m_bBoundarySet) {
        m_fLeftBoundary = -((rect.origin.x + rect.size.width) - m_obFullScreenSize.x);
        m_fRightBoundary = -rect.origin.x;
        m_fTopBoundary = -rect.origin.y;
        m_fBottomBoundary = -((rect.origin.y + rect.size.height) - m_obFullScreenSize.y);

        if (m_fRightBoundary < m_fLeftBoundary) {
            // screen width is larger than world's boundary width
            //set both in the middle of the world
            m_fRightBoundary = m_fLeftBoundary = (m_fLeftBoundary + m_fRightBoundary) / 2;
        }
        if (m_fTopBoundary < m_fBottomBoundary) {
            // screen width is larger than world's boundary width
            //set both in the middle of the world
            m_fTopBoundary = m_fBottomBoundary = (m_fTopBoundary + m_fBottomBoundary) / 2;
        }

        if ((m_fTopBoundary == m_fBottomBoundary) && (m_fLeftBoundary == m_fRightBoundary)) {
            m_bBoundaryFullyCovered = true;
        }
    }

    return true;
}

void Follow::step(float dt)
{
    CC_UNUSED_PARAM(dt);

    if (m_bBoundarySet) {
        // whole map fits inside a single screen, no need to modify the position - unless map boundaries are increased
        if (m_bBoundaryFullyCovered)
            return;

        Vec2 tempPos = m_obHalfScreenSize - m_pobFollowedNode->getPosition();

        m_pTarget->setPosition(Vec2(clampf(tempPos.x, m_fLeftBoundary, m_fRightBoundary),
            clampf(tempPos.y, m_fBottomBoundary, m_fTopBoundary)));
    } else {
        m_pTarget->setPosition(m_obHalfScreenSize - m_pobFollowedNode->getPosition());
    }
}

bool Follow::isDone()
{
    return (!m_pobFollowedNode->isRunning());
}

void Follow::stop()
{
    m_pTarget = NULL;
    Action::stop();
}
