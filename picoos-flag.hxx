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

#if (DOX!=0) || (POSCFG_FEATURE_FLAGS != 0)

namespace pos {

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

  protected:
    POSFLAG_t handle;
  };
}

#if DOX!=0 || POSCFG_ENABLE_NANO != 0

namespace nos {

/**
 * Flag functions.
 */
  class Flag : public pos::Flag
  {
  public:

/*
 * Constructors.
 */
    inline Flag()
    {
    };

    inline Flag(const Flag& other) : pos::Flag(other)
    {
    };

    inline Flag(const POSFLAG_t other) : pos::Flag(other)
    {
    };

#if DOX!=0 || NOSCFG_FEATURE_FLAGS != 0
/**
 * Allocates a flag object. A flag object behaves like an array of
 * one bit semaphores. The object can hold up to ::MVAR_BITS - 1 flags.
 * The flags can be used to simulate events, so a single thread can wait
 * for several events simultaneously.
 * @param   name      Name of the new flag object to create. If the last
 *                    character in the name is an asteriks (*), the operating
 *                    system automatically assigns the flag an unique name.
 *                    name (the registry feature must be enabled for this
 *                    automatism). This parameter can be NULL if the nano
 *                    layer registry feature is not used and will not be
 *                    used in future.
 * @return  semaphore creation status. -1 is returned when the
 *          semaphore could not be created.
 * @note    ::NOSCFG_FEATURE_FLAGS must be defined to 1
 *          to have flag support compiled in. @n
 *          You must use ::nosFlagDestroy to destroy the flag object again.@n
 *          Even if the function posFlagDestroy would work also, it is
 *          required to call ::nosFlagDestroy. Only this function removes
 *          the flag object from the registry. @n
 *          Dependent of your configuration, this function can
 *          be defined as macro to decrease code size.
 * @sa      nosFlagGet, nosFlagSet, nosFlagDestroy
 */
    inline VAR_t create(const char *name)
    {
      handle = nosFlagCreate(name);
      return (handle == NULL) ? -1 : 0;
    };

    using pos::Flag::create;

#if DOX!=0 || POSCFG_FEATURE_FLAGDESTROY != 0
/**
 * Frees an unused flag object again.
 * @note    ::NOSCFG_FEATURE_FLAGS must be defined to 1
 *          to have flag support compiled in.@n
 *          ::POSCFG_FEATURE_FLAGDESTROY must be defined to 1
 *          to have this function compiled in.@n
 *          Dependent of your configuration, this function can
 *          be defined as macro to decrease code size.
 * @sa      nosFlagCreate
 */
    inline void destroy()
    {
      nosFlagDestroy(handle);
      handle = (POSFLAG_t)0;
    }

#endif
#endif /* NOSCFG_FEATURE_FLAGS */

  };
}

#endif /* POSCFG_ENABLE_NANO */
#endif  /* POSCFG_FEATURE_FLAGS */
#endif /* _PICOOS_FLAG_HXX */
