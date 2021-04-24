#ifndef MMATH_H
#define MMATH_H

#include <stdio.h>
#include <math.h>
#include "types.h"

#define m_sin(n)    sinf(n)
#define m_cos(n)    cosf(n)
#define m_tan(n)    tanf(n)
#define m_cast(n)   (f32 *)&n
#define m_rads(n)   (n * 0.017453f)
#define m_degs(n)   (n * 57.29578f)

// TODO: Add RNG functions (randi, randf, etc.)
// TODO: Add Quake III inv sqrt function
//        -> maybe test more to see if this yields perf. improvements
// TODO: maybe sort functions in alphabetical order???

/* ============================ *
 * =====    Vector2D      ===== *
 * ============================ */

typedef struct _TAG_vec2
{
    f32 x;
    f32 y;
} vec2_t;

vec2_t      vec2_add(vec2_t v1, vec2_t v2);
vec2_t      vec2_sub(vec2_t v1, vec2_t v2);
vec2_t      vec2_scal(vec2_t vec, f32 scalar);
f32         vec2_dot(vec2_t v1, vec2_t v2);
f32         vec2_mag(vec2_t vec);
vec2_t      vec2_normalize(vec2_t vec);
vec2_t      vec2_rotate(vec2_t vec, f32 angle);

/* ============================ *
 * =====    Vector3D      ===== *
 * ============================ */

typedef struct _TAG_vec3
{
    f32 x;
    f32 y;
    f32 z;
} vec3_t;

vec3_t      vec3_add(vec3_t v1, vec3_t v2);
vec3_t      vec3_sub(vec3_t v1, vec3_t v2);
vec3_t      vec3_scal(vec3_t vec, f32 scalar);
f32         vec3_dot(vec3_t v1, vec3_t v2);
vec3_t      vec3_cross(vec3_t v1, vec3_t v2);
f32         vec3_mag(vec3_t vec);
vec3_t      vec3_normalize(vec3_t vec);

/* ============================ *
 * =====    MATRIX4       ===== *
 * ============================ */

typedef struct _TAG_mat4
{
    f32 col1[4];
    f32 col2[4];
    f32 col3[4];
    f32 col4[4];
} mat4_t;

mat4_t      mat4_identity(void);
mat4_t      mat4_translate(f32 x, f32 y, f32 z);
mat4_t      mat4_translate_v(vec3_t vec);
mat4_t      mat4_translate_remove(mat4_t matrix);
void        mat4_print(mat4_t matrix);
mat4_t      mat4_rotate(f32 angle, f32 x, f32 y, f32 z);
mat4_t      mat4_rotate_v(f32 angle, vec3_t vec);
mat4_t      mat4_perspective(f32 fov, f32 aspect_ratio, f32 near, f32 far);
mat4_t      mat4_lookat(vec3_t eye, vec3_t center, vec3_t up);
mat4_t      mat4_scale(f32 scale_value);
mat4_t      mat4_mult(mat4_t m1, mat4_t m2);

/* ============================ *
 * =====      MISC        ===== *
 * ============================ */

u32         m_randi(u32 index);
f32         m_randf(u32 index);
f32         m_sqrt(f32 number);
f32         m_isqrt(f32 number);

////////////////////////////////////////////////////////////////////////////////
// ====== MMATH IMPLEMENTATION ================================================/
////////////////////////////////////////////////////////////////////////////////

#ifdef MMATH_IMPL

////////////////////////////////////////////////////////////////////////////////
// VECTOR2D IMPLEMENTATION

vec2_t      
vec2_add(vec2_t v1, 
         vec2_t v2)
{
    v1.x += v2.x;
    v1.y += v2.y;

    return v1;
}

vec2_t      
vec2_sub(vec2_t v1, 
         vec2_t v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;

    return v1;
}

vec2_t      
vec2_scal(vec2_t vec, 
          f32 scalar)
{
    vec.x *= scalar;
    vec.y *= scalar;

    return vec;
}

f32         
vec2_dot(vec2_t v1, 
         vec2_t v2)
{
    return ((v1.x * v2.x) + (v1.y * v2.y));
}

f32         
vec2_mag(vec2_t vec)
{
    return m_sqrt((vec.x * vec.x) + (vec.y * vec.y));
}

vec2_t      
vec2_normalize(vec2_t vec)
{
    f32 val = m_isqrt((vec.x * vec.x) + (vec.y * vec.y));

    vec.x *= val;
    vec.y *= val;

    return vec;
}

vec2_t      
vec2_rotate(vec2_t vec, 
            f32 angle)
{
    f32 r_angle = m_rads(angle);
    f32 c = m_cos(r_angle);
    f32 s = m_sin(r_angle);

    vec.x = ((vec.x * c) - (vec.y * s));
    vec.y = ((vec.x * s) + (vec.y * c));

    return vec;
}

