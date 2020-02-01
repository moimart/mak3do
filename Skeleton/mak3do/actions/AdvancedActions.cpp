#include "AdvancedActions.h"
#include <mak3do/drawing/fx/PostEffect.h>
#include <mak3do/drawing/objects/Light.h>
#include <mak3do/drawing/objects/Model.h>

using namespace mak3do;
using namespace mak3do::fx;

CustomActionPtr CustomAction::make(float duration, std::function<Vec3(void)> initfunc, CustomFunction update)
{
    auto ca = std::make_shared<CustomAction>();
    ca->initWithFunction(duration, initfunc, update);

    return ca;
}

bool CustomAction::initWithFunction(float duration, std::function<Vec3(void)> initfunc, CustomFunction update)
{
    if (ActionInterval::initWithDuration(duration)) {
        if (initfunc != nullptr)
            m_data = initfunc();

        m_dt = 0;
        m_update_function = update;
        return true;
    }

    return false;
}

void CustomAction::startWithTarget(Node* pTarget)
{
    ActionInterval::startWithTarget(pTarget);

    m_dt = 0;
}

void CustomAction::update(float time)
{
    if (m_pTarget && m_update_function != nullptr) {
        m_dt += Director::getInstance()->getDeltaTime(); //time;
        m_update_function(m_dt, m_data, m_pTarget);
    }
}

CustomScalePtr CustomScale::make(float duration, AxisFlag flags, float speed, std::function<float(float, float, float)> func)
{
    auto pScaler = std::make_shared<CustomScale>();
    pScaler->initWithFunction(duration, flags, speed, func);

    return pScaler;
}

bool CustomScale::initWithFunction(float duration, AxisFlag flags, float speed, std::function<float(float, float, float)> func)
{
    if (ActionInterval::initWithDuration(duration)) {
        m_function = func;
        m_speed = speed;
        m_axisFlag = flags;
        return true;
    }

    return false;
}

void CustomScale::startWithTarget(Node* pTarget)
{
    ActionInterval::startWithTarget(pTarget);

    Node3D* target = dynamic_cast<Node3D*>(pTarget);

    if (target != NULL) {
        target->getScale(&m_origScaleX, &m_origScaleY, &m_origScaleZ);
    }
}

void CustomScale::update(float time)
{
    if (m_pTarget) {
        Node3D* target = dynamic_cast<Node3D*>(m_pTarget);

        if (target != NULL) {
            if ((m_axisFlag & AxisFlag::AXIS_X) == AxisFlag::AXIS_X)
                target->setScaleX(m_function(time, m_speed, m_origScaleX));

            if ((m_axisFlag & AxisFlag::AXIS_Y) == AxisFlag::AXIS_Y)
                target->setScaleY(m_function(time, m_speed, m_origScaleY));

            if ((m_axisFlag & AxisFlag::AXIS_Z) == AxisFlag::AXIS_Z)
                target->setScaleZ(m_function(time, m_speed, m_origScaleZ));
        }
    }
}

LerpPtr Lerp::make(float d, float v0, float v1, std::function<void(float)> func)
{
    auto l = std::make_shared<Lerp>();
    if (l->init(d, v0, v1, func))
        return l;

    return LerpPtr();
}

bool Lerp::init(float d, float v0, float v1, std::function<void(float)> func)
{
    if (ActionInterval::initWithDuration(d)) {
        m_v0 = v0;
        m_v1 = v1;
        m_func = func;
        return true;
    }

    return false;
}

void Lerp::update(float time)
{
    if (m_func != nullptr)
        m_func(m_v0 + (m_v1 - m_v0) * time);
}

TweenPtr Tween::make(float d, float v0, float v1, Easing::CameraFunction easing, std::function<void(float)> func)
{
    auto l = std::make_shared<Tween>();
    if (l->init(d, v0, v1, easing, func))
        return l;

    return TweenPtr();
}

bool Tween::init(float d, float v0, float v1, Easing::CameraFunction easing, std::function<void(float)> func)
{
    if (ActionInterval::initWithDuration(d)) {
        m_v0 = v0;
        m_v1 = v1;
        m_func = func;
        m_easing = easing;
        return true;
    }

    return false;
}

void Tween::update(float time)
{
    if (m_func != nullptr && m_easing != nullptr) {
        Vec3 b1 = Vec3(m_v0);
        Vec3 b2 = Vec3(m_v0);
        Vec3 c1 = Vec3(m_v1);
        Vec3 c2 = Vec3(m_v1);
        Vec3 bu(0, 1, 0);
        Vec3 cu = Vec3::ZERO;

        Vec3 r, rr, rrr;

        m_easing(&r, &rr, &rrr,
            std::make_tuple(b1, b2, c1, c2, bu, cu),
            time, m_fDuration);

        m_func(r.x);
    }
}

