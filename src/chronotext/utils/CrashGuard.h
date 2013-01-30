#pragma once

#include "cinder/Thread.h"

/*
 * TODO: TEST ON WINDOWS
 */

class CrashGuard
{
    std::shared_ptr<std::thread> mThread;
    int mCount;

    void run();
    
public:
    CrashGuard();
    void update();
};