#pragma once

#include "ActionInstant.h"
#include "ActionInterval.h"
#include <functional>
#include <mak3do/base/math/Vec3.h>
#include <mak3do/base/math/math.h>
#include <mak3do/drawing/tools/CameraEasing.h>

using namespace cocos2d;

namespace mak3do {

class CustomAction;
typedef std::shared_ptr<CustomAction> CustomActionPtr;

class CustomAction : public ActionInterval {
public:
    typedef std::function<void(float, Vec3&, Node*)> CustomFunction;

    bool initWithFunction(float duration, std::function<Vec3(void)> initfunc, CustomFunction update);

    virtual void startWithTarget(Node* pTarget);
    virtual void update(float time);

    static CustomActionPtr make(float duration, std::function<Vec3(void)> initfunc, CustomFunction update);

protected:
    Vec3 m_data;
    CustomFunction m_update_function;
    float m_dt;
};

class CustomScale;
typedef std::shared_ptr<CustomScale> CustomScalePtr;

class CustomScale : public ActionInterval {
public:
    enum AxisFlag {
        AXIS_X = 0x001,
        AXIS_Y = 0x001 << 1,
        AXIS_Z = 0x001 << 2,
        AXIS_ALL = AXIS_X | AXIS_Y | AXIS_Z
    };

    bool initWithFunction(float duration, AxisFlag flags, float speed, std::function<float(float, float, float)> func);

    virtual void startWithTarget(Node* pTarget);
    virtual void update(float time);

public:
    /** creates the action with the same scale factor for X and Y */
    static CustomScalePtr make(float duration, AxisFlag flags, float speed, std::function<float(float, float, float)> func);

protected:
    std::function<float(float, float, float)> m_function;
    float m_origScaleX, m_origScaleY, m_origScaleZ;
    float m_speed;

    AxisFlag m_axisFlag;
};

class Lerp;
typedef std::shared_ptr<Lerp> LerpPtr;

class Lerp : public ActionInterval {
public:
    static LerpPtr make(float d, float v0, float v1, std::function<void(float)> func);

    bool init(float d, float v0, float v1, std::function<void(float)> func);

    void update(float time);

protected:
    float m_v0{ 0 };
    float m_v1{ 1 };
    std::function<void(float)> m_func{ nullptr };
};

class Tween;
typedef std::shared_ptr<Tween> TweenPtr;

class Tween : public ActionInterval {
public:
    static TweenPtr make(float d, float v0, float v1, Easing::CameraFunction easing, std::function<void(float)> func);

    bool init(float d, float v0, float v1, Easing::CameraFunction easing, std::function<void(float)> func);

    void update(float time);

protected:
    float m_v0{ 0 };
    float m_v1{ 1 };
    Easing::CameraFunction m_easing{ nullptr };
    std::function<void(float)> m_func{ nullptr };
};

class SpinLerp;
typedef std::shared_ptr<SpinLerp> SpinLerpPtr;

class SpinLerp : public ActionInterval {
public:
    /** creates the action */
    static SpinLerpPtr make(float fDuration, float fDeltaAngle);
    bool initWithDuration(float fDuration, float fDeltaAngle);

    static SpinLerpPtr make(float fDuration, float targetAngleX, float targetAngleY, float targetAngleZ);
    bool initWithDuration(float fDuration, float targetAngleX, float targetAngleY, float targetAngleZ);

    virtual void startWithTarget(Node* pTarget);
    virtual void update(float time);

protected:
    float m_fAngleX;
    float m_fStartAngleX;
    float m_fAngleY;
    float m_fStartAngleY;
    float m_fAngleZ;
    float m_fStartAngleZ;
};

class SpinBy;
typedef std::shared_ptr<SpinBy> SpinByPtr;

class SpinBy : public ActionInterval {
public:
    /** creates the action */
    static SpinByPtr make(float fDuration, float fDeltaAngle);
    /** initializes the action */
    bool initWithDuration(float fDuration, float fDeltaAngle);

    static SpinByPtr make(float fDuration, float fDeltaAngleX, float fDeltaAngleY, float fDeltaAngleZ);
    bool initWithDuration(float fDuration, float fDeltaAngleX, float fDeltaAngleY, float fDeltaAngleZ);

    virtual void startWithTarget(Node* pTarget);
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

namespace fx {

    class CustomUpdate;
    typedef std::shared_ptr<CustomUpdate> CustomUpdatePtr;

    class CustomUpdate : public ActionInterval {
    public:
        static CustomUpdatePtr make(float duration, const std::string& uniform, std::function<float(float, float)> func);
        bool initWithFunction(float duration, const std::string& uniform, std::function<float(float, float)> func);

        virtual void startWithTarget(Node* pTarget);
        virtual void update(float time);

    protected:
        std::function<float(float, float)> m_function;
        float m_origValue;
        std::string m_uniform;
    };
}
class Callback;
typedef std::shared_ptr<Callback> CallbackPtr;

class Callback : public ActionInstant {
public:
    typedef std::function<void()> VoidCallback;
    typedef std::function<void(void*)> DataCallback;

    static CallbackPtr make(VoidCallback callback);
    static CallbackPtr make(DataCallback callback);

    virtual void update(float time);

    Callback(VoidCallback callback);
    Callback(DataCallback, void* data);

protected:
    VoidCallback m_callback;
    DataCallback m_dataCallback;
    void* m_data;
};

class LightTo;
typedef std::shared_ptr<LightTo> LightToPtr;

class LightTo : public ActionInterval {
public:
    void update(float time);

    virtual void startWithTarget(Node* pTarget);
    bool initWithDuration(float duration, Vec3& ambience, Vec3& diffuse, Vec3& specular, Vec3& position, float intensity);

public:
    static LightToPtr make(float duration, Vec3& ambience, Vec3& diffuse, Vec3& specular, Vec3& position, float intensity);

private:
    Vec3 m_fromAmbience, m_fromDiffuse, m_fromSpecular, m_fromPosition, m_toAmbience, m_toDiffuse, m_toSpecular, m_toPosition;
    float m_fromIntensity, m_toIntensity;
};
}
