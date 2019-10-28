/*
  Copyright (c) 2014, Leonardo Guilherme de Freitas
  All rights reserved.

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

     1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.

     2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.

     3. This notice may not be removed or altered from any source
     distribution.
*/

#ifndef KW_MACROS_H
#define KW_MACROS_H

/* this file was shamelessly copied from SDL */

/* Some compilers use a special export keyword */
#ifndef DECLSPEC
# if defined(__WIN32__) || defined (_MSC_VER)
#  ifdef __BORLANDC__
#   ifdef KiWi_EXPORTS
#    define DECLSPEC
#   else
#    define DECLSPEC    __declspec(dllimport)
#   endif
#  else
#   define DECLSPEC __declspec(dllexport)
#  endif
# else
#  if defined(__GNUC__) && __GNUC__ >= 4
#   define DECLSPEC __attribute__ ((visibility("default")))
#  elif defined(__GNUC__) && __GNUC__ >= 2
#   define DECLSPEC __declspec(dllexport)
#  else
#   define DECLSPEC
#  endif
# endif
#endif

/* By default SDL uses the C calling convention */
#ifndef KIWICALL
#if (defined(__WIN32__) || defined (_MSC_VER)) && !defined(__GNUC__)
#define KIWICALL __cdecl
#else
#define KIWICALL
#endif
#endif /* KIWICALL */

#endif
