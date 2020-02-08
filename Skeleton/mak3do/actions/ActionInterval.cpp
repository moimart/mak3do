/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2011 Zynga Inc.
 
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

#include "ActionInterval.h"
#include "../Node.h"
#include <stdarg.h>

/** @def CC_ENABLE_STACKABLE_ACTIONS
 If enabled, actions that alter the position property (eg: CCMoveBy, CCJumpBy, CCBezierBy, etc..) will be stacked.
 If you run 2 or more 'position' actions at the same time on a node, then end position will be the sum of all the positions.
 If disabled, only the last run action will take effect.

 Enabled by default. Disable to be compatible with v2.0 and older versions.

 @since v2.1
 */
#ifndef CC_ENABLE_STACKABLE_ACTIONS
#define CC_ENABLE_STACKABLE_ACTIONS 1
#endif

using namespace mak3do;

class ExtraAction;
typedef std::shared_ptr<ExtraAction> ExtraActionPtr;

// Extra action for making a Sequence or Spawn when only adding one action to it.
class ExtraAction : public FiniteTimeAction {
public:
    static ExtraActionPtr make();
    virtual ActionPtr reverse(void);
    virtual void update(float time);
    virtual void step(float dt);
};

ExtraActionPtr ExtraAction::make()
{
    return std::make_shared<ExtraAction>();
}

ActionPtr ExtraAction::reverse(void)
{
    return ExtraAction::make();
}

void ExtraAction::update(float time)
{
}

void ExtraAction::step(float dt)
{
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
    //logger::assertion(pAction != NULL, "");
    m_pInnerAction = pAction;
    m_fSpeed = fSpeed;
    return true;
}

void Speed::startWithTarget(NodePtr pTarget)
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
        m_pInnerAction = pAction;
    }
}


//
// IntervalAction
//
ActionIntervalPtr ActionInterval::make(float d)
{
    auto pAction = std::make_shared<ActionInterval>();
    pAction->initWithDuration(d);

    return pAction;
}

bool ActionInterval::initWithDuration(float d)
{
    m_fDuration = d;

    // prevent division by 0
    // This comparison could be in step:, but it might decrease the performance
    // by 3% in heavy based action games.
    if (m_fDuration == 0) {
        m_fDuration = FLT_EPSILON;
    }

    m_elapsed = 0;
    m_bFirstTick = true;

    return true;
}

bool ActionInterval::isDone(void)
{
    return m_elapsed >= m_fDuration;
}

void ActionInterval::step(float dt)
{
    if (m_bFirstTick) {
        m_bFirstTick = false;
        m_elapsed = 0;
    } else {
        m_elapsed += dt;
    }

    this->update(MAX(0, // needed for rewind. elapsed could be negative
        MIN(1, m_elapsed / MAX(m_fDuration, FLT_EPSILON) // division by 0
            )));
}

void ActionInterval::setAmplitudeRate(float amp)
{
}

float ActionInterval::getAmplitudeRate(void)
{
    return 0;
}

void ActionInterval::startWithTarget(NodePtr pTarget)
{
    FiniteTimeAction::startWithTarget(pTarget);
    m_elapsed = 0.0f;
    m_bFirstTick = true;
}

ActionPtr ActionInterval::reverse(void)
{
    return nullptr;
}

//
// Sequence
//

SequencePtr Sequence::makeWithTwoActions(FiniteTimeActionPtr pActionOne, FiniteTimeActionPtr pActionTwo)
{
    auto pSequence = std::make_shared<Sequence>();
    pSequence->initWithTwoActions(pActionOne, pActionTwo);

    return pSequence;
}

SequencePtr Sequence::make(const std::vector<ActionPtr>& actions)
{
    SequencePtr pRet;

    do {
        auto count = actions.size();
        assert(count > 0);

        auto prev = std::static_pointer_cast<FiniteTimeAction>(actions[0]);

        if (count > 1) {
            for (unsigned int i = 1; i < count; ++i) {
                prev = makeWithTwoActions(prev, std::static_pointer_cast<FiniteTimeAction>(actions[i]));
            }
        } else {
            // If only one action is added to Sequence, make up a Sequence by adding a simplest finite time action.
            prev = makeWithTwoActions(prev, ExtraAction::make());
        }

        pRet = std::static_pointer_cast<Sequence>(prev);

    } while (0);

    return pRet;
}

bool Sequence::initWithTwoActions(FiniteTimeActionPtr pActionOne, FiniteTimeActionPtr pActionTwo)
{
    float d = pActionOne->getDuration() + pActionTwo->getDuration();
    ActionInterval::initWithDuration(d);

    m_pActions[0] = pActionOne;
    m_pActions[1] = pActionTwo;

    return true;
}

void Sequence::startWithTarget(NodePtr pTarget)
{
    ActionInterval::startWithTarget(pTarget);
    m_split = m_pActions[0]->getDuration() / m_fDuration;
    m_last = -1;
}

void Sequence::stop(void)
{
    // Issue #1305
    if (m_last != -1) {
        m_pActions[m_last]->stop();
    }

    ActionInterval::stop();
}

