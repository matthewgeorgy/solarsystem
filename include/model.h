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

#endif // MODEL_H