////////////////////////////////////////////////////////////////////////////////
// VECTOR3D IMPLEMENTATION

vec3_t      
vec3_add(vec3_t v1, 
         vec3_t v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
    v1.z += v2.z;

    return v1;
}

vec3_t      
vec3_sub(vec3_t v1, 
         vec3_t v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;
    v1.z -= v2.z;

    return v1;
}

vec3_t      
vec3_scal(vec3_t vec, 
          f32 scalar)
{
    vec.x *= scalar;
    vec.y *= scalar;
    vec.z *= scalar;

    return vec;
}

f32     
vec3_dot(vec3_t v1, 
         vec3_t v2)
{
    return ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z));
}

vec3_t      
vec3_cross(vec3_t v1,
           vec3_t v2)
{
    vec3_t cross_prod;
    
    cross_prod.x = (v1.y * v2.z) - (v1.z * v2.y);
    cross_prod.y = (v1.z * v2.x) - (v1.x * v2.z);
    cross_prod.z = (v1.x * v2.y) - (v1.y * v2.x);

    return cross_prod;
}

f32
vec3_mag(vec3_t vec)
{
    return m_sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
}

vec3_t 
vec3_normalize(vec3_t vec)
{
    f32 val = m_isqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));

    vec.x *= val;
    vec.y *= val;
    vec.z *= val;

    return vec;
}

////////////////////////////////////////////////////////////////////////////////
// MATRIX4 IMPLEMENTATION

mat4_t
mat4_identity(void)
{
    mat4_t matrix = {0};
    f32 *ptr = m_cast(matrix);

    for (u8 i = 0; i < 4; i++)
    {
        *ptr = 1.0f;
        ptr += 5;
    }

    return matrix;
}

mat4_t
mat4_translate(f32 x,
               f32 y,
               f32 z)
{
    mat4_t matrix = mat4_identity();

    matrix.col4[0] = x;
    matrix.col4[1] = y;
    matrix.col4[2] = z;

    return matrix;
}

mat4_t
mat4_translate_v(vec3_t vec)
{
    mat4_t matrix = mat4_identity();

    matrix.col4[0] = vec.x;
    matrix.col4[1] = vec.y;
    matrix.col4[2] = vec.z;

    return matrix;
}

mat4_t
mat4_translate_remove(mat4_t matrix)
{
    for (u8 i = 0; i < 3; i++)
    {
        matrix.col4[i] = 0.0f;
    }

    return matrix;
}

void
mat4_print(mat4_t matrix)
{
    f32 *ptr;

    for (u8 i = 0; i < 4; i++)
    {
        ptr = &matrix.col1[i];
        for (u8 j = 0; j < 4; j++)
        {
            printf("%f ", *ptr);
            ptr += 4;
        }
        printf("\n");
    }
} 

mat4_t
mat4_rotate(f32 angle, 
            f32 x, 
            f32 y, 
            f32 z)
{
    vec3_t vec = {x, y, z};
    vec = vec3_normalize(vec);
    f32 c = m_cos(m_rads(angle));
    f32 s = m_sin(m_rads(angle));
    f32 c1 = 1 - c;

    mat4_t matrix = {0};

    matrix.col1[0] = (c1 * vec.x * vec.x) + c;
    matrix.col1[1] = (c1 * vec.x * vec.y) + s * vec.z;
    matrix.col1[2] = (c1 * vec.x * vec.z) - s * vec.y;

    matrix.col2[0] = (c1 * vec.x * vec.y) - s * vec.z;
    matrix.col2[1] = (c1 * vec.y * vec.y) + c;
    matrix.col2[2] = (c1 * vec.y * vec.z) + s * vec.x;

    matrix.col3[0] = (c1 * vec.x * vec.z) + s * vec.y;
    matrix.col3[1] = (c1 * vec.y * vec.z) - s * vec.x;
    matrix.col3[2] = (c1 * vec.z * vec.z) + c;

    matrix.col4[3] = 1.0f;

    return matrix;
}

