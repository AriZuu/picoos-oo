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
 * @file    picoos-timer.hxx
 * @brief   Pico]OS oo-wrapper Timer
 * @author  Ari Suutari <ari@stonepile.fi>
 */

#ifndef _PICOOS_TIMER_HXX
#define _PICOOS_TIMER_HXX

extern "C" {

#include <picoos.h>

}

#if (DOX!=0) || (POSCFG_FEATURE_TIMER != 0)

namespace pos {

/**
 * A timer object is a counting variable that is counted down by the
 * system timer interrupt tick rate. If the variable reaches zero,
 * a semaphore, that is bound to the timer, will be signaled.
 * If the timer is in auto reload mode, the timer is restarted and
 * will signal the semaphore again and again, depending on the
 * period rate the timer is set to.
 */
  class Timer
  {
  public:

/* 
 * Constructors.
 */
    inline Timer()
    {
#ifdef _DBG
      handle = (POSTIMER_t)0;
#endif
    };

    inline Timer(const Timer& other)
    {
      handle = other.handle;
    };

    inline Timer(const POSTIMER_t other)
    {
      handle = other;
    };

/**
 * Allocates a timer object. After a timer is allocated with this function,
 * it must be set up with posTimerSet and than started with posTimerStart.
 * @return  timer creation status. -1 is returned when the
 *          timer could not be created.
 * @note    ::POSCFG_FEATURE_TIMER must be defined to 1 
 *          to have timer support compiled in.
 * @sa      ::posTimerCreate, set, start, destroy
 */
    inline VAR_t create()
    {
      handle = ::posTimerCreate();
      return (handle == NULL) ? -1 : 0;
    }

/**
 * Sets up a timer object.
 * @param   sema seaphore object that shall be signaled when timer fires.
 * @param   waitticks  number of initial wait ticks. The timer fires the
 *                     first time when this ticks has been expired.
 * @param   periodticks  After the timer has fired, it is reloaded with
 *                       this value, and will fire again when this count
 *                       of ticks has been expired (auto reload mode).
 *                       If this value is set to zero, the timer
 *                       won't be restarted (= one shot mode).
 * @return  zero on success.
 * @note    ::POSCFG_FEATURE_TIMER must be defined to 1 
 *          to have timer support compiled in.
 * @sa      ::posTimerSet, create, start
 */
    inline VAR_t set(const Sema& sema,
                     UINT_t waitticks, UINT_t periodticks)
    {
      return ::posTimerSet(handle, sema, waitticks, periodticks);
    }
/**
 * Starts a timer. The timer will fire first time when the
 * waitticks counter has been reached zero. If the periodticks
 * were set, the timer will be reloaded with this value.
 * @return  zero on success.
 * @note    ::POSCFG_FEATURE_TIMER must be defined to 1 
 *          to have timer support compiled in.
 * @sa      ::posTimerStart, stop, fired
 */
    inline VAR_t start()
    {
      return ::posTimerStart(handle);
    }

/**
 * Stops a timer. The timer will no more fire. The timer
 * can be reenabled with posTimerStart.
 * @return  zero on success.
 * @note    ::POSCFG_FEATURE_TIMER must be defined to 1 
 *          to have timer support compiled in.
 * @sa      ::posTimerStop, start, destroy
 */
    inline VAR_t stop()
    {
      return ::posTimerStop(handle);
    }

#if (DOX!=0) || (POSCFG_FEATURE_TIMERDESTROY != 0)
/**
 * Deletes a timer object and free its resources.
 * @note    ::POSCFG_FEATURE_TIMER must be defined to 1 
 *          to have timer support compiled in. @n
 *          ::POSCFG_FEATURE_TIMERDESTROY must be defined to 1
 *          to have this function compiled in.
 * @sa      ::posTimerDestroy, create
 */
    inline void destroy()
    {
      ::posTimerDestroy(handle);
    }
#endif
#if (DOX!=0) || (POSCFG_FEATURE_TIMERFIRED != 0)
/**
 * The function is used to test if a timer has fired.
 * @return  1 when the timer has fired, otherwise 0.
 *          A negative value is returned on error.
 * @note    ::POSCFG_FEATURE_TIMER must be defined to 1 
 *          to have timer support compiled in. @n
 *          ::POSCFG_FEATURE_TIMERFIRED must be defined to 1
 *          to have this function compiled in.
 * @sa      ::posTimerFired, create, set, start
 */
    inline VAR_t fired()
    {
      return ::posTimerFired(handle);
    }
#endif

/*
 * Assignment operators.
 */
    inline Timer& operator=(const Timer& other)
    {
      handle = other.handle;
      return *this;
    };

    inline Timer& operator=(const POSTIMER_t other)
    {
      handle = handle;
      return *this;
    };

  protected:
    POSTIMER_t handle;
  };
}

#if DOX!=0 || POSCFG_ENABLE_NANO != 0

namespace nos {

/**
 * Timer functions.
 */
  class Timer : public pos::Timer
  {
  public:

/*
 * Constructors.
 */
    inline Timer()
    {
    };

    inline Timer(const Timer& other) : pos::Timer(other)
    {
    };

    inline Timer(const POSTIMER_t other) : pos::Timer(other)
    {
    };

#if DOX!=0 || NOSCFG_FEATURE_TIMER != 0
/**
 * Allocates a timer object. After a timer is allocated with this function,
 * it must be set up with ::nosTimerSet and than started with ::nosTimerStart.
 * @return  semaphore creation status. -1 is returned when the
 *          semaphore could not be created.
 * @param   name      Name of the new timer object to create. If the last
 *                    character in the name is an asteriks (*), the operating
 *                    system automatically assigns the timer an unique
 *                    name (the registry feature must be enabled for this
 *                    automatism). This parameter can be NULL if the nano
 *                    layer registry feature is not used and will not be
 *                    used in future.
 * @note    ::NOSCFG_FEATURE_TIMER must be defined to 1
 *          to have timer support compiled in. @n
 *          You must use ::nosTimerDestroy to destroy the flag object again.@n
 *          Even if the function posTimerDestroy would work also, it is
 *          required to call ::nosTimerDestroy. Only this function removes
 *          the flag object from the registry. @n
 *          Dependent of your configuration, this function can
 *          be defined as macro to decrease code size.
 * @sa      nosTimerSet, nosTimerStart, nosTimerDestroy
 */
    inline VAR_t create(const char *name)
    {
      handle = nosTimerCreate(name);
      return (handle == NULL) ? -1 : 0;
    };

    using pos::Timer::create;

#if DOX!=0 || POSCFG_FEATURE_TIMERDESTROY != 0
/**
 * Timer function.
 * Deletes a timer object and free its resources.
 * @param   tmr  handle to the timer object.
 * @note    ::NOSCFG_FEATURE_TIMER must be defined to 1
 *          to have timer support compiled in. @n
 *          ::POSCFG_FEATURE_TIMERDESTROY must be defined to 1
 *          to have this function compiled in. @n
 *          Dependent of your configuration, this function can
 *          be defined as macro to decrease code size.
 * @sa      nosTimerCreate
 */
    inline void destroy()
    {
      nosTimerDestroy(handle);
      handle = (POSTIMER_t)0;
    }

#endif
#endif /* NOSCFG_FEATURE_TIMER */

  };
}

#endif /* POSCFG_ENABLE_NANO */
#endif  /* POSCFG_FEATURE_TIMER */
#endif /* _PICOOS_TIMER_HXX */