void Sequence::update(float t)
{
    int found = 0;
    float new_t = 0.0f;

    if (t < m_split) {
        // action[0]
        found = 0;
        if (m_split != 0)
            new_t = t / m_split;
        else
            new_t = 1;

    } else {
        // action[1]
        found = 1;
        if (m_split == 1)
            new_t = 1;
        else
            new_t = (t - m_split) / (1 - m_split);
    }

    if (found == 1) {

        if (m_last == -1) {
            // action[0] was skipped, execute it.
            m_pActions[0]->startWithTarget(m_target);
            m_pActions[0]->update(1.0f);
            m_pActions[0]->stop();
        } else if (m_last == 0) {
            // switching to action 1. stop action 0.
            m_pActions[0]->update(1.0f);
            m_pActions[0]->stop();
        }
    } else if (found == 0 && m_last == 1) {
        // Reverse mode ?
        // XXX: Bug. this case doesn't contemplate when _last==-1, found=0 and in "reverse mode"
        // since it will require a hack to know if an action is on reverse mode or not.
        // "step" should be overriden, and the "reverseMode" value propagated to inner Sequences.
        m_pActions[1]->update(0);
        m_pActions[1]->stop();
    }
    // Last action found and it is done.
    if (found == m_last && m_pActions[found]->isDone()) {
        return;
    }

    // Last action found and it is done
    if (found != m_last) {
        m_pActions[found]->startWithTarget(m_target);
    }

    m_pActions[found]->update(new_t);
    m_last = found;
}

ActionPtr Sequence::reverse(void)
{
    return Sequence::makeWithTwoActions(std::static_pointer_cast<FiniteTimeAction>(m_pActions[1]->reverse()), std::static_pointer_cast<FiniteTimeAction>(m_pActions[0]->reverse()));
}

//
// Repeat
//

RepeatPtr Repeat::make(FiniteTimeActionPtr pAction, unsigned int times)
{
    auto pRepeat = std::make_shared<Repeat>();
    pRepeat->initWithAction(pAction, times);

    return pRepeat;
}

bool Repeat::initWithAction(FiniteTimeActionPtr pAction, unsigned int times)
{
    float d = pAction->getDuration() * times;

    if (ActionInterval::initWithDuration(d)) {
        m_uTimes = times;
        m_pInnerAction = pAction;

        m_bActionInstant = false; //dynamic_cast<ActionInstant*>(pAction) ? true : false;
        //an instant action needs to be executed one time less in the update method since it uses startWithTarget to execute the action
        if (m_bActionInstant) {
            m_uTimes -= 1;
        }
        m_uTotal = 0;

        return true;
    }

    return false;
}

void Repeat::startWithTarget(NodePtr pTarget)
{
    m_uTotal = 0;
    m_fNextDt = m_pInnerAction->getDuration() / m_fDuration;
    ActionInterval::startWithTarget(pTarget);
    m_pInnerAction->startWithTarget(pTarget);
}

void Repeat::stop(void)
{
    m_pInnerAction->stop();
    ActionInterval::stop();
}

// issue #80. Instead of hooking step:, hook update: since it can be called by any
// container action like Repeat, Sequence, Ease, etc..
void Repeat::update(float dt)
{
    if (dt >= m_fNextDt) {
        while (dt > m_fNextDt && m_uTotal < m_uTimes) {

            m_pInnerAction->update(1.0f);
            m_uTotal++;

            m_pInnerAction->stop();
            m_pInnerAction->startWithTarget(m_target);
            m_fNextDt += m_pInnerAction->getDuration() / m_fDuration;
        }

        // fix for issue #1288, incorrect end value of repeat
        if (dt >= 1.0f && m_uTotal < m_uTimes) {
            m_uTotal++;
        }

        // don't set an instant action back or update it, it has no use because it has no duration
        if (!m_bActionInstant) {
            if (m_uTotal == m_uTimes) {
                m_pInnerAction->update(1);
                m_pInnerAction->stop();
            } else {
                // issue #390 prevent jerk, use right update
                m_pInnerAction->update(dt - (m_fNextDt - m_pInnerAction->getDuration() / m_fDuration));
            }
        }
    } else {
        m_pInnerAction->update(fmodf(dt * m_uTimes, 1.0f));
    }
}

bool Repeat::isDone(void)
{
    return m_uTotal == m_uTimes;
}

ActionPtr Repeat::reverse(void)
{
    return Repeat::make(std::static_pointer_cast<FiniteTimeAction>(m_pInnerAction->reverse()), m_uTimes);
}

RepeatForeverPtr RepeatForever::make(ActionIntervalPtr pAction)
{
    auto pRet = std::make_shared<RepeatForever>();
    pRet->initWithAction(pAction);

    return pRet;
}

bool RepeatForever::initWithAction(ActionIntervalPtr pAction)
{
    assert(pAction != nullptr);
    m_pInnerAction = pAction;
    return true;
}

void RepeatForever::startWithTarget(NodePtr pTarget)
{
    ActionInterval::startWithTarget(pTarget);
    m_pInnerAction->startWithTarget(pTarget);
}

void RepeatForever::step(float dt)
{
    m_pInnerAction->step(dt);
    if (m_pInnerAction->isDone()) {
        float diff = m_pInnerAction->getElapsed() - m_pInnerAction->getDuration();
        m_pInnerAction->startWithTarget(m_target);
        // to prevent jerk. issue #390, 1247
        m_pInnerAction->step(0.0f);
        m_pInnerAction->step(diff);
    }
}

bool RepeatForever::isDone()
{
    return false;
}

