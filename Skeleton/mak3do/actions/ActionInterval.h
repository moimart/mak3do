/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2011 Ricardo Quesada
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
#include <mak3do/base/Node.h>
#include <vector>

namespace mak3do {
/**
     * @addtogroup actions
     * @{
     */

/** 
    @brief An interval action is an action that takes place within a certain period of time.
    It has an start time, and a finish time. The finish time is the parameter
    duration plus the start time.

    These ActionInterval actions have some interesting properties, like:
    - They can run normally (default)
    - They can run reversed with the reverse method
    - They can run with the time altered with the Accelerate, AccelDeccel and Speed actions.

    For example, you can simulate a Ping Pong effect running the action normally and
    then running it again in Reverse mode.

    Example:

    Action *pingPongAction = Sequence::actions(action, action->reverse(), NULL);
    */

class ActionInterval;
typedef std::shared_ptr<ActionInterval> ActionIntervalPtr;

class ActionInterval : public FiniteTimeAction {
public:
    /** how many seconds had elapsed since the actions started to run. */
    inline float getElapsed(void) { return m_elapsed; }

    /** initializes the action */
    bool initWithDuration(float d);

    /** returns true if the action has finished */
    virtual bool isDone(void);
    /**
         *  @js NA
         *  @lua NA
         */
    virtual void step(float dt);
    virtual void startWithTarget(Node* pTarget);
    /** returns a reversed action */
    virtual ActionPtr reverse(void);

public:
    /*Ptr makes the action */
    static ActionIntervalPtr make(float d);

public:
    //extension in GridAction
    void setAmplitudeRate(float amp);
    float getAmplitudeRate(void);

protected:
    float m_elapsed;
    bool m_bFirstTick;
};

class Sequence;
typedef std::shared_ptr<Sequence> SequencePtr;

/** @brief Runs actions sequentially, one after another
     */
class Sequence : public ActionInterval {
public:
    /** initializes the action 
         * @lua NA
         */
    bool initWithTwoActions(FiniteTimeActionPtr pActionOne, FiniteTimeActionPtr pActionTwo);

    /**
         * @lua NA
         */
    virtual void startWithTarget(Node* pTarget);
    /**
         * @lua NA
         */
    virtual void stop(void);
    /**
         * @lua NA
         */
    virtual void update(float t);
    virtual ActionPtr reverse(void);

public:
    /** helper constructor to create an array of sequenceable actions given an array 
         * @js NA
         */
    static SequencePtr make(const std::vector<ActionPtr>& actions);
    /** helper constructor to create an array of sequence-able actions 
         * @js NA
         * @lua NA
         */
    /*Ptr makes the action
         * @js NA
         */
    static SequencePtr makeWithTwoActions(FiniteTimeActionPtr pActionOne, FiniteTimeActionPtr pActionTwo);

protected:
    FiniteTimeActionPtr m_pActions[2];
    float m_split;
    int m_last;
};

class Repeat;
typedef std::shared_ptr<Repeat> RepeatPtr;

/** @brief Repeats an action a number of times.
     * To repeat an action forever use the RepeatForever action.
     */
class Repeat : public ActionInterval {
public:
    /** initializes a Repeat action. Times is an unsigned integer between 1 and pow(2,30) */
    bool initWithAction(FiniteTimeActionPtr pAction, unsigned int times);
    /**
         *  @js NA
         *  @lua NA
         */
    virtual void startWithTarget(Node* pTarget);
    virtual void stop(void);
    virtual void update(float dt);
    virtual bool isDone(void);
    virtual ActionPtr reverse(void);

    inline void setInnerAction(FiniteTimeActionPtr pAction)
    {
        if (m_pInnerAction != pAction) {
            m_pInnerAction = pAction;
        }
    }

    inline FiniteTimeActionPtr getInnerAction()
    {
        return m_pInnerAction;
    }

public:
    /*Ptr makes a Repeat action. Times is an unsigned integer between 1 and pow(2,30) */
    static RepeatPtr make(FiniteTimeActionPtr pAction, unsigned int times);

protected:
    unsigned int m_uTimes;
    unsigned int m_uTotal;
    float m_fNextDt;
    bool m_bActionInstant;
    /** Inner action */
    FiniteTimeActionPtr m_pInnerAction;
};

class RepeatForever;
typedef std::shared_ptr<RepeatForever> RepeatForeverPtr;
/** @brief Repeats an action for ever.
    To repeat the an action for a limited number of times use the Repeat action.
    @warning This action can't be Sequenceable because it is not an IntervalAction
    */
class RepeatForever : public ActionInterval {
public:
    /**
         *  @js ctor
         */
    RepeatForever()
        : m_pInnerAction(NULL)
    {
    }

