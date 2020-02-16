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

#pragma once

#include "Action.h"
#include <string>

namespace mak3do {

class ActionInstant;
typedef std::shared_ptr<ActionInstant> ActionInstantPtr;

class ActionInstant : public FiniteTimeAction {
public:
    virtual bool isDone(void);
    virtual void step(float dt);
    virtual void update(float time);
    //CCFiniteTimeAction method
    virtual ActionPtr reverse(void);
};

class Show;
typedef std::shared_ptr<Show> ShowPtr;

class Show : public ActionInstant {
public:
    virtual void update(float time);
    virtual ActionPtr reverse(void);

public:
    static ShowPtr make();
};

class Hide;
typedef std::shared_ptr<Hide> HidePtr;

class Hide : public ActionInstant {
public:
    virtual void update(float time);
    virtual ActionPtr reverse(void);

public:
    /** Allocates and initializes the action */
    static HidePtr create();
};

class ToggleVisibility;
typedef std::shared_ptr<ToggleVisibility> ToggleVisibilityPtr;

class ToggleVisibility : public ActionInstant {
public:
    virtual void update(float time);

public:
    /** Allocates and initializes the action */
    static ToggleVisibilityPtr make();
};

class RemoveSelf;
typedef std::shared_ptr<RemoveSelf> RemoveSelfPtr;

class RemoveSelf : public ActionInstant {
public:
    virtual void update(float time);
    virtual ActionPtr reverse(void);

public:
    /** create the action */
    static RemoveSelfPtr make(bool isNeedCleanUp = true);
    /** init the action */
    bool init(bool isNeedCleanUp);

protected:
    bool m_bIsNeedCleanUp;
};

class FlipX;
typedef std::shared_ptr<FlipX> FlipXPtr;

class FlipX : public ActionInstant {
public:
    /** create the action */
    static FlipXPtr make(bool x);

    /** init the action */
    bool initWithFlipX(bool x);
    //super methods
    virtual void update(float time);
    virtual ActionPtr reverse(void);

protected:
    bool m_bFlipX{ false };
};

class FlipY;
typedef std::shared_ptr<FlipY> FlipYPtr;

class FlipY : public ActionInstant {
public:
    /** create the action */
    static FlipYPtr make(bool y);

    /** init the action */
    bool initWithFlipY(bool y);
    //super methods
    virtual void update(float time);
    virtual ActionPtr reverse(void);

protected:
    bool m_bFlipY{ false };
};

class Place;
typedef std::shared_ptr<Place> PlacePtr;

class Place : public ActionInstant //<NSCopying>
{
public:
    static PlacePtr make(const Vec2& pos);
    /** Initializes a Place action with a position */
    bool initWithPosition(const Vec2& pos);
    //super methods
    virtual void update(float time);

protected:
    Vec2 m_tPosition;
};

class CallFunc;
typedef std::shared_ptr<CallFunc> CallFuncPtr;

class CallFunc : public ActionInstant //<NSCopying>
{
public:
    /**
         *  @js ctor
         */
    CallFunc()
        : m_pSelectorTarget(NULL)
        , m_nScriptHandler(0)
        , m_pCallFunc(NULL)
    {
    }
    /**
         * @js NA
         * @lua NA
         */
    virtual ~CallFunc();

    /** creates the action with the callback 

        * typedef void (CCObject::*SEL_CallFunc)();
        * @lua NA
        */
    static CallFuncPtr make(CCObject* pSelectorTarget, cocos2d::SEL_CallFunc selector);

    /** creates the action with the handler script function 
         * @js NA
         */
    static CallFuncPtr make(int nHandler);

    /** initializes the action with the callback 
        
        * typedef void (CCObject::*SEL_CallFunc)();
        * @lua NA
        */
    virtual bool initWithTarget(CCObject* pSelectorTarget);
    /** executes the callback 
         * @lua NA
         */
    virtual void execute();
    /** super methods
         * @lua NA
         */
    virtual void update(float time);

    inline CCObject* getTargetCallback()
    {
        return m_pSelectorTarget;
    }
    /**
         * @lua NA
         */
    inline void setTargetCallback(CCObject* pSel)
    {
        if (pSel != m_pSelectorTarget) {
            CC_SAFE_RETAIN(pSel);
            CC_SAFE_RELEASE(m_pSelectorTarget);
            m_pSelectorTarget = pSel;
        }
    }

protected:
    /** Target that will be called */
    CCObject* m_pSelectorTarget;

    int m_nScriptHandler;

    union {
        cocos2d::SEL_CallFunc m_pCallFunc;
        cocos2d::SEL_CallFuncN m_pCallFuncN;
        cocos2d::SEL_CallFuncND m_pCallFuncND;
        cocos2d::SEL_CallFuncO m_pCallFuncO;
    };
};

}
