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
 * @file    picoos-flag.hxx
 * @brief   Pico]OS oo-wrapper Flag
 * @author  Ari Suutari <ari@stonepile.fi>
 */

#ifndef _PICOOS_FLAG_HXX
#define _PICOOS_FLAG_HXX

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
      handle = (POSFLAG_t)0;
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

#if (DOX!=0) || (POSCFG_FEATURE_FLAGS != 0)
/** @defgroup flag Flag Functions
 * @ingroup userapip
 * Flags are one-bit semaphores. They can be used to simulate events.
 * A thread can simultaneousely wait for multiple flags to be set,
 * so it is possible to post multiple events to this thread.
 * The count of events a flag object can handle is only limited by the
 * underlaying architecutre, the maximum count is defined as ::MVAR_BITS - 1.
 * @{
 */
/**
 * Flag function.
 * Allocates a flag object. A flag object behaves like an array of
 * one bit semaphores. The object can hold up to ::MVAR_BITS - 1 flags.
 * The flags can be used to simulate events, so a single thread can wait
 * for several events simultaneously.
 * @return  handle to the new flag object. NULL is returned on error.
 * @note    ::POSCFG_FEATURE_FLAGS must be defined to 1 
 *          to have flag support compiled in.
 * @sa      posFlagGet, posFlagSet, posFlagDestroy
 */
    inline POSFLAG_t posFlagCreate(void);

#if (DOX!=0) || (POSCFG_FEATURE_FLAGDESTROY != 0)
/**
 * Flag function.
 * Frees an unused flag object again.
 * @param   flg  handle to the flag object.
 * @note    ::POSCFG_FEATURE_FLAGS must be defined to 1 
 *          to have flag support compiled in.@n
 *          ::POSCFG_FEATURE_FLAGDESTROY must be defined to 1
 *          to have this function compiled in.
 * @sa      posFlagCreate
 */
    inline void posFlagDestroy(POSFLAG_t flg);
#endif

/**
 * Flag function.
 * Sets a flag bit in the flag object and sets the task that
 * pends on the flag object to running state.
 * @param   flg     handle to the flag object.
 * @param   flgnum  Number of the flag to set. The flag number
 *                  must be in the range of 0 .. ::MVAR_BITS - 2.
 * @return  zero on success.
 * @note    ::POSCFG_FEATURE_FLAGS must be defined to 1 
 *          to have flag support compiled in.
 * @sa      posFlagCreate, posFlagGet, posFlagWait
 */
    inline VAR_t posFlagSet(POSFLAG_t flg, UVAR_t flgnum);

/**
 * Flag function.
 * Pends on a flag object and waits until one of the flags 
 * in the flag object is set.
 * @param   flg   handle to the flag object.
 * @param   mode  can be POSFLAG_MODE_GETSINGLE or POSFLAG_MODE_GETMASK.
 * @return  the number of the next flag that is set when mode is set
 *          to POSFLAG_MODE_GETSINGLE. When mode is set to 
 *          POSFLAG_MODE_GETMASK, a bit mask with all set flags is
 *          returned. A negative value is returned on error.
 * @note    ::POSCFG_FEATURE_FLAGS must be defined to 1 
 *          to have flag support compiled in.
 * @sa      posFlagCreate, posFlagSet, posFlagWait
 */
    inline VAR_t posFlagGet(POSFLAG_t flg, UVAR_t mode);

#if (DOX!=0) || (POSCFG_FEATURE_FLAGWAIT != 0)
/**
 * Flag function.
 * Pends on a flag object and waits until one of the flags 
 * in the flag object is set or a timeout has happened.
 * @param   flg   handle to the flag object.
 * @param   timeoutticks  timeout in timer ticks
 *          (see ::HZ define and ::MS macro).
 *          If this parameter is set to zero, the function immediately
 *          returns. If this parameter is set to INFINITE, the
 *          function will never time out.
 * @return  a mask of all set flags (positive value).
 *          If zero is returned, the timeout was reached.
 *          A negative value denotes an error.
 * @note    ::POSCFG_FEATURE_FLAGS must be defined to 1 
 *          to have flag support compiled in.@n
 *          ::POSCFG_FEATURE_FLAGWAIT must be defined to 1
 *          to have this function compiled in.
 * @sa      posFlagCreate, posFlagSet, posFlagGet, HZ, MS
 */
    inline VAR_t posFlagWait(POSFLAG_t flg, UINT_t timeoutticks);
#endif

#define POSFLAG_MODE_GETSINGLE   0
#define POSFLAG_MODE_GETMASK     1

#endif  /* POSCFG_FEATURE_FLAGS */

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

#endif /* _PICOOS_FLAG_HXX */