    /** initializes the action */
    bool initWithAction(ActionIntervalPtr pAction);
    /**
         *  @js NA
         *  @lua NA
         */
    virtual void startWithTarget(Node* pTarget);
    virtual void step(float dt);
    virtual bool isDone(void);
    virtual ActionPtr reverse(void);

    inline void setInnerAction(ActionIntervalPtr pAction)
    {
        if (m_pInnerAction != pAction) {
            m_pInnerAction = pAction;
        }
    }

    inline ActionIntervalPtr getInnerAction()
    {
        return m_pInnerAction;
    }

public:
    /*Ptr makes the action */
    static RepeatForeverPtr make(ActionIntervalPtr pAction);

protected:
    /** Inner action */
    ActionIntervalPtr m_pInnerAction;
};

class Spawn;
typedef std::shared_ptr<Spawn> SpawnPtr;

/** @brief Spawn a new action immediately
     */
class Spawn : public ActionInterval {
public:
    /** initializes the Spawn action with the 2 actions to spawn 
         * @lua NA
         */
    bool initWithTwoActions(FiniteTimeActionPtr pAction1, FiniteTimeActionPtr pAction2);

    /**
         * @lua NA
         */
    virtual void startWithTarget(Node* pTarget);
    /**
         * @lua NA
         */
    virtual void stop(void);
    /**
         * @lua NA
         */
    virtual void update(float time);
    virtual ActionPtr reverse(void);

public:
    static SpawnPtr make(const std::vector<ActionPtr>& actions);

    /*Ptr makes the Spawn action 
         * @js NA
         */
    static SpawnPtr makeWithTwoActions(FiniteTimeActionPtr pAction1, FiniteTimeActionPtr pAction2);

protected:
    FiniteTimeActionPtr m_pOne;
    FiniteTimeActionPtr m_pTwo;
};

class RotateTo;
typedef std::shared_ptr<RotateTo> RotateToPtr;

/** @brief Rotates a Node object to a certain angle by modifying it's
     rotation attribute.
     The direction will be decided by the shortest angle.
    */
class RotateTo : public ActionInterval {
public:
    /*Ptr makes the action */
    static RotateToPtr make(float fDuration, float fDeltaAngle);
    /** initializes the action */
    bool initWithDuration(float fDuration, float fDeltaAngle);

    /*Ptr makes the action with separate rotation angles */
    static RotateToPtr make(float fDuration, float fDeltaAngleX, float fDeltaAngleY);
    virtual bool initWithDuration(float fDuration, float fDeltaAngleX, float fDeltaAngleY);
    /**
         *  @js NA
         *  @lua NA
         */
    virtual void startWithTarget(Node* pTarget);
    virtual void update(float time);

protected:
    float m_fDstAngleX;
    float m_fStartAngleX;
    float m_fDiffAngleX;

    float m_fDstAngleY;
    float m_fStartAngleY;
    float m_fDiffAngleY;
};

class RotateBy;
typedef std::shared_ptr<RotateBy> RotateByPtr;

/** @brief Rotates a Node object clockwise a number of degrees by modifying it's rotation attribute.
    */
class RotateBy : public ActionInterval {
public:
    /*Ptr makes the action */
    static RotateByPtr make(float fDuration, float fDeltaAngle);
    /** initializes the action */
    bool initWithDuration(float fDuration, float fDeltaAngle);

    static RotateByPtr make(float fDuration, float fDeltaAngleX, float fDeltaAngleY);
    bool initWithDuration(float fDuration, float fDeltaAngleX, float fDeltaAngleY);
    /**
         *  @js NA
         *  @lua NA
         */
    virtual void startWithTarget(Node* pTarget);
    virtual void update(float time);
    virtual ActionPtr reverse(void);

protected:
    float m_fAngleX;
    float m_fStartAngleX;
    float m_fAngleY;
    float m_fStartAngleY;
};

class MoveBy;
typedef std::shared_ptr<MoveBy> MoveByPtr;

/**  Moves a Node object x,y pixels by modifying it's position attribute.
     x and y are relative to the position of the object.
     Several MoveBy actions can be concurrently called, and the resulting
     movement will be the sum of individual movements.
     @since v2.1beta2-custom
     */
class MoveBy : public ActionInterval {
public:
    /** initializes the action */
    bool initWithDuration(float duration, const Vec2& deltaPosition);
    /**
         *  @js NA
         *  @lua NA
         */
    virtual void startWithTarget(Node* pTarget);
    virtual ActionPtr reverse(void);
    virtual void update(float time);

public:
    /*Ptr makes the action */
    static MoveByPtr make(float duration, const Vec2& deltaPosition);

protected:
    Vec2 m_positionDelta;
    Vec2 m_startPosition;
    Vec2 m_previousPosition;
};

class MoveTo;
typedef std::shared_ptr<MoveTo> MoveToPtr;

/** Moves a Node object to the position x,y. x and y are absolute coordinates by modifying it's position attribute.
     Several MoveTo actions can be concurrently called, and the resulting
     movement will be the sum of individual movements.
     @since v2.1beta2-custom
     */
class MoveTo : public MoveBy {
public:
    /** initializes the action */
    bool initWithDuration(float duration, const Vec2& position);
    /**
         *  @js NA
         *  @lua NA
         */