ActionPtr RepeatForever::reverse()
{
    return RepeatForever::make(std::static_pointer_cast<ActionInterval>(m_pInnerAction->reverse()));
}

//
// Spawn
//

SpawnPtr Spawn::make(const std::vector<ActionPtr>& actions)
{
    SpawnPtr pRet;

    do {
        auto count = actions.size();

        assert(count > 0);

        auto prev = std::static_pointer_cast<FiniteTimeAction>(actions[0]);

        if (count > 1) {
            for (unsigned int i = 1; i < actions.size(); ++i) {
                prev = makeWithTwoActions(prev, std::static_pointer_cast<FiniteTimeAction>(actions[i]));
            }
        } else {
            // If only one action is added to Spawn, make up a Spawn by adding a simplest finite time action.
            prev = makeWithTwoActions(prev, ExtraAction::make());
        }

        pRet = std::static_pointer_cast<Spawn>(prev);

    } while (0);

    return pRet;
}

SpawnPtr Spawn::makeWithTwoActions(FiniteTimeActionPtr pAction1, FiniteTimeActionPtr pAction2)
{
    auto pSpawn = std::make_shared<Spawn>();
    pSpawn->initWithTwoActions(pAction1, pAction2);

    return pSpawn;
}

bool Spawn::initWithTwoActions(FiniteTimeActionPtr pAction1, FiniteTimeActionPtr pAction2)
{
    assert(pAction1 != nullptr);
    assert(pAction2 != nullptr);

    bool bRet = false;

    float d1 = pAction1->getDuration();
    float d2 = pAction2->getDuration();

    if (ActionInterval::initWithDuration(MAX(d1, d2))) {
        m_pOne = pAction1;
        m_pTwo = pAction2;

        if (d1 > d2) {
            m_pTwo = Sequence::makeWithTwoActions(pAction2, DelayTime::make(d1 - d2));
        } else if (d1 < d2) {
            m_pOne = Sequence::makeWithTwoActions(pAction1, DelayTime::make(d2 - d1));
        }

        bRet = true;
    }

    return bRet;
}

void Spawn::startWithTarget(NodePtr pTarget)
{
    ActionInterval::startWithTarget(pTarget);
    m_pOne->startWithTarget(pTarget);
    m_pTwo->startWithTarget(pTarget);
}

void Spawn::stop(void)
{
    m_pOne->stop();
    m_pTwo->stop();
    ActionInterval::stop();
}

void Spawn::update(float time)
{
    if (m_pOne) {
        m_pOne->update(time);
    }
    if (m_pTwo) {
        m_pTwo->update(time);
    }
}

ActionPtr Spawn::reverse(void)
{
    return Spawn::makeWithTwoActions(std::static_pointer_cast<FiniteTimeAction>(m_pOne->reverse()), std::static_pointer_cast<FiniteTimeAction>(m_pTwo->reverse()));
}

//
// RotateTo
//

RotateToPtr RotateTo::make(float fDuration, float fDeltaAngle)
{
    auto pRotateTo = std::make_shared<RotateTo>();
    pRotateTo->initWithDuration(fDuration, fDeltaAngle);

    return pRotateTo;
}

bool RotateTo::initWithDuration(float fDuration, float fDeltaAngle)
{
    if (ActionInterval::initWithDuration(fDuration)) {
        m_fDstAngleX = m_fDstAngleY = fDeltaAngle;
        return true;
    }

    return false;
}

RotateToPtr RotateTo::make(float fDuration, float fDeltaAngleX, float fDeltaAngleY)
{
    auto pRotateTo = std::make_shared<RotateTo>();
    pRotateTo->initWithDuration(fDuration, fDeltaAngleX, fDeltaAngleY);

    return pRotateTo;
}

bool RotateTo::initWithDuration(float fDuration, float fDeltaAngleX, float fDeltaAngleY)
{
    if (ActionInterval::initWithDuration(fDuration)) {
        m_fDstAngleX = fDeltaAngleX;
        m_fDstAngleY = fDeltaAngleY;

        return true;
    }

    return false;
}

void RotateTo::startWithTarget(NodePtr pTarget)
{
    ActionInterval::startWithTarget(pTarget);

    // Calculate X
    m_fStartAngleX = pTarget->yaw();
    if (m_fStartAngleX > 0) {
        m_fStartAngleX = fmodf(m_fStartAngleX, 360.0f);
    } else {
        m_fStartAngleX = fmodf(m_fStartAngleX, -360.0f);
    }

    m_fDiffAngleX = m_fDstAngleX - m_fStartAngleX;
    if (m_fDiffAngleX > 180) {
        m_fDiffAngleX -= 360;
    }
    if (m_fDiffAngleX < -180) {
        m_fDiffAngleX += 360;
    }

    //Calculate Y: It's duplicated from calculating X since the rotation wrap should be the same
    m_fStartAngleY = m_target->roll();

    if (m_fStartAngleY > 0) {
        m_fStartAngleY = fmodf(m_fStartAngleY, 360.0f);
    } else {
        m_fStartAngleY = fmodf(m_fStartAngleY, -360.0f);
    }

    m_fDiffAngleY = m_fDstAngleY - m_fStartAngleY;
    if (m_fDiffAngleY > 180) {
        m_fDiffAngleY -= 360;
    }

    if (m_fDiffAngleY < -180) {
        m_fDiffAngleY += 360;
    }
}

