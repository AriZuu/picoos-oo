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
 * @file    picoos-mutex.hxx
 * @brief   Pico]OS oo-wrapper Mutex
 * @author  Ari Suutari <ari@stonepile.fi>
 */

#ifndef _PICOOS_MUTEX_HXX
#define _PICOOS_MUTEX_HXX

extern "C" {

#include <picoos.h>

}

namespace pos {

/**
 * xxxxxxx
 */
  class ClassName
  {
  public:

/* 
 * Constructors.
 */
    inline ClassName()
    {
#ifdef _DBG
      handle = (POSTASK_t)0;
#endif
    };

    inline ClassName(const ClassName& other)
    {
      handle = other.handle;
    };

    inline ClassName(const POSTASK_t other)
    {
      handle = other;
    };

#if (DOX!=0) || (POSCFG_FEATURE_MUTEXES != 0)
/** @defgroup mutex Mutex Functions
 * @ingroup userapip
 * Mutexes are used for task synchronization. A source code
 * area that is protected by a mutex can only be executed by
 * one task at the time. The mechanism is comparable with
 * a semaphore that is initialized with a counter of 1.
 * A mutex is a special semaphore, that allows a
 * task having the mutex locked can execute the mutex lock
 * functions again and again without being blocked
 * (this is called reentrancy).
 * @{
 */

/**
 * Mutex function.
 * Allocates a new mutex object.
 * @return  the pointer to the new mutex object. NULL is returned on error.
 * @note    ::POSCFG_FEATURE_MUTEXES must be defined to 1 
 *          to have mutex support compiled in.
 * @sa      posMutexDestroy, posMutexLock, posMutexTryLock, posMutexUnlock
 */
    inline POSMUTEX_t posMutexCreate(void);

#if (DOX!=0) || (POSCFG_FEATURE_MUTEXDESTROY != 0)
/**
 * Mutex function.
 * Frees a no more needed mutex object.
 * @param   mutex  handle to the mutex object.
 * @note    ::POSCFG_FEATURE_MUTEXES must be defined to 1 
 *          to have mutex support compiled in.@n
 *          ::POSCFG_FEATURE_MUTEXDESTROY must be defined to 1
 *          to have this function compiled in.
 * @sa      posMutexCreate
 */
    inline void posMutexDestroy(POSMUTEX_t mutex);
#endif

#if (DOX!=0) || (POSCFG_FEATURE_MUTEXTRYLOCK != 0)
/**
 * Mutex function.
 * Tries to get the mutex lock. This function does not block when the
 * mutex is not available, instead it returns a value showing that
 * the mutex could not be locked.
 * @param   mutex  handle to the mutex object.
 * @return  zero when the mutex lock could be set. Otherwise, when
 *          the mutex lock is yet helt by an other task, the function
 *          returns 1. A negative value is returned on error.
 * @note    ::POSCFG_FEATURE_MUTEXES must be defined to 1 
 *          to have mutex support compiled in.@n
 *          ::POSCFG_FEATURE_MUTEXTRYLOCK must be defined to 1
 *          to have this function compiled in.
 * @sa      posMutexLock, posMutexUnlock, posMutexCreate
 */
    inline VAR_t posMutexTryLock(POSMUTEX_t mutex);
#endif

/**
 * Mutex function.
 * This function locks a code section so that only one task can execute
 * the code at a time. If an other task already has the lock, the task
 * requesting the lock will be blocked until the mutex is unlocked again.
 * Note that a ::posMutexLock appears always in a pair with ::posMutexUnlock.
 * @param   mutex  handle to the mutex object.
 * @return  zero on success.
 * @note    ::POSCFG_FEATURE_MUTEXES must be defined to 1 
 *          to have mutex support compiled in.
 * @sa      posMutexTryLock, posMutexUnlock, posMutexCreate
 */
    inline VAR_t posMutexLock(POSMUTEX_t mutex);

/**
 * Mutex function.
 * This function unlocks a section of code so that other tasks
 * are able to execute it.
 * @param   mutex  handle to the mutex object.
 * @return  zero on success.
 * @note    ::POSCFG_FEATURE_MUTEXES must be defined to 1 
 *          to have mutex support compiled in.
 * @sa      posMutexLock, posMutexTryLock, posMutexCreate
 */
    inline VAR_t posMutexUnlock(POSMUTEX_t mutex);

#endif /* POSCFG_FEATURE_MUTEXES */

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

  private:
    POSTASK_t handle;
  };
}

#endif /* _PICOOS_MUTEX_HXX */
