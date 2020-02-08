#pragma once

#include "../types.h"

namespace mak3do {
class LightImpl {
public:
    LightImpl(Light* parent);
private:
    Light* m_abstract;
    void* m_native;
};
}