void RotateTo::update(float time)
{
    if (m_target) {
        m_target->yaw(m_fStartAngleX + m_fDiffAngleX * time);
        m_target->roll(m_fStartAngleY + m_fDiffAngleY * time);
    }
}

//
// RotateBy
//

RotateByPtr RotateBy::make(float fDuration, float fDeltaAngle)
{
    auto pRotateBy = std::make_shared<RotateBy>();
    pRotateBy->initWithDuration(fDuration, fDeltaAngle);

    return pRotateBy;
}

bool RotateBy::initWithDuration(float fDuration, float fDeltaAngle)
{
    if (ActionInterval::initWithDuration(fDuration)) {
        m_fAngleX = m_fAngleY = fDeltaAngle;
        return true;
    }

    return false;
}

RotateByPtr RotateBy::make(float fDuration, float fDeltaAngleX, float fDeltaAngleY)
{
    auto pRotateBy = std::make_shared<RotateBy>();
    pRotateBy->initWithDuration(fDuration, fDeltaAngleX, fDeltaAngleY);

    return pRotateBy;
}

bool RotateBy::initWithDuration(float fDuration, float fDeltaAngleX, float fDeltaAngleY)
{
    if (ActionInterval::initWithDuration(fDuration)) {
        m_fAngleX = fDeltaAngleX;
        m_fAngleY = fDeltaAngleY;
        return true;
    }

    return false;
}

void RotateBy::startWithTarget(NodePtr pTarget)
{
    ActionInterval::startWithTarget(pTarget);
    m_fStartAngleX = pTarget->yaw();
    m_fStartAngleY = pTarget->roll();
}

void RotateBy::update(float time)
{
    // XXX: shall I add % 360
    if (m_target) {
        m_target->yaw(m_fStartAngleX + m_fAngleX * time);
        m_target->roll(m_fStartAngleY + m_fAngleY * time);
    }
}

ActionPtr RotateBy::reverse(void)
{
    return RotateBy::make(m_fDuration, -m_fAngleX, -m_fAngleY);
}

//
// MoveBy
//

MoveByPtr MoveBy::make(float duration, const Vec3& deltaPosition)
{
    auto pRet = std::make_shared<MoveBy>();
    pRet->initWithDuration(duration, deltaPosition);

    return pRet;
}

bool MoveBy::initWithDuration(float duration, const Vec3& deltaPosition)
{
    if (ActionInterval::initWithDuration(duration)) {
        m_positionDelta = deltaPosition;
        return true;
    }

    return false;
}

void MoveBy::startWithTarget(NodePtr pTarget)
{
    ActionInterval::startWithTarget(pTarget);
    m_previousPosition = m_startPosition = pTarget->position();
}

ActionPtr MoveBy::reverse(void)
{
    return MoveBy::make(m_fDuration, Vec3(-m_positionDelta.x, -m_positionDelta.y));
}

void MoveBy::update(float t)
{
    if (m_target) {
#if _ENABLE_STACKABLE_ACTIONS
        Vec3 currentPos = m_target->getPosition();
        Vec3 diff = currentPos - m_previousPosition;
        m_startPosition = m_startPosition + diff;
        Vec3 newPos = m_startPosition + (ccpMult(m_positionDelta, t));
        m_target->setPosition(newPos);
        m_previousPosition = newPos;
#else
        m_target->position(m_startPosition + (m_positionDelta * t));
#endif // _ENABLE_STACKABLE_ACTIONS
    }
}

//
// MoveTo
//

MoveToPtr MoveTo::make(float duration, const Vec3& position)
{
    auto pRet = std::make_shared<MoveTo>();
    pRet->initWithDuration(duration, position);

    return pRet;
}

bool MoveTo::initWithDuration(float duration, const Vec3& position)
{
    if (ActionInterval::initWithDuration(duration)) {
        m_endPosition = position;
        return true;
    }

    return false;
}

void MoveTo::startWithTarget(NodePtr pTarget)
{
    MoveBy::startWithTarget(pTarget);
    m_positionDelta = m_endPosition - pTarget->position();
}

//
// SkewTo
//
SkewToPtr SkewTo::make(float t, float sx, float sy)
{
    auto pSkewTo = std::make_shared<SkewTo>();
    pSkewTo->initWithDuration(t, sx, sy);

    return pSkewTo;
}

bool SkewTo::initWithDuration(float t, float sx, float sy)
{
    bool bRet = false;

    if (ActionInterval::initWithDuration(t)) {
        m_fEndSkewX = sx;
        m_fEndSkewY = sy;

        bRet = true;
    }

    return bRet;
}

void SkewTo::startWithTarget(NodePtr pTarget)
{
    ActionInterval::startWithTarget(pTarget);

    //m_fStartSkewX = pTarget->getSkewX();

    if (m_fStartSkewX > 0) {
        m_fStartSkewX = fmodf(m_fStartSkewX, 180.f);
    } else {
        m_fStartSkewX = fmodf(m_fStartSkewX, -180.f);
    }

    m_fDeltaX = m_fEndSkewX - m_fStartSkewX;

    if (m_fDeltaX > 180) {
        m_fDeltaX -= 360;
    }
    if (m_fDeltaX < -180) {
        m_fDeltaX += 360;
    }

    //m_fStartSkewY = pTarget->getSkewY();

    if (m_fStartSkewY > 0) {
        m_fStartSkewY = fmodf(m_fStartSkewY, 360.f);
    } else {
        m_fStartSkewY = fmodf(m_fStartSkewY, -360.f);
    }

    m_fDeltaY = m_fEndSkewY - m_fStartSkewY;

    if (m_fDeltaY > 180) {
        m_fDeltaY -= 360;
    }
    if (m_fDeltaY < -180) {
        m_fDeltaY += 360;
    }
}

