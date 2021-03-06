#include <mak3do/scenegraph/Director.h>
#include <mak3do/scenegraph/ActionRunner.h>
#include <mak3do/scenegraph/Scheduler.h>

#ifdef M3D_PLAT_APPLE
#include <mak3do/scenegraph/apple/DirectorImpl.h>
#elif M3D_PLAT_ANDROID
#include <mak3do/scenegraph/android/DirectorImpl.h>
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

static int gettimeofday(struct _timeval* tp, void* tzp)
{
    if (tp) {
        gettimeofday((struct timeval*)tp, 0);
    }
    return 0;
}

Director::Director()
: m_pimpl(std::make_shared<DirectorImpl>(this))
, m_action_runner(std::make_shared<ActionRunner>())
, m_scheduler(std::make_shared<Scheduler>())
{
    struct _timeval now;

    if (mak3do::gettimeofday(&now, nullptr) != 0) {
        m_dt = 0;
        return;
    }
    
    m_last_update = now;
}

float Director::delta() const
{
    return m_dt;
}

void Director::dt()
{
    struct _timeval now;

    if (mak3do::gettimeofday(&now, nullptr) != 0) {
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
    dt();
    
    m_scheduler->update(m_dt);
    m_action_runner->update(m_dt);
    m_pimpl->loop(m_dt);
    m_scheduler->cleanup();
}

ActionRunnerPtr Director::action_runner() const
{
    return m_action_runner;
}

SchedulerPtr Director::scheduler() const
{
    return m_scheduler;
}

void Director::scene(ScenePtr scene)
{
    m_pimpl->setScene(scene);
}
}