    virtual void startWithTarget(Node* pTarget);

public:
    /*Ptr makes the action */
    static MoveToPtr make(float duration, const Vec2& position);

protected:
    Vec2 m_endPosition;
};

class SkewTo;
typedef std::shared_ptr<SkewTo> SkewToPtr;

/** Skews a Node object to given angles by modifying it's skewX and skewY attributes
    @since v1.0
    */
class SkewTo : public ActionInterval {
public:
    /**
         *  @js ctor
         */
    SkewTo();
    virtual bool initWithDuration(float t, float sx, float sy);
    /**
         *  @js NA
         *  @lua NA
         */
    virtual void startWithTarget(Node* pTarget);
    virtual void update(float time);

public:
    /*Ptr makes the action */
    static SkewToPtr make(float t, float sx, float sy);

protected:
    float m_fSkewX;
    float m_fSkewY;
    float m_fStartSkewX;
    float m_fStartSkewY;
    float m_fEndSkewX;
    float m_fEndSkewY;
    float m_fDeltaX;
    float m_fDeltaY;
};

class SkewBy;
typedef std::shared_ptr<SkewBy> SkewByPtr;
/** Skews a Node object by skewX and skewY degrees
    @since v1.0
    */
class SkewBy : public SkewTo {
public:
    virtual bool initWithDuration(float t, float sx, float sy);
    virtual void startWithTarget(Node* pTarget);
    virtual ActionPtr reverse(void);

public:
    /*Ptr makes the action */
    static SkewByPtr make(float t, float deltaSkewX, float deltaSkewY);
};

class JumpBy;
typedef std::shared_ptr<JumpBy> JumpByPtr;
/** @brief Moves a Node object simulating a parabolic jump movement by modifying it's position attribute.
    */
class JumpBy : public ActionInterval {
public:
    /** initializes the action */
    bool initWithDuration(float duration, const Vec2& position, float height, unsigned int jumps);
    /**
         *  @js NA
         *  @lua NA
         */
    virtual void startWithTarget(Node* pTarget);
    virtual void update(float time);
    virtual ActionPtr reverse(void);

public:
    /*Ptr makes the action */
    static JumpByPtr make(float duration, const Vec2& position, float height, unsigned int jumps);

protected:
    Vec2 m_startPosition;
    Vec2 m_delta;
    float m_height;
    unsigned int m_nJumps;
    Vec2 m_previousPos;
};

class JumpTo;
typedef std::shared_ptr<JumpTo> JumpToPtr;
/** @brief Moves a Node object to a parabolic position simulating a jump movement by modifying it's position attribute.
    */
class JumpTo : public JumpBy {
public:
    virtual void startWithTarget(Node* pTarget);

public:
    /*Ptr makes the action */
    static JumpToPtr make(float duration, const Vec2& position, float height, int jumps);
};

/** @typedef bezier configuration structure
     */
typedef struct _ccBezierConfig {
    //! end position of the bezier
    Vec2 endPosition;
    //! Bezier control Vec2 1
    Vec2 controlVec2_1;
    //! Bezier control Vec2 2
    Vec2 controlVec2_2;
} BezierConfig;

class BezierBy;
typedef std::shared_ptr<BezierBy> BezierByPtr;
/** @brief An action that moves the target with a cubic Bezier curve by a certain distance.
     */
class BezierBy : public ActionInterval {
public:
    /** initializes the action with a duration and a bezier configuration 
         *  @lua NA
         */
    bool initWithDuration(float t, const BezierConfig& c);

