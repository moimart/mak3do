/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2009 Jason Booth

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

/*
 * Elastic, Back and Bounce actions based on code from:
 * http://github.com/NikhilK/silverlightfx/
 *
 * by http://github.com/NikhilK
 */

#include "ActionEasing.h"
#include <mak3do/base/Log.h>

using namespace mak3do;

#ifndef M_PI_X_2
#define M_PI_X_2 (float)M_PI * 2.0f
#endif

//
// EaseAction
//

ActionEasePtr ActionEase::make(ActionIntervalPtr pAction)
{
    ActionEasePtr pRet = std::make_shared<ActionEase>();
    if (pRet->initWithAction(pAction))
        return pRet;

    return nullptr;
}

bool ActionEase::initWithAction(ActionIntervalPtr pAction)
{
    logger::assertion(pAction != NULL, "");

    if (ActionInterval::initWithDuration(pAction->getDuration())) {
        m_pInner = pAction;
        pAction->retain();

        return true;
    }

    return false;
}

void ActionEase::startWithTarget(Node* pTarget)
{
    ActionInterval::startWithTarget(pTarget);
    m_pInner->startWithTarget(m_pTarget);
}

void ActionEase::stop(void)
{
    m_pInner->stop();
    ActionInterval::stop();
}

void ActionEase::update(float time)
{
    m_pInner->update(time);
}

ActionPtr ActionEase::reverse(void)
{
    return ActionEase::make(std::static_pointer_cast<ActionInterval>(m_pInner->reverse()));
}

ActionPtr ActionEase::getInnerAction()
{
    return m_pInner;
}

//
// EaseRateAction
//

EaseRateActionPtr EaseRateAction::make(ActionIntervalPtr pAction, float fRate)
{
    EaseRateActionPtr pRet = std::make_shared<EaseRateAction>();
    if (pRet->initWithAction(pAction, fRate))
        return pRet;

    return nullptr;
}

bool EaseRateAction::initWithAction(ActionIntervalPtr pAction, float fRate)
{
    if (ActionEase::initWithAction(pAction)) {
        m_fRate = fRate;
        return true;
    }

    return false;
}

EaseRateAction::~EaseRateAction(void)
{
}

ActionPtr EaseRateAction::reverse(void)
{
    return EaseRateAction::make(std::static_pointer_cast<ActionInterval>(m_pInner->reverse()), 1 / m_fRate);
}

//
// EeseIn
//

EaseInPtr EaseIn::make(ActionIntervalPtr pAction, float fRate)
{
    EaseInPtr pRet = std::make_shared<EaseIn>();
    if (pRet->initWithAction(pAction, fRate))
        return pRet;

    return nullptr;
}

void EaseIn::update(float time)
{
    m_pInner->update(powf(time, m_fRate));
}

ActionPtr EaseIn::reverse(void)
{
    return EaseIn::make(std::static_pointer_cast<ActionInterval>(m_pInner->reverse()), 1 / m_fRate);
}

//
// EaseOut
//
EaseOutPtr EaseOut::make(ActionIntervalPtr pAction, float fRate)
{
    EaseOutPtr pRet = std::make_shared<EaseOut>();
    if (pRet->initWithAction(pAction, fRate))
        return pRet;

    return nullptr;
}

void EaseOut::update(float time)
{
    m_pInner->update(powf(time, 1 / m_fRate));
}

ActionPtr EaseOut::reverse()
{
    return EaseOut::make(std::static_pointer_cast<ActionInterval>(m_pInner->reverse()), 1 / m_fRate);
}

//
// EaseInOut
//
EaseInOutPtr EaseInOut::make(ActionIntervalPtr pAction, float fRate)
{
    EaseInOutPtr pRet = std::make_shared<EaseInOut>();
    if (pRet->initWithAction(pAction, fRate))
        return pRet;

    return nullptr;
}

void EaseInOut::update(float time)
{
    time *= 2;
    if (time < 1) {
        m_pInner->update(0.5f * powf(time, m_fRate));
    } else {
        m_pInner->update(1.0f - 0.5f * powf(2 - time, m_fRate));
    }
}

// InOut and OutIn are symmetrical
ActionPtr EaseInOut::reverse(void)
{
    return EaseInOut::make(std::static_pointer_cast<ActionInterval>(m_pInner->reverse()), m_fRate);
}

