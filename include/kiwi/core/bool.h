#ifndef KW_BOOL_H
#define KW_BOOL_H

/**
 * \file KW_bool.h
 *
 * A helper enumeration with true and false values
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  KW_FALSE = 0,
  KW_TRUE = 1
} KW_bool;

#ifdef __cplusplus
}
#endif

#endif
