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

#pragma once

#include "ActionInterval.h"

namespace cocos2d {
class CCObject;
}

namespace mak3do {

class ActionEase;
typedef std::shared_ptr<ActionEase> ActionEasePtr;

class ActionEase : public ActionInterval {
public:
    /** initializes the action */
    bool initWithAction(ActionIntervalPtr pAction);

    virtual void startWithTarget(Node* pTarget);
    virtual void stop(void);
    virtual void update(float time);
    virtual ActionPtr reverse(void);
    virtual ActionPtr getInnerAction();

public:
    static ActionEasePtr make(ActionIntervalPtr pAction);

protected:
    /** The inner action */
    ActionIntervalPtr m_pInner;
};

/** 
     @brief Base class for Easing actions with rate parameters
     @ingroup Actions
     */
class EaseRateAction;
typedef std::shared_ptr<EaseRateAction> EaseRateActionPtr;

class EaseRateAction : public ActionEase {
public:
    /**
         *  @js NA
         *  @lua NA
         */
    virtual ~EaseRateAction(void);

    /** set rate value for the actions */
    inline void setRate(float rate) { m_fRate = rate; }
    /** get rate value for the actions */
    inline float getRate(void) { return m_fRate; }

    /** Initializes the action with the inner action and the rate parameter */
    bool initWithAction(ActionIntervalPtr pAction, float fRate);
    /**
         *  @js NA
         *  @lua NA
         */

    virtual ActionPtr reverse(void);

public:
    /*Ptr make s the action with the inner action and the rate parameter */
    static EaseRateActionPtr make(ActionIntervalPtr pAction, float fRate);

protected:
    float m_fRate;
};

/** 
     @brief CCEaseIn action with a rate
     @ingroup Actions
     */
class EaseIn;
typedef std::shared_ptr<EaseIn> EaseInPtr;

class EaseIn : public EaseRateAction {
public:
    virtual void update(float time);
    virtual ActionPtr reverse(void);
public:
    /*Ptr make s the action with the inner action and the rate parameter */
    static EaseInPtr make(ActionIntervalPtr pAction, float fRate);
};

/** 
     @brief CCEaseOut action with a rate
     @ingroup Actions
     */
class EaseOut;
typedef std::shared_ptr<EaseOut> EaseOutPtr;

class EaseOut : public EaseRateAction {
public:
    virtual void update(float time);
    virtual ActionPtr reverse();
    /**
         *  @js NA
         *  @lua NA
         */

public:
    /*Ptr make s the action with the inner action and the rate parameter */
    static EaseOutPtr make(ActionIntervalPtr pAction, float fRate);
};

/** 
     @brief CCEaseInOut action with a rate
     @ingroup Actions
     */
class EaseInOut;
typedef std::shared_ptr<EaseInOut> EaseInOutPtr;

class EaseInOut : public EaseRateAction {
public:
    virtual void update(float time);
    virtual ActionPtr reverse(void);

public:
    /*Ptr make s the action with the inner action and the rate parameter */
    static EaseInOutPtr make(ActionIntervalPtr pAction, float fRate);
};

/** 
     @brief CCEase Exponential In
     @ingroup Actions
     */
class EaseExponentialIn;
typedef std::shared_ptr<EaseExponentialIn> EaseExponentialInPtr;

class EaseExponentialIn : public ActionEase {
public:
    virtual void update(float time);
    virtual ActionPtr reverse(void);

public:
    static EaseExponentialInPtr make(ActionIntervalPtr pAction);
};

/** 
     @brief Ease Exponential Out
     @ingroup Actions
     */
class EaseExponentialOut;
typedef std::shared_ptr<EaseExponentialOut> EaseExponentialOutPtr;

class EaseExponentialOut : public ActionEase {
public:
    virtual void update(float time);
    virtual ActionPtr reverse(void);

public:
    static EaseExponentialOutPtr make(ActionIntervalPtr pAction);
};

/** 
     @brief Ease Exponential InOut
     @ingroup Actions
     */
class EaseExponentialInOut;
typedef std::shared_ptr<EaseExponentialInOut> EaseExponentialInOutPtr;

class EaseExponentialInOut : public ActionEase {
public:
    virtual void update(float time);
    virtual ActionPtr reverse();

public:
    static EaseExponentialInOutPtr make(ActionIntervalPtr pAction);
};

/** 
     @brief Ease Sine In
     @ingroup Actions
     */
class EaseSineIn;
typedef std::shared_ptr<EaseSineIn> EaseSineInPtr;

class EaseSineIn : public ActionEase {
public:
    virtual void update(float time);
    virtual ActionPtr reverse(void);

public:
    static EaseSineInPtr make(ActionIntervalPtr pAction);
};

/** 
     @brief Ease Sine Out
     @ingroup Actions
     */
class EaseSineOut;
typedef std::shared_ptr<EaseSineOut> EaseSineOutPtr;

class EaseSineOut : public ActionEase {
public:
    virtual void update(float time);
    virtual ActionPtr reverse(void);

public:
    static EaseSineOutPtr make(ActionIntervalPtr pAction);
};

/** 
     @brief Ease Sine InOut
     @ingroup Actions
     */
class EaseSineInOut;
typedef std::shared_ptr<EaseSineInOut> EaseSineInOutPtr;

class EaseSineInOut : public ActionEase {
public:
    virtual void update(float time);
    virtual ActionPtr reverse();

public:
    static EaseSineInOutPtr make(ActionIntervalPtr pAction);
};

/** 
     @brief Ease Elastic abstract class
     @since v0.8.2
     @ingroup Actions
     */
class EaseElastic;
typedef std::shared_ptr<EaseElastic> EaseElasticPtr;

class EaseElastic : public ActionEase {
public:
    /** get period of the wave in radians. default is 0.3 */
    inline float getPeriod(void) { return m_fPeriod; }
    /** set period of the wave in radians. */
    inline void setPeriod(float fPeriod) { m_fPeriod = fPeriod; }

