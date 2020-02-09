#pragma once

#include "../Node.h"

namespace mak3do {
class Action;
typedef std::shared_ptr<Action> ActionPtr;

//FIXME Delete shared_from_this inherit once it subclasses Object
class Action : public std::enable_shared_from_this<Action> {
public:
    enum Tag {
        //! Default tag
        kActionTagInvalid = -1,
    };

    Action(void);

    virtual ~Action(void);

    std::string description() const;

    //! return true if the action has finished
    virtual bool done(void);

    //! called before the action start. It will also set the target.
    virtual void start(NodePtr pTarget);

    /** 
        called after the action has finished. It will set the 'target' to nil.
        IMPORTANT: You should never call "[action stop]" manually. Instead, use: "target->stopAction(action);"
        */
    virtual void stop();

    virtual void step(float dt);
    virtual void update(float time);

    inline NodePtr target(void) const { return m_target; }
    inline void target(NodePtr pTarget) { m_target = pTarget; }

    inline NodePtr original_target(void) const { return m_pOriginalTarget; }
    /** Set the original target, since target can be nil.
        Is the target that were used to run the action. Unless you are doing something complex, like ActionManager, you should NOT call this method.
        The target is 'assigned', it is not 'retained'.
        @since v0.8.2
        */
    inline void original_target(NodePtr pOriginalTarget) { m_pOriginalTarget = pOriginalTarget; }

    inline int tag(void) const { return m_nTag; }
    inline void tag(int nTag) { m_nTag = nTag; }

public:
    /** Create an action */
    static ActionPtr make();

protected:
    NodePtr m_pOriginalTarget;
    /** The "target".
        The target will be set with the 'startWithTarget' method.
        When the 'stop' method is called, target will be set to nil.
        The target is 'assigned', it is not 'retained'.
        */
    NodePtr m_target;
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
    inline float duration() const { return m_fDuration; }
    //! set duration in seconds of the action
    inline void duration(float duration) { m_fDuration = duration; }

    /** returns a reversed action */
    virtual ActionPtr reverse(void);

protected:
    //! duration in seconds
    float m_fDuration;
};

class ActionInterval;
typedef std::shared_ptr<ActionInterval> ActionIntervalPtr;
class RepeatForever;

}
