/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/os/TaskManager.h"

#include <boost/asio.hpp>

namespace chr
{
    namespace CONTEXT
    {
        void setup(boost::asio::io_service &io_service);
        void shutdown();
    }
}

namespace context
{
    chr::TaskManager& taskManager();
    
    namespace os
    {
        /*
         * RETURNS FALSE IF NOT INVOKED ON THE SKETCH-THREAD
         */
        bool isThreadSafe();
        
        /*
         * RETURNS FALSE IF THE FUNCTION CAN'T BE POSTED
         *
         * CAUSES:
         *
         * - IO-SERVICE IS NOT DEFINED
         * - THE CONTEXT IS BEING SHUT-DOWN (TODO)
         *
         * SYNCHRONOUS POSTING ONLY:
         * - NOT INVOKED ON THE SKETCH-THREAD
         */
        bool post(std::function<void()> &&fn, bool forceSync = false);
    }
};
