//
//  Director.cpp
//  Skeleton
//
//  Created by Moises Martinez on 20.01.20.
//  Copyright © 2020 Moises Martinez. All rights reserved.
//

#include "Director.h"

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
{
}

void Director::loop()
{
    m_pimpl->loop();
}

void Director::setScene(ScenePtr scene)
{
    
}
}
