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
 * @file    picoos-atomic.hxx
 * @brief   Pico]OS oo-wrapper Atomic
 * @author  Ari Suutari <ari@stonepile.fi>
 */

#ifndef _PICOOS_ATOMIC_HXX
#define _PICOOS_ATOMIC_HXX

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

#if (DOX!=0) || (POSCFG_FEATURE_ATOMICVAR != 0)
/** @defgroup atomic Atomic Variables
 * @ingroup userapip
 * Atomic variables are variables that can be accessed in an atomic manner,
 * that means a read-modify-write instruction is done in virtually one
 * single cycle. For example, the atomic access to a variable is necessary
 * when two tasks will do read-modify-write operations on a common
 * variable. Under usual circumstances you can ran into trouble when
 * a task that is just modifying the variable (that means it has read
 * and modified the variable but has not yet written the result back) is
 * interrupted by a second task that also modifies the variable. Thus the
 * modification the first task has done would be lost. Atomic variables
 * prevent this possible race condition. @n@n
 * pico]OS supports four functions to operate on atomic variables:
 * ::posAtomicSet, ::posAtomicGet, ::posAtomicAdd and ::posAtomicSub.
 * @{
 */
/**
 * Atomic Variable Function.
 * Sets an atomic variable to the specified value.
 * @param   var    pointer to the atomic variable that shall be set.
 * @param   value  the value the atomic variable shall be set to.
 * @note    ::POSCFG_FEATURE_ATOMICVAR must be defined to 1 
 *          to have atomic variable support compiled in.
 * @sa      posAtomicGet, posAtomicAdd, posAtomicSub
 */
    inline void posAtomicSet(POSATOMIC_t *var, INT_t value);

/**
 * Atomic Variable Function.
 * Returns the current value of an atomic variable.
 * @param   var    pointer to the atomic variable which value
 *                 shall be read and returned.
 * @return  the value of the atomic variable.
 * @note    ::POSCFG_FEATURE_ATOMICVAR must be defined to 1 
 *          to have atomic variable support compiled in.
 * @sa      posAtomicSet, posAtomicAdd, posAtomicSub
 */
    inline INT_t posAtomicGet(POSATOMIC_t *var);

/**
 * Atomic Variable Function.
 * Adds a value onto the current value of the atomic variable.
 * @param   var    pointer to the atomic variable.
 * @param   value  value that shall be added to the atomic variable.
 * @return  the content of the atomic variable before it was incremented.
 * @note    ::POSCFG_FEATURE_ATOMICVAR must be defined to 1 
 *          to have atomic variable support compiled in.
 * @sa      posAtomicSet, posAtomicGet, posAtomicSub
 */
    inline INT_t posAtomicAdd(POSATOMIC_t *var, INT_t value);

/**
 * Atomic Variable Function.
 * Substracts a value from the current value of the atomic variable.
 * @param   var    pointer to the atomic variable.
 * @param   value  value that shall be substracted from the atomic variable.
 * @return  the content of the atomic variable before it was decremented.
 * @note    ::POSCFG_FEATURE_ATOMICVAR must be defined to 1 
 *          to have atomic variable support compiled in.
 * @sa      posAtomicSet, posAtomicGet, posAtomicAdd
 */
    inline INT_t posAtomicSub(POSATOMIC_t *var, INT_t value);

#endif /* POSCFG_FEATURE_ATOMICVAR */
/** @} */

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

#endif /* _PICOOS_ATOMIC_HXX */
