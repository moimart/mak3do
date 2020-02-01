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

#include "ActionInstant.h"
#include <mak3do/base/Node.h>

using namespace mak3do;

bool ActionInstant::isDone()
{
    return true;
}

void ActionInstant::step(float dt)
{
    CC_UNUSED_PARAM(dt);
    update(1);
}

void ActionInstant::update(float time)
{
    CC_UNUSED_PARAM(time);
}

ActionPtr ActionInstant::reverse()
{
    return make();
}

//
// Show
//

ShowPtr Show::make()
{
    return std::make_shared<Show>();
}

void Show::update(float time)
{
    CC_UNUSED_PARAM(time);
    m_pTarget->setVisible(true);
}

ActionPtr Show::reverse()
{
    return Hide::make();
}

//
// Hide
//
HidePtr Hide::create()
{
    auto pRet = std::make_shared<Hide>();
    return pRet;
}

void Hide::update(float time)
{
    CC_UNUSED_PARAM(time);
    m_pTarget->setVisible(false);
}

ActionPtr Hide::reverse()
{
    return Show::make();
}

// ToggleVisibility
//
ToggleVisibilityPtr ToggleVisibility::make()
{
    return std::make_shared<ToggleVisibility>();
}

void ToggleVisibility::update(float time)
{
    CC_UNUSED_PARAM(time);
    m_pTarget->setVisible(!m_pTarget->isVisible());
}

//
// Remove Self
//
RemoveSelfPtr RemoveSelf::make(bool isNeedCleanUp /*= true*/)
{
    auto pRet = std::make_shared<RemoveSelf>();
    pRet->init(isNeedCleanUp);

    return pRet;
}

bool RemoveSelf::init(bool isNeedCleanUp)
{
    m_bIsNeedCleanUp = isNeedCleanUp;
    return true;
}

void RemoveSelf::update(float time)
{
    CC_UNUSED_PARAM(time);
    m_pTarget->removeFromParentAndCleanup(m_bIsNeedCleanUp);
}

ActionPtr RemoveSelf::reverse()
{
    return RemoveSelf::make(m_bIsNeedCleanUp);
}

//
// FlipX
//

FlipXPtr FlipX::make(bool x)
{
    auto pRet = std::make_shared<FlipX>();
    pRet->initWithFlipX(x);

    return pRet;
}

bool FlipX::initWithFlipX(bool x)
{
    m_bFlipX = x;
    return true;
}

void FlipX::update(float time)
{
    CC_UNUSED_PARAM(time);
    //FIXME: TODO: ((Sprite2D*) (m_pTarget))->setFlipX(m_bFlipX);
}

ActionPtr FlipX::reverse()
{
    return FlipX::make(!m_bFlipX);
}

//
// FlipY
//

FlipYPtr FlipY::make(bool y)
{
    auto pRet = std::make_shared<FlipY>();
    pRet->initWithFlipY(y);

    return pRet;
}

bool FlipY::initWithFlipY(bool y)
{
    m_bFlipY = y;
    return true;
}

void FlipY::update(float time)
{
    CC_UNUSED_PARAM(time);
    //((Sprite2D*) (m_pTarget))->setFlipY(m_bFlipY);
}

ActionPtr FlipY::reverse()
{
    return FlipY::make(!m_bFlipY);
}

//
// Place
//

PlacePtr Place::make(const Vec2& pos)
{
    auto pRet = std::make_shared<Place>();
    pRet->initWithPosition(pos);

    return pRet;
}

bool Place::initWithPosition(const Vec2& pos)
{
    m_tPosition = pos;
    return true;
}

void Place::update(float time)
{
    CC_UNUSED_PARAM(time);
    m_pTarget->setPosition(m_tPosition);
}

//
// CallFunc
//
CallFuncPtr CallFunc::make(CCObject* pSelectorTarget, cocos2d::SEL_CallFunc selector)
{
    auto pRet = std::make_shared<CallFunc>();
    pRet->initWithTarget(pSelectorTarget);

    pRet->m_pCallFunc = selector;

    return pRet;
}

CallFuncPtr CallFunc::make(int nHandler)
{
    auto pRet = std::make_shared<CallFunc>();
    pRet->m_nScriptHandler = nHandler;

    return pRet;
}

bool CallFunc::initWithTarget(CCObject* pSelectorTarget)
{
    if (pSelectorTarget) {
        pSelectorTarget->retain();
    }

    if (m_pSelectorTarget) {
        m_pSelectorTarget->release();
    }

    m_pSelectorTarget = pSelectorTarget;
    return true;
}

CallFunc::~CallFunc(void)
{
    CC_SAFE_RELEASE(m_pSelectorTarget);
}

void CallFunc::update(float time)
{
    CC_UNUSED_PARAM(time);
    this->execute();
}

void CallFunc::execute()
{
    if (m_pCallFunc)
        (m_pSelectorTarget->*m_pCallFunc)();
}
