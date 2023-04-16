
// Este código es de dominio público.
// angel.rodriguez@esne.edu
// 2013.12 - 2021.04

#ifndef VIEW_HEADER
#define VIEW_HEADER

    #include <Color_Buffer.hpp>
    #include <cstdlib>
    #include "math.hpp"
    #include "Rasterizer.hpp"
    #include <vector>
    #include "Camera.h"

    namespace example
    {

        using  std::vector;
        using argb::Rgb888;
        using argb::Color_Buffer;
      
        class View
        {
        private:
           
            typedef Rgb888                Color;
            typedef Color_Buffer< Color > Color_Buffer;
            typedef Point4f               Vertex;
            typedef vector< Vertex >      Vertex_Buffer;
            typedef vector< int    >      Index_Buffer;
            typedef vector< Color  >      Vertex_Colors;

        private:

            static constexpr char mesh_file_path[] = "../../shared/assets/stanford-bunny.obj";

            Color_Buffer               color_buffer;
            Rasterizer< Color_Buffer > rasterizer;
            // Datos de la malla
			// En la practica tiene que haber varias mallas
			// Sacar esto en una clase aparte de mallas
            Vertex_Buffer     original_vertices;
            Index_Buffer      original_indices;
            Vertex_Colors     original_colors;
            Vertex_Buffer     transformed_vertices;
            vector< Point4i > display_vertices;
            Camera camera;
            unsigned width;
            unsigned height;

        public:
            struct Light
            {
                Vector3f position;
                Color color;
                Light(const Vector3f& position_, const Color& color_)
                    : position(position_), color(color_)
                {}
            };
            View(unsigned width, unsigned height);

            void update ();
            void render ();
            void transform_vertices();
            Camera& get_camera();
        private:
            Light light;
            Color ambient_color;
            Vector3f get_normal(const Vertex& v0, const Vertex& v1, const Vertex& v2);
            Color add_colors(const Color& c1, const Color& c2) const;
            Color multiply_color_by_scalar(const Color& c, float scalar) const;
            bool  is_frontface (const Vertex * const projected_vertices, const int * const indices);
            float rand_clamp   () { return float(rand () & 0xff) * 0.0039215f; }

        };

    }

#endif