    /**
         *  @lua NA
         */
    virtual void startWithTarget(Node* pTarget);
    /**
         *  @lua NA
         */
    virtual void update(float time);
    virtual ActionPtr reverse(void);

public:
    /*Ptr makes the action with a duration and a bezier configuration 
         *  @code
         *  when this function bound to js,the input params are changed
         *  js: var create(var t, var Vec2Table)
         *  @endcode
         */
    static BezierByPtr make(float t, const BezierConfig& c);

protected:
    BezierConfig m_sConfig;
    Vec2 m_startPosition;
    Vec2 m_previousPosition;
};

class BezierTo;
typedef std::shared_ptr<BezierTo> BezierToPtr;
/** @brief An action that moves the target with a cubic Bezier curve to a destination Vec2.
     @since v0.8.2
     */
class BezierTo : public BezierBy {
public:
    /**
         *  @lua NA
         */
    virtual void startWithTarget(Node* pTarget);

public:
    /*Ptr makes the action with a duration and a bezier configuration
         *  @code
         *  when this function bound to js,the input params are changed
         *  js: var create(var t, var Vec2Table)
         *  @endcode
         */
    static BezierToPtr make(float t, const BezierConfig& c);
    /**
         *  @lua NA
         */
    bool initWithDuration(float t, const BezierConfig& c);

protected:
    BezierConfig m_sToConfig;
};

class ScaleTo;
typedef std::shared_ptr<ScaleTo> ScaleToPtr;
/** @brief Scales a Node object to a zoom factor by modifying it's scale attribute.
     @warning This action doesn't support "reverse"
     */
class ScaleTo : public ActionInterval {
public:
    /** initializes the action with the same scale factor for X and Y */
    bool initWithDuration(float duration, float s);

    /** initializes the action with and X factor and a Y factor */
    bool initWithDuration(float duration, float sx, float sy);
    /**
         *  @js NA
         *  @lua NA
         */
    virtual void startWithTarget(Node* pTarget);
    virtual void update(float time);

public:
    /*Ptr makes the action with the same scale factor for X and Y */
    static ScaleToPtr make(float duration, float s);

    /*Ptr makes the action with and X factor and a Y factor */
    static ScaleToPtr make(float duration, float sx, float sy);

protected:
    float m_fScaleX;
    float m_fScaleY;
    float m_fStartScaleX;
    float m_fStartScaleY;
    float m_fEndScaleX;
    float m_fEndScaleY;
    float m_fDeltaX;
    float m_fDeltaY;
};

class ScaleBy;
typedef std::shared_ptr<ScaleBy> ScaleByPtr;
/** @brief Scales a Node object a zoom factor by modifying it's scale attribute.
    */
class ScaleBy : public ScaleTo {
public:
    virtual void startWithTarget(Node* pTarget);
    virtual ActionPtr reverse(void);

public:
    /*Ptr makes the action with the same scale factor for X and Y */
    static ScaleByPtr make(float duration, float s);

    /*Ptr makes the action with and X factor and a Y factor */
    static ScaleByPtr make(float duration, float sx, float sy);
};

class Blink;
typedef std::shared_ptr<Blink> BlinkPtr;
/** @brief Blinks a Node object by modifying it's visible attribute
    */
class Blink : public ActionInterval {
public:
    /** initializes the action */
    bool initWithDuration(float duration, unsigned int uBlinks);
    /**
         *  @js NA
         *  @lua NA
         */

    virtual void update(float time);
    virtual ActionPtr reverse(void);

public:
    /*Ptr makes the action */
    static BlinkPtr make(float duration, unsigned int uBlinks);

    virtual void startWithTarget(Node* pTarget);
    virtual void stop();

protected:
    unsigned int m_nTimes;
    bool m_bOriginalState;
};

class Fader : public ActionInterval {
public:
    void setLimit(float limit) { m_limit = limit; };

protected:
    float m_limit{ 1.f };
};

class FadeIn;
typedef std::shared_ptr<FadeIn> FadeInPtr;
/** @brief Fades In an object that implements the RGBAProtocol protocol. It modifies the opacity from 0 to 255.
     The "reverse" of this action is FadeOut
     */
class FadeIn : public Fader {
public:
    virtual void update(float time);
    virtual ActionPtr reverse(void);

    static FadeInPtr make(float d);
    static FadeInPtr make(float d, float limit);
};

class FadeOut;
typedef std::shared_ptr<FadeOut> FadeOutPtr;
/** @brief Fades Out an object that implements the RGBAProtocol protocol. It modifies the opacity from 255 to 0.
     The "reverse" of this action is FadeIn
    */
class FadeOut : public Fader {
public:
    virtual void update(float time);
    virtual ActionPtr reverse(void);

