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

#if (DOX!=0) || (POSCFG_FEATURE_MSGBOXES != 0)

namespace pos {

/**
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
 * A message buffer must be allocated with ::pos::Message::alloc by the
 * sending task, and the receiving task must free this buffer
 * again with ::pos::Message::free. @n
 * To select the simple message box type, you have to set
 * the define ::POSCFG_MSG_MEMORY to 0. When you want to have the
 * full message buffer support, you must set ::POSCFG_MSG_MEMORY to 1.
 */

  class Message
  {
  public:
/**
 * Construct message when ::POSCFG_MSG_MEMORY is 1.
 */
    inline Message()
    {
      msg = NULL;
    };

#if (DOX!=0) || (POSCFG_MSG_MEMORY == 0)
    /**
     * Construct message when ::POSCFG_MSG_MEMORY is 0.
     */
    inline Message(void* buf)
    {
      msg = buf;
    };
#endif

#if (DOX!=0) || (POSCFG_MSG_MEMORY != 0)
/**
 * Allocates a new message buffer. The maximum buffer size is
 * set at compilation time by the define ::POSCFG_MSG_BUFSIZE.
 * Usually the sending task would allocate a new message buffer, fill
 * in its data and send it via ::posMessageSend to the receiving task.
 * @return  the pointer to the new buffer. NULL is returned on error.
 * @note    ::POSCFG_FEATURE_MSGBOXES must be defined to 1 
 *          to have message box support compiled in.@n
 *          ::POSCFG_MSG_MEMORY must be defined to 1
 *          to have this function compiled in.
 * @sa      ::posMessageAlloc, send, get, free
 */
    inline void* alloc()
    {
      msg = ::posMessageAlloc();
      return msg;
    }

/**
 * Frees a message buffer again.
 * Usually the receiving task would call this function after
 * it has processed a message to free the message buffer again.
 * @note    ::POSCFG_FEATURE_MSGBOXES must be defined to 1 
 *          to have message box support compiled in.@n
 *          ::POSCFG_MSG_MEMORY must be defined to 1
 *          to have this function compiled in.
 * @sa      ::posMessageFree, get, send, alloc
 */
    inline void free()
    {
      ::posMessageFree(msg);
      msg = NULL;
    }
#endif

/**
 * Sends a message to a task.
 * @param   task  the task to send the message to.
 * @return  zero on success. When an error condition exist, a
 *          negative value is returned and the message buffer is freed.
 * @note    ::POSCFG_FEATURE_MSGBOXES must be defined to 1 
 *          to have message box support compiled in.
 * @sa      ::posMessageSend, alloc, get
 */
    inline VAR_t send(const Task& task)
    {
      return ::posMessageSend(msg, task);
    }

/**
 * Gets a new message from the message box.
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
 * @sa      ::posMessageGet, free, available,
 *          wait, send
 */
    inline void* get()
    {
      msg = ::posMessageGet();
      return msg;
    }

/**
 * Tests if a new message is available
 * in the message box. This function can be used to prevent
 * the task from blocking.
 * @return  1 (=true) when a new message is available.
 *          Otherwise zero is returned. A negative value
 *          is returned on error.
 * @note    ::POSCFG_FEATURE_MSGBOXES must be defined to 1 
 *          to have message box support compiled in.
 * @sa      ::posMessageAvailable, get, wait
 */
    static inline VAR_t available()
    {
      return ::posMessageAvailable();
    }

#if (DOX!=0) || (POSCFG_FEATURE_MSGWAIT != 0)
/**
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
 * @sa      ::posMessageWait, free, get, available,
 *          send, HZ, MS
 */
    inline void* wait(UINT_t timeoutticks)
    {
      msg = ::posMessageWait(timeoutticks);
      return msg;
    }
#endif

/*
 * Assignment operators.
 */
    inline Message& operator=(const Message& other)
    {
      msg = other.msg;
      return *this;
    };

  private:
    void* msg;
  };
}

#if POSCFG_ENABLE_NANO != 0

namespace nos {

/**
 * Messagephore functions.
 */
  class Message : public pos::Message
  {
  public:

/*
 * Constructors.
 */
    inline Message()
    {
    };

#if DOX!=0 || NOSCFG_FEATURE_MSGBOXES != 0

/**
 * Allocates a new message buffer. To increase the execution speed,
 * it is recommended to set ::POSCFG_MSG_MEMORY to 1. Otherwise,
 * ::nosMessageAlloc will need to call ::nosMemAlloc to allocate memory
 * (and this is possibly slower than the pico]OS internal message allocator).
 * @n Usually the sending task would allocate a new message buffer, fill
 * in its data and send it via ::nosMessageSend to the receiving task.
 * The receiving task is responsible for freeing the message buffer again.
 * @param   msgSize   size of the requested message buffer in bytes.
 * @return  the pointer to the new buffer. NULL is returned if the
 *          system is low on memory or the requested msgSize is larger
 *          than ::POSCFG_MSG_BUFSIZE (only if ::POSCFG_MSG_MEMORY is
 *          set to 1).
 * @note    ::NOSCFG_FEATURE_MSGBOXES must be defined to 1
 *          to have message box support compiled in.@n
 *          If ::POSCFG_MSG_MEMORY is set to 0, you also need to
 *          enable the nano layer memory manager by setting
 *          ::NOSCFG_FEATURE_MEMALLOC to 1.
 * @sa      nosMessageSend, nosMessageGet, nosMessageFree
 */
    inline void* alloc()
    {
      msg = nosMessageAlloc();
      return msg;
    }

/**
 * Frees a message buffer again.
 * Usually the receiving task would call this function after
 * it has processed a message to free the message buffer again.
 * @param   buf  Pointer to the message buffer that is no more used.
 * @note    ::NOSCFG_FEATURE_MSGBOXES must be defined to 1
 *          to have message box support compiled in.
 * @sa      nosMessageGet, nosMessageSend, nosMessageAlloc
 */
    inline void free()
    {
      nosMessageFree(msg);
      msg = NULL;
    }

/**
 * Sends a message to a task.
 * @param   buf  Pointer to the message to send.
 *               The message buffer must have been allocated by
 *               calling ::nosMessageAlloc before.
 * @param   taskhandle  handle to the task to send the message to.
 * @return  zero on success. When an error condition exist, a
 *          negative value is returned and the message buffer is freed.
 * @note    ::NOSCFG_FEATURE_MSGBOXES must be defined to 1
 *          to have message box support compiled in.
 * @sa      nosMessageAlloc, nosMessageGet
 */
    inline VAR_t send(const Task& task)
    {
      return nosMessageSend(msg, task);
    }

#endif /* NOSCFG_FEATURE_MSGBOXES */

  };
}

#endif /* POSCFG_ENABLE_NANO */
#endif /* POSCFG_FEATURE_MSGBOXES */
#endif /* _PICOOS_MSG_HXX */
