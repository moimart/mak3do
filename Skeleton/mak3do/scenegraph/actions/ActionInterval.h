#pragma once

#include "Action.h"
#include <mak3do/scenegraph/Node.h>
#include <mak3do/scenegraph/Color.h>

namespace mak3do {

class ActionInterval : public FiniteTimeAction {
public:
    inline float getElapsed(void) { return m_elapsed; }

    bool init(float d);

    virtual bool done(void) override;
    virtual void step(float dt) override;
    virtual void start(NodePtr pTarget) override;
    virtual ActionPtr reverse(void) override;

public:
    static ActionIntervalPtr make(float d);

public:
    void amplitude_rate(float amp);
    float amplitude_rate() const;

protected:
    float m_elapsed;
    bool m_bFirstTick;
};

class Speed : public Action {
public:
    Speed()
        : m_fSpeed(0.0)
        , m_pInnerAction(NULL)
    {
    }

    inline float speed(void) const { return m_fSpeed; }
    inline void speed(float fSpeed) { m_fSpeed = fSpeed; }
    bool init(ActionIntervalPtr pAction, float fSpeed);

    virtual void start(NodePtr pTarget) override;
    virtual void stop() override;
    virtual void step(float dt) override;
    virtual bool done(void) override;
    virtual ActionPtr reverse(void);

    void inner_action(ActionIntervalPtr pAction);

    inline ActionIntervalPtr inner_action() const
    {
        return m_pInnerAction;
    }

public:
    static SpeedPtr make(ActionIntervalPtr pAction, float fSpeed);

protected:
    float m_fSpeed;
    ActionIntervalPtr m_pInnerAction;
};

class Sequence : public ActionInterval {
public:
    bool init(FiniteTimeActionPtr pActionOne, FiniteTimeActionPtr pActionTwo);

    virtual void start(NodePtr pTarget);
    virtual void stop(void);
    virtual void update(float t);
    virtual ActionPtr reverse(void);

public:
    static SequencePtr make(const std::vector<ActionPtr>& actions);
    static SequencePtr make(FiniteTimeActionPtr pActionOne, FiniteTimeActionPtr pActionTwo);

protected:
    FiniteTimeActionPtr m_pActions[2];
    float m_split;
    int m_last;
};

class Repeat : public ActionInterval {
public:
    bool init(FiniteTimeActionPtr pAction, unsigned int times);

    virtual void start(NodePtr pTarget) override;
    virtual void stop(void) override;
    virtual void update(float dt) override;
    virtual bool done(void) override;
    virtual ActionPtr reverse(void) override;

    inline void inner_action(FiniteTimeActionPtr pAction)
    {
        if (m_pInnerAction != pAction) {
            m_pInnerAction = pAction;
        }
    }

    inline FiniteTimeActionPtr inner_action() const
    {
        return m_pInnerAction;
    }

public:
    static RepeatPtr make(FiniteTimeActionPtr pAction, unsigned int times);

protected:
    unsigned int m_uTimes;
    unsigned int m_uTotal;
    float m_fNextDt;
    bool m_bActionInstant;
    FiniteTimeActionPtr m_pInnerAction;
};

class RepeatForever : public ActionInterval {
public:
    RepeatForever()
        : m_pInnerAction(NULL)
    {
    }

    bool init(ActionIntervalPtr pAction);

    virtual void start(NodePtr pTarget) override;
    virtual void step(float dt) override;
    virtual bool done(void) override;
    virtual ActionPtr reverse(void) override;

    inline void inner_action(ActionIntervalPtr pAction)
    {
        if (m_pInnerAction != pAction) {
            m_pInnerAction = pAction;
        }
    }

    inline ActionIntervalPtr inner_action() const
    {
        return m_pInnerAction;
    }

public:
    static RepeatForeverPtr make(ActionIntervalPtr pAction);

protected:
    ActionIntervalPtr m_pInnerAction;
};

class Spawn : public ActionInterval {
public:

    bool init(FiniteTimeActionPtr pAction1, FiniteTimeActionPtr pAction2);

