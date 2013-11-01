/*
 * Copyright (c) 2013, Ari Suutari <ari@stonepile.fi>.
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
 * @file    picoos.hxx
 * @brief   Pico]OS oo-wrapper
 * @author  Ari Suutari <ari@stonepile.fi>
 */

/**
 * @mainpage picoos-oo oo-wrapper for pico]OS
 *
 * <b> Table Of Contents </b>@n@htmlonly<font size="-1">@endhtmlonly
 * <ul><li>@ref overview</li>
 *     <li>@ref ::pos "Object-oriented wrapper for pico layer"</li>
 *     <li>@ref ::nos "Object-oriented wrapper for nano layer"</li>
 * </ul>@htmlonly</font>@endhtmlonly<hr>
 *
 * @section overview Overview
 * This library contains C++ API for pico]OS pico & nano layers. It is
 * mostly implemented using inline methods to achieve small code size
 * (compiler optimizes the result in most cases to direct pico]OS C-api call).
 *
 * API has been divided into separate ::pos and ::nos namespaces.
 * Class names correspond to functionality groups available in
 * C-api, ie. posTaskCreate becomes method called "create" in class "Task" 
 * in namespace "pos".
 */

#ifndef _PICOOS_HXX
#define _PICOOS_HXX

extern "C" {

#include <picoos.h>

}


/**
 * @brief Object-oriented wrapper for pico layer
 */

namespace pos {
}

/**
 * @brief Object-oriented wrapper for nano layer
 */

namespace nos {
}

#include <picoos-task.hxx>

#include <picoos-atomic.hxx>
#include <picoos-flag.hxx>
#include <picoos-msg.hxx>
#include <picoos-mutex.hxx>
#include <picoos-sema.hxx>
#include <picoos-softint.hxx>
#include <picoos-timer.hxx>

#endif /* _PICOOS_HXX */