    static FadeOutPtr make(float d);
    static FadeOutPtr make(float d, float limit);
};

class FadeTo;
typedef std::shared_ptr<FadeTo> FadeToPtr;
/** @brief Fades an object that implements the RGBAProtocol protocol. It modifies the opacity from the current value to a custom one.
     @warning This action doesn't support "reverse"
     */
class FadeTo : public ActionInterval {
public:
    /** initializes the action with duration and opacity */
    bool initWithDuration(float duration, unsigned char opacity);

    virtual void startWithTarget(Node* pTarget);
    virtual void update(float time);

public:
    /*Ptr makes an action with duration and opacity */
    static FadeToPtr make(float duration, unsigned char opacity);

protected:
    unsigned char m_toOpacity;
    unsigned char m_fromOpacity;
};

class TintTo;
typedef std::shared_ptr<TintTo> TintToPtr;
/** @brief Tints a Node that implements the NodeRGB protocol from current tint to a custom one.
     @warning This action doesn't support "reverse"
     @since v0.7.2
    */
class TintTo : public ActionInterval {
public:
    /** initializes the action with duration and color */
    bool initWithDuration(float duration, unsigned char red, unsigned char green, unsigned char blue);

    virtual void startWithTarget(Node* pTarget);
    virtual void update(float time);

public:
    /*Ptr makes an action with duration and color */
    static TintToPtr make(float duration, unsigned char red, unsigned char green, unsigned char blue);

protected:
    color::RGB m_to;
    color::RGB m_from;
};

class TintBy;
typedef std::shared_ptr<TintBy> TintByPtr;
/** @brief Tints a Node that implements the NodeRGB protocol from current tint to a custom one.
     @since v0.7.2
     */
class TintBy : public ActionInterval {
public:
    /** initializes the action with duration and color */
    bool initWithDuration(float duration, short deltaRed, short deltaGreen, short deltaBlue);

    virtual void startWithTarget(Node* pTarget);
    virtual void update(float time);
    virtual ActionPtr reverse(void);

public:
    /*Ptr makes an action with duration and color */
    static TintByPtr make(float duration, short deltaRed, short deltaGreen, short deltaBlue);

protected:
    short m_deltaR;
    short m_deltaG;
    short m_deltaB;

    short m_fromR;
    short m_fromG;
    short m_fromB;
};

class DelayTime;
typedef std::shared_ptr<DelayTime> DelayTimePtr;
/** @brief Delays the action a certain amount of seconds
    */
class DelayTime : public ActionInterval {
public:
    virtual void update(float time);
    virtual ActionPtr reverse(void);

public:
    /*Ptr makes the action */
    static DelayTimePtr make(float d);
};

class ReverseTime;
typedef std::shared_ptr<ReverseTime> ReverseTimePtr;
/** @brief Executes an action in reverse order, from time=duration to time=0
     
     @warning Use this action carefully. This action is not
     sequenceable. Use it as the default "reversed" method
     of your own actions, but using it outside the "reversed"
     scope is not recommended.
    */
class ReverseTime : public ActionInterval {
public:
    /** initializes the action */
    bool initWithAction(FiniteTimeActionPtr pAction);

    virtual void startWithTarget(Node* pTarget);
    virtual void stop(void);
    virtual void update(float time);
    virtual ActionPtr reverse(void);

public:
    /*Ptr makes the action */
    static ReverseTimePtr make(FiniteTimeActionPtr pAction);

protected:
    FiniteTimeActionPtr m_pOther;
};

class TargetedAction;
typedef std::shared_ptr<TargetedAction> TargetedActionPtr;
/** Overrides the target of an action so that it always runs on the target
     * specified at action creation rather than the one specified by runAction.
     */
class TargetedAction : public ActionInterval {
public:
    /**
         *  @js ctor
         */
    TargetedAction();

    /*Ptr make an action with the specified action and forced target */
    static TargetedActionPtr make(Node* pTarget, FiniteTimeActionPtr pAction);

    /** Init an action with the specified action and forced target */
    bool initWithTarget(Node* pTarget, FiniteTimeActionPtr pAction);

    virtual void startWithTarget(Node* pTarget);
    virtual void stop(void);
    virtual void update(float time);

    /** This is the target that the action will be forced to run with */
    CC_SYNTHESIZE_RETAIN(Node*, m_pForcedTarget, ForcedTarget);

private:
    FiniteTimeActionPtr m_pAction;
};

}
