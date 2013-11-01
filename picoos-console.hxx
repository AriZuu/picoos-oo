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
 * @brief   Pico]OS oo-wrapper Console
 * @author  Ari Suutari <ari@stonepile.fi>
 */

#ifndef _PICOOS_CONSOLE_HXX
#define _PICOOS_CONSOLE_HXX

extern "C" {

#include <picoos.h>

}

namespace nos {

#if DOX!=0 || NOSCFG_FEATURE_CONIN != 0 || NOSCFG_FEATURE_CONOUT != 0
/**
 * Console input and output. Typically uses serial port.
 */
  class Console
  {
  private:

/*
 * Constructors.
 */
    inline Console()
    {
    };

  public:
#if DOX!=0 || NOSCFG_FEATURE_CONIN != 0

/**
 * Wait and get the code of the next pressed key.
 * This function blocks until the user presses a key on the keyboard
 * and returns the code of the pressed key as result.
 * @return  ASCII-code of the pressed key
 * @note    ::NOSCFG_FEATURE_CONIN must be defined to 1
 *          to have this function compiled in.
 * @sa      ::nosKeyGet, keyPressed, ::c_nos_keyinput, ::NOSCFG_CONIO_KEYBUFSIZE
 */
    static inline char keyGet()
    {
      return ::nosKeyGet();
    }

/**
 * Test if a key was pressed.
 * This function tests if a key code is available in the keyboard buffer.
 * Even if no key is pressed yet, the function will immediately return.
 * @return  TRUE (nonzero) when a key code is available.
 * @note    ::NOSCFG_FEATURE_CONIN must be defined to 1
 *          to have this function compiled in.
 * @sa      ::nosKeyPressed, keyGet, ::c_nos_keyinput, ::NOSCFG_CONIO_KEYBUFSIZE
 */
    static inline UVAR_t keyPressed()
    {
      return ::nosKeyPressed();
    }

#endif  /* NOSCFG_FEATURE_CONIN */

#if DOX!=0 || NOSCFG_FEATURE_CONOUT != 0

/**
 * Print a character to the console or terminal.
 * This function prints a single character to the console.
 * No CR/LF conversion is performed.
 * @note    ::NOSCFG_FEATURE_CONOUT must be defined to 1
 *          to have this function compiled in.
 * @sa      ::nosPrintChar, print, ::p_putchar
 */
    static inline void printChar(char c)
    {
      ::nosPrintChar(c);
    }

/**
 * Print a character string to the console or terminal.
 * This function prints a string of characters (text) to the console.
 * A CR/LF conversion is performed, CR is preceding each LF.
 * @param   s  pointer to zero terminated ASCII string
 * @note    ::NOSCFG_FEATURE_CONOUT must be defined to 1
 *          to have this function compiled in.
 * @sa      ::nosPrint, printChar, ::p_putchar
 */
    static inline void print(const char *s)
    {
      ::nosPrint(s);
    }

#endif

  };

#endif  /* NOSCFG_FEATURE_CON* */
}

#endif /* _PICOOS_CONSOLE_HXX */
