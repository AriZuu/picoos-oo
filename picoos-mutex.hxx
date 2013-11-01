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

#if (DOX!=0) || (POSCFG_FEATURE_MUTEXES != 0)
/**
 * Mutexes are used for task synchronization. A source code
 * area that is protected by a mutex can only be executed by
 * one task at the time. The mechanism is comparable with
 * a semaphore that is initialized with a counter of 1.
 * A mutex is a special semaphore, that allows a
 * task having the mutex locked can execute the mutex lock
 * functions again and again without being blocked
 * (this is called reentrancy).
 */
  class Mutex
  {
  public:

/* 
 * Constructors.
 */
    inline Mutex()
    {
#ifdef _DBG
      handle = (POSMUTEX_t)0;
#endif
    };

    inline Mutex(const Mutex& other)
    {
      handle = other.handle;
    };

    inline Mutex(const POSMUTEX_t other)
    {
      handle = other;
    };

/**
 * Allocates a new mutex object.
 * @return  mutex creation status. -1 is returned when the
 *          mutex could not be created.
 * @note    ::POSCFG_FEATURE_MUTEXES must be defined to 1 
 *          to have mutex support compiled in.
 * @sa      ::posMutexCreate, destroy, lock, tryLock, unlock
 */
    inline VAR_t create()
    {
      handle = ::posMutexCreate();
      return (handle == NULL) ? -1 : 0;
    }

#if (DOX!=0) || (POSCFG_FEATURE_MUTEXDESTROY != 0)
/**
 * Frees a no more needed mutex object.
 * @note    ::POSCFG_FEATURE_MUTEXES must be defined to 1 
 *          to have mutex support compiled in.@n
 *          ::POSCFG_FEATURE_MUTEXDESTROY must be defined to 1
 *          to have this function compiled in.
 * @sa      ::posMutexDestroy, create
 */
    inline void destroy()
    {
      ::posMutexDestroy(handle);
    }
#endif

#if (DOX!=0) || (POSCFG_FEATURE_MUTEXTRYLOCK != 0)
/**
 * Tries to get the mutex lock. This function does not block when the
 * mutex is not available, instead it returns a value showing that
 * the mutex could not be locked.
 * @return  zero when the mutex lock could be set. Otherwise, when
 *          the mutex lock is yet helt by an other task, the function
 *          returns 1. A negative value is returned on error.
 * @note    ::POSCFG_FEATURE_MUTEXES must be defined to 1 
 *          to have mutex support compiled in.@n
 *          ::POSCFG_FEATURE_MUTEXTRYLOCK must be defined to 1
 *          to have this function compiled in.
 * @sa      ::posMutexTryLock, lock, unlock, create
 */
    inline VAR_t tryLock()
    {
      return ::posMutexTryLock(handle);
    }
#endif

/**
 * This function locks a code section so that only one task can execute
 * the code at a time. If an other task already has the lock, the task
 * requesting the lock will be blocked until the mutex is unlocked again.
 * Note that a ::posMutexLock appears always in a pair with ::posMutexUnlock.
 * @return  zero on success.
 * @note    ::POSCFG_FEATURE_MUTEXES must be defined to 1 
 *          to have mutex support compiled in.
 * @sa      ::posMutexLock, tryLock, unlock, create
 */
    inline VAR_t lock()
    {
      return ::posMutexLock(handle);
    }

/**
 * This function unlocks a section of code so that other tasks
 * are able to execute it.
 * @return  zero on success.
 * @note    ::POSCFG_FEATURE_MUTEXES must be defined to 1 
 *          to have mutex support compiled in.
 * @sa      ::posMutexUnlock, lock, tryLock, create
 */
    inline VAR_t unlock()
    {
      return ::posMutexUnlock(handle);
    }

/*
 * Assignment operators.
 */
    inline Mutex& operator=(const Mutex& other)
    {
      handle = other.handle;
      return *this;
    };

    inline Mutex& operator=(const POSMUTEX_t other)
    {
      handle = handle;
      return *this;
    };

  private:
    POSMUTEX_t handle;
  };

#endif /* POSCFG_FEATURE_MUTEXES */
}

#endif /* _PICOOS_MUTEX_HXX */