    virtual void start(NodePtr pTarget);
    virtual void stop(void);
    virtual void update(float time);
    virtual ActionPtr reverse(void);

public:
    static SpawnPtr make(const std::vector<ActionPtr>& actions);
    static SpawnPtr make(FiniteTimeActionPtr pAction1, FiniteTimeActionPtr pAction2);

protected:
    FiniteTimeActionPtr m_pOne;
    FiniteTimeActionPtr m_pTwo;
};

class RotateTo : public ActionInterval {
public:
    static RotateToPtr make(float fDuration, float fDeltaAngle);
    bool initWithDuration(float fDuration, float fDeltaAngle);

    static RotateToPtr make(float fDuration, float fDeltaAngleX, float fDeltaAngleY);
    virtual bool init(float fDuration, float fDeltaAngleX, float fDeltaAngleY);
    virtual void start(NodePtr pTarget);
    virtual void update(float time);

protected:
    float m_fDstAngleX;
    float m_fStartAngleX;
    float m_fDiffAngleX;

    float m_fDstAngleY;
    float m_fStartAngleY;
    float m_fDiffAngleY;
};

class RotateBy : public ActionInterval {
public:
    static RotateByPtr make(float fDuration, float fDeltaAngle);
    bool initWithDuration(float fDuration, float fDeltaAngle);

    static RotateByPtr make(float fDuration, float fDeltaAngleX, float fDeltaAngleY);
    bool init(float fDuration, float fDeltaAngleX, float fDeltaAngleY);
    virtual void start(NodePtr pTarget);
    virtual void update(float time);
    virtual ActionPtr reverse(void);

protected:
    float m_fAngleX;
    float m_fStartAngleX;
    float m_fAngleY;
    float m_fStartAngleY;
};

class MoveBy : public ActionInterval {
public:
    bool init(float duration, const Vec3& deltaPosition);
    
    virtual void start(NodePtr pTarget);
    virtual ActionPtr reverse(void);
    virtual void update(float time);

public:
    static MoveByPtr make(float duration, const Vec3& deltaPosition);

protected:
    Vec3 m_positionDelta;
    Vec3 m_startPosition;
    Vec3 m_previousPosition;
};

class MoveTo : public MoveBy {
public:
    bool init(float duration, const Vec3& position);

    virtual void start(NodePtr pTarget);

public:
    static MoveToPtr make(float duration, const Vec3& position);

protected:
    Vec3 m_endPosition;
};

class SkewTo : public ActionInterval {
public:
    SkewTo();
    virtual bool init(float t, float sx, float sy);

    virtual void start(NodePtr pTarget);
    virtual void update(float time);

public:
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

class SkewBy : public SkewTo {
public:
    virtual bool init(float t, float sx, float sy);
    virtual void start(NodePtr pTarget);
    virtual ActionPtr reverse(void);

public:
    static SkewByPtr make(float t, float deltaSkewX, float deltaSkewY);
};

class JumpBy : public ActionInterval {
public:
    bool init(float duration, const Vec3& position, float height, unsigned int jumps);

    virtual void start(NodePtr pTarget);
    virtual void update(float time);
    virtual ActionPtr reverse(void);

public:
    static JumpByPtr make(float duration, const Vec3& position, float height, unsigned int jumps);

protected:
    Vec3 m_startPosition;
    Vec3 m_delta;
    float m_height;
    unsigned int m_nJumps;
    Vec3 m_previousPos;
};

class JumpTo : public JumpBy {
public:
    virtual void start(NodePtr pTarget);

public:
    static JumpToPtr make(float duration, const Vec3& position, float height, int jumps);
};

class BezierBy : public ActionInterval {
public:
    bool init(float t, const BezierConfig& c);

    virtual void start(NodePtr pTarget);
    virtual void update(float time);
    virtual ActionPtr reverse(void);

public:
    static BezierByPtr make(float t, const BezierConfig& c);

protected:
    BezierConfig m_sConfig;
    Vec3 m_startPosition;
    Vec3 m_previousPosition;
};

class BezierTo;
typedef std::shared_ptr<BezierTo> BezierToPtr;
class BezierTo : public BezierBy {
public:
    virtual void start(NodePtr pTarget);

public:
    static BezierToPtr make(float t, const BezierConfig& c);
    bool init(float t, const BezierConfig& c);

protected:
    BezierConfig m_sToConfig;
};

class ScaleTo : public ActionInterval {
public:
    bool init(float duration, float s);
    bool init(float duration, float sx, float sy);

