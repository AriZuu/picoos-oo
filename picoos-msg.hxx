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
 * @file    picoos-msg.hxx
 * @brief   Pico]OS oo-wrapper Msg
 * @author  Ari Suutari <ari@stonepile.fi>
 */

#ifndef _PICOOS_MSG_HXX
#define _PICOOS_MSG_HXX

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

#if (DOX!=0) || (POSCFG_FEATURE_MSGBOXES != 0)
/** @defgroup msg Message Box Functions
 * @ingroup userapip
 * Message boxes are a mechanism that is used for inter-process or,
 * in the case of pico]OS, inter-task communication. All tasks
 * can post messages to each other, and the receiving task will
 * look into its message box and continues when it
 * got a new message. Note that a message box in the pico]OS
 * can hold chunk of messages, so that no message will be lost
 * while the receiving task is still busy processing the last
 * message. @n
 * There are two possible types of message boxes: The simple type
 * can only hold a pointer to a user supplied buffer. The other
 * message box type can hold whole messages with different sizes.
 * A message buffer must be allocated with posMessageAlloc by the
 * sending task, and the receiving task must free this buffer
 * again with ::posMessageFree. @n
 * To select the simple message box type, you have to set
 * the define ::POSCFG_MSG_MEMORY to 0. When you want to have the
 * full message buffer support, you must set ::POSCFG_MSG_MEMORY to 1.
 * @{
 */

#if (DOX!=0) || (POSCFG_MSG_MEMORY != 0)
/**
 * Message box function.
 * Allocates a new message buffer. The maximum buffer size is
 * set at compilation time by the define ::POSCFG_MSG_BUFSIZE.
 * Usually the sending task would allocate a new message buffer, fill
 * in its data and send it via ::posMessageSend to the receiving task.
 * @return  the pointer to the new buffer. NULL is returned on error.
 * @note    ::POSCFG_FEATURE_MSGBOXES must be defined to 1 
 *          to have message box support compiled in.@n
 *          ::POSCFG_MSG_MEMORY must be defined to 1
 *          to have this function compiled in.
 * @sa      posMessageSend, posMessageGet, posMessageFree
 */
    inline void* posMessageAlloc(void);

/**
 * Message box function.
 * Frees a message buffer again.
 * Usually the receiving task would call this function after
 * it has processed a message to free the message buffer again.
 * @param   buf  pointer to the message buffer that is no more used.
 * @note    ::POSCFG_FEATURE_MSGBOXES must be defined to 1 
 *          to have message box support compiled in.@n
 *          ::POSCFG_MSG_MEMORY must be defined to 1
 *          to have this function compiled in.
 * @sa      posMessageGet, posMessageSend, posMessageAlloc
 */
    inline void posMessageFree(void *buf);
#endif

/**
 * Message box function.
 * Sends a message to a task.
 * @param   buf  pointer to the message to send.
 *               If ::POSCFG_MSG_MEMORY is defined to 1,
 *               this pointer must point to the buffer
 *               that was allocated with ::posMessageAlloc.
 *               Note that this parameter must not be NULL.
 *               Exception: ::POSCFG_MSG_MEMORY = 0 and
 *               ::POSCFG_FEATURE_MSGWAIT = 0.
 * @param   taskhandle  handle to the task to send the message to.
 * @return  zero on success. When an error condition exist, a
 *          negative value is returned and the message buffer is freed.
 * @note    ::POSCFG_FEATURE_MSGBOXES must be defined to 1 
 *          to have message box support compiled in.
 * @sa      posMessageAlloc, posMessageGet
 */
    inline VAR_t posMessageSend(void *buf, POSTASK_t taskhandle);

/**
 * Message box function. Gets a new message from the message box.
 * If no message is available, the task blocks until a new message
 * is received.
 * @return  pointer to the received message. Note that the
 *          message memory must be freed again with ::posMessageFree
 *          when ::POSCFG_MSG_MEMORY is defined to 1. @n
 *          NULL may be returned when the system has not
 *          enough events. In this case, please increase the
 *          value for ::POSCFG_MAX_EVENTS in your poscfg.h .
 * @note    ::POSCFG_FEATURE_MSGBOXES must be defined to 1 
 *          to have message box support compiled in.
 * @sa      posMessageFree, posMessageAvailable,
 *          posMessageWait, posMessageSend
 */
#if (DOX!=0) || (POSCFG_SMALLCODE == 0) || (POSCFG_FEATURE_MSGWAIT == 0)
    inline void* posMessageGet(void);
#else
/* this define is for small code and it saves stack memory */
#define     posMessageGet()  posMessageWait(INFINITE)
#endif

/**
 * Message box function.
 * Tests if a new message is available
 * in the message box. This function can be used to prevent
 * the task from blocking.
 * @return  1 (=true) when a new message is available.
 *          Otherwise zero is returned. A negative value
 *          is returned on error.
 * @note    ::POSCFG_FEATURE_MSGBOXES must be defined to 1 
 *          to have message box support compiled in.
 * @sa      posMessageGet, posMessageWait
 */
    inline VAR_t posMessageAvailable(void);

#if (DOX!=0) || (POSCFG_FEATURE_MSGWAIT != 0)
/**
 * Message box function.
 * Gets a new message from the message box.
 * If no message is available, the task blocks until a new message
 * is received or the timeout has been reached.
 * @param   timeoutticks  timeout in timer ticks
 *          (see ::HZ define and ::MS macro).
 *          If this parameter is set to zero, the function immediately
 *          returns. If this parameter is set to INFINITE, the
 *          function will never time out.
 * @return  pointer to the received message. Note that the
 *          message memory must be freed again with posMessageFree
 *          when ::POSCFG_MSG_MEMORY is defined to 1.
 *          NULL is returned when no message was received
 *          within the specified time (=timeout). @n
 *          Hint: NULL may also be returned when the system has
 *          not enough events. In this case, please increase the
 *          value for ::POSCFG_MAX_EVENTS in your poscfg.h .
 * @note    ::POSCFG_FEATURE_MSGBOXES must be defined to 1 
 *          to have message box support compiled in.@n
 *          ::POSCFG_FEATURE_MSGWAIT must be defined to 1
 *          to have this function compiled in.
 * @sa      posMessageFree, posMessageGet, posMessageAvailable,
 *          posMessageSend, HZ, MS
 */
    inline void* posMessageWait(UINT_t timeoutticks);
#endif

#endif  /* POSCFG_FEATURE_MSGBOXES */

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

#endif /* _PICOOS_MSG_HXX */