void SkewTo::update(float t)
{
    //m_target->setSkewX(m_fStartSkewX + m_fDeltaX * t);
    //m_target->setSkewY(m_fStartSkewY + m_fDeltaY * t);
}

SkewTo::SkewTo()
    : m_fSkewX(0.0)
    , m_fSkewY(0.0)
    , m_fStartSkewX(0.0)
    , m_fStartSkewY(0.0)
    , m_fEndSkewX(0.0)
    , m_fEndSkewY(0.0)
    , m_fDeltaX(0.0)
    , m_fDeltaY(0.0)
{
}

//
// SkewBy
//
SkewByPtr SkewBy::make(float t, float sx, float sy)
{
    auto pSkewBy = std::make_shared<SkewBy>();
    pSkewBy->initWithDuration(t, sx, sy);

    return pSkewBy;
}

bool SkewBy::initWithDuration(float t, float deltaSkewX, float deltaSkewY)
{
    bool bRet = false;

    if (SkewTo::initWithDuration(t, deltaSkewX, deltaSkewY)) {
        m_fSkewX = deltaSkewX;
        m_fSkewY = deltaSkewY;

        bRet = true;
    }

    return bRet;
}

void SkewBy::startWithTarget(NodePtr pTarget)
{
    SkewTo::startWithTarget(pTarget);
    m_fDeltaX = m_fSkewX;
    m_fDeltaY = m_fSkewY;
    m_fEndSkewX = m_fStartSkewX + m_fDeltaX;
    m_fEndSkewY = m_fStartSkewY + m_fDeltaY;
}

ActionPtr SkewBy::reverse()
{
    return make(m_fDuration, -m_fSkewX, -m_fSkewY);
}

//
// JumpBy
//

JumpByPtr JumpBy::make(float duration, const Vec3& position, float height, unsigned int jumps)
{
    auto pJumpBy = std::make_shared<JumpBy>();
    pJumpBy->initWithDuration(duration, position, height, jumps);

    return pJumpBy;
}

bool JumpBy::initWithDuration(float duration, const Vec3& position, float height, unsigned int jumps)
{
    if (ActionInterval::initWithDuration(duration)) {
        m_delta = position;
        m_height = height;
        m_nJumps = jumps;

        return true;
    }

    return false;
}

void JumpBy::startWithTarget(NodePtr pTarget)
{
    ActionInterval::startWithTarget(pTarget);
    m_previousPos = m_startPosition = pTarget->position();
}

void JumpBy::update(float t)
{
    // parabolic jump (since v0.8.2)
    if (m_target) {
        float frac = fmodf(t * m_nJumps, 1.0f);
        float y = m_height * 4 * frac * (1 - frac);
        y += m_delta.y * t;

        float x = m_delta.x * t;
#if CC_ENABLE_STACKABLE_ACTIONS
        Vec3 currentPos = m_target->position();

        Vec3 diff = currentPos - m_previousPos;
        m_startPosition = diff + m_startPosition;

        Vec3 newPos = m_startPosition + Vec3(x, y);
        m_target->position(newPos);

        m_previousPos = newPos;
#else
        m_target->setPosition(m_startPosition + Vec3(x, y));
#endif // !CC_ENABLE_STACKABLE_ACTIONS
    }
}

ActionPtr JumpBy::reverse(void)
{
    return JumpBy::make(m_fDuration, Vec3(-m_delta.x, -m_delta.y),
        m_height, m_nJumps);
}

//
// JumpTo
//

JumpToPtr JumpTo::make(float duration, const Vec3& position, float height, int jumps)
{
    auto pJumpTo = std::make_shared<JumpTo>();
    pJumpTo->initWithDuration(duration, position, height, jumps);

    return pJumpTo;
}

void JumpTo::startWithTarget(NodePtr pTarget)
{
    JumpBy::startWithTarget(pTarget);
    m_delta = Vec3(m_delta.x - m_startPosition.x, m_delta.y - m_startPosition.y);
}

// Bezier cubic formula:
//    ((1 - t) + t)3 = 1
// Expands to¡­
//   (1 - t)3 + 3t(1-t)2 + 3t2(1 - t) + t3 = 1
static inline float bezierat(float a, float b, float c, float d, float t)
{
    return (powf(1 - t, 3) * a + 3 * t * (powf(1 - t, 2)) * b + 3 * powf(t, 2) * (1 - t) * c + powf(t, 3) * d);
}

//
// BezierBy
//

BezierByPtr BezierBy::make(float t, const BezierConfig& c)
{
    auto pBezierBy = std::make_shared<BezierBy>();
    pBezierBy->initWithDuration(t, c);

    return pBezierBy;
}

bool BezierBy::initWithDuration(float t, const BezierConfig& c)
{
    if (ActionInterval::initWithDuration(t)) {
        m_sConfig = c;
        return true;
    }

    return false;
}

