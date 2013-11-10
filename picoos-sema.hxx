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

#ifndef _PICOOS_SEMA_HXX
#define _PICOOS_SEMA_HXX

extern "C" {

#include <picoos.h>

}

#if (DOX!=0) || (POSCFG_FEATURE_SEMAPHORES != 0)

namespace pos {

/**
 * Semaphores are basically used for task synchronization.
 * Task synchronization means that only a defined number of tasks can
 * execute a piece of code. Usually, a semaphore is initialized with
 * the value 1, so only one task can hold the semaphore at a time
 * (Please read the chapter about the mutex functions also if you
 * are interested in task synchronization).@n
 * The second thing semaphores can be used for is sending signals
 * to waiting tasks. Imagine you have an interrupt service routine
 * that is triggered every time when a big chunk of data is available
 * on a device. The data is to big to process them directly in the ISR.
 * The ISR will only trigger a semaphore (it will signalize the semaphore),
 * and a high priorized task waiting for the semaphore will be set to
 * running state and will process the data from the device. In this
 * case, the semaphore would be initialized with zero when it is created.
 * The first task requesting the semaphore would block immediately, and
 * can only proceed its work when the semaphore is triggered from outside.@n
 * 
 * Semaphores are implemented as counters. A task requesting a semaphore
 * (via ::pos::Sema::get or ::pos::Sema::wait) will decrement the counter. If the
 * counter is zero, the task willing to decrement the counter is blocked.
 * When a semaphore is signaled (via ::pos::Sema::signal), the counter is
 * incremented. If the counter reaches a positive, nonzero value,
 * the highest priorized task pending on the semaphore is set to
 * running state and can decrement the counter by itself.
 */
  class Sema 
  {
  public:

/* 
 * Constructors.
 */
    inline Sema()
    {
#ifdef _DBG
      handle = (POSSEMA_t)0;
#endif
    };

    inline Sema(const Sema& other)
    {
      handle = other.handle;
    };

    inline Sema(const POSSEMA_t other)
    {
      handle = other;
    };
/**
 * Allocates a new semaphore object.
 * @param   initcount  Initial semaphore count
 *                     (see detailed semaphore description).
 * @return  semaphore creation status. -1 is returned when the
 *          semaphore could not be created.
 * @note    ::POSCFG_FEATURE_SEMAPHORES must be defined to 1 
 *          to have semaphore support compiled in.
 * @sa      ::posSemaCreate, destroy, get, wait, signal
 */
    inline VAR_t create(INT_t initcount)
    {
      handle = ::posSemaCreate(initcount);
      return (handle == NULL) ? -1 : 0;
    };

#if (DOX!=0) || (POSCFG_FEATURE_SEMADESTROY != 0)
/**
 * Frees a no more needed semaphore object.
 * @note    ::POSCFG_FEATURE_SEMAPHORES must be defined to 1 
 *          to have semaphore support compiled in.@n
 *          ::POSCFG_FEATURE_SEMADESTROY must be defined to 1
 *          to have this function compiled in.
 * @sa      ::posSemaDestroy, create
 */
    inline void destroy()
    {
      ::posSemaDestroy(handle);
      handle = (POSSEMA_t)0;
    };
#endif

/**
 * This function tries to get the semaphore object. If the semaphore
 * is in nonsignalized state (that means its counter is zero or less),
 * this function blocks the task execution until the semaphore
 * gets signaled.
 * @return  zero on success.
 * @note    ::POSCFG_FEATURE_SEMAPHORES must be defined to 1 
 *          to have semaphore support compiled in.
 * @sa      ::posSemaGet, wait, signal, create
 */
    inline VAR_t get()
    {
      return ::posSemaGet(handle);
    };

/**
 * This function signalizes a semaphore object, that means it increments
 * the semaphore counter and sets tasks pending on the semaphore to 
 * running state, when the counter reaches a positive, nonzero value.
 * @return  zero on success.
 * @note    ::POSCFG_FEATURE_SEMAPHORES must be defined to 1 
 *          to have semaphore support compiled in.
 * @sa      ::posSemaSignal, get, wait, create
 */
    inline VAR_t signal()
    {
      return ::posSemaSignal(handle);
    };

#if (DOX!=0) || (POSCFG_FEATURE_SEMAWAIT != 0)
/**
 * This function tries to get the semaphore object. If the semaphore
 * is in nonsignalized state (that means its counter is zero or less),
 * this function blocks the task execution until the semaphore
 * gets signaled or a timeout happens.
 * @param   timeoutticks  timeout in timer ticks
 *          (see ::HZ define and ::MS macro).
 *          If this parameter is set to zero, the function immediately
 *          returns. If this parameter is set to INFINITE, the
 *          function will never time out.
 * @return  zero on success. A positive value (1 or TRUE) is returned
 *          when the timeout was reached.
 * @note    ::POSCFG_FEATURE_SEMAPHORES must be defined to 1 
 *          to have semaphore support compiled in.@n
 *          ::POSCFG_FEATURE_SEMAWAIT must be defined to 1
 *          to have this function compiled in.
 * @sa      ::posSemaWait, get, signal, create, HZ, MS
 */
    inline VAR_t wait(UINT_t timeoutticks)
    {
      return ::posSemaWait(handle, timeoutticks);
    };

#endif

/*
 * Assignment operators.
 */
    inline Sema& operator=(const Sema& other)
    {
      handle = other.handle;
      return *this;
    };

