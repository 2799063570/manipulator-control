#ifndef RSTYPE_H
#define RSTYPE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* General types */
typedef uint8_t boolean;
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;
typedef float float32;
typedef double float64;

typedef uint16_t RSHD; // robot servcie handle

#ifdef __cplusplus
}
#endif

#endif // RSTYPE_H