SpinLerpPtr SpinLerp::make(float fDuration, float fDeltaAngle)
{
    auto pRotateBy = std::make_shared<SpinLerp>();
    pRotateBy->initWithDuration(fDuration, fDeltaAngle);

    return pRotateBy;
}

bool SpinLerp::initWithDuration(float fDuration, float fDeltaAngle)
{
    if (ActionInterval::initWithDuration(fDuration)) {
        m_fAngleX = m_fAngleY = fDeltaAngle;
        return true;
    }

    return false;
}

SpinLerpPtr SpinLerp::make(float fDuration, float fDeltaAngleX, float fDeltaAngleY, float fDeltaAngleZ)
{
    auto pRotateBy = std::make_shared<SpinLerp>();
    pRotateBy->initWithDuration(fDuration, fDeltaAngleX, fDeltaAngleY, fDeltaAngleZ);

    return pRotateBy;
}

bool SpinLerp::initWithDuration(float fDuration, float fDeltaAngleX, float fDeltaAngleY, float fDeltaAngleZ)
{
    if (ActionInterval::initWithDuration(fDuration)) {
        m_fAngleX = fDeltaAngleX;
        m_fAngleY = fDeltaAngleY;
        m_fAngleZ = fDeltaAngleZ;
        return true;
    }

    return false;
}

void SpinLerp::startWithTarget(Node* pTarget)
{
    ActionInterval::startWithTarget(pTarget);
    Model* model = (Model*)m_pTarget;
    m_fStartAngleX = model->getYaw();
    m_fStartAngleY = model->getRoll();
    m_fStartAngleZ = model->getPitch();
}

void SpinLerp::update(float time)
{
    if (m_pTarget) {
        Model* model = (Model*)m_pTarget;
        model->setYaw(m_fStartAngleX + (m_fAngleX - m_fStartAngleX) * time);
        model->setRoll(m_fStartAngleY + (m_fAngleY - m_fStartAngleY) * time);
        model->setPitch(m_fStartAngleZ + (m_fAngleZ - m_fStartAngleZ) * time);
    }
}

SpinByPtr SpinBy::make(float fDuration, float fDeltaAngle)
{
    auto pRotateBy = std::make_shared<SpinBy>();
    pRotateBy->initWithDuration(fDuration, fDeltaAngle);

    return pRotateBy;
}

bool SpinBy::initWithDuration(float fDuration, float fDeltaAngle)
{
    if (ActionInterval::initWithDuration(fDuration)) {
        m_fAngleX = m_fAngleY = fDeltaAngle;
        return true;
    }

    return false;
}

SpinByPtr SpinBy::make(float fDuration, float fDeltaAngleX, float fDeltaAngleY, float fDeltaAngleZ)
{
    auto pRotateBy = std::make_shared<SpinBy>();
    pRotateBy->initWithDuration(fDuration, fDeltaAngleX, fDeltaAngleY, fDeltaAngleZ);

    return pRotateBy;
}

bool SpinBy::initWithDuration(float fDuration, float fDeltaAngleX, float fDeltaAngleY, float fDeltaAngleZ)
{
    if (ActionInterval::initWithDuration(fDuration)) {
        m_fAngleX = fDeltaAngleX;
        m_fAngleY = fDeltaAngleY;
        m_fAngleZ = fDeltaAngleZ;
        return true;
    }

    return false;
}

void SpinBy::startWithTarget(Node* pTarget)
{
    ActionInterval::startWithTarget(pTarget);
    Model* model = (Model*)m_pTarget;
    m_fStartAngleX = model->getYaw();
    m_fStartAngleY = model->getRoll();
    m_fStartAngleZ = model->getPitch();
}

void SpinBy::update(float time)
{
    // XXX: shall I add % 360
    if (m_pTarget) {
        Model* model = (Model*)m_pTarget;
        model->setYaw(m_fStartAngleX + m_fAngleX * time);
        model->setRoll(m_fStartAngleY + m_fAngleY * time);
        model->setPitch(m_fStartAngleZ + m_fAngleZ * time);
    }
}

ActionPtr SpinBy::reverse(void)
{
    return SpinBy::make(m_fDuration, -m_fAngleX, -m_fAngleY, -m_fAngleZ);
}

