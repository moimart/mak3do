//
//  Director.hpp
//  Skeleton
//
//  Created by Moises Martinez on 20.01.20.
//  Copyright © 2020 Moises Martinez. All rights reserved.
//

#pragma once

#include <stdio.h>

namespace mak3do {
class Director {
public:
    static Director* get();
    void loop();
};
}