//
// EaseExponentialIn
//
EaseExponentialInPtr EaseExponentialIn::make(ActionIntervalPtr pAction)
{
    EaseExponentialInPtr pRet = std::make_shared<EaseExponentialIn>();
    if (pRet->initWithAction(pAction))
        return pRet;

    return nullptr;
}

void EaseExponentialIn::update(float time)
{
    m_pInner->update(time == 0 ? 0 : powf(2, 10 * (time / 1 - 1)) - 1 * 0.001f);
}

ActionPtr EaseExponentialIn::reverse(void)
{
    return EaseExponentialOut::make(std::static_pointer_cast<ActionInterval>(m_pInner->reverse()));
}

//
// EaseExponentialOut
//
EaseExponentialOutPtr EaseExponentialOut::make(ActionIntervalPtr pAction)
{
    EaseExponentialOutPtr pRet = std::make_shared<EaseExponentialOut>();
    if (pRet->initWithAction(pAction))
        return pRet;

    return nullptr;
}

void EaseExponentialOut::update(float time)
{
    m_pInner->update(time == 1 ? 1 : (-powf(2, -10 * time / 1) + 1));
}

ActionPtr EaseExponentialOut::reverse(void)
{
    return EaseExponentialIn::make(std::static_pointer_cast<ActionInterval>(m_pInner->reverse()));
}

//
// EaseExponentialInOut
//

EaseExponentialInOutPtr EaseExponentialInOut::make(ActionIntervalPtr pAction)
{
    EaseExponentialInOutPtr pRet = std::make_shared<EaseExponentialInOut>();
    if (pRet->initWithAction(pAction))
        return pRet;

    return nullptr;
}

void EaseExponentialInOut::update(float time)
{
    time /= 0.5f;
    if (time < 1) {
        time = 0.5f * powf(2, 10 * (time - 1));
    } else {
        time = 0.5f * (-powf(2, -10 * (time - 1)) + 2);
    }

    m_pInner->update(time);
}

ActionPtr EaseExponentialInOut::reverse()
{
    return EaseExponentialInOut::make(std::static_pointer_cast<ActionInterval>(m_pInner->reverse()));
}

//
// EaseSineIn
//

EaseSineInPtr EaseSineIn::make(ActionIntervalPtr pAction)
{
    EaseSineInPtr pRet = std::make_shared<EaseSineIn>();
    if (pRet->initWithAction(pAction))
        return pRet;

    return nullptr;
}

void EaseSineIn::update(float time)
{
    m_pInner->update(-1 * cosf(time * (float)M_PI_2) + 1);
}

ActionPtr EaseSineIn::reverse(void)
{
    return EaseSineOut::make(std::static_pointer_cast<ActionInterval>(m_pInner->reverse()));
}

//
// EaseSineOut
//

EaseSineOutPtr EaseSineOut::make(ActionIntervalPtr pAction)
{
    EaseSineOutPtr pRet = std::make_shared<EaseSineOut>();
    if (pRet->initWithAction(pAction))
        return pRet;

    return nullptr;
}

void EaseSineOut::update(float time)
{
    m_pInner->update(sinf(time * (float)M_PI_2));
}

ActionPtr EaseSineOut::reverse(void)
{
    return EaseSineIn::make(std::static_pointer_cast<ActionInterval>(m_pInner->reverse()));
}

//
// EaseSineInOut
//

EaseSineInOutPtr EaseSineInOut::make(ActionIntervalPtr pAction)
{
    EaseSineInOutPtr pRet = std::make_shared<EaseSineInOut>();
    if (pRet->initWithAction(pAction))
        return pRet;

    return nullptr;
}

void EaseSineInOut::update(float time)
{
    m_pInner->update(-0.5f * (cosf((float)M_PI * time) - 1));
}

ActionPtr EaseSineInOut::reverse()
{
    return EaseSineInOut::make(std::static_pointer_cast<ActionInterval>(m_pInner->reverse()));
}

//
// EaseElastic
//

EaseElasticPtr EaseElastic::make(ActionIntervalPtr pAction)
{
    return EaseElastic::make(pAction, 0.3f);
}

EaseElasticPtr EaseElastic::make(ActionIntervalPtr pAction, float fPeriod /* = 0.3f*/)
{
    EaseElasticPtr pRet = std::make_shared<EaseElastic>();
    if (pRet->initWithAction(pAction, fPeriod))
        return pRet;

    return nullptr;
}

bool EaseElastic::initWithAction(ActionIntervalPtr pAction, float fPeriod /* = 0.3f*/)
{
    if (ActionEase::initWithAction(pAction)) {
        m_fPeriod = fPeriod;
        return true;
    }

    return false;
}

