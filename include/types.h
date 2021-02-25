#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

#define local       static
#define global      static
#define internal    static

typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;
typedef uint64_t    u64;

typedef int8_t      s8;
typedef int16_t     s16;
typedef int32_t     s32;
typedef int64_t     s64;

typedef float       f32;
typedef double      f64;

typedef size_t      usize;

#define U8_MAX      UINT8_MAX
#define U16_MAX     UINT16_MAX
#define U32_MAX     UINT32_MAX
#define U64_MAX     UINT64_MAX

#define S8_MAX      INT8_MAX
#define S16_MAX     INT16_MAX
#define S32_MAX     INT32_MAX
#define S64_MAX     INT64_MAX

#endif // TYPES_H
