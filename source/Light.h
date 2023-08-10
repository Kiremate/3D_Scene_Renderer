#pragma once
#include "math.hpp"
#include <Color_Buffer.hpp>

using argb::Rgb888;
typedef Rgb888 Color;
/**
 * @struct Light
 * @brief Represents a light source in a 3D scene.
 */
struct Light
{
    Vector3f position; ///< Position of the light source in the 3D scene
    Color color; ///< Diffuse color of the light
    Color specular_color; ///< Specular color of the light
    float shininess; ///< Shininess factor for specular reflection

    /**
     * @brief Constructor to initialize a Light object.
     *
     * @param position_ Position of the light source in the 3D scene
     * @param color_ Diffuse color of the light
     * @param specular_color_ Specular color of the light
     * @param shininess_ Shininess factor for specular reflection
     */
    Light(const Vector3f& position_, const Color& color_, const Color& specular_color_, float shininess_)
        : position(position_), color(color_), specular_color(specular_color_), shininess(shininess_)
    {}
};
