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
 * @file    picoos-list.hxx
 * @brief   Pico]OS oo-wrapper List
 * @author  Ari Suutari <ari@stonepile.fi>
 */

#ifndef _PICOOS_LIST_HXX
#define _PICOOS_LIST_HXX

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

#if (DOX!=0) || (POSCFG_FEATURE_LISTS != 0)
/** @defgroup lists Lists
 * @ingroup userapip
 * Lists are multifunctional, often they are used for buffer queues or
 * other elements that need to be listed. pico]OS provides a set of
 * functions for managing nonblocking and blocking lists. @n
 * Nonblocking means that elements can be put to or taken from a list
 * without blocking the active task while an other task is also attempting
 * to access the list. This behaviour is very usefull for interrupt service
 * routines that need to send buffers through a queue to the
 * application task. @n
 * An example program that demonstrates the usage of lists is available
 * in the examples directory: lists.c
 * @{
 */

#define POSLIST_HEAD  0
#define POSLIST_TAIL  1

/**
 * List Function.
 * Adds an element to a list.
 * @param   listhead  pointer to the head of the list.
 * @param   pos       position where to add the element. Can be
 *                    POSLIST_HEAD to add the element to the head of
 *                    the list or POSLIST_TAIL to add the element to
 *                    the tail of the list.
 * @param   new       pointer to the list element to add.
 * @note    ::POSCFG_FEATURE_LISTS must be defined to 1 
 *          to have list support compiled in. @n
 *          Note that list heads must be initialized before elements
 *          can be added to the list.
 * @sa      posListGet, posListLen, posListRemove, posListJoin, posListInit
 */
    inline void posListAdd(POSLISTHEAD_t *listhead, UVAR_t pos,
                                  POSLIST_t *new);

/**
 * List Function.
 * Takes an element from a list.
 * @param   listhead  pointer to the head of the list.
 * @param   pos       position where to take the element from. Can be
 *                    POSLIST_HEAD to take the element from the head of
 *                    the list or POSLIST_TAIL to take the element from
 *                    the tail of the list.
 * @param   timeout   If timeout is set to zero, the function does not
 *                    wait for a new list element when the list is empty
 *                    (poll mode).
 *                    If timeout is set to INFINITE, the function waits
 *                    infinite (without timeout) for a new list element
 *                    to arrive. Any other value describes a timeout
 *                    in timerticks (see ::HZ and ::MS ). If the list
 *                    is still empty after the timeout ticks expired,
 *                    the function returns a NULL pointer.
 * @return  On success, this function returns the pointer to the
 *          element and the element is removed from the list. The
 *          function returns a NULL pointer when the list is empty
 *          (timeout == 0) or the timeout has expired (timeout != 0).
 * @note    ::POSCFG_FEATURE_LISTS must be defined to 1 
 *          to have list support compiled in. @n
 *          To be able to wait with timeout (timeout is set to nonzero and
 *          is not equal to INFINITE), the feature ::POSCFG_FEATURE_SEMAWAIT
 *          must be enabled. Note that only one task per time can wait
 *          for a new list element (timeout != 0). If multiple tasks attempt
 *          to wait with or without timeout for the same list, the behaviour
 *          of this function is undefined.
 * @sa      posListAdd, posListLen, posListRemove, posListJoin, posListInit
 */
    inline POSLIST_t* posListGet(POSLISTHEAD_t *listhead, UVAR_t pos,
                                        UINT_t timeout);

/**
 * List Function.
 * Removes an element from a list.
 * @param   listelem  pointer to the element to remove.
 * @note    ::POSCFG_FEATURE_LISTS must be defined to 1 
 *          to have list support compiled in.
 * @sa      posListAdd, posListGet, posListLen, posListJoin, posListInit
 */
    inline void posListRemove(POSLIST_t *listelem);

