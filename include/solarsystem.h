#ifndef SOLARSYSTEM_H
#define SOLARSYSTEM_H

#include "GLFW/glfw3.h"
#include "mmath.h"

#define SCR_WIDTH			1280
#define SCR_HEIGHT			720
#define RING_AMOUNT			65000
#define AST_BELT_AMOUNT		3000
#define m_pi				3.141592654f

/////////////////////////////////////////////////////////////////////////
// CAMERA

typedef struct _TAG_camera
{
    vec3_t pos = {-130.0f, 35.0f, 110.0f};
    vec3_t front = {0.9f, -0.2f, -0.35f};
    vec3_t up = {0.0f, 1.0f, 0.0f};
    f32 mouse_sens = 0.08f;
    f32 speed = 0.85f;
} camera_t;

/////////////////////////////////////////////////////////////////////////
// GLFW FUNCTIONS

void framebuffer_size_callback(GLFWwindow *window, s32 width, s32 height);

void process_input(GLFWwindow *window);

void mouse_callback(GLFWwindow *window, f64 x_pos, f64 y_pos);

GLFWwindow *ss_glfw_init(void);

/////////////////////////////////////////////////////////////////////////
// COMPUTATIONS

vec3_t compute_rock_pos(f32 radius);

vec3_t compute_tilt_axis(f32 angle);

/////////////////////////////////////////////////////////////////////////
// LOADSHADER

#include <stdlib.h>

typedef enum _TAG_shader_enum
{
    VERTEX,
    FRAGMENT,
    PROGRAM
} shader_enum;

void check_compile_errors(u32 data, shader_enum type, const char *filename);

u32 load_shader(const char *vs_path, const char *fs_path);

/////////////////////////////////////////////////////////////////////////
// LOADTEXTURE

u32 load_texture(const char *filename);

/////////////////////////////////////////////////////////////////////////
// IMGUI

void imgui_init(GLFWwindow *window);

void imgui_newframe(void);

void imgui_render(void);

/////////////////////////////////////////////////////////////////////////
// OPENGL

void ss_fill_array_buffer(u32 attrib_index, u8 attrib_size, usize buffer_size, void *buffer_data);

void ss_fill_element_buffer(usize buffer_size, void *buffer_data);

#endif // SOLARSYSTEM_H
