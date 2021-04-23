#include "../include/glad/glad.h"
#define MMATH_IMPL
#include "../include/mmath.h"
#include "../include/imgui/imgui.h"
#include "../include/imgui/imgui_impl_glfw.h"
#include "../include/imgui/imgui_impl_opengl3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

#include "../include/solarsystem.h"

f32 ss_time;
f32 ss_time_factor;
f32 ss_rot_factor;
bool imgui_control;
bool sim_running;
camera_t ss_camera;

/////////////////////////////////////////////////////////////////////////
// GLFW FUNCTIONS

void 
framebuffer_size_callback(GLFWwindow *window,
						  s32 width,
						  s32 height)
{
    glViewport(0, 0, width, height);
}

void 
process_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        imgui_control = true;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        imgui_control = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    if (glfwGetKey(window, GLFW_KEY_W))
    {
        ss_camera.pos = vec3_add(ss_camera.pos, 
                                 vec3_scal(ss_camera.front, ss_camera.speed));
    }
    if (glfwGetKey(window, GLFW_KEY_S))
    {
        ss_camera.pos = vec3_sub(ss_camera.pos, 
                                 vec3_scal(ss_camera.front, ss_camera.speed));
    }
    if (glfwGetKey(window, GLFW_KEY_A))
    {
        vec3_t temp = vec3_cross(ss_camera.front, ss_camera.up);
        temp = vec3_normalize(temp);
        temp = vec3_scal(temp, ss_camera.speed);
        ss_camera.pos = vec3_sub(ss_camera.pos, temp);
    }
    if (glfwGetKey(window, GLFW_KEY_D))
    {
        vec3_t temp = vec3_cross(ss_camera.front, ss_camera.up);
        temp = vec3_normalize(temp);
        temp = vec3_scal(temp, ss_camera.speed);
        ss_camera.pos = vec3_add(ss_camera.pos, temp);
    }
}

void
mouse_callback(GLFWwindow *window,
			   f64 x_pos,
			   f64 y_pos)
{
    static bool first_mouse = true;
    static f64 yaw = -90.0f;
    static f64 pitch = 0.0f;
    static f64 last_x = SCR_WIDTH / 2.0;
    static f64 last_y = SCR_HEIGHT / 2.0;

    if (!imgui_control)
    {
        if (first_mouse)
        {
            last_x = x_pos;
            last_y = y_pos;
            first_mouse = false;
        }

        f64 x_off = x_pos - last_x;
        f64 y_off = last_y - y_pos;
        last_x = x_pos;
        last_y = y_pos;

        x_off *= ss_camera.mouse_sens;
        y_off *= ss_camera.mouse_sens;

        yaw += x_off;
        pitch += y_off;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        vec3_t direction;
        direction.x = m_cos(m_rads((f32)yaw)) * m_cos(m_rads((f32)pitch));
        direction.y = m_sin(m_rads((f32)pitch));
        direction.z = m_sin(m_rads((f32)yaw)) * m_cos(m_rads((f32)pitch));

        ss_camera.front = vec3_normalize(direction);
    }
    else
    {
        first_mouse = true;
    }
}

GLFWwindow*
ss_glfw_init(void)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,
                                          "Solar System", NULL, NULL);
    if (!window)
    {
        printf("failed to create window!\n");
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
	glfwSwapInterval(0);
    glfwSetFramebufferSizeCallback(window, &framebuffer_size_callback);
    glfwSetCursorPosCallback(window, &mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("failed to initialize GLAD!\n");
        glfwTerminate();
        return NULL;
    }

    return window;
}

/////////////////////////////////////////////////////////////////////////
// COMPUTATIONS

vec3_t
compute_rock_pos(f32 radius)
{
    vec3_t pos;

    pos.x = radius * m_sin(0.0082f * ss_time);
    pos.y = 0.0f;
    pos.z = 0.8f * radius * m_cos(0.0082f * ss_time);

    return pos;
}

vec3_t 
compute_tilt_axis(f32 angle)
{
    vec3_t axis;

    axis.x = m_sin(m_rads(angle));
    axis.y = m_cos(m_rads(angle));
    axis.z = 0.0f;

    return axis;
}

