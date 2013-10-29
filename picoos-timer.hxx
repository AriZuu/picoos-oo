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

/** @defgroup timer Timer Functions
 * @ingroup userapip
 * A timer object is a counting variable that is counted down by the
 * system timer interrupt tick rate. If the variable reaches zero,
 * a semaphore, that is bound to the timer, will be signaled.
 * If the timer is in auto reload mode, the timer is restarted and
 * will signal the semaphore again and again, depending on the
 * period rate the timer is set to.
 * @{
 */
/** @def HZ
 * Hertz, timer ticks per second.
 * HZ is a define that is set to the number of ticks
 * the timer interrupt does in a second.
 * For example, the jiffies variable is incremented
 * HZ times per second. Also, much OS functions take
 * timeout parameters measured in timer ticks; thus
 * the HZ define can be taken as time base: HZ = 1 second,
 * 10*HZ = 10s, HZ/10 = 100ms, etc.
 * @sa jiffies, MS
 */
#if DOX!=0
#define HZ (timerticks per second)
#endif

/** @brief  Global timer variable.
 * The jiffies counter variable is incremented ::HZ times per second.
 *
 * The maximum count the jiffie counter can reach until it wraps around
 * is system dependent.
 * @note    ::POSCFG_FEATURE_JIFFIES must be defined to 1 
 *          to have jiffies support compiled in.
 * @sa HZ, POS_TIMEAFTER
 */
#if (DOX!=0) || (POSCFG_FEATURE_JIFFIES != 0)
#if (DOX!=0) || (POSCFG_FEATURE_LARGEJIFFIES == 0)
    inline  volatile JIF_t  jiffies;
#else
    inline  JIF_t posGetJiffies(void);
#define jiffies  posGetJiffies()
#endif

/**
 * This macro is used to test if a specified time has expired.
 * It handles timer variable wrap arounds correctly.
 * The macro is used in conjunction with the jiffies variable,
 * the current jiffies should be passed as first parameter
 * to the macro. Example:@n
 * exptime = jiffies + HZ/2;@n
 * if (POS_TIMEAFTER(jiffies, exptime)) printf("500ms expired!\n");
 * @sa jiffies, HZ
 */
#define POS_TIMEAFTER(x, y)    ((((SJIF_t)(x)) - ((SJIF_t)(y))) >= 0)

#endif  /* POSCFG_FEATURE_JIFFIES */

#if (DOX!=0) || (POSCFG_FEATURE_TIMER != 0)

/**
 * Timer function.
 * Allocates a timer object. After a timer is allocated with this function,
 * it must be set up with posTimerSet and than started with posTimerStart.
 * @return  handle to the new timer object. NULL is returned on error.
 * @note    ::POSCFG_FEATURE_TIMER must be defined to 1 
 *          to have timer support compiled in.
 * @sa      posTimerSet, posTimerStart, posTimerDestroy
 */
    inline POSTIMER_t posTimerCreate(void);

/**
 * Timer function.
 * Sets up a timer object.
 * @param   tmr  handle to the timer object.
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
 * @sa      posTimerCreate, posTimerStart
 */
    inline VAR_t posTimerSet(POSTIMER_t tmr, POSSEMA_t sema,
                                    UINT_t waitticks, UINT_t periodticks);
/**
 * Timer function.
 * Starts a timer. The timer will fire first time when the
 * waitticks counter has been reached zero. If the periodticks
 * were set, the timer will be reloaded with this value.
 * @param   tmr  handle to the timer object.
 * @return  zero on success.
 * @note    ::POSCFG_FEATURE_TIMER must be defined to 1 
 *          to have timer support compiled in.
 * @sa      posTimerStop, posTimerFired
 */
    inline VAR_t posTimerStart(POSTIMER_t tmr);

/**
 * Timer function.
 * Stops a timer. The timer will no more fire. The timer
 * can be reenabled with posTimerStart.
 * @param   tmr  handle to the timer object.
 * @return  zero on success.
 * @note    ::POSCFG_FEATURE_TIMER must be defined to 1 
 *          to have timer support compiled in.
 * @sa      posTimerStart, posTimerDestroy
 */
    inline VAR_t posTimerStop(POSTIMER_t tmr);

#if (DOX!=0) || (POSCFG_FEATURE_TIMERDESTROY != 0)
/**
 * Timer function.
 * Deletes a timer object and free its resources.
 * @param   tmr  handle to the timer object.
 * @note    ::POSCFG_FEATURE_TIMER must be defined to 1 
 *          to have timer support compiled in. @n
 *          ::POSCFG_FEATURE_TIMERDESTROY must be defined to 1
 *          to have this function compiled in.
 * @sa      posTimerCreate
 */
    inline void posTimerDestroy(POSTIMER_t tmr);
#endif
#if (DOX!=0) || (POSCFG_FEATURE_TIMERFIRED != 0)
/**
 * Timer function.
 * The function is used to test if a timer has fired.
 * @param   tmr  handle to the timer object.
 * @return  1 when the timer has fired, otherwise 0.
 *          A negative value is returned on error.
 * @note    ::POSCFG_FEATURE_TIMER must be defined to 1 
 *          to have timer support compiled in. @n
 *          ::POSCFG_FEATURE_TIMERFIRED must be defined to 1
 *          to have this function compiled in.
 * @sa      posTimerCreate, posTimerSet, posTimerStart
 */
    inline VAR_t posTimerFired(POSTIMER_t tmr);
#endif

#endif  /* POSCFG_FEATURE_TIMER */

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

#endif /* _PICOOS_TIMER_HXX */
