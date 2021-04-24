#include "../include/glad/glad.h"
#include "../include/mesh.h"

mesh_t::mesh_t(std::vector<vertex_t> vertices,
               std::vector<u32> indices)
{
    this->vertices = vertices;
    this->indices = indices;

    this->setup_mesh();
}

void
mesh_t::setup_mesh()
{
    glCreateVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glCreateBuffers(1, &this->vbo);
    glCreateBuffers(1, &this->ebo);

    glNamedBufferStorage(this->vbo,
                         this->vertices.size() * sizeof(vertex_t),
                         this->vertices.data(),
                         GL_DYNAMIC_STORAGE_BIT);  
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

    glNamedBufferStorage(this->ebo,
                         this->indices.size() * sizeof(u32), 
                         this->indices.data(),
                         GL_DYNAMIC_STORAGE_BIT);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);

    // vertex positions
    glEnableVertexAttribArray(0);   
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          sizeof(vertex_t), (void *)0);
    // vertex normals
    glEnableVertexAttribArray(1);   
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 
                          sizeof(vertex_t), (void *)offsetof(vertex_t, normal));
}

void
mesh_t::draw(void)
{
    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES,
                   this->indices.size(),
                   GL_UNSIGNED_INT,
                   (void *)0);
}
