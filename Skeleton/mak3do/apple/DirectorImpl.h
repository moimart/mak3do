#pragma once

#include "../types.h"

namespace mak3do {

class DirectorImpl {
public:
    DirectorImpl(mak3do::Director* parent);
    
    void loop();
private:
    Director* m_parent;
};

}