void BezierBy::startWithTarget(NodePtr pTarget)
{
    ActionInterval::startWithTarget(pTarget);
    m_previousPosition = m_startPosition = pTarget->position();
}

void BezierBy::update(float time)
{
    if (m_target) {
        float xa = 0;
        float xb = m_sConfig.control_1.x;
        float xc = m_sConfig.control_2.x;
        float xd = m_sConfig.endPosition.x;

        float ya = 0;
        float yb = m_sConfig.control_1.y;
        float yc = m_sConfig.control_2.y;
        float yd = m_sConfig.endPosition.y;

        float x = bezierat(xa, xb, xc, xd, time);
        float y = bezierat(ya, yb, yc, yd, time);

#if _ENABLE_STACKABLE_ACTIONS
        Vec3 currentPos = m_target->getPosition();
        Vec3 diff = currentPos - m_previousPosition;
        m_startPosition = m_startPosition + diff;

        Vec3 newPos = m_startPosition + Vec3(x, y);
        m_target->setPosition(newPos);

        m_previousPosition = newPos;
#else
        m_target->position(m_startPosition + Vec3(x, y, 0));
#endif // !CC_ENABLE_STACKABLE_ACTIONS
    }
}

ActionPtr BezierBy::reverse(void)
{
    BezierConfig r;

    r.endPosition = -m_sConfig.endPosition;
    r.control_1 = m_sConfig.control_2 + -m_sConfig.endPosition;
    r.control_2 = m_sConfig.control_1 + -m_sConfig.endPosition;

    BezierByPtr pAction = BezierBy::make(m_fDuration, r);
    return pAction;
}

//
// BezierTo
//

BezierToPtr BezierTo::make(float t, const BezierConfig& c)
{
    auto pBezierTo = std::make_shared<BezierTo>();
    pBezierTo->initWithDuration(t, c);

    return pBezierTo;
}

bool BezierTo::initWithDuration(float t, const BezierConfig& c)
{
    bool bRet = false;

    if (ActionInterval::initWithDuration(t)) {
        m_sToConfig = c;
    }

    return bRet;
}

void BezierTo::startWithTarget(NodePtr pTarget)
{
    BezierBy::startWithTarget(pTarget);
    m_sConfig.control_1 = m_sToConfig.control_1 - m_startPosition;
    m_sConfig.control_2 = m_sToConfig.control_2 - m_startPosition;
    m_sConfig.endPosition = m_sToConfig.endPosition - m_startPosition;
}

//
// ScaleTo
//
ScaleToPtr ScaleTo::make(float duration, float s)
{
    auto pScaleTo = std::make_shared<ScaleTo>();
    pScaleTo->initWithDuration(duration, s);

    return pScaleTo;
}

bool ScaleTo::initWithDuration(float duration, float s)
{
    if (ActionInterval::initWithDuration(duration)) {
        m_fEndScaleX = s;
        m_fEndScaleY = s;

        return true;
    }

    return false;
}

ScaleToPtr ScaleTo::make(float duration, float sx, float sy)
{
    auto pScaleTo = std::make_shared<ScaleTo>();
    pScaleTo->initWithDuration(duration, sx, sy);

    return pScaleTo;
}

bool ScaleTo::initWithDuration(float duration, float sx, float sy)
{
    if (ActionInterval::initWithDuration(duration)) {
        m_fEndScaleX = sx;
        m_fEndScaleY = sy;

        return true;
    }

    return false;
}

void ScaleTo::startWithTarget(NodePtr pTarget)
{
    ActionInterval::startWithTarget(pTarget);
    m_fStartScaleX = pTarget->scale();
    m_fDeltaX = m_fEndScaleX - m_fStartScaleX;
}

void ScaleTo::update(float time)
{
    if (m_target) {
        m_target->scale(m_fStartScaleX + m_fDeltaX * time);
    }
}

//
// ScaleBy
//

ScaleByPtr ScaleBy::make(float duration, float s)
{
    auto pScaleBy = std::make_shared<ScaleBy>();
    pScaleBy->initWithDuration(duration, s);

    return pScaleBy;
}

ScaleByPtr ScaleBy::make(float duration, float sx, float sy)
{
    auto pScaleBy = std::make_shared<ScaleBy>();
    pScaleBy->initWithDuration(duration, sx, sy);

    return pScaleBy;
}

void ScaleBy::startWithTarget(NodePtr pTarget)
{
    ScaleTo::startWithTarget(pTarget);
    m_fDeltaX = m_fStartScaleX * m_fEndScaleX - m_fStartScaleX;
    m_fDeltaY = m_fStartScaleY * m_fEndScaleY - m_fStartScaleY;
}

ActionPtr ScaleBy::reverse(void)
{
    return ScaleBy::make(m_fDuration, 1 / m_fEndScaleX, 1 / m_fEndScaleY);
}

//
// Blink
//

BlinkPtr Blink::make(float duration, unsigned int uBlinks)
{
    auto pBlink = std::make_shared<Blink>();
    pBlink->initWithDuration(duration, uBlinks);

    return pBlink;
}

bool Blink::initWithDuration(float duration, unsigned int uBlinks)
{
    if (ActionInterval::initWithDuration(duration)) {
        m_nTimes = uBlinks;
        return true;
    }

    return false;
}

void Blink::stop()
{
    m_target->visible(m_bOriginalState);
    ActionInterval::stop();
}