ActionPtr EaseElastic::reverse(void)
{
    logger::assertion(0, "Override me");

    return NULL;
}

//
// EaseElasticIn
//

EaseElasticInPtr EaseElasticIn::make(ActionIntervalPtr pAction)
{
    return EaseElasticIn::make(pAction, 0.3f);
}

EaseElasticInPtr EaseElasticIn::make(ActionIntervalPtr pAction, float fPeriod /* = 0.3f*/)
{
    EaseElasticInPtr pRet = std::make_shared<EaseElasticIn>();
    if (pRet->initWithAction(pAction, fPeriod))
        return pRet;

    return nullptr;
}

void EaseElasticIn::update(float time)
{
    float newT = 0;
    if (time == 0 || time == 1) {
        newT = time;
    } else {
        float s = m_fPeriod / 4;
        time = time - 1;
        newT = -powf(2, 10 * time) * sinf((time - s) * M_PI_X_2 / m_fPeriod);
    }

    m_pInner->update(newT);
}

ActionPtr EaseElasticIn::reverse(void)
{
    return EaseElasticOut::make(std::static_pointer_cast<ActionInterval>(m_pInner->reverse()), m_fPeriod);
}

//
// EaseElasticOut
//

EaseElasticOutPtr EaseElasticOut::make(ActionIntervalPtr pAction)
{
    return EaseElasticOut::make(pAction, 0.3f);
}

EaseElasticOutPtr EaseElasticOut::make(ActionIntervalPtr pAction, float fPeriod /* = 0.3f*/)
{
    EaseElasticOutPtr pRet = std::make_shared<EaseElasticOut>();
    if (pRet->initWithAction(pAction, fPeriod))
        return pRet;

    return nullptr;
}

void EaseElasticOut::update(float time)
{
    float newT = 0;
    if (time == 0 || time == 1) {
        newT = time;
    } else {
        float s = m_fPeriod / 4;
        newT = powf(2, -10 * time) * sinf((time - s) * M_PI_X_2 / m_fPeriod) + 1;
    }

    m_pInner->update(newT);
}

ActionPtr EaseElasticOut::reverse(void)
{
    return EaseElasticIn::make(std::static_pointer_cast<ActionInterval>(m_pInner->reverse()), m_fPeriod);
}

//
// EaseElasticInOut
//

EaseElasticInOutPtr EaseElasticInOut::make(ActionIntervalPtr pAction)
{
    return EaseElasticInOut::make(pAction, 0.3f);
}

EaseElasticInOutPtr EaseElasticInOut::make(ActionIntervalPtr pAction, float fPeriod /* = 0.3f*/)
{
    EaseElasticInOutPtr pRet = std::make_shared<EaseElasticInOut>();
    if (pRet->initWithAction(pAction, fPeriod))
        return pRet;

    return nullptr;
}

void EaseElasticInOut::update(float time)
{
    float newT = 0;
    if (time == 0 || time == 1) {
        newT = time;
    } else {
        time = time * 2;
        if (!m_fPeriod) {
            m_fPeriod = 0.3f * 1.5f;
        }

        float s = m_fPeriod / 4;

        time = time - 1;
        if (time < 0) {
            newT = -0.5f * powf(2, 10 * time) * sinf((time - s) * M_PI_X_2 / m_fPeriod);
        } else {
            newT = powf(2, -10 * time) * sinf((time - s) * M_PI_X_2 / m_fPeriod) * 0.5f + 1;
        }
    }

    m_pInner->update(newT);
}

ActionPtr EaseElasticInOut::reverse(void)
{
    return EaseElasticInOut::make(std::static_pointer_cast<ActionInterval>(m_pInner->reverse()), m_fPeriod);
}

//
// EaseBounce
//

EaseBouncePtr EaseBounce::make(ActionIntervalPtr pAction)
{
    EaseBouncePtr pRet = std::make_shared<EaseBounce>();
    if (pRet->initWithAction(pAction))
        return pRet;

    return nullptr;
}

float EaseBounce::bounceTime(float time)
{
    if (time < 1 / 2.75) {
        return 7.5625f * time * time;
    } else if (time < 2 / 2.75) {
        time -= 1.5f / 2.75f;
        return 7.5625f * time * time + 0.75f;
    } else if (time < 2.5 / 2.75) {
        time -= 2.25f / 2.75f;
        return 7.5625f * time * time + 0.9375f;
    }

    time -= 2.625f / 2.75f;
    return 7.5625f * time * time + 0.984375f;
}

