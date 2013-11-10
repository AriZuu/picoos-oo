/*
 * Copyright (c) 2013, Ari Suutari <ari@stonepile.fi>.
 * Original documentation text Copyright (c) 2004-2012, Dennis Kuschel.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote
 *     products derived from this software without specific prior written
 *     permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT,  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    picoos-task.hxx
 * @brief   Pico]OS oo-wrapper Task
 * @author  Ari Suutari <ari@stonepile.fi>
 */

#ifndef _PICOOS_TASK_HXX
#define _PICOOS_TASK_HXX

extern "C" {

#include <picoos.h>

}

namespace pos {

/**
 * Task Control
 */
  class Task
  {
  public:

/* 
 * Constructors.
 */
    inline Task()
    {
#ifdef _DBG
      handle = (POSTASK_t)0;
#endif
    };

    inline Task(const Task& other)
    {
      handle = other.handle;
    };

    inline Task(const POSTASK_t other)
    {
      handle = other;
    };

#if (DOX!=0) || (POSCFG_FEATURE_YIELD != 0)
/**
 * This function can be called to give off processing time so other tasks
 * ready to run will be scheduled (= cooparative multitasking).
 * @note    ::POSCFG_FEATURE_YIELD must be defined to 1
 *          to have this function compiled in.
 * @sa      ::posTaskYield, sleep
 */
    static inline void yield(void)
    {
      ::posTaskYield();
    };

#endif

#if (DOX!=0) || (POSCFG_FEATURE_SLEEP != 0)
/**
 * Delay task execution for a couple of timer ticks.
 * @param   ticks  delay time in timer ticks
 *          (see ::HZ define and ::MS macro)
 * @note    ::POSCFG_FEATURE_SLEEP must be defined to 1
 *          to have this function compiled in.@n
 *          It is not guaranteed that the task will proceed
 *          execution exactly when the time has elapsed.
 *          A higher priorized task or a task having the same
 *          priority may steal the processing time.
 *          Sleeping a very short time is inaccurate.
 * @sa      ::posTaskSleep, yield, HZ, MS
 */
    static inline void sleep(UINT_t ticks)
    {
      ::posTaskSleep(ticks);
    };

#endif

#if (DOX!=0) || (POSCFG_TASKSTACKTYPE == 0)
/**
 * Creates a new task. The stack memory is managed by the user.
 * @param   funcptr     pointer to the function that shall be executed
 *                      by the new task.
 * @param   funcarg     optional argument passed to function.
 * @param   priority    task priority. Must be in the range
 *                      0 .. ::POSCFG_MAX_PRIO_LEVEL - 1.
 *                      The higher the number, the higher the priority.
 * @param   stackstart  pointer to the stack memory for the new task.
 *          task could not be created.
 * @return  task creation status. -1 is returned when the
 *          task could not be created.
 * @note    ::POSCFG_TASKSTACKTYPE <b>must be defined to 0</b>
 *          to have this format of the function compiled in.
 * @sa      ::posTaskCreate, exit
 */
    inline VAR_t create(POSTASKFUNC_t funcptr, void *funcarg,
                        VAR_t priority, void *stackstart)
    {
      handle = ::posTaskCreate(funcptr, funcarg, priority, stackstart);
      return (handle == NULL) ? -1 : 0;
    };

#ifndef POSNANO
/**
 * Operating System Initialization.
 * This function initializes the operating system and starts the
 * first tasks: The idle task and the first user task.
 * @param   firstfunc   pointer to the first task function that
 *                      will run in the multitasking environment.
 * @param   funcarg     optional argument passed to the first task.
 * @param   priority    task priority. Must be in the range
 *                      0 .. ::POSCFG_MAX_PRIO_LEVEL - 1.
 *                      The higher the number, the higher the priority.
 * @param   stackFirstTask  pointer to the stack memory for the first task.
 * @param   stackIdleTask   pointer to the stack memory for the idle task.
 * @note    ::POSCFG_TASKSTACKTYPE <b>must be defined to 0</b>
 *          to have this format of the function compiled in.
 * @sa      ::posInit
 */
    static inline void init(POSTASKFUNC_t firstfunc, void *funcarg,
                            VAR_t priority,
                            void *stackFirstTask, void *stackIdleTask)
    {
      ::posInit(firstfunc, funcarg, priority, stackFirstTask, stackIdleTask);
    };

#endif
#endif
#if (DOX!=0) || (POSCFG_TASKSTACKTYPE == 1)
/**
 * Creates a new task. The stack memory is managed by the achitecture
 * specific portion of the operating system, the size can be set by the user.
 * @param   funcptr     pointer to the function that shall be executed
 *                      by the new task.
 * @param   funcarg     optional argument passed to function.
 * @param   priority    task priority. Must be in the range
 *                      0 .. ::POSCFG_MAX_PRIO_LEVEL - 1.
 *                      The higher the number, the higher the priority.
 * @param   stacksize   size of the stack memory. This parameter is
 *                      passed to the architecture specific portion of
 *                      the operating system.
 * @return  task creation status. -1 is returned when the
 *          task could not be created.
 * @note    ::POSCFG_TASKSTACKTYPE <b>must be defined to 1</b>
 *          to have this format of the function compiled in.
 * @sa      ::posTaskCreate, exit
 */
    inline VAR_t create(POSTASKFUNC_t funcptr, void *funcarg,
                        VAR_t priority, UINT_t stacksize)
    {
      handle = ::posTaskCreate(funcptr, funcarg, priority, stacksize);
      return (handle == NULL) ? -1 : 0;
    };

#ifndef POSNANO
/**
 * Operating System Initialization.
 * This function initializes the operating system and starts the
 * first tasks: The idle task and the first user task.
 * @param   firstfunc   pointer to the first task function that
 *                      will run in the multitasking environment.
 * @param   funcarg     optional argument passed to the first task.
 * @param   priority    task priority. Must be in the range
 *                      0 .. ::POSCFG_MAX_PRIO_LEVEL - 1.
 *                      The higher the number, the higher the priority.
 * @param   taskStackSize  size of the stack memory for the first task.
 * @param   idleStackSize  size of the stack memory for the idle task.
 * @note    ::POSCFG_TASKSTACKTYPE <b>must be defined to 1</b>
 *          to have this format of the function compiled in.
 * @sa      posInit
 */
    static inline void init(POSTASKFUNC_t firstfunc, void *funcarg,
                            VAR_t priority, UINT_t taskStackSize,
                            UINT_t idleStackSize)
    {
      ::posInit(firstfunc, funcarg, priority, taskStackSize, idleStackSize);
    };

#endif
#endif
#if (DOX!=0) || (POSCFG_TASKSTACKTYPE == 2)
/**
 * Task function.
 * Creates a new task. The stack memory is fixed, its size is set by
 * the architecture specific portion of the operating system.
 * @param   funcptr     pointer to the function that shall be executed
 *                      by the new task.
 * @param   funcarg     optional argument passed to function.
 * @param   priority    task priority. Must be in the range
 *                      0.. ::POSCFG_MAX_PRIO_LEVEL - 1.
 *                      The higher the number, the higher the priority.
 * @return  task creation status. -1 is returned when the
 *          task could not be created.
 * @note    ::POSCFG_TASKSTACKTYPE <b>must be defined to 2</b>
 *          to have this format of the function compiled in.
 * @sa      ::posTaskCreate, exit
 */
    inline VAR_t create(POSTASKFUNC_t funcptr, void *funcarg,
                        VAR_t priority)
    {
      handle = ::posTaskCreate(funcptr, funcarg, priority);
      return (handle == NULL) ? -1 : 0;
    };

#ifndef POSNANO
/**
 * Operating System Initialization.
 * This function initializes the operating system and starts the
 * first tasks: The idle task and the first user task.
 * @param   firstfunc   pointer to the first task function that
 *                      will run in the multitasking environment.
 * @param   funcarg     optional argument passed to the first task.
 * @param   priority    task priority. Must be in the range
 *                      0 .. ::POSCFG_MAX_PRIO_LEVEL - 1.
 *                      The higher the number, the higher the priority.
 * @note    ::POSCFG_TASKSTACKTYPE <b>must be defined to 2</b>
 *          to have this format of the function compiled in.
 * @sa      posInit
 */
    static inline void init(POSTASKFUNC_t firstfunc, void *funcarg,
                            VAR_t priority)
    {
      ::posInit(firstfunc, funcarg, priority);
    };

#endif
#endif


#if (DOX!=0) || (POSCFG_FEATURE_EXIT != 0)
/**
 * Terminate execution of a task.
 * @note    ::POSCFG_FEATURE_EXIT must be defined to 1 
 *          to have this function compiled in.
 * @sa      ::posTaskExit, create
 */
    static inline void exit(void)
    {
      ::posTaskExit();
    };

#endif

#if (DOX!=0) || (POSCFG_FEATURE_GETTASK != 0)
/**
 * Get the currently running task.
 * @return  the task.
 * @note    ::POSCFG_FEATURE_GETTASK must be defined to 1 
 *          to have this function compiled in.
 * @sa      ::posTaskGetCurrent, create, setPriority
 */
    static inline Task getCurrent(void)
    {
      return ::posTaskGetCurrent();
    };

#endif

#if (DOX!=0) || (POSCFG_FEATURE_TASKUNUSED != 0)
/**
 * Tests if a task is yet in use by the operating system.
 * This function can be used to test if a task has been
 * fully terminated (and the stack memory is no more in use).
 * @return  1 (=true) when the task is unused. If the task
 *          is still in use, zero is returned.
 *          A negative value is returned on error.
 * @note    ::POSCFG_FEATURE_TASKUNUSED must be defined to 1 
 *          to have this function compiled in.
 * @sa      ::posTaskUnused, create, exit
 */
    inline VAR_t unused()
    {
      return ::posTaskUnused(handle);
    };

#endif

#if (DOX!=0) || (POSCFG_FEATURE_SETPRIORITY != 0)
/**
 * Change the priority of a task. Note that in a non-roundrobin
 * scheduling environment every priority level can only exist once.
 * @param   priority    new priority. Must be in the range
 *                      0 .. ::POSCFG_MAX_PRIO_LEVEL - 1.
 *                      The higher the number, the higher the priority.
 * @return  zero on success.
 * @note    ::POSCFG_FEATURE_SETPRIORITY must be defined to 1 
 *          to have this function compiled in.
 * @sa      ::posTaskSetPriority, getPriority, getCurrent, create
 */
    inline VAR_t setPriority(VAR_t priority)
    {
      return ::posTaskSetPriority(handle, priority);
    };

#endif

#if (DOX!=0) || (POSCFG_FEATURE_GETPRIORITY != 0)
/**
 * Get the priority of a task.
 * @return  the priority of the task. A negative value is returned on error.
 * @note    ::POSCFG_FEATURE_GETPRIORITY must be defined to 1 
 *          to have this function compiled in.
 * @sa      ::posTaskGetPriority, setPriority, getCurrent, create
 */
    inline VAR_t getPriority()
    {
      return ::posTaskGetPriority(handle);
    };

#endif

#if (DOX!=0) || (POSCFG_FEATURE_INHIBITSCHED != 0)
/**
 * Locks the scheduler. When this function is called, no task switches
 * will be done any more, until the counterpart function ::posTaskSchedUnlock
 * is called. This function is usefull for short critical sections that
 * require exclusive access to variables. Note that interrupts still
 * remain enabled.
 * @note    ::POSCFG_FEATURE_INHIBITSCHED must be defined to 1 
 *          to have this function compiled in.
 * @sa      ::posTaskSchedLock, schedUnlock
 */
    static inline void schedLock(void)
    {
      ::posTaskSchedLock();
    };

/**
 * Unlocks the scheduler. This function is called to leave a critical section.
 * If a context switch request is pending, the context switch will happen
 * directly after calling this function.
 * @note    ::POSCFG_FEATURE_INHIBITSCHED must be defined to 1 
 *          to have this function compiled in.
 * @sa      ::posTaskSchedUnlock, schedLock
 */
    static inline void schedUnlock(void)
    {
      ::posTaskSchedUnlock();
    };

#endif

#if (DOX!=0) || (POSCFG_TASKCB_USERSPACE > 0)
/**
 * Returns a pointer to the user memory in the current task control block.
 * @note    ::POSCFG_TASKCB_USERSPACE must be defined to a nonzero value
 *          to have this function compiled in. ::POSCFG_TASKCB_USERSPACE
 *          is also used to set the size of the user memory (in bytes).
 * @return  pointer to user memory space.
 * @sa      ::posTaskGetUserspace
 */
    static inline void* getUserspace(void)
    {
      return ::posTaskGetUserspace();
    };

#endif

#if (DOX!=0) || (POSCFG_FEATURE_IDLETASKHOOK != 0)
/**
 * Install or remove an optional idle task hook function.
 * The hook function is called every time the system is idle.
 * It is possible to use this hook to implement your own idle task;
 * in this case the function does not need to return to the system.
 * You may insert a call to ::posTaskYield into your idle task loop
 * to get a better task performance.
 * @param   idlefunc  function pointer to the new idle task handler.
 *                    If this parameter is set to NULL, the idle
 *                    task function hook is removed again.
 * @return  This function may return a pointer to the last hook
 *          function set. If so (pointer is not NULL), the previous
 *          hook function should be called from within your
 *          idle task hook. This enables chaining of hook functions.
 * @note    ::POSCFG_FEATURE_IDLETASKHOOK must be defined to 1 
 *          to have this function compiled in.
 * @sa      ::posInstallIdleTaskHook
 */
    static inline POSIDLEFUNC_t installIdleTaskHook(POSIDLEFUNC_t idlefunc)
    {
      return ::posInstallIdleTaskHook(idlefunc);
    };

#endif

/**
 * Assign a name to a pico]OS task.
 *
 * This is useful when you are debugging your application by
 * using the global tasklist that is referenced by the variable
 * ::picodeb_tasklist. It is easier to navigate through
 * the list when the tasks have names. Examples:
 *
 * @code
 *   Task task;
 *   task.create(workertaskfunc, NULL, 2);
 *   task.setName("worker task");
 * @endcode
 *
 *  @note It is not necessary to use this macro when you are
 *       using the function ::nosTaskCreate to start a new task.
 *
 * @sa ::POS_SETTASKNAME
 */
    inline void setName(const char* name)
    {
      POS_SETTASKNAME(handle, name);
    };

/*
 * Assignment operators.
 */
    inline Task& operator=(const Task& other)
    {
      handle = other.handle;
      return *this;
    };

