#pragma once

#include "types.h"
#include "../Node.h"

namespace mak3do {

class Action : public std::enable_shared_from_this<Action> {
public:
    virtual ~Action() = default;

    std::string description() const;

    virtual bool done(void);
    virtual void start(NodePtr pTarget);
    virtual void stop();

    virtual void step(float dt);
    virtual void update(float time);

    inline NodePtr target(void) const { return m_target; }
    inline void target(NodePtr pTarget) { m_target = pTarget; }

    inline NodePtr original_target(void) const { return m_original_target; }
    inline void original_target(NodePtr pOriginalTarget) { m_original_target = pOriginalTarget; }

    inline int tag(void) const { return m_nTag; }
    inline void tag(int nTag) { m_nTag = nTag; }

public:
    static ActionPtr make();

protected:
    NodePtr m_original_target { nullptr };
    NodePtr m_target { nullptr };
    int m_nTag;
};



class FiniteTimeAction : public Action {
public:
    virtual ~FiniteTimeAction() {}
    inline float duration() const { return m_fDuration; }
    inline void duration(float duration) { m_fDuration = duration; }
   virtual ActionPtr reverse(void);

protected:
    float m_fDuration { 0 };
};

}