ActionPtr EaseBounce::reverse()
{
    return EaseBounce::make(std::static_pointer_cast<ActionInterval>(m_pInner->reverse()));
}

//
// EaseBounceIn
//

EaseBounceInPtr EaseBounceIn::make(ActionIntervalPtr pAction)
{
    EaseBounceInPtr pRet = std::make_shared<EaseBounceIn>();
    if (pRet->initWithAction(pAction))
        return pRet;

    return nullptr;
}

void EaseBounceIn::update(float time)
{
    float newT = 1 - bounceTime(1 - time);
    m_pInner->update(newT);
}

ActionPtr EaseBounceIn::reverse(void)
{
    return EaseBounceOut::make(std::static_pointer_cast<ActionInterval>(m_pInner->reverse()));
}

//
// EaseBounceOut
//

EaseBounceOutPtr EaseBounceOut::make(ActionIntervalPtr pAction)
{
    EaseBounceOutPtr pRet = std::make_shared<EaseBounceOut>();
    if (pRet->initWithAction(pAction))
        return pRet;

    return nullptr;
}

void EaseBounceOut::update(float time)
{
    float newT = bounceTime(time);
    m_pInner->update(newT);
}

ActionPtr EaseBounceOut::reverse(void)
{
    return EaseBounceIn::make(std::static_pointer_cast<ActionInterval>(m_pInner->reverse()));
}

//
// EaseBounceInOut
//

EaseBounceInOutPtr EaseBounceInOut::make(ActionIntervalPtr pAction)
{
    EaseBounceInOutPtr pRet = std::make_shared<EaseBounceInOut>();
    if (pRet->initWithAction(pAction))
        return pRet;

    return nullptr;
}

void EaseBounceInOut::update(float time)
{
    float newT = 0;
    if (time < 0.5f) {
        time = time * 2;
        newT = (1 - bounceTime(1 - time)) * 0.5f;
    } else {
        newT = bounceTime(time * 2 - 1) * 0.5f + 0.5f;
    }

    m_pInner->update(newT);
}

ActionPtr EaseBounceInOut::reverse()
{
    return EaseBounceInOut::make(std::static_pointer_cast<ActionInterval>(m_pInner->reverse()));
}

//
// EaseBackIn
//

EaseBackInPtr EaseBackIn::make(ActionIntervalPtr pAction)
{
    EaseBackInPtr pRet = std::make_shared<EaseBackIn>();
    if (pRet->initWithAction(pAction))
        return pRet;

    return nullptr;
}

void EaseBackIn::update(float time)
{
    float overshoot = 1.70158f;
    m_pInner->update(time * time * ((overshoot + 1) * time - overshoot));
}

ActionPtr EaseBackIn::reverse(void)
{
    return EaseBackOut::make(std::static_pointer_cast<ActionInterval>(m_pInner->reverse()));
}

//
// EaseBackOut
//

EaseBackOutPtr EaseBackOut::make(ActionIntervalPtr pAction)
{
    EaseBackOutPtr pRet = std::make_shared<EaseBackOut>();
    if (pRet->initWithAction(pAction))
        return pRet;

    return nullptr;
}

void EaseBackOut::update(float time)
{
    float overshoot = 1.70158f;

    time = time - 1;
    m_pInner->update(time * time * ((overshoot + 1) * time + overshoot) + 1);
}

ActionPtr EaseBackOut::reverse(void)
{
    return EaseBackIn::make(std::static_pointer_cast<ActionInterval>(m_pInner->reverse()));
}

//
// EaseBackInOut
//

EaseBackInOutPtr EaseBackInOut::make(ActionIntervalPtr pAction)
{
    EaseBackInOutPtr pRet = std::make_shared<EaseBackInOut>();
    if (pRet->initWithAction(pAction))
        return pRet;

    return nullptr;
}

void EaseBackInOut::update(float time)
{
    float overshoot = 1.70158f * 1.525f;

    time = time * 2;
    if (time < 1) {
        m_pInner->update((time * time * ((overshoot + 1) * time - overshoot)) / 2);
    } else {
        time = time - 2;
        m_pInner->update((time * time * ((overshoot + 1) * time + overshoot)) / 2 + 1);
    }
}

ActionPtr EaseBackInOut::reverse()
{
    return EaseBackInOut::make(std::static_pointer_cast<ActionInterval>(m_pInner->reverse()));
}