CustomUpdatePtr CustomUpdate::make(float duration, const std::string& uniform, std::function<float(float, float)> func)
{
    auto cu = std::make_shared<CustomUpdate>();
    cu->initWithFunction(duration, uniform, func);

    return cu;
}

bool CustomUpdate::initWithFunction(float duration, const std::string& uniform, std::function<float(float, float)> func)
{
    if (ActionInterval::initWithDuration(duration)) {
        m_function = func;
        m_uniform = uniform;
        return true;
    }

    return false;
}

void CustomUpdate::startWithTarget(Node* pTarget)
{
    ActionInterval::startWithTarget(pTarget);

    fx::IPostEffecUpdate* target = dynamic_cast<fx::IPostEffecUpdate*>(pTarget);

    if (target != NULL) {
        m_origValue = target->getUniformValue(m_uniform);
    }
}

void CustomUpdate::update(float time)
{
    if (m_pTarget) {
        fx::IPostEffecUpdate* target = dynamic_cast<fx::IPostEffecUpdate*>(m_pTarget);

        if (target != NULL) {
            target->updateUniform(m_uniform, m_function(time, m_origValue));
        }
    }
}

Callback::Callback(Callback::VoidCallback callback)
    : m_callback(callback)
    , m_dataCallback(nullptr)
{
}

Callback::Callback(Callback::DataCallback callback, void* data)
    : m_callback(nullptr)
    , m_dataCallback(callback)
    , m_data(data)
{
}

CallbackPtr Callback::make(Callback::VoidCallback callback)
{
    return std::make_shared<Callback>(callback);
}

void Callback::update(float time)
{
    CC_UNUSED_PARAM(time);

    if (m_callback != nullptr)
        m_callback();
    else
        m_dataCallback(m_data);
}

LightToPtr LightTo::make(float duration, Vec3& ambience, Vec3& diffuse, Vec3& specular, Vec3& position, float intensity)
{
    auto pAction = std::make_shared<LightTo>();
    pAction->initWithDuration(duration, ambience, diffuse, specular, position, intensity);

    return pAction;
}

bool LightTo::initWithDuration(float duration, Vec3& ambience, Vec3& diffuse, Vec3& specular, Vec3& position, float intensity)
{
    if (ActionInterval::initWithDuration(duration)) {
        m_toAmbience = ambience;
        m_toDiffuse = diffuse;
        m_toSpecular = specular;
        m_toPosition = position;
        m_toIntensity = intensity;
        return true;
    }

    return false;
}

void LightTo::update(float time)
{
    Vec3 curAmb(
        m_fromAmbience.x + (m_toAmbience.x - m_fromAmbience.x) * time,
        m_fromAmbience.y + (m_toAmbience.y - m_fromAmbience.y) * time,
        m_fromAmbience.z + (m_toAmbience.z - m_fromAmbience.z) * time);

    Vec3 curDif(
        m_fromDiffuse.x + (m_toDiffuse.x - m_fromDiffuse.x) * time,
        m_fromDiffuse.y + (m_toDiffuse.y - m_fromDiffuse.y) * time,
        m_fromDiffuse.z + (m_toDiffuse.z - m_fromDiffuse.z) * time);

    Vec3 curSpec(
        m_fromSpecular.x + (m_toSpecular.x - m_fromSpecular.x) * time,
        m_fromSpecular.y + (m_toSpecular.y - m_fromSpecular.y) * time,
        m_fromSpecular.z + (m_toSpecular.z - m_fromSpecular.z) * time);

    Vec3 curPos(
        m_fromPosition.x + (m_toPosition.x - m_fromPosition.x) * time,
        m_fromPosition.y + (m_toPosition.y - m_fromPosition.y) * time,
        m_fromPosition.z + (m_toPosition.z - m_fromPosition.z) * time);

    float curIntensity = m_fromIntensity + (m_toIntensity - m_fromIntensity) * time;

    Light* light = dynamic_cast<Light*>(m_pTarget);
    if (light != nullptr) {
        light->setAmbientDiffuseSpecularIntensity(curAmb, curDif, curSpec, curIntensity);
        light->setPosition(curPos);
    }
}

void LightTo::startWithTarget(Node* pTarget)
{
    ActionInterval::startWithTarget(pTarget);

    Light* light = dynamic_cast<Light*>(m_pTarget);

    if (light != nullptr) {
        m_fromAmbience = light->getAmbient();
        m_fromDiffuse = light->getDiffuse();
        m_fromSpecular = light->getSpecular();
        m_fromPosition = light->getPosition3D();
        m_fromIntensity = light->getIntensity();
    }
}