#if (DOX!=0) || (POSCFG_FEATURE_LISTJOIN != 0)
/**
 * List Function.
 * Joins two lists together. The elements contained in the joinlist 
 * are moved to the baselist. After this operation the joinlist is empty.
 * @param   baselisthead  pointer to the head of the list that shall
 *                        receive the elements of the second list.
 * @param   pos           position where the elements of the other list
 *                        shall be inserted. Can be POSLIST_HEAD to
 *                        insert the elements at the head of the
 *                        baselist or POSLIST_TAIL to insert the
 *                        elements at the tail of the baselist.
 * @param   joinlisthead  pointer to the list which contents shall be
 *                        moved to the baselist.
 * @note    ::POSCFG_FEATURE_LISTS must be defined to 1 
 *          to have list support compiled in. @n
 *          ::POSCFG_FEATURE_LISTJOIN must be defined to 1
 *          to have this function compiled in.
 * @sa      posListAdd, posListGet, posListJoin, posListInit
 */
    inline void posListJoin(POSLISTHEAD_t *baselisthead, UVAR_t pos,
                                   POSLISTHEAD_t *joinlisthead);
#endif

#if (DOX!=0) || (POSCFG_FEATURE_LISTLEN != 0)
/**
 * List Function.
 * Returns the length of a list.
 * @param   listhead  pointer to the head of the list.
 * @return  the length of the list
 * @note    ::POSCFG_FEATURE_LISTS must be defined to 1 
 *          to have list support compiled in. @n
 *          ::POSCFG_FEATURE_LISTLEN must be defined to 1
 *          to have this function compiled in.
 * @sa      posListAdd, posListGet, posListRemove
 */
    inline UINT_t posListLen(POSLISTHEAD_t *listhead);
#endif

/**
 * List Function.
 * Initializes the head of a list. This function must be called first
 * before elements can be added to the list.
 * @param   listhead    pointer to the listhead to initialize.
 * @note    ::POSCFG_FEATURE_LISTS must be defined to 1 
 *          to have list support compiled in. @n
 *          If a list is no more used, the function ::posListTerm
 *          should be called to free operating system resources.
 * @sa      posListTerm, posListAdd, posListGet
 */
    inline void posListInit(POSLISTHEAD_t *listhead);

/**
 * List Function.
 * Frees operating system resources when a list is no more needed.
 * @param   listhead    pointer to the head of the list.
 * @note    ::POSCFG_FEATURE_LISTS must be defined to 1 
 *          to have list support compiled in.
 */
    inline void posListTerm(POSLISTHEAD_t *listhead);

#if (DOX!=0)
/**
 * List Macro.
 * This macro enables the access to the data structure that is
 * linked with a list element.
 * @param   elem    pointer to the list element of type ::POSLIST_t
 * @param   type    type of the data structure where ::POSLIST_t is
 *                  an element from.
 * @param   member  the member name of the list element ::POSLIST_t
 *                  in the structure @e type.
 * @returns a pointer to the data structure where the list element
 *          is a member from.
 */
#define POSLIST_ELEMENT(elem, type, member)
#else
#define POSLIST_ELEMENT(elem, type, member) \
        ((type*)((char*)(elem)-(char*)(&((type*)NULL)->member)))
#endif

/** List Macro. Tests if a list is empty.
 * Pay attention to task synchronization when using this macro.
 */
#define POSLIST_IS_EMPTY(plisthead) \
          ((plisthead)->next == (POSLIST_t*)(plisthead))

/** List Macro. Returns a pointer to the next element in a list.
 * Pay attention to task synchronization when using this macro.
 */
#define POSLIST_NEXT_ENTRY(plist)               (plist)->next

/** List Macro. Returns a pointer to the previous element in a list.
 * Pay attention to task synchronization when using this macro.
 */
#define POSLIST_PREV_ENTRY(plist)               (plist)->prev

/** List Macro. Returns a pointer to the first entry of a list.
 * Pay attention to task synchronization when using this macro.
 */
#define POSLIST_FIRST_ENTRY(plisthead)          (plisthead)->next