    inline Task& operator=(const POSTASK_t other)
    {
      handle = handle;
      return *this;
    };

    inline operator POSTASK_t() const
    {
      return handle;
    }

  protected:
    POSTASK_t handle;
  };
}

#if (DOX!=0) || POSCFG_ENABLE_NANO != 0

namespace nos {

/**
 * Task Control
 */
  class Task : public pos::Task
  {
  public:

/* 
 * Constructors.
 */
    inline Task()
    {
    };

    inline Task(const Task& other) : pos::Task(other)
    {
    };

    inline Task(const POSTASK_t other) : pos::Task(other)
    {
    };

#if (DOX!=0) || (NOSCFG_FEATURE_TASKCREATE != 0)
/**
 * Creates a new task.
 * @param   funcptr    pointer to the function that shall be executed
 *                     by the new task.
 * @param   funcarg    optional argument passed to function funcptr.
 * @param   priority   task priority. Must be in the range
 *                     0 .. ::POSCFG_MAX_PRIO_LEVEL - 1.
 *                     The higher the number, the higher the priority.
 * @param   stacksize  Size of the stack memory. If set to zero,
 *                     a default stack size is assumed
 *                     (see define ::NOSCFG_DEFAULT_STACKSIZE).
 * @param   name       Name of the new task to create. If the last character
 *                     in the name is an asteriks (*), the operating system
 *                     automatically assigns the task a unique name (the
 *                     registry feature must be enabled for this automatism).
 *                     This parameter can be NULL if the nano layer registry
 *                     feature is not used and will not be used in future.
 * @return  task creation status. -1 is returned when the
 *          task could not be created.
 * @note    ::NOSCFG_FEATURE_TASKCREATE must be defined to 1
 *          to have this function compiled in.
 * @sa      ::nosTaskCreate, exit
 */
    inline VAR_t create(POSTASKFUNC_t funcptr,
                        void *funcarg,
                        VAR_t priority,
                        UINT_t stacksize,
                        const char* name)
    {
      handle = ::nosTaskCreate(funcptr, funcarg, priority, stacksize, name);
      return (handle == NULL) ? -1 : 0;
    };

    using pos::Task::create;

#endif

  };
}

#endif /* POSCFG_ENABLE_NANO */
#endif /* _PICOOS_TASK_HXX */
