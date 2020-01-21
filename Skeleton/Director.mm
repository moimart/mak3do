//
//  Director.cpp
//  Skeleton
//
//  Created by Moises Martinez on 20.01.20.
//  Copyright © 2020 Moises Martinez. All rights reserved.
//

#include "Director.h"
#include <iostream>
#import "SceneRenderer.h"

namespace mak3do {

Director* Director::get()
{
    static Director* instance = nullptr;
    
    if (instance == nullptr) {
        instance = new Director();
    }
    
    return instance;
}

void Director::loop()
{
    [[SceneRenderer shared] render];
}

}
