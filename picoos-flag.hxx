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

#if (DOX!=0) || (POSCFG_FEATURE_FLAGS != 0)
/**
 * Flags are one-bit semaphores. They can be used to simulate events.
 * A thread can simultaneousely wait for multiple flags to be set,
 * so it is possible to post multiple events to this thread.
 * The count of events a flag object can handle is only limited by the
 * underlaying architecutre, the maximum count is defined as ::MVAR_BITS - 1.
 */
  class Flag
  {
  public:

/* 
 * Constructors.
 */
    inline Flag()
    {
#ifdef _DBG
      handle = (POSFLAG_t)0;
#endif
    };

    inline Flag(const Flag& other)
    {
      handle = other.handle;
    };

    inline Flag(const POSFLAG_t other)
    {
      handle = other;
    };

/**
 * Allocates a flag object. A flag object behaves like an array of
 * one bit semaphores. The object can hold up to ::MVAR_BITS - 1 flags.
 * The flags can be used to simulate events, so a single thread can wait
 * for several events simultaneously.
 * @return  flag creation status. -1 is returned when the
 *          flag could not be created.
 * @note    ::POSCFG_FEATURE_FLAGS must be defined to 1 
 *          to have flag support compiled in.
 * @sa      ::posFlagCreate, get, set, destroy
 */
    inline VAR_t create()
    {
      handle = ::posFlagCreate();
      return (handle == NULL) ? -1 : 0;
    }

#if (DOX!=0) || (POSCFG_FEATURE_FLAGDESTROY != 0)
/**
 * Frees an unused flag object again.
 * @param   flg  handle to the flag object.
 * @note    ::POSCFG_FEATURE_FLAGS must be defined to 1 
 *          to have flag support compiled in.@n
 *          ::POSCFG_FEATURE_FLAGDESTROY must be defined to 1
 *          to have this function compiled in.
 * @sa      ::posFlagDestroy, create
 */
    inline void destroy()
    {
      ::posFlagDestroy(handle);
    }
#endif

/**
 * Sets a flag bit in the flag object and sets the Flag that
 * pends on the flag object to running state.
 * @param   flg     handle to the flag object.
 * @param   flgnum  Number of the flag to set. The flag number
 *                  must be in the range of 0 .. ::MVAR_BITS - 2.
 * @return  zero on success.
 * @note    ::POSCFG_FEATURE_FLAGS must be defined to 1 
 *          to have flag support compiled in.
 * @sa      ::posFlagSet, create, get, wait
 */
    inline VAR_t set(UVAR_t flgnum)
    {
      return ::posFlagSet(handle, flgnum);
    }

/**
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
 * @sa      ::posFlagGet, create, set, wait
 */
    inline VAR_t get(UVAR_t mode)
    {
      return ::posFlagGet(handle, mode);
    }

#if (DOX!=0) || (POSCFG_FEATURE_FLAGWAIT != 0)
/**
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
 * @sa      ::posFlagWait, create, set, get, HZ, MS
 */
    inline VAR_t wait(UINT_t timeoutticks)
    {
      return ::posFlagWait(handle, timeoutticks);
    }
#endif

/*
 * Assignment operators.
 */
    inline Flag& operator=(const Flag& other)
    {
      handle = other.handle;
      return *this;
    };

    inline Flag& operator=(const POSFLAG_t other)
    {
      handle = handle;
      return *this;
    };

  private:
    POSFLAG_t handle;
  };

#endif  /* POSCFG_FEATURE_FLAGS */
}

#endif /* _PICOOS_FLAG_HXX */
