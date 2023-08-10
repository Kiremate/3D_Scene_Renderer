// Mesh.cpp
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


Mesh::Mesh(const std::string& file_path, Camera& camera_, Color_Buffer& color_buffer_, Rasterizer<Color_Buffer>& rasterizer_, float width_, float height_) : camera(camera_), color_buffer(color_buffer_), rasterizer(rasterizer_), width(width_), height(height_)
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

        meshData.original_vertices.resize(number_of_vertices);
        meshData.transformed_vertices.resize(number_of_vertices);

        meshData.original_normals.resize(number_of_vertices);  // Added this line
        meshData.transformed_normals.resize(number_of_vertices);  // Added this line

        for (size_t index = 0; index < number_of_vertices; index++)
        {
            auto& vertex = mesh->mVertices[index];

            meshData.original_vertices[index] = example::Vertex(vertex.x, -vertex.y, vertex.z, 1.f);


            auto& normal = mesh->mNormals[index];
            meshData.original_normals[index] = Vector3f(normal.x, -normal.y, normal.z);
        }

        size_t number_of_triangles = mesh->mNumFaces;

        meshData.original_indices.resize(number_of_triangles * 3);

        example::Index_Buffer::iterator indices_iterator = meshData.original_indices.begin();

        for (size_t index = 0; index < number_of_triangles; index++)
        {
            auto& face = mesh->mFaces[index];

            assert(face.mNumIndices == 3);

            auto indices = face.mIndices;

            *indices_iterator++ = int(indices[0]);
            *indices_iterator++ = int(indices[1]);
            *indices_iterator++ = int(indices[2]);
        }

        meshData.original_colors.resize(number_of_vertices);

        for (size_t index = 0; index < number_of_vertices; index++)
        {
            meshData.original_colors[index].set(rand() % 256 / 255.f, rand() % 256 / 255.f, rand() % 256 / 255.f);
        }
    }
    meshData.transformed_vertices.resize(meshData.original_vertices.size());
    meshData.display_vertices.resize(meshData.original_vertices.size());
}


void Mesh::update()
{
    // Comment out or remove the following line to stop the rotation
        // angle += 0.025f;

        // Se crean las matrices de transformación:

    Matrix44 identity(1);
    Matrix44 scaling = scale(identity, 4.f);

    // Use an identity matrix instead of rotation_y to disable rotation
    Matrix44 rotation_y = identity;
    // Matrix44 rotation_y = rotate_around_y(identity, angle);

    Matrix44 translation = translate(identity, Vector3f{ 0.f, 0.5f, -10.f });
    Matrix44 projection = perspective(20, 1, 15, float(this->width) / this->height);

    // Get the camera's view matrix
    Matrix44 view_matrix = camera.get_view_matrix();

    // Use transformation matrix from Node class.
    Matrix44 transformation = projection * view_matrix * Node::transformation;

    // Se transforman todos los vértices usando la matriz de transformación resultante:

    for (size_t index = 0, number_of_vertices = meshData.original_vertices.size(); index < number_of_vertices; index++)
    {
        // Se multiplican todos los vértices originales con la matriz de transformación y
        // se guarda el resultado en otro vertex buffer:

        Vertex& vertex = meshData.transformed_vertices[index] = transformation * meshData.original_vertices[index];

        // La matriz de proyección en perspectiva hace que el último componente del vector
        // transformado no tenga valor 1.0, por lo que hay que normalizarlo dividiendo:
        Vector3f& normal = meshData.transformed_normals[index] = transform_vector(transformation, meshData.original_normals[index]);
        meshData.transformed_normals[index] = glm::normalize(transform_vector(transformation, meshData.original_normals[index]));

        float divisor = 1.f / vertex.w;

        vertex.x *= divisor;
        vertex.y *= divisor;
        vertex.z *= divisor;
        vertex.w = 1.f;
    }
}

void Mesh::render(const Light& light, Rgb888  ambient_lighting)
{
    Matrix44 identity(1);
    Matrix44 scaling = scale(identity, float(this->width / 2), float(this->height / 2), 100000000.f);
    Matrix44 translation = translate(identity, Vector3f{ float(this->width / 2), float(this->height / 2), 0.f });
    Matrix44 transformation = translation * scaling;

    for (size_t index = 0, number_of_vertices = meshData.transformed_vertices.size(); index < number_of_vertices; index++)
    {
        meshData.display_vertices[index] = Point4i(transformation * meshData.transformed_vertices[index]);
    }

    for (int* indices = meshData.original_indices.data(), *end = indices + meshData.original_indices.size(); indices < end; indices += 3)
    {
        if (is_frontface(meshData.transformed_vertices.data(), indices))
        {
            Color original_color = meshData.original_colors[*indices];

            // calculate lighting using Phong model
            Vector3f L = light.position - Vector3f(meshData.transformed_vertices[*indices].x, meshData.transformed_vertices[*indices].y, meshData.transformed_vertices[*indices].z);
            Vector3f N = meshData.transformed_normals[*indices];  // vertex normal
            Vector3f V = -Vector3f(meshData.transformed_vertices[*indices].x, meshData.transformed_vertices[*indices].y, meshData.transformed_vertices[*indices].z);
            Vector3f R = 2 * glm::dot(N, L) * N - L;  // reflection of L

            L = glm::normalize(L);
            V = glm::normalize(V);
            R = glm::normalize(R);

            Color diffuse_part = original_color * light.color * glm::max(0.f, glm::dot(N, L));
            Color specular_part = light.specular_color * glm::pow(glm::max(0.f, glm::dot(V, R)), light.shininess);

            Color phong_color = ambient_lighting + diffuse_part + specular_part;

            rasterizer.set_color(phong_color);

            // Draw the triangle
            rasterizer.fill_convex_polygon_z_buffer(meshData.display_vertices.data(), indices, indices + 3);
        }
    }
}

bool Mesh::is_frontface(const Vertex* const projected_vertices, const int* const indices)
{
    const Vertex& v0 = projected_vertices[indices[0]];
    const Vertex& v1 = projected_vertices[indices[1]];
    const Vertex& v2 = projected_vertices[indices[2]];

    // Se asumen coordenadas proyectadas y polígonos definidos en sentido horario.
    // Se comprueba a qué lado de la línea que pasa por v0 y v1 queda el punto v2:

    return ((v1[0] - v0[0]) * (v2[1] - v0[1]) - (v2[0] - v0[0]) * (v1[1] - v0[1]) < 0.f);
}