    /** Initializes the action with the inner action and the period in radians (default is 0.3) */
    bool initWithAction(ActionIntervalPtr pAction, float fPeriod = 0.3f);

    virtual ActionPtr reverse(void);

public:
    /*Ptr make s the action with the inner action and the period in radians (default is 0.3) */
    static EaseElasticPtr make(ActionIntervalPtr pAction, float fPeriod);
    static EaseElasticPtr make(ActionIntervalPtr pAction);

protected:
    float m_fPeriod;
};

/** 
     @brief Ease Elastic In action.
     @warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
     @since v0.8.2
     @ingroup Actions
     */
class EaseElasticIn;
typedef std::shared_ptr<EaseElasticIn> EaseElasticInPtr;

class EaseElasticIn : public EaseElastic {
public:
    virtual void update(float time);
    virtual ActionPtr reverse(void);

public:
    /*Ptr make s the action with the inner action and the period in radians (default is 0.3) */
    static EaseElasticInPtr make(ActionIntervalPtr pAction, float fPeriod);
    static EaseElasticInPtr make(ActionIntervalPtr pAction);
};

/** 
     @brief Ease Elastic Out action.
     @warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
     @since v0.8.2
     @ingroup Actions
     */
class EaseElasticOut;
typedef std::shared_ptr<EaseElasticOut> EaseElasticOutPtr;

class EaseElasticOut : public EaseElastic {
public:
    virtual void update(float time);
    virtual ActionPtr reverse(void);

public:
    /*Ptr make s the action with the inner action and the period in radians (default is 0.3) */
    static EaseElasticOutPtr make(ActionIntervalPtr pAction, float fPeriod);
    static EaseElasticOutPtr make(ActionIntervalPtr pAction);
};

/** 
     @brief Ease Elastic InOut action.
     @warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
     @since v0.8.2
     @ingroup Actions
     */
class EaseElasticInOut;
typedef std::shared_ptr<EaseElasticInOut> EaseElasticInOutPtr;

class EaseElasticInOut : public EaseElastic {
public:
    virtual void update(float time);
    virtual ActionPtr reverse(void);

public:
    /*Ptr make s the action with the inner action and the period in radians (default is 0.3) */
    static EaseElasticInOutPtr make(ActionIntervalPtr pAction, float fPeriod);
    static EaseElasticInOutPtr make(ActionIntervalPtr pAction);
};

/** 
     @brief CCEaseBounce abstract class.
     @since v0.8.2
     @ingroup Actions
    */
class EaseBounce;
typedef std::shared_ptr<EaseBounce> EaseBouncePtr;

class EaseBounce : public ActionEase {
public:
    float bounceTime(float time);
    virtual ActionPtr reverse();

public:
    static EaseBouncePtr make(ActionIntervalPtr pAction);
};

/** 
     @brief CCEaseBounceIn action.
     @warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
     @since v0.8.2
     @ingroup Actions
    */
class EaseBounceIn;
typedef std::shared_ptr<EaseBounceIn> EaseBounceInPtr;

class EaseBounceIn : public EaseBounce {
public:
    virtual void update(float time);
    virtual ActionPtr reverse(void);

public:
    static EaseBounceInPtr make(ActionIntervalPtr pAction);
};

/** 
     @brief EaseBounceOut action.
     @warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
     @since v0.8.2
     @ingroup Actions
     */
class EaseBounceOut;
typedef std::shared_ptr<EaseBounceOut> EaseBounceOutPtr;

class EaseBounceOut : public EaseBounce {
public:
    virtual void update(float time);
    virtual ActionPtr reverse(void);

public:
    static EaseBounceOutPtr make(ActionIntervalPtr pAction);
};

/** 
     @brief CCEaseBounceInOut action.
     @warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
     @since v0.8.2
     @ingroup Actions
     */
class EaseBounceInOut;
typedef std::shared_ptr<EaseBounceInOut> EaseBounceInOutPtr;

class EaseBounceInOut : public EaseBounce {
public:
    virtual void update(float time);
    virtual ActionPtr reverse();

public:
    static EaseBounceInOutPtr make(ActionIntervalPtr pAction);
};

/** 
     @brief CCEaseBackIn action.
     @warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
     @since v0.8.2
     @ingroup Actions
     */
class EaseBackIn;
typedef std::shared_ptr<EaseBackIn> EaseBackInPtr;

class EaseBackIn : public ActionEase {
public:
    virtual void update(float time);
    virtual ActionPtr reverse(void);

public:
    static EaseBackInPtr make(ActionIntervalPtr pAction);
};

/** 
     @brief CCEaseBackOut action.
     @warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
     @since v0.8.2
     @ingroup Actions
     */
class EaseBackOut;
typedef std::shared_ptr<EaseBackOut> EaseBackOutPtr;

class EaseBackOut : public ActionEase {
public:
    virtual void update(float time);
    virtual ActionPtr reverse(void);

public:
    static EaseBackOutPtr make(ActionIntervalPtr pAction);
};

/** 
     @brief CCEaseBackInOut action.
     @warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
     @since v0.8.2
     @ingroup Actions
     */
class EaseBackInOut;
typedef std::shared_ptr<EaseBackInOut> EaseBackInOutPtr;

class EaseBackInOut : public ActionEase {
public:
    virtual void update(float time);
    virtual ActionPtr reverse();

public:
    static EaseBackInOutPtr make(ActionIntervalPtr pAction);
};

}