/////////////////////////////////////////////////////////////////////////
// LOADSHADER

void 
check_compile_errors(u32 data, 
                     shader_enum type,
                     const char *filename)
{
    s32 success;
    char info_log[1024];
    if (type == PROGRAM)
    {
        glGetProgramiv(data, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(data, 1024, NULL, info_log);
            printf("PROGRAM LINKING ERROR OF TYPE:\n %s\n\n", info_log);
        }
    }
    else if (type == VERTEX || type == FRAGMENT)
    {
        glGetShaderiv(data, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(data, 1024, NULL, info_log);
            printf("SHADER COMPILATION ERROR OF TYPE: %s\n %s\n\n", filename, info_log);
        }
    }
    else
    {
        printf("INVALID SHADER TYPE\n");
    }
}

u32 
load_shader(const char *vs_path, 
            const char *fs_path)
{
    FILE *fptr;
    errno_t err;
    s32 file_len;
    GLchar *shader_source;
        
    // VERTEX SHADER
    err = fopen_s(&fptr, vs_path, "rb");
    fseek(fptr, 0, SEEK_END);
    file_len = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);
    shader_source = (GLchar *)malloc(file_len + 1);
    fread(shader_source, 1, file_len, fptr);
    shader_source[file_len] = 0;
    fclose(fptr);

    u32 vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &shader_source, NULL);
    glCompileShader(vertex);
    check_compile_errors(vertex, VERTEX, vs_path);
    free(shader_source);

    // FRAGMENT SHADER
    err = fopen_s(&fptr, fs_path, "rb");
    fseek(fptr, 0, SEEK_END);
    file_len = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);
    shader_source = (GLchar *)malloc(file_len + 1);
    fread(shader_source, 1, file_len, fptr);
    shader_source[file_len] = 0;
    fclose(fptr);

    u32 fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &shader_source, NULL);
    glCompileShader(fragment);
    check_compile_errors(fragment, FRAGMENT, fs_path);
    free(shader_source);

    // SHADER PROGRAM
    u32 program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    check_compile_errors(program, PROGRAM, vs_path);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program;
}

/////////////////////////////////////////////////////////////////////////
// LOADTEXTURE

u32
load_texture(const char *filename)
{
    u32 texture_id;
    glCreateTextures(GL_TEXTURE_2D, 1, &texture_id);
    glBindTextureUnit(0, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    s32 width, height, num_comps;
    u8 *texture_data = stbi_load(filename, &width, &height, &num_comps, 0);
    if (texture_data)
    {
        GLenum format = GL_RGB;
        if (num_comps == 4)
        {
            format = GL_RGBA;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height,
                     0, format, GL_UNSIGNED_BYTE, texture_data);
        stbi_image_free(texture_data);
    }
    else
    {
        printf("failed to load texture from: %s\n", filename);
    }

    return texture_id;
}

/////////////////////////////////////////////////////////////////////////
// IMGUI

void
imgui_init(GLFWwindow *window)
{
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
}

void
imgui_newframe(void)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void
imgui_render(void)
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    ImGui::EndFrame();
}

/////////////////////////////////////////////////////////////////////////
// OPENGL

void
ss_fill_array_buffer(u32 attrib_index,
                     u8 attrib_size,
                     usize buffer_size,
                     void *buffer_data)
{
    u32 buffer;
    glCreateBuffers(1, &buffer);
    glNamedBufferStorage(buffer,
                         buffer_size,
                         buffer_data,
                         GL_DYNAMIC_STORAGE_BIT);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glVertexAttribPointer(attrib_index, attrib_size,
                          GL_FLOAT, GL_FALSE,
                          0, (void *)0);
    glEnableVertexAttribArray(attrib_index);
}

void
ss_fill_element_buffer(usize buffer_size,
                       void *buffer_data)
{
    u32 buffer;
    glCreateBuffers(1, &buffer);
    glNamedBufferStorage(buffer,
                         buffer_size,
                         buffer_data,
                         GL_DYNAMIC_STORAGE_BIT);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
}
