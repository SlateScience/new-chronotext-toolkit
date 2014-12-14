/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * REFERENCE VERSION: https://github.com/arielm/new-chronotext-toolkit/blob/ContextRework/src/chronotext/os/Task.h
 */

#pragma once

#include "cinder/Thread.h"

#include <boost/thread/mutex.hpp>

namespace chronotext
{
    class TaskManager;
    
    class Task
    {
    public:
        static const bool VERBOSE;
        
        struct State
        {
            bool initialized;
            bool started;
            bool ended;
            bool cancelRequired;
            
            State();
        };
        
        virtual bool init() { return true; }
        virtual void shutdown() {}
        virtual void run() = 0;
        
        int getId() const;
        
        bool hasStarted();
        bool hasEnded();
        bool isCancelRequired();
        
        void sleep(float milliseconds);
        
    protected:
        State state;
        
        TaskManager *manager;
        int taskId;
        bool synchronous;
        
        std::thread _thread;
        boost::mutex _mutex;
        
        virtual ~Task();
        
        bool post(std::function<void()> &&fn);
        
    private:
        friend class TaskManager;
        
        void start(bool forceSync = false);
        void cancel();
        void detach();
        
        bool performInit(TaskManager *manager, int taskId);
        void performShutdown();
        void performRun();
    };
}

namespace chr = chronotext;
