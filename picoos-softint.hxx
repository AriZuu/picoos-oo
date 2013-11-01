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
 * @file    picoos-softint.hxx
 * @brief   Pico]OS oo-wrapper SoftInt
 * @author  Ari Suutari <ari@stonepile.fi>
 */

#ifndef _PICOOS_SOFTINT_HXX
#define _PICOOS_SOFTINT_HXX

extern "C" {

#include <picoos.h>

}

namespace pos {

#if (DOX!=0) || (POSCFG_FEATURE_SOFTINTS != 0)
/**
 * pico]OS has a built in mechanism to simulate software interrupts.
 * For example, software interrupts can be used to connect hardware
 * interrupts, that are outside the scope of pico]OS, to the realtime
 * operating system. A hardware interrupt will trigger a software
 * interrupt that can then signalize a semaphore object.@n
 * @n
 * Note that hardware interrupts, that do not call ::c_pos_intEnter and
 * ::c_pos_intExit, can't do calls to pico]OS functions, except to the
 * function ::posSoftInt.@n
 * @n
 * All software interrupts, that are triggered by a call to ::posSoftInt,
 * are chained into a global list. This list is then executed as soon
 * as possible, but at least when the pico]OS scheduler is called
 * (that is, for example, when a time slice has expired or a task
 * gives of processing time by itself).@n
 * @n
 * A software interrupt runs at interrupt level, that means with
 * interrupts disabled (pico]OS calls ::POS_SCHED_LOCK before executing
 * the software interrupt handler). The execution of software interrupt
 * handlers can not be inhibited by setting the ::posTaskSchedLock flag.
 * @n
 * Note that software interrupts need additional space on the
 * processors call stack. Make sure to have space for at least
 * 5 additional subroutine calls, plus the calls you will make in the
 * ISR handler routine.
 */
  class SoftInt
  {
  private:

/*
 * Constructors.
 */
    inline SoftInt()
    {
    };

  public:
/**
 * Rises a software interrupt. The software interrupt handler will
 * be executed as soon as possible. See above for a detailed description.
 * @param   intno number of the interrupt to rise. Must be in the
 *          range of 0 to ::POSCFG_SOFTINTERRUPTS - 1.
 * @param   param optional parameter that will be passed to the
 *          software interrupt handler funtion.
 * @note    ::POSCFG_FEATURE_SOFTINTS must be defined to 1 
 *          to have software interrupt support compiled in.
 * @note    If a software interrupt is triggered from inside the
 *          pico]OS scope, usually from application level, it is
 *          recommended to do a call to ::posTaskYield after this
 *          function is called. This will immediately start the
 *          interrupt handler.
 * @sa      ::posSoftInt, setHandler, delHandler, POSCFG_SOFTINTQUEUELEN
 */
    static inline void raise(UVAR_t intno, UVAR_t param)
    {
      ::posSoftInt(intno, param);
    }

/**
 * Sets a software interrupt handler function. Before a software interrupt
 * can be rised by a call to ::posSoftInt, this function must be called
 * to tell pico]OS the handler function for the interrupt.
 * @param   intno number of the interrupt to rise. Must be in the
 *          range of 0 to ::POSCFG_SOFTINTERRUPTS - 1.
 * @param   inthandler pointer to the interrupt handler function.
 * @note    ::POSCFG_FEATURE_SOFTINTS must be defined to 1 
 *          to have software interrupt support compiled in.
 * @return  zero on success.
 * @sa      ::posSoftIntSetHandler, delHandler, raise
 */
    static inline VAR_t setHandler(UVAR_t intno, POSINTFUNC_t inthandler)
    {
      return ::posSoftIntSetHandler(intno, inthandler);
    }

#if (DOX!=0) || (POSCFG_FEATURE_SOFTINTDEL != 0)
/**
 * Deletes a software interrupt handler function, that was set
 * with ::posSoftIntSetHandler before.
 * @param   intno number of the interrupt to rise. Must be in the
 *          range of 0 to ::POSCFG_SOFTINTERRUPTS - 1.
 * @note    ::POSCFG_FEATURE_SOFTINTS must be defined to 1 
 *          to have software interrupt support compiled in.@n
 *          ::POSCFG_FEATURE_SOFTINTDEL must be defined to 1
 *          to have this function compiled in.
 * @return  zero on success.
 * @sa      ::posSoftIntDelHandler, setHandler, raise
 */
    static inline VAR_t delHandler(UVAR_t intno)
    {
      return ::posSoftIntDelHandler(intno);
    }
#endif

  };

#endif  /* POSCFG_FEATURE_SOFTINTS */
}

#endif /* _PICOOS_SOFTINT_HXX */