void Blink::startWithTarget(NodePtr pTarget)
{
    ActionInterval::startWithTarget(pTarget);
    m_bOriginalState = pTarget->visible();
}

void Blink::update(float time)
{
    if (m_target && !isDone()) {
        float slice = 1.0f / m_nTimes;
        float m = fmodf(time, slice);
        m_target->visible(m > slice / 2 ? true : false);
    }
}

ActionPtr Blink::reverse(void)
{
    // return 'self'
    return Blink::make(m_fDuration, m_nTimes);
}

//
// FadeIn
//

FadeInPtr FadeIn::make(float d)
{
    auto pAction = std::make_shared<FadeIn>();
    pAction->setLimit(1);
    pAction->initWithDuration(d);

    return pAction;
}

FadeInPtr FadeIn::make(float d, float limit)
{
    auto pAction = std::make_shared<FadeIn>();
    pAction->setLimit(limit);
    pAction->initWithDuration(d);

    return pAction;
}

void FadeIn::update(float time)
{
    //RGBAInterface* protocol = dynamic_cast<RGBAInterface*>(m_target);
    //if (protocol) {
    //    protocol->setOpacity((unsigned char)(255.f * m_limit * time));
    //}
    /*m_target->setOpacity((unsigned char)(255 * time));*/
}

ActionPtr FadeIn::reverse(void)
{
    return FadeOut::make(m_fDuration);
}

//
// FadeOut
//

FadeOutPtr FadeOut::make(float d)
{
    auto pAction = std::make_shared<FadeOut>();
    pAction->setLimit(1);
    pAction->initWithDuration(d);

    return pAction;
}

FadeOutPtr FadeOut::make(float d, float limit)
{
    auto pAction = std::make_shared<FadeOut>();
    pAction->setLimit(limit);
    pAction->initWithDuration(d);

    return pAction;
}

void FadeOut::update(float time)
{
    //RGBAInterface* protocol = dynamic_cast<RGBAInterface*>(m_target);
    //if (protocol) {
    //    protocol->setOpacity((unsigned char)(255.f * m_limit * (1 - time)));
    //}
    /*m_target->setOpacity(unsigned char(255 * (1 - time)));*/
}

ActionPtr FadeOut::reverse(void)
{
    return FadeIn::make(m_fDuration);
}

//
// FadeTo
//

FadeToPtr FadeTo::make(float duration, unsigned char opacity)
{
    auto pFadeTo = std::make_shared<FadeTo>();
    pFadeTo->initWithDuration(duration, opacity);

    return pFadeTo;
}

bool FadeTo::initWithDuration(float duration, unsigned char opacity)
{
    if (ActionInterval::initWithDuration(duration)) {
        m_toOpacity = opacity;
        return true;
    }

    return false;
}

void FadeTo::startWithTarget(NodePtr pTarget)
{
    ActionInterval::startWithTarget(pTarget);

    //RGBAInterface* protocol = dynamic_cast<RGBAInterface*>(pTarget);
    //if (protocol) {
    //    m_fromOpacity = protocol->getOpacity();
    //}
    /*m_fromOpacity = pTarget->getOpacity();*/
}

void FadeTo::update(float time)
{
    //RGBAInterface* protocol = dynamic_cast<RGBAInterface*>(m_target);
    //if (protocol) {
    //    protocol->setOpacity((unsigned char)(m_fromOpacity + (m_toOpacity - m_fromOpacity) * time));
    //}
    /*m_target->setOpacity((unsigned char)(m_fromOpacity + (m_toOpacity - m_fromOpacity) * time));*/
}

//
// TintTo
//
TintToPtr TintTo::make(float duration, unsigned char red, unsigned char green, unsigned char blue)
{
    auto pTintTo = std::make_shared<TintTo>();
    pTintTo->initWithDuration(duration, red, green, blue);

    return pTintTo;
}

bool TintTo::initWithDuration(float duration, unsigned char red, unsigned char green, unsigned char blue)
{
    if (ActionInterval::initWithDuration(duration)) {
        m_to = color::RGB(red, green, blue);
        return true;
    }

    return false;
}

void TintTo::startWithTarget(NodePtr pTarget)
{
    ActionInterval::startWithTarget(pTarget);
    //RGBAInterface* protocol = dynamic_cast<RGBAInterface*>(m_target);
    //if (protocol) {
    //    m_from = protocol->getColor();
    //}
    /*m_from = pTarget->getColor();*/
}

void TintTo::update(float time)
{
   // RGBAInterface* protocol = dynamic_cast<RGBAInterface*>(m_target);
    //if (protocol) {
    //    protocol->setColor(color::RGB((m_from.r + (m_to.r - m_from.r) * time),
    //        (m_from.g + (m_to.g - m_from.g) * time),
    //        (m_from.b + (m_to.b - m_from.b) * time)));
    //}
}

//
// TintBy
//

TintByPtr TintBy::make(float duration, short deltaRed, short deltaGreen, short deltaBlue)
{
    auto pTintBy = std::make_shared<TintBy>();
    pTintBy->initWithDuration(duration, deltaRed, deltaGreen, deltaBlue);

    return pTintBy;
}

bool TintBy::initWithDuration(float duration, short deltaRed, short deltaGreen, short deltaBlue)
{
    if (ActionInterval::initWithDuration(duration)) {
        m_deltaR = deltaRed;
        m_deltaG = deltaGreen;
        m_deltaB = deltaBlue;

        return true;
    }

    return false;
}

