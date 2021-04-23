#ifndef PLANET_HPP
#define PLANET_HPP

#include "mmath.h"

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

#endif // PLANET_HPP
