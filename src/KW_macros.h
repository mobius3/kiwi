#ifndef KW_MACROS_H
#define KW_MACROS_H

/* this file was shamelessly copied from SDL */

/* Some compilers use a special export keyword */
#ifndef DECLSPEC
# if defined(__WIN32__)
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
#if defined(__WIN32__) && !defined(__GNUC__)
#define KIWICALL __cdecl
#else
#define KIWICALL
#endif
#endif /* KIWICALL */

#endif
