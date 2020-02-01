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

#pragma once

#include <mak3do/base/Node.h>
#include <mak3do/base/legacy/CCObject.h>
#include <mak3do/base/legacy/PlatformMacros.h>

namespace mak3do {
class Action;
typedef std::shared_ptr<Action> ActionPtr;
/** 
    @brief Base class for Action objects.
     */

//FIXME Delete shared_from_this inherit once it subclasses Object
class Action : public cocos2d::CCObject, public std::enable_shared_from_this<Action> {
public:
    enum Tag {
        //! Default tag
        kActionTagInvalid = -1,
    };

    Action(void);

    virtual ~Action(void);

    std::string description() const;

    //! return true if the action has finished
    virtual bool isDone(void);

    //! called before the action start. It will also set the target.
    virtual void startWithTarget(Node* pTarget);

    /** 
        called after the action has finished. It will set the 'target' to nil.
        IMPORTANT: You should never call "[action stop]" manually. Instead, use: "target->stopAction(action);"
        */
    virtual void stop(void);

    //! called every frame with it's delta time. DON'T override unless you know what you are doing.
    virtual void step(float dt);

    /** 
        called once per frame. time a value between 0 and 1

        For example: 
        - 0 means that the action just started
        - 0.5 means that the action is in the middle
        - 1 means that the action is over
        */
    virtual void update(float time);

    inline Node* getTarget(void) { return m_pTarget; }
    /** The action will modify the target properties. */
    inline void setTarget(Node* pTarget) { m_pTarget = pTarget; }

    inline Node* getOriginalTarget(void) { return m_pOriginalTarget; }
    /** Set the original target, since target can be nil.
        Is the target that were used to run the action. Unless you are doing something complex, like ActionManager, you should NOT call this method.
        The target is 'assigned', it is not 'retained'.
        @since v0.8.2
        */
    inline void setOriginalTarget(Node* pOriginalTarget) { m_pOriginalTarget = pOriginalTarget; }

    inline int getTag(void) { return m_nTag; }
    inline void setTag(int nTag) { m_nTag = nTag; }

public:
    /** Create an action */
    static ActionPtr make();

protected:
    Node* m_pOriginalTarget;
    /** The "target".
        The target will be set with the 'startWithTarget' method.
        When the 'stop' method is called, target will be set to nil.
        The target is 'assigned', it is not 'retained'.
        */
    Node* m_pTarget;
    /** The action tag. An identifier of the action */
    int m_nTag;
};

/** 
    @brief 
     Base class actions that do have a finite time duration.
     Possible actions:
       - An action with a duration of 0 seconds
       - An action with a duration of 35.5 seconds

     Infinite time actions are valid
     */

class FiniteTimeAction;
typedef std::shared_ptr<FiniteTimeAction> FiniteTimeActionPtr;

class FiniteTimeAction : public Action {
public:
    /**
         *  @js ctor
         */
    FiniteTimeAction()
        : m_fDuration(0)
    {
    }
    /**
         *  @js NA
         *  @lua NA
         */
    virtual ~FiniteTimeAction() {}
    //! get duration in seconds of the action
    inline float getDuration(void) { return m_fDuration; }
    //! set duration in seconds of the action
    inline void setDuration(float duration) { m_fDuration = duration; }

    /** returns a reversed action */
    virtual ActionPtr reverse(void);

protected:
    //! duration in seconds
    float m_fDuration;
};

class ActionInterval;
typedef std::shared_ptr<ActionInterval> ActionIntervalPtr;
class RepeatForever;

class Speed;
typedef std::shared_ptr<Speed> SpeedPtr;
/** 
     @brief Changes the speed of an action, making it take longer (speed>1)
     or less (speed<1) time.
     Useful to simulate 'slow motion' or 'fast forward' effect.
     @warning This action can't be Sequenceable because it is not an CCIntervalAction
     */
class Speed : public Action {
public:
    /**
         *  @js ctor
         */
    Speed()
        : m_fSpeed(0.0)
        , m_pInnerAction(NULL)
    {
    }

    inline float getSpeed(void) { return m_fSpeed; }
    /** alter the speed of the inner function in runtime */
    inline void setSpeed(float fSpeed) { m_fSpeed = fSpeed; }

    /** initializes the action */
    bool initWithAction(ActionIntervalPtr pAction, float fSpeed);
    /**
         *  @js NA
         *  @lua NA
         */

    virtual void startWithTarget(Node* pTarget);
    virtual void stop();
    virtual void step(float dt);
    virtual bool isDone(void);
    virtual ActionPtr reverse(void);

    void setInnerAction(ActionIntervalPtr pAction);

    inline ActionIntervalPtr getInnerAction()
    {
        return m_pInnerAction;
    }

public:
    /** create the action */
    static SpeedPtr make(ActionIntervalPtr pAction, float fSpeed);

protected:
    float m_fSpeed;
    ActionIntervalPtr m_pInnerAction;
};

/** 
    @brief CCFollow is an action that "follows" a node.

    Eg:
    layer->runAction(CCFollow::actionWithTarget(hero));

    Instead of using CCCamera as a "follower", use this action instead.
    @since v0.99.2
    */
class Follow : public Action {
public:
    /**
         *  @js ctor
         */
    Follow()
        : m_pobFollowedNode(NULL)
        , m_bBoundarySet(false)
        , m_bBoundaryFullyCovered(false)
        , m_fLeftBoundary(0.0)
        , m_fRightBoundary(0.0)
        , m_fTopBoundary(0.0)
        , m_fBottomBoundary(0.0)
    {
    }
    /**
         *  @js NA
         *  @lua NA
         */
    virtual ~Follow(void);

    inline bool isBoundarySet(void) { return m_bBoundarySet; }
    /** alter behavior - turn on/off boundary */
    inline void setBoudarySet(bool bValue) { m_bBoundarySet = bValue; }

    /** initializes the action with a set boundary */
    bool initWithTarget(Node* pFollowedNode, const Rect4& rect = Rect4::ZERO);
    /**
         *  @js NA
         *  @lua NA
         */
    virtual void step(float dt);
    virtual bool isDone(void);
    virtual void stop(void);

public:
    /** creates the action with a set boundary,
        It will work with no boundary if @param rect is equal to CCRectZero.
        */
    static Follow* create(Node* pFollowedNode, const Rect4& rect = Rect4::ZERO);

protected:
    // node to follow
    Node* m_pobFollowedNode;

    // whether camera should be limited to certain area
    bool m_bBoundarySet;

    // if screen size is bigger than the boundary - update not needed
    bool m_bBoundaryFullyCovered;

    // fast access to the screen dimensions
    Vec2 m_obHalfScreenSize;
    Vec2 m_obFullScreenSize;

    // world boundaries
    float m_fLeftBoundary;
    float m_fRightBoundary;
    float m_fTopBoundary;
    float m_fBottomBoundary;
};

}
