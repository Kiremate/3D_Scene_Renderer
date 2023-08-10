#pragma once
#include "Node.h"
#include "MeshData.h"
#include "Rasterizer.hpp"
#include <string>
#include "Camera.h"
#include "Light.h"
using namespace example;
using  std::vector;
using argb::Rgb888;
using argb::Color_Buffer;
/**
 * @class Mesh
 * @brief Represents a 3D mesh in a scene.
 *
 * The Mesh class is responsible for storing the mesh data,
 * transforming it, and rendering it to a color buffer.
 */
class Mesh : public Node
{
private:
	typedef Rgb888                Color;
	typedef Color_Buffer< Color > Color_Buffer;

	bool  is_frontface(const Vertex* const projected_vertices, const int* const indices);
	float width;
	float height;
public:
	MeshData meshData; // Contains all the vertex Info 
	Camera& camera; // Contains a reference to the existing Camera
	Color_Buffer& color_buffer; // Contains a reference to the existing Color_Buffer
	Rasterizer<Color_Buffer>& rasterizer; // Contains a reference to the existing Rasterizer
	/**
	 * @brief Constructs a new Mesh object.
	 *
	 * @param file_path Path to the mesh file.
	 * @param camera Reference to the existing camera.
	 * @param color_buffer Reference to the existing color buffer.
	 * @param rasterizer Reference to the existing rasterizer.
	 * @param width Width of the mesh.
	 * @param height Height of the mesh.
	 */
	Mesh(const std::string& file_path, Camera& camera, Color_Buffer& color_buffer, Rasterizer<Color_Buffer>& rasterizer, float width, float height);
	/**
	  * @brief Transforms a vector by a given matrix.
	  *
	  * @param matrix Transformation matrix.
	  * @param vector Vector to transform.
	  * @return Transformed vector.
	  */
	inline Vector3f transform_vector(const Matrix44& matrix, const Vector3f& vector)
	{
		glm::vec4 temp = matrix * glm::vec4(vector, 0.0f);
		return glm::vec3(temp);
	}
	/**
	* @brief Updates the transform matrix of the mesh.
	*/
	void update(); // Updates the Transform matrix
	/**
	   * @brief Renders the mesh with given lighting conditions.
	   *
	   * @param light Reference to the light object.
	   * @param ambient_lighting Ambient lighting color.
	   */
	void render(const Light& light, Rgb888  ambient_lighting); // Renders the Mesh
};
