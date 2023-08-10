
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
    #include <memory>
    #include "Mesh.h"
    #include "Light.h"
    namespace example
    {

        using  std::vector;
        using argb::Rgb888;
        using argb::Color_Buffer;
        using std::shared_ptr;   
        /**
         * @class View
         * @brief Class for managing the rendering of 3D objects in a scene.
         *
         * The View class handles updating and rendering the objects in a scene.
         * It uses a camera to calculate the view matrix, a light for shading, and
         * a color buffer and rasterizer for rendering the scene.
         */
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
            std::shared_ptr<Node> root; ///< Root Node for the scene graph
            vector< shared_ptr<Mesh> > meshes;  ///< Declare the vector of Mesh pointers here
            Color_Buffer               color_buffer;  ///< Color buffer for rendering
            Rasterizer< Color_Buffer > rasterizer;    ///< Rasterizer for rendering

            Camera camera;   ///< Camera for view calculations
            unsigned width;  ///< Width of the view
            unsigned height; ///< Height of the view

        public:
       
            /**
               * @brief Constructor for View.
               *
               * @param width Width of the view.
               * @param height Height of the view.
               */
            View(unsigned width, unsigned height);

            void update ();
            void render ();
            Camera& get_camera();
        private:
            Light light;  ///< Light for shading calculations
            Color ambient_color;  ///< Ambient color for rendering
            float rand_clamp() { return float(rand() & 0xff) * 0.0039215f; }  ///< Clamps a random value between 0 and 1.
        };

    }

#endif