void TintBy::startWithTarget(NodePtr pTarget)
{
    ActionInterval::startWithTarget(pTarget);

    /*
    RGBAInterface* protocol = dynamic_cast<RGBAInterface*>(pTarget);
    if (protocol) {
        auto color = protocol->getColor();
        m_fromR = color.r;
        m_fromG = color.g;
        m_fromB = color.b;
    }
     */
}

void TintBy::update(float time)
{
    /*
    RGBAInterface* protocol = dynamic_cast<RGBAInterface*>(m_target);
    if (protocol) {
        protocol->setColor(color::RGB((m_fromR + m_deltaR * time),
            (m_fromG + m_deltaG * time),
            (m_fromB + m_deltaB * time)));
    }
     */
}

ActionPtr TintBy::reverse(void)
{
    return TintBy::make(m_fDuration, -m_deltaR, -m_deltaG, -m_deltaB);
}

//
// DelayTime
//
DelayTimePtr DelayTime::make(float d)
{
    auto pAction = std::make_shared<DelayTime>();
    pAction->initWithDuration(d);

    return pAction;
}

void DelayTime::update(float time)
{
    return;
}

ActionPtr DelayTime::reverse(void)
{
    return DelayTime::make(m_fDuration);
}

//
// ReverseTime
//

ReverseTimePtr ReverseTime::make(FiniteTimeActionPtr pAction)
{
    // casting to prevent warnings
    auto pReverseTime = std::make_shared<ReverseTime>();
    pReverseTime->initWithAction(pAction);

    return pReverseTime;
}

bool ReverseTime::initWithAction(FiniteTimeActionPtr pAction)
{
    assert(pAction != nullptr);
    assert(pAction != m_pOther);

    if (ActionInterval::initWithDuration(pAction->getDuration())) {
        m_pOther = pAction;

        return true;
    }

    return false;
}

void ReverseTime::startWithTarget(NodePtr pTarget)
{
    ActionInterval::startWithTarget(pTarget);
    m_pOther->startWithTarget(pTarget);
}

void ReverseTime::stop(void)
{
    m_pOther->stop();
    ActionInterval::stop();
}

void ReverseTime::update(float time)
{
    if (m_pOther) {
        m_pOther->update(1 - time);
    }
}

ActionPtr ReverseTime::reverse(void)
{
    return make(std::static_pointer_cast<FiniteTimeAction>(shared_from_this()));
}

// SpinBy

SpinByPtr SpinBy::make(float fDuration, float fDeltaAngle)
{
    auto pRotateBy = std::make_shared<SpinBy>();
    pRotateBy->initWithDuration(fDuration, Vec3(fDeltaAngle,0,0));

    return pRotateBy;
}

SpinByPtr SpinBy::make(float fDuration, const Vec3& deltaAngle)
{
    auto pRotateBy = std::make_shared<SpinBy>();
    pRotateBy->initWithDuration(fDuration, deltaAngle);

    return pRotateBy;
}

bool SpinBy::initWithDuration(float fDuration, const Vec3& deltaAngle)
{
    if (ActionInterval::initWithDuration(fDuration)) {
        m_fAngleX = deltaAngle.x;
        m_fAngleY = deltaAngle.y;
        m_fAngleZ = deltaAngle.z;
        return true;
    }

    return false;
}

void SpinBy::startWithTarget(NodePtr target)
{
    ActionInterval::startWithTarget(target);
    m_fStartAngleX = target->yaw();
    m_fStartAngleY = target->roll();
    m_fStartAngleZ = target->pitch();
}

void SpinBy::update(float time)
{
    if (m_target) {
        Quaternion rot;
        rot.yaw_pitch_roll(m_fStartAngleX + m_fAngleX * time,
                            m_fStartAngleY + m_fAngleY * time,
                            m_fStartAngleZ + m_fAngleZ * time);
        m_target->rotation(rot);
    }
}

ActionPtr SpinBy::reverse(void)
{
    return SpinBy::make(m_fDuration, Vec3(-m_fAngleX, -m_fAngleY, -m_fAngleZ));
}

// TargetedAction

TargetedAction::TargetedAction()
    : m_pForcedTarget(NULL)
{
}

TargetedActionPtr TargetedAction::make(NodePtr pTarget, FiniteTimeActionPtr pAction)
{
    auto p = std::make_shared<TargetedAction>();
    p->initWithTarget(pTarget, pAction);

    return p;
}

bool TargetedAction::initWithTarget(NodePtr pTarget, FiniteTimeActionPtr pAction)
{
    if (ActionInterval::initWithDuration(pAction->getDuration())) {
        m_pForcedTarget = pTarget;
        m_pAction = pAction;
        return true;
    }
    return false;
}

void TargetedAction::startWithTarget(NodePtr pTarget)
{
    ActionInterval::startWithTarget(pTarget);
    m_pAction->startWithTarget(m_pForcedTarget);
}

void TargetedAction::stop(void)
{
    m_pAction->stop();
}

void TargetedAction::update(float time)
{
    m_pAction->update(time);
}

NodePtr TargetedAction::getForcedTarget() const
{
    return m_pForcedTarget;
}

void TargetedAction::setForcedTarget(NodePtr node)
{
    m_pForcedTarget = node;
}