mat4_t
mat4_rotate_v(f32 angle,
              vec3_t vec)
{
    vec = vec3_normalize(vec);
    f32 c = m_cos(m_rads(angle));
    f32 s = m_sin(m_rads(angle));
    f32 c1 = 1 - c;

    mat4_t matrix = {0};

    matrix.col1[0] = (c1 * vec.x * vec.x) + c;
    matrix.col1[1] = (c1 * vec.x * vec.y) + s * vec.z;
    matrix.col1[2] = (c1 * vec.x * vec.z) - s * vec.y;

    matrix.col2[0] = (c1 * vec.x * vec.y) - s * vec.z;
    matrix.col2[1] = (c1 * vec.y * vec.y) + c;
    matrix.col2[2] = (c1 * vec.y * vec.z) + s * vec.x;

    matrix.col3[0] = (c1 * vec.x * vec.z) + s * vec.y;
    matrix.col3[1] = (c1 * vec.y * vec.z) - s * vec.x;
    matrix.col3[2] = (c1 * vec.z * vec.z) + c;

    matrix.col4[3] = 1.0f;

    return matrix;
}

mat4_t
mat4_perspective(f32 fov,
                 f32 aspect_ratio,
                 f32 near,
                 f32 far)
{
    f32 t = m_tan(m_rads(fov) / 2.0f);
    f32 fdelta = far - near;
    
    mat4_t matrix = {0};

    matrix.col1[0] = 1 / (aspect_ratio * t);

    matrix.col2[1] = 1 / t;

    matrix.col3[2] = -1 * ((far + near) / fdelta);
    matrix.col3[3] = -1;

    matrix.col4[2] = ((-2.0f * far * near) / fdelta);

    return matrix;
}

mat4_t
mat4_lookat(vec3_t eye, 
            vec3_t center, 
            vec3_t up)
{
    const vec3_t f = vec3_normalize(vec3_sub(center, eye));  
    const vec3_t s = vec3_normalize(vec3_cross(f, up));
    const vec3_t u = vec3_cross(s, f);

    mat4_t matrix;

    matrix.col1[0] = s.x;
    matrix.col1[1] = u.x;
    matrix.col1[2] = -f.x;
    matrix.col1[3] = 0;

    matrix.col2[0] = s.y;
    matrix.col2[1] = u.y;
    matrix.col2[2] = -f.y;
    matrix.col2[3] = 0;

    matrix.col3[0] = s.z;
    matrix.col3[1] = u.z;
    matrix.col3[2] = -f.z;
    matrix.col3[3] = 0;

    matrix.col4[0] = -vec3_dot(s, eye);
    matrix.col4[1] = -vec3_dot(u, eye);
    matrix.col4[2] = vec3_dot(f, eye);
    matrix.col4[3] = 1;

    return matrix;
}

mat4_t
mat4_scale(f32 scale_value)
{
    mat4_t matrix = {0};
    f32 *ptr = m_cast(matrix);

    for (u8 i = 0; i < 3; i++)
    {
        *ptr = scale_value;
        ptr += 5;
    }
    *ptr = 1.0f;

    return matrix;
}

mat4_t
mat4_mult(mat4_t m1,
          mat4_t m2)
{
    mat4_t res;
    f32 *p1 = m_cast(m1);
    f32 *p2 = m_cast(m2);
    f32 *pres = m_cast(res);

    for (u8 y = 0; y < 4; y++)
    {
        for (u8 x = 0; x < 4; x++)
        {
            f32 sum = 0.0f;
            for (u8 e = 0; e < 4; e++)
            {
                sum += p1[x + e * 4] * p2[e + y * 4];
            }
            pres[x + y * 4] = sum;
        }
    }

    return res;
}

////////////////////////////////////////////////////////////////////////////////
// MISC IMPLEMENTATION

u32
m_randi(u32 index)
{
    index = (index << 13) ^ index;
    return ((index * (index * index * 15731 + 789221) + 1376312589) & 0x7FFFFFFF);
}

f32
m_randf(u32 index)
{
    index = (index << 13) ^ index;
    return (((index * (index * index * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f) * 0.5f;
}

f32
m_sqrt(f32 number)
{
    int     i;
    f32     x2, y;

    x2 = number * 0.5f;
    y = number;
    i = *(int *)&y;                 // evil floating point bit hack
    i = 0x5F3759DF - (i >> 1);      // what the fuck?
    y = *(f32 *)&i;
    y = y * (1.5f - (x2 * y * y));  // 1st iteration
    y = y * (1.5f - (x2 * y * y));  // 2nd iteration

    return number * y;
}

f32
m_isqrt(f32 number)
{
    int     i;
    f32     x2, y;

    x2 = number * 0.5f;
    y = number;
    i = *(int *)&y;                 // evil floating point bit hack
    i = 0x5F3759DF - (i >> 1);      // what the fuck?
    y = *(f32 *)&i;
    y = y * (1.5f - (x2 * y * y));  // 1st iteration
    y = y * (1.5f - (x2 * y * y));  // 2nd iteration

    return y;               // multiply by original num to reverse and get sqrt
}

#endif // MMATH_IMPL

#endif // MMATH_H

