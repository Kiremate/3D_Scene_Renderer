// Mesh.cpp
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Mesh::Mesh(const std::string& file_path)
{
    Assimp::Importer importer;

    auto scene = importer.ReadFile
    (
        file_path.c_str(),
        aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType
    );

    if (scene && scene->mNumMeshes > 0)
    {
        auto mesh = scene->mMeshes[0];

        size_t number_of_vertices = mesh->mNumVertices;

        original_vertices.resize(number_of_vertices);
        transformed_vertices.resize(number_of_vertices);

        for (size_t index = 0; index < number_of_vertices; index++)
        {
            auto& vertex = mesh->mVertices[index];

            original_vertices[index] = example::Vertex(vertex.x, -vertex.y, vertex.z, 1.f);
        }

        size_t number_of_triangles = mesh->mNumFaces;

        original_indices.resize(number_of_triangles * 3);

        example::Index_Buffer::iterator indices_iterator = original_indices.begin();

        for (size_t index = 0; index < number_of_triangles; index++)
        {
            auto& face = mesh->mFaces[index];

            assert(face.mNumIndices == 3);

            auto indices = face.mIndices;

            *indices_iterator++ = int(indices[0]);
            *indices_iterator++ = int(indices[1]);
            *indices_iterator++ = int(indices[2]);
        }

        original_colors.resize(number_of_vertices);

        for (size_t index = 0; index < number_of_vertices; index++)
        {
            original_colors[index].set(rand() % 256 / 255.f, rand() % 256 / 255.f, rand() % 256 / 255.f);
        }
    }
}
