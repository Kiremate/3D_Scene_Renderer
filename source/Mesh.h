#pragma once
#include "Node.h"
#include "MeshData.h"
#include <string>
class Mesh : public Node
{
public:
    example::Vertex_Buffer original_vertices;
    example::Vertex_Buffer transformed_vertices;
    example::Index_Buffer original_indices;
    example::Vertex_Colors original_colors;

    Mesh(const std::string& file_path);
};
