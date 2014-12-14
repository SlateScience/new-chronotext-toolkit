/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * REFERENCE VERSION: https://github.com/arielm/new-chronotext-toolkit/blob/ContextRework/src/chronotext/os/TaskManager.h
 */

#pragma once

#include "chronotext/os/Task.h"

#include <map>
#include <set>
#include <queue>

#include <boost/asio.hpp>

namespace chronotext
{
    class TaskManager : public std::enable_shared_from_this<TaskManager>
    {
    public:
        static int MAX_CONCURRENT_THREADS;

        TaskManager(boost::asio::io_service &io);
        
        /*
         * RETURNS NULL IF:
         *
         * - NOT INVOKED ON THE IO-THREAD
         * - NO TASK IS REGISTERED WITH THIS ID
         *
         * NOTE: THE RETURNED POINTER IS NOT INTENDED FOR STORAGE
         */
        Task* getTask(int taskId);
        
        /*
         * RETURNS 0 IF THE TASK CAN'T BE REGISTERED
         *
         * CAUSES:
         *
         * - NOT INVOKED ON THE IO-THREAD
         * - THE TASK IS ALREADY REGISTERED (TASKS ARE NOT REUSABLE)
         * - Task::init() RETURNED FALSE
         */
        int registerTask(std::shared_ptr<Task> task);
        
        /*
         * RETURNS FALSE IF THE TASK CAN'T BE ADDED
         *
         * CAUSES:
         *
         * - NOT INVOKED ON THE IO-THREAD
         * - NO TASK IS REGISTERED WITH THIS ID
         * - THE TASK HAS ALREADY BEEN ADDED
         * - IO-SERVICE IS NOT DEFINED
         */
        bool addTask(int taskId, bool forceSync = false);
        
        inline bool addTask(std::shared_ptr<Task> &&task, bool forceSync = false)
        {
            int taskId = registerTask(std::forward<std::shared_ptr<Task>>(task));
            return taskId ? addTask(taskId, forceSync) : false;
        }
        
        /*
         * RETURNS FALSE IF THE TASK CAN'T BE CANCELLED (OR REMOVED IF NOT YET STARTED)
         *
         * CAUSES:
         *
         * - NOT INVOKED ON THE IO-THREAD
         * - NO TASK IS REGISTERED WITH THIS ID
         *
         * ASYNCHRONOUS TASKS ONLY:
         * - THE TASK IS ALREADY AWAITING CANCELLATION
         */
        bool cancelTask(int taskId);
        
    protected:
        boost::asio::io_service &io;

        std::thread::id threadId;
        int taskCount;
        
        std::map<int, std::shared_ptr<Task>> tasks;
        std::set<int> startedTasks;
        std::set<int> postponedTasks;
        std::queue<int> taskQueue;
        
        bool isThreadSafe();
        
    private:
        friend class Task;
        
        TaskManager();
        TaskManager(const TaskManager &other) = delete;
        
        /*
         * RETURNS FALSE IF THE LAMBDA CAN'T BE POSTED
         *
         * CAUSES:
         *
         * - IO-SERVICE IS NOT DEFINED
         * - THE CONTEXT IS BEING SHUT-DOWN (TODO)
         *
         * SYNCHRONOUS TASKS ONLY:
         * - NOT INVOKED ON THE IO-THREAD
         */
        bool post(std::function<void()> &&fn, bool forceSync = false);
        
        void endTask(int taskId);
        void nextTask();
    };
}
