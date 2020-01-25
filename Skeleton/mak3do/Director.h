//
//  Director.hpp
//  Skeleton
//
//  Created by Moises Martinez on 20.01.20.
//  Copyright © 2020 Moises Martinez. All rights reserved.
//

#pragma once

#include <stdio.h>
#include "types.h"

namespace mak3do {

class Director {
public:
    static Director* get();
    void loop();
    
    void setScene(ScenePtr scene);
    
protected:
    Director();
    
private:
    DirectorImplPtr m_pimpl;
};
}

