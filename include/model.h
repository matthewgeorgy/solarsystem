#ifndef MODEL_H
#define MODEL_H

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "mesh.h"

class model_t
{
public:
    // data
    std::vector<mesh_t>     meshes;
    std::string             directory;

    // funcs
    model_t(std::string path);
    void draw(void);
    void load_model(std::string path);
    void process_node(aiNode *node, const aiScene *scene);
    mesh_t process_mesh(aiMesh *mesh);
};

model_t::model_t(std::string path)
{
    this->load_model(path);
}

void
model_t::draw(void)
{
    for (u32 i = 0; i < this->meshes.size(); i++)
    {
        this->meshes[i].draw();
    }
}

void
model_t::load_model(std::string path)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | 
                                                 aiProcess_FlipUVs |
                                                 aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        printf("ERROR::ASSIMP:: %s\n", import.GetErrorString());
        return;
    }

    this->directory = path.substr(0, path.find_last_of('\\'));
    this->process_node(scene->mRootNode, scene);
}

void
model_t::process_node(aiNode *node, const aiScene *scene)
{
    // process all the node's meshes (if any)
    for (u32 i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        this->meshes.push_back(this->process_mesh(mesh));
    }
    // then do the same for each of its children
    for (u32 i = 0; i < node->mNumChildren; i++)
    {
        this->process_node(node->mChildren[i], scene);
    }
}

mesh_t
model_t::process_mesh(aiMesh *mesh)
{
    std::vector<vertex_t> vertices;
    std::vector<u32> indices;

    for (u32 i = 0; i < mesh->mNumVertices; i++)
    {
        vertex_t vertex;
        vec3_t vector;

        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }

        vertices.push_back(vertex);
    }

    // walk through each mesh's faces and obtain corresponding vertex indices
    for (u32 i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        for (u32 j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }
    
    return mesh_t(vertices, indices);
}

#endif // MODEL_H
