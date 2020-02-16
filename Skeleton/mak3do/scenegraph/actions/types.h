#pragma once
#include <memory>
#include <mak3do/math/all>

namespace mak3do {
typedef struct _BezierConfig {
    Vec3 endPosition;
    Vec3 control_1;
    Vec3 control_2;
} BezierConfig;

class Action;
class FiniteTimeAction;
class ActionInterval;
class Speed;
class Sequence;
class Repeat;
class RepeatForever;
class Spawn;
class RotateTo;
class RotateBy;
class MoveBy;
class MoveTo;
class SkewTo;
class SkewBy;
class JumpBy;
class JumpTo;
class BezierBy;
class ScaleTo;
class ScaleBy;
class Blink;
class FadeIn;
class FadeOut;
class FadeTo;
class TintTo;
class TintBy;
class DelayTime;
class ReverseTime;
class TargetedAction;
class SpinBy;

typedef std::shared_ptr<SpinBy> SpinByPtr;
typedef std::shared_ptr<TargetedAction> TargetedActionPtr;
typedef std::shared_ptr<ReverseTime> ReverseTimePtr;
typedef std::shared_ptr<DelayTime> DelayTimePtr;
typedef std::shared_ptr<TintBy> TintByPtr;
typedef std::shared_ptr<TintTo> TintToPtr;
typedef std::shared_ptr<FadeTo> FadeToPtr;
typedef std::shared_ptr<FadeOut> FadeOutPtr;
typedef std::shared_ptr<FadeIn> FadeInPtr;
typedef std::shared_ptr<Blink> BlinkPtr;
typedef std::shared_ptr<ScaleBy> ScaleByPtr;
typedef std::shared_ptr<ScaleTo> ScaleToPtr;
typedef std::shared_ptr<BezierBy> BezierByPtr;
typedef std::shared_ptr<JumpTo> JumpToPtr;
typedef std::shared_ptr<JumpBy> JumpByPtr;
typedef std::shared_ptr<SkewBy> SkewByPtr;
typedef std::shared_ptr<SkewTo> SkewToPtr;
typedef std::shared_ptr<MoveTo> MoveToPtr;
typedef std::shared_ptr<MoveBy> MoveByPtr;
typedef std::shared_ptr<RotateBy> RotateByPtr;
typedef std::shared_ptr<RotateTo> RotateToPtr;
typedef std::shared_ptr<Spawn> SpawnPtr;
typedef std::shared_ptr<RepeatForever> RepeatForeverPtr;
typedef std::shared_ptr<Repeat> RepeatPtr;
typedef std::shared_ptr<Sequence> SequencePtr;
typedef std::shared_ptr<Speed> SpeedPtr;
typedef std::shared_ptr<ActionInterval> ActionIntervalPtr;
typedef std::shared_ptr<FiniteTimeAction> FiniteTimeActionPtr;
typedef std::shared_ptr<Action> ActionPtr;

}
