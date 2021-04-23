#ifndef MESH_H
#define MESH_H

#include <vector>
#include "solarsystem.h"
#include "mmath.h"

typedef struct _TAG_vertex
{
    vec3_t position;
    vec3_t normal;
} vertex_t;

class mesh_t
{
public:
    // mesh data
    std::vector<vertex_t>    vertices;
    std::vector<u32>         indices;

    // render data
    u32 vao, vbo, ebo;

    // funcs
    mesh_t(std::vector<vertex_t> vertices, std::vector<u32> indices);
    void draw(void);
    void setup_mesh();
};

#endif // MESH_H