    inline Sema& operator=(const POSSEMA_t other)
    {
      handle = handle;
      return *this;
    };

    inline operator POSSEMA_t() const
    {
      return handle;
    }

  protected:
    POSSEMA_t handle;
  };
}

#if POSCFG_ENABLE_NANO != 0

namespace nos {

/**
 * Semaphore functions.
 */
  class Sema : public pos::Sema
  {
  public:

/*
 * Constructors.
 */
    inline Sema()
    {
    };

    inline Sema(const Sema& other) : pos::Sema(other)
    {
    };

    inline Sema(const POSSEMA_t other) : pos::Sema(other)
    {
    };

#if DOX!=0 || NOSCFG_FEATURE_SEMAPHORES != 0
/**
 * Allocates a new semaphore object.
 * @param   initcount Initial semaphore count (see detailed semaphore
 *                    description in pico laye documentation).
 * @param   options   Currently unused. Please set this parameter to 0 (zero).
 * @param   name      Name of the new semaphore object to create. If the last
 *                    character in the name is an asteriks (*), the operating
 *                    system automatically assigns the semaphore a unique
 *                    name (the registry feature must be enabled for this
 *                    automatism). This parameter can be NULL if the nano
 *                    layer registry feature is not used and will not be
 *                    used in future.
 * @return  semaphore creation status. -1 is returned when the
 *          semaphore could not be created.
 * @note    ::NOSCFG_FEATURE_SEMAPHORES must be defined to 1
 *          to have semaphore support compiled in. @n
 *          You must use ::nosSemaDestroy to destroy the semaphore again.@n
 *          Even if the function posSemaDestroy would work also, it is
 *          required to call ::nosSemaDestroy. Only this function removes
 *          the semaphore from the registry. @n
 *          Dependent of your configuration, this function can
 *          be defined as macro to decrease code size.
 * @sa      nosSemaDestroy, nosSemaGet, nosSemaWait, nosSemaSignal
 */

    inline VAR_t create(INT_t initcount, UVAR_t options,
                        const char *name)
    {
      handle = nosSemaCreate(initcount, options, name);
      return (handle == NULL) ? -1 : 0;
    };

    using pos::Sema::create;

#if DOX!=0 || POSCFG_FEATURE_SEMADESTROY != 0
/**
 * Frees a no more needed semaphore object.
 * @param   sema  handle to the semaphore object.
 * @note    ::NOSCFG_FEATURE_SEMAPHORES must be defined to 1
 *          to have semaphore support compiled in.@n
 *          ::POSCFG_FEATURE_SEMADESTROY must be defined to 1
 *          to have this function compiled in. @n
 *          Dependent of your configuration, this function can
 *          be defined as macro to decrease code size.
 * @sa      nosSemaCreate
 */
    inline void destroy()
    {
      nosSemaDestroy(handle);
      handle = (POSSEMA_t)0;
    }

#endif
#endif /* NOSCFG_FEATURE_SEMAPHORES */

  };
}

#endif /* POSCFG_ENABLE_NANO */
#endif /* POSCFG_FEATURE_SEMAPHORES  */
#endif /* _PICOOS_SEMA_HXX */
