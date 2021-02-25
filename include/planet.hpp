#ifndef PLANET_HPP
#define PLANET_HPP

#include "solarsystem.h"

class planet_t
{
public:    
    // data
    f32 distance;
    f32 orb_period;
    f32 radius;
    u32 texture;
    usize indices;
    vec3_t rot_axis;
    vec3_t pos;
    f32 rot_speed;
    f32 tilt_angle;
    f32 rot_angle;
    bool should_render;

    // funcs
    planet_t(f32 distance, f32 orb_period, f32 radius,
             usize indices, const char *texture,
             f32 tilt_angle, f32 rot_speed);
    void update_pos(void);
    void render(u32 vao, u32 shader);
};

planet_t::planet_t(f32 distance, f32 orb_period, f32 radius,
                   usize indices, const char *texture,
                   f32 tilt_angle, f32 rot_speed)
{
    this->distance = distance;
    this->orb_period = orb_period;
    this->radius = radius;
    this->indices = indices;
    this->texture = load_texture(texture);
    this->tilt_angle = tilt_angle;
    this->rot_axis = compute_tilt_axis(-this->tilt_angle);
    this->rot_speed = rot_speed;
    this->pos = {0.0f, 0.0f, this->distance};
    this->should_render = true;
    this->rot_angle = 0.0f;
}

void
planet_t::update_pos(void)
{
    this->pos.x = this->distance * m_sin(this->orb_period * ss_time);
    this->pos.z = 0.8f * this->distance * m_cos(this->orb_period * ss_time);
}

void
planet_t::render(u32 vao, u32 shader)
{
    if (this->should_render)
    {
        glBindVertexArray(vao);
        glUseProgram(shader);

        // Once we create the global header for our program,
        // we can add the "project" matrix into here as well.
        //
        // We could also pass in the view matrix so that everything 
        // related to rendering is done within this function.
        mat4_t scale = mat4_scale(this->radius);
        mat4_t align = mat4_rotate(-90.0f, 1.0f, 0.0f, 0.0f);
        mat4_t tilt = mat4_rotate_v(this->tilt_angle, {0.0f, 0.0f, 1.0f});

        glUniformMatrix4fv(glGetUniformLocation(shader, "align"),
                           1, GL_FALSE, m_cast(align));
        glUniformMatrix4fv(glGetUniformLocation(shader, "scale"),
                           1, GL_FALSE, m_cast(scale));
        glUniformMatrix4fv(glGetUniformLocation(shader, "tilt"),
                           1, GL_FALSE, m_cast(tilt));

        this->update_pos();
        mat4_t translate = mat4_translate_v(this->pos);
        this->rot_angle += this->rot_speed * ss_rot_factor;
        mat4_t spin = mat4_rotate_v(this->rot_angle,
                                    this->rot_axis);

        glUniformMatrix4fv(glGetUniformLocation(shader, "translate"),
                           1, GL_FALSE, m_cast(translate));
        glUniformMatrix4fv(glGetUniformLocation(shader, "spin"),
                           1, GL_FALSE, m_cast(spin));

        glBindTextureUnit(0, this->texture);
        glDrawElements(GL_TRIANGLES,
                       this->indices,
                       GL_UNSIGNED_INT,
                       (void *)0);
    }
}

#endif // PLANET_HPP
