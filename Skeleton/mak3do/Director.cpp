//
//  Director.cpp
//  Skeleton
//
//  Created by Moises Martinez on 20.01.20.
//  Copyright © 2020 Moises Martinez. All rights reserved.
//

#include "Director.h"
#include "ActionRunner.h"

#ifdef M3D_PLAT_APPLE
#include "apple/DirectorImpl.h"
#elif M3D_PLAT_ANDROID
#include "android/DirectorImpl.h"
#endif

namespace mak3do {

Director* Director::get()
{
    static Director* instance = nullptr;
    
    if (instance == nullptr) {
        instance = new Director();
    }
    
    return instance;
}

Director::Director()
: m_pimpl(std::make_shared<DirectorImpl>(this))
, m_action_runner(std::make_shared<ActionRunner>())
{
}

static int gettimeofday(struct _timeval* tp, void* tzp)
{
    if (tp) {
        gettimeofday((struct timeval*)tp, 0);
    }
    return 0;
}

void Director::deltaTime()
{
    struct _timeval now;

    if (mak3do::gettimeofday(&now, NULL) != 0) {
        m_dt = 0;
        return;
    }

    // new delta time. Re-fixed issue #1277
    if (m_next_dt) {
        m_dt = 0;
        m_next_dt = false;
    } else {
        m_dt = (now.tv_sec - m_last_update.tv_sec) + (now.tv_usec - m_last_update.tv_usec) / 1000000.0f;
        m_dt = MAX(0, m_dt);
    }
    
    m_last_update = now;
}

void Director::loop()
{
    deltaTime();
    
    m_action_runner->update(m_dt);
    m_pimpl->loop();
}

ActionRunnerPtr Director::defaultActionRunner() const
{
    return m_action_runner;
}

void Director::setScene(ScenePtr scene)
{
    m_pimpl->setScene(scene);
}
}