/** List Macro. Returns a pointer to the last element of a list.
 * Pay attention to task synchronization when using this macro.
 */
#define POSLIST_LAST_ENTRY(plisthead)           (plisthead)->prev

/** List Macro. Tests if an element is the first one in a list.
 * Pay attention to task synchronization when using this macro.
 */
#define POSLIST_IS_FIRST_ENTRY(element) ((element)->prev==(element)->head)

/** List Macro. Tests if an element is the last one in a list.
 * Pay attention to task synchronization when using this macro.
 */
#define POSLIST_IS_LAST_ENTRY(element)  ((element)->next==(element)->head)

/** List Macro. Tests if the end of a list is reached when using a for-loop.
 */
#define POSLIST_IS_END(plisthead, element) \
          ((element)==(POSLIST_t*)(plisthead))


#if (DOX!=0)
/**
 * List Macro.
 * This macro expands to a for-loop that walks over all list entries
 * in the specified list. The body of the for-loop must be enclosured
 * in braces { }.
 * @param   plisthead   pointer to the head of the list
 * @param   runvar      run variable of type ::POSLIST_t. This variable
 *                      is the index variable in the for-loop.
 * @note    When using this macro you must pay attention about task
 *          synchronization. You may need to protect all list operations by
 *          a semaphore to ensure list integrity while executing this loop.
 * @note    It is not allowed to take an element from the list while
 *          being in the loop. But if you plan such an operation,
 *          please see the defintion of the macros
 *          ::POSLIST_FOREACH_BEGIN and ::POSLIST_FOREACH_END.
 */
#define POSLIST_FOR_EACH_ENTRY(plisthead, runvar)
#else
#define POSLIST_FOR_EACH_ENTRY(plisthead, runvar) \
  for (runvar = POSLIST_FIRST_ENTRY(plisthead);   \
       !POSLIST_IS_END(plisthead, runvar);        \
       runvar = POSLIST_NEXT_ENTRY(runvar))
#endif

#if (DOX!=0)
/**
 * List Macro.
 * This macro expands to a complex for-loop that walks over all list entries
 * in the specified list. This macro allows complex operations on the list
 * while being in the loop, and it simplifies the access to the data
 * structures that are linked to the list elements.
 * @param   plisthead   pointer to the head of the list
 * @param   runvar      run variable of the type your data structure is.
 *                      Note that this variable must be a structure pointer.
 *                      This variable is the index variable in the for-loop.
 * @param   type        type of your data structure where ::POSLIST_t is
 *                      an element from.
 * @param   listmember  the member name of the list element ::POSLIST_t
 *                      in your data structure @e type.
 * @note    When using this macro you must pay attention about task
 *          synchronization. You may need to protect all list operations by
 *          a semaphore to ensure list integrity while executing this loop.
 * @note    The end of the loop must be marked by the macro
 *          ::POSLIST_FOREACH_END.
 * @sa      POSLIST_FOREACH_END
 */
#define POSLIST_FOREACH_BEGIN(plisthead, runvar, type, listmember)
#else
#define POSLIST_FOREACH_BEGIN(plisthead, runvar, type, listmember) \
  do { POSLIST_t *r, *n; \
    for (r = POSLIST_FIRST_ENTRY(plisthead); \
         !POSLIST_IS_END(plisthead, r); r = n) { \
         n = POSLIST_NEXT_ENTRY(r); \
      runvar = POSLIST_ELEMENT(r, type, listmember);
#endif

#if (DOX!=0)
/**
 * List Macro.
 * This macro is the counterpart to ::POSLIST_FOREACH_BEGIN and must
 * be placed at the end of a for-loop that was initiated with
 * ::POSLIST_FOREACH_BEGIN.
 */
#define POSLIST_FOREACH_END
#else
#define POSLIST_FOREACH_END \
  } } while(0)
#endif

#endif /* POSCFG_FEATURE_LISTS */

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

#endif /* _PICOOS_LIST_HXX */
