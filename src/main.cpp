#include "../include/solarsystem.h"
#include "../include/planet.hpp"
#include "../include/model.h"

int
main(void)
{
    /////////////////////////////////////////////////////////////////////////
    // SETTING UP GLFW

    GLFWwindow *window = ss_glfw_init();

    /////////////////////////////////////////////////////////////////////////
    // SHADER CONFIGURATION

    u32 planet_shader = load_shader("..\\src\\shaders\\planet.vert",
                             "..\\src\\shaders\\planet.frag");
    u32 sun_shader = load_shader("..\\src\\shaders\\sun.vert",
                                 "..\\src\\shaders\\sun.frag");
    u32 skybox_shader = load_shader("..\\src\\shaders\\skybox.vert",
                                    "..\\src\\shaders\\skybox.frag");
    u32 ring_shader = load_shader("..\\src\\shaders\\ring.vert",
                                  "..\\src\\shaders\\ring.frag");
    u32 belt_shader = load_shader("..\\src\\shaders\\belt.vert",
                                  "..\\src\\shaders\\belt.frag");

    /////////////////////////////////////////////////////////////////////////
    // SPHERES

    std::vector<f32> vertices;
    std::vector<f32>().swap(vertices);
    std::vector<f32> tex_coords;
    std::vector<f32>().swap(tex_coords);
    std::vector<f32> normals;
    std::vector<f32>().swap(normals);

    f32 x, y, z, xy;
    f32 s, t;

    f32 sector_count = 36;
    f32 stack_count = 18;
    f32 sector_step = (2 * m_pi) / sector_count;
    f32 stack_step = m_pi / stack_count;
    f32 sector_angle, stack_angle;

    for (s32 i = 0; i <= stack_count; i++)
    {
        stack_angle = (m_pi / 2) - (i * stack_step);
        xy = m_cos(stack_angle);
        z = m_sin(stack_angle);

        for (s32 j = 0; j <= sector_count; j++)
        {
            sector_angle = j * sector_step;

            // vertex coords
            x = xy * m_cos(sector_angle);
            y = xy * m_sin(sector_angle);
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // tex coords
            s = (f32)j / sector_count;
            t = (f32)i / stack_count;
            tex_coords.push_back(s);
            tex_coords.push_back(t);

            // normals
            normals.push_back(x);
            normals.push_back(y);
            normals.push_back(z);

        }
    }

    std::vector<u32> indices;
    std::vector<u32>().swap(indices);

    s32 k1, k2;
    for (u8 i = 0; i < stack_count; i++)
    {
        k1 = i * (sector_count + 1);
        k2 = k1 + (sector_count + 1);

        for (u8 j = 0; j < sector_count; j++, k1++, k2++)
        {
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            if (i != (stack_count - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////
    // INSTANCES

    mat4_t *ring_inst_matrices = (mat4_t *)malloc(sizeof(mat4_t) * RING_AMOUNT);
    srand((u32)glfwGetTime());
    f32 radius = 13.5f;
    f32 offset = 2.75f;
    for (u32 i = 0; i < RING_AMOUNT; i++)
    {
        mat4_t model;

        // translation
        f32 angle = (f32)i / (f32)RING_AMOUNT * 360.0f;
        f32 disp = (rand() % (s32)(2 * offset * 100)) / 100.0f - offset;
        f32 mat_x = m_sin(angle) * radius + disp;
        disp = (rand() % (s32)(2 * offset * 100)) / 100.0f - offset;
        f32 mat_y = disp * 0.05f;
        disp = (rand() % (s32)(2 * offset * 100)) / 100.0f - offset;
        f32 mat_z = m_cos(angle) * radius + disp;
        model = mat4_translate(mat_x, mat_y, mat_z);

        // scale
        f32 scale_value = (rand() % 10) / 250.0f;
        mat4_t scale = mat4_scale(scale_value);

        // rotation
        f32 rot_angle = (f32)(rand() % 360);
        mat4_t rotate = mat4_rotate_v(rot_angle, {0.4f, 0.6f, 0.8f});

        scale = mat4_mult(scale, rotate);
        model = mat4_mult(model, scale);

        ring_inst_matrices[i] = model;
    }

    mat4_t *astbelt_inst_matrices = (mat4_t *)malloc(sizeof(mat4_t) * AST_BELT_AMOUNT);
    srand((u32)glfwGetTime());
    radius = 77.5f;
    offset = 2.5f;
    for (u32 i = 0; i < AST_BELT_AMOUNT; i++)
    {
        mat4_t model;

        // translation
        f32 angle = (f32)i / (f32)AST_BELT_AMOUNT * 360.0f;
        f32 disp = (rand() % (s32)(2 * offset * 100)) / 100.0f - offset;
        f32 mat_x = m_sin(angle) * radius + disp;
        disp = (rand() % (s32)(2 * offset * 100)) / 100.0f - offset;
        f32 mat_y = disp * 0.4f;
        disp = (rand() % (s32)(2 * offset * 100)) / 100.0f - offset;
        f32 mat_z = m_cos(angle) * radius + disp;
        model = mat4_translate(mat_x, mat_y, mat_z);

        // scale
        f32 scale_value = (rand() % 20) / 100.0f;
        mat4_t scale = mat4_scale(scale_value);

        // rotation
        f32 rot_angle = (f32)(rand() % 360);
        mat4_t rotate = mat4_rotate_v(rot_angle, {0.4f, 0.6f, 0.8f});

        scale = mat4_mult(scale, rotate);
        model = mat4_mult(model, scale);

        astbelt_inst_matrices[i] = model;
    }

    /////////////////////////////////////////////////////////////////////////
    // ROCK

    model_t ring = model_t("..\\assets\\rock\\rock.obj");
    model_t belt = model_t("..\\assets\\rock\\rock.obj");

    /////////////////////////////////////////////////////////////////////////
    // BUFFER OBJECTS

    // Sphere / Planet vertex data
    u32 vao;
    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);
    ss_fill_array_buffer(0, 3, vertices.size() * sizeof(f32), vertices.data());
    ss_fill_array_buffer(1, 2, tex_coords.size() * sizeof(f32), tex_coords.data());
    ss_fill_array_buffer(2, 3, normals.size() * sizeof(f32), normals.data());
    ss_fill_element_buffer(indices.size() * sizeof(u32), indices.data());

    f32 skybox_vertices[] = 
    {
        -1000.0f, -1000.0f, -1000.0f,
        -1000.0f,  1000.0f, -1000.0f,
         1000.0f, -1000.0f, -1000.0f,
         1000.0f,  1000.0f, -1000.0f,
         1000.0f, -1000.0f,  1000.0f,
         1000.0f,  1000.0f,  1000.0f,
        -1000.0f, -1000.0f,  1000.0f,
        -1000.0f,  1000.0f,  1000.0f,
    };
    u32 skybox_indices[] =
    {
        0, 1, 2,
        2, 1, 3,
        2, 3, 4, 
        4, 3, 5,
        4, 5, 6,
        6, 5, 7,
        6, 7, 0,
        0, 7, 1,
        6, 0, 2,
        2, 4, 6,
        7, 5, 3,
        7, 3, 1
    };

    // Skybox vertex data
    u32 skybox_vao;
    glCreateVertexArrays(1, &skybox_vao);
    glBindVertexArray(skybox_vao);
    ss_fill_array_buffer(0, 3, sizeof(skybox_vertices), skybox_vertices);
    ss_fill_element_buffer(sizeof(skybox_indices), skybox_indices);

    glBindVertexArray(ring.meshes[0].vao);
    ss_fill_array_buffer(2, 4, sizeof(mat4_t) * RING_AMOUNT, ring_inst_matrices);
    for (u8 i = 0; i < 4; i++)
    {
        glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE,
                              sizeof(mat4_t), (void *)(16 * i));
        glEnableVertexAttribArray(2 + i);
        glVertexAttribDivisor(2 + i, 1);
    }

    glBindVertexArray(belt.meshes[0].vao);
    ss_fill_array_buffer(2, 4, sizeof(mat4_t) * AST_BELT_AMOUNT, astbelt_inst_matrices);
    for (u8 i = 0; i < 4; i++)
    {
        glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE,
                              sizeof(mat4_t), (void *)(16 * i));
        glEnableVertexAttribArray(2 + i);
        glVertexAttribDivisor(2 + i, 1);
    }
    
    /////////////////////////////////////////////////////////////////////////
    // SKYBOX

    u32 skybox_texture;
    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &skybox_texture);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    s32 width, height, num_comps;
    u8 *data = stbi_load("../assets/skybox/space3.jpg", 
                         &width, &height, &num_comps, 0);
    if (data)
    {
        for (u32 i = 0; i < 6; i++)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height,
                         0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }
    else
    {
        printf("failed to load cubemap texture!\n");
    }

    /////////////////////////////////////////////////////////////////////////
    // TRANSFORMATIONS

    mat4_t model = mat4_identity();
    mat4_t view;
    mat4_t project = mat4_perspective(45.0f, 16.0f / 9.0f, 0.1f, 2000.0f);
    mat4_t tilt = mat4_rotate(-26.7f, 0.0f, 0.0f, 1.0f);

    /////////////////////////////////////////////////////////////////////////
    // PLANETS

    planet_t sun = planet_t(0.0f, 0.0f, 20.0f, indices.size(),
                            "../assets/sun.jpg", 0.0f, 0.0f);
    planet_t mercury = planet_t(30.0f, 1.0f, 1.0f, indices.size(),
                                "../assets/mercury.jpg", 0.0f, 0.0171f);
    planet_t venus = planet_t(42.0f, 0.39f, 2.85f, indices.size(),
                              "../assets/venus.jpg", -177.4f, 0.0041f);
    planet_t earth = planet_t(55.0f, 0.24f, 3.0f, indices.size(),
                              "../assets/earth2048.bmp", -23.0f, 1.0f);
    planet_t mars = planet_t(68.0f, 0.13f, 1.5f, indices.size(),
                             "../assets/mars.jpg", -25.2f, 0.9756f);
    planet_t jupiter = planet_t(125.0f, 0.020f, 11.0f, indices.size(),
                                "../assets/jupiter.jpg", -3.1f, 2.4202f);
    planet_t saturn = planet_t(165.0f, 0.0082f, 9.0f, indices.size(),
                               "../assets/saturn.jpg", -26.7f, 2.2479f);
    planet_t uranus = planet_t(195.0f, 0.0029f, 4.0f, indices.size(),
                               "../assets/uranus.jpg", -97.8f, 1.3936f);
    planet_t neptune = planet_t(209.0f, 0.0015f, 3.88f, indices.size(),
                                "../assets/neptune.jpg", -28.3f, 0.6666f);
    planet_t pluto = planet_t(218.0f, 0.00097f, 0.19f, indices.size(),
                              "../assets/pluto.jpg", -122.5f, 0.1569f);

    /////////////////////////////////////////////////////////////////////////
    // SHADER LOCS
    
    u32 sun_shader_view_loc = glGetUniformLocation(sun_shader, "view");
    u32 sun_shader_project_loc = glGetUniformLocation(sun_shader, "project");

    u32 planet_shader_view_loc = glGetUniformLocation(planet_shader, "view");
    u32 planet_shader_project_loc = glGetUniformLocation(planet_shader, "project");

    u32 ring_shader_view_loc = glGetUniformLocation(ring_shader, "view");
    u32 ring_shader_project_loc = glGetUniformLocation(ring_shader, "project");
    u32 ring_shader_tilt_loc = glGetUniformLocation(ring_shader, "tilt");
    u32 ring_shader_translate_loc = glGetUniformLocation(ring_shader, "translate");
    u32 ring_shader_rotate_loc = glGetUniformLocation(ring_shader, "rotate");

    u32 skybox_shader_view_loc = glGetUniformLocation(skybox_shader, "view");
    u32 skybox_shader_project_loc = glGetUniformLocation(skybox_shader, "project");

    u32 belt_shader_view_loc = glGetUniformLocation(belt_shader, "view");
    u32 belt_shader_project_loc = glGetUniformLocation(belt_shader, "project");

    glUseProgram(sun_shader);
    glUniformMatrix4fv(sun_shader_project_loc, 1, GL_FALSE, m_cast(project));
    glUseProgram(planet_shader);
    glUniformMatrix4fv(planet_shader_project_loc, 1, GL_FALSE, m_cast(project));
    glUseProgram(ring_shader);
    glUniformMatrix4fv(ring_shader_project_loc, 1, GL_FALSE, m_cast(project));
    glUseProgram(skybox_shader);
    glUniformMatrix4fv(skybox_shader_project_loc, 1, GL_FALSE, m_cast(project));
    glUseProgram(belt_shader);
    glUniformMatrix4fv(belt_shader_project_loc, 1, GL_FALSE, m_cast(project));

    /////////////////////////////////////////////////////////////////////////
    // RENDER LOOP

    imgui_init(window);
    glEnable(GL_DEPTH_TEST);
    f32 delta_time;
    f32 last_frame = 0.0f;
    f32 current_frame;
    f32 fps;
    while (!glfwWindowShouldClose(window))
    {
        if (sim_running)
            ss_time += ss_time_factor;
        // delta time
        current_frame = (f32)glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
        
        process_input(window);
        view = mat4_lookat(ss_camera.pos,
                           vec3_add(ss_camera.pos, ss_camera.front),
                           ss_camera.up);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // IMGUI
        imgui_newframe();

        ImGui::Begin("Planet Renders");
        ImGui::Checkbox("Mercury", &mercury.should_render);
        ImGui::Checkbox("Venus", &venus.should_render);
        ImGui::Checkbox("Earth", &earth.should_render);
        ImGui::Checkbox("Mars", &mars.should_render);
        ImGui::Checkbox("Jupiter", &jupiter.should_render);
        ImGui::Checkbox("Saturn", &saturn.should_render);
        ImGui::Checkbox("Uranus", &uranus.should_render);
        ImGui::Checkbox("Neptune", &neptune.should_render);
        ImGui::Checkbox("Pluto", &pluto.should_render);
        ImGui::End();

        ImGui::Begin("Camera");
        ImGui::SliderFloat("Speed", &(ss_camera.speed), 0.5f, 2.5f);
        ImGui::SliderFloat("Mouse Sens", &(ss_camera.mouse_sens), 0.0f, 0.1f);
        ImGui::End();

        ImGui::Begin("Time / Speed");
        ImGui::SliderFloat("Orbital Speed", &ss_time_factor, 0.0f, 1.0f);
        ImGui::SliderFloat("Rotation Speed", &ss_rot_factor, 0.0f, 10.0f);
        if (ImGui::Button("Play/Pause"))
        {
            if (sim_running)
                sim_running = false;
            else
                sim_running = true;
        }
        ImGui::End();

        ImGui::Begin("Performance");
        ImGui::Text("Frame time: %f ms", delta_time * 1000.0f);
        fps = 1 / delta_time;
        ImGui::Text("FPS: %f", fps);
        ImGui::End();

        // Sun Render
        glUseProgram(sun_shader);
        glUniformMatrix4fv(sun_shader_view_loc, 1, GL_FALSE, m_cast(view));
        sun.render(vao, sun_shader);

        // Planet Render
        glUseProgram(planet_shader);
        glUniformMatrix4fv(planet_shader_view_loc, 1, GL_FALSE, m_cast(view));
        mercury.render(vao, planet_shader);
        venus.render(vao, planet_shader);
        earth.render(vao, planet_shader);
        mars.render(vao, planet_shader);
        jupiter.render(vao, planet_shader);
        saturn.render(vao, planet_shader);
        uranus.render(vao, planet_shader);
        neptune.render(vao, planet_shader);
        pluto.render(vao, planet_shader);

        // Rock Render
        glUseProgram(belt_shader);
        glBindVertexArray(belt.meshes[0].vao);
        glUniformMatrix4fv(belt_shader_view_loc, 1, GL_FALSE, m_cast(view));
        glDrawElementsInstanced(GL_TRIANGLES, belt.meshes[0].indices.size(),
                                GL_UNSIGNED_INT, 0, AST_BELT_AMOUNT);
        if (saturn.should_render)
        {
            mat4_t translate = mat4_translate_v(compute_rock_pos(saturn.distance));
            mat4_t rotate = mat4_rotate_v(saturn.rot_angle,
                                          compute_tilt_axis(26.7f));
            glUseProgram(ring_shader);
            glBindVertexArray(ring.meshes[0].vao);
            glUniformMatrix4fv(ring_shader_view_loc, 1, GL_FALSE, m_cast(view));
            glUniformMatrix4fv(ring_shader_tilt_loc, 1, GL_FALSE, m_cast(tilt));
            glUniformMatrix4fv(ring_shader_translate_loc, 1, GL_FALSE, m_cast(translate));
            glUniformMatrix4fv(ring_shader_rotate_loc, 1, GL_FALSE, m_cast(rotate));
            // Instanced draw of rings
            glBindVertexArray(ring.meshes[0].vao);
            glDrawElementsInstanced(GL_TRIANGLES, ring.meshes[0].indices.size(),
                                    GL_UNSIGNED_INT, 0, RING_AMOUNT);
        }

        // Skybox Render
        glBindVertexArray(skybox_vao);
        glUseProgram(skybox_shader);

        view = mat4_translate_remove(view);
        glUniformMatrix4fv(skybox_shader_view_loc, 1, GL_FALSE, m_cast(view));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        imgui_render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