    virtual void start(NodePtr pTarget);
    virtual void update(float time);

public:
    static ScaleToPtr make(float duration, float s);
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

class ScaleBy : public ScaleTo {
public:
    virtual void start(NodePtr pTarget);
    virtual ActionPtr reverse(void);

public:
    static ScaleByPtr make(float duration, float s);
    static ScaleByPtr make(float duration, float sx, float sy);
};

class Blink : public ActionInterval {
public:
    bool init(float duration, unsigned int uBlinks);

    virtual void update(float time);
    virtual ActionPtr reverse(void);

public:
    static BlinkPtr make(float duration, unsigned int uBlinks);

    virtual void start(NodePtr pTarget);
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

class FadeIn : public Fader {
public:
    virtual void update(float time);
    virtual ActionPtr reverse(void);

    static FadeInPtr make(float d);
    static FadeInPtr make(float d, float limit);
};

class FadeOut : public Fader {
public:
    virtual void update(float time);
    virtual ActionPtr reverse(void);

    static FadeOutPtr make(float d);
    static FadeOutPtr make(float d, float limit);
};

class FadeTo : public ActionInterval {
public:
    bool init(float duration, unsigned char opacity);

    virtual void start(NodePtr pTarget);
    virtual void update(float time);

public:
    static FadeToPtr make(float duration, unsigned char opacity);

protected:
    unsigned char m_toOpacity;
    unsigned char m_fromOpacity;
};

class TintTo : public ActionInterval {
public:
    bool init(float duration, unsigned char red, unsigned char green, unsigned char blue);

    virtual void start(NodePtr pTarget);
    virtual void update(float time);
public:
    static TintToPtr make(float duration, unsigned char red, unsigned char green, unsigned char blue);

protected:
    color::RGB m_to;
    color::RGB m_from;
};

class TintBy : public ActionInterval {
public:
    bool init(float duration, short deltaRed, short deltaGreen, short deltaBlue);

    virtual void start(NodePtr pTarget);
    virtual void update(float time);
    virtual ActionPtr reverse(void);

public:
    static TintByPtr make(float duration, short deltaRed, short deltaGreen, short deltaBlue);

protected:
    short m_deltaR;
    short m_deltaG;
    short m_deltaB;

    short m_fromR;
    short m_fromG;
    short m_fromB;
};

class DelayTime : public ActionInterval {
public:
    virtual void update(float time);
    virtual ActionPtr reverse(void);

public:
    static DelayTimePtr make(float d);
};

class ReverseTime : public ActionInterval {
public:
    bool init(FiniteTimeActionPtr pAction);

    virtual void start(NodePtr pTarget);
    virtual void stop(void);
    virtual void update(float time);
    virtual ActionPtr reverse(void);

public:
    static ReverseTimePtr make(FiniteTimeActionPtr pAction);

protected:
    FiniteTimeActionPtr m_pOther;
};

class TargetedAction : public ActionInterval {
public:
    TargetedAction();

    static TargetedActionPtr make(NodePtr pTarget, FiniteTimeActionPtr pAction);

    bool initWithTarget(NodePtr pTarget, FiniteTimeActionPtr pAction);

    virtual void start(NodePtr pTarget);
    virtual void stop(void);
    virtual void update(float time);

    NodePtr getForcedTarget() const;
    void setForcedTarget(NodePtr node);

private:
    NodePtr m_pForcedTarget;
    FiniteTimeActionPtr m_pAction;
};

class SpinBy : public ActionInterval {
public:
    
    static SpinByPtr make(float fDuration, float fDeltaAngle);
    static SpinByPtr make(float fDuration, const Vec3& deltaAngle);
    
    bool init(float fDuration, const Vec3& deltaAngle);

    virtual void start(NodePtr target);
    virtual void update(float time);
    virtual ActionPtr reverse(void);

protected:
    float m_fAngleX;
    float m_fStartAngleX;
    float m_fAngleY;
    float m_fStartAngleY;
    float m_fAngleZ;
    float m_fStartAngleZ;
};

}
