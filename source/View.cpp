
// Este código es de dominio público.
// angel.rodriguez@esne.edu
// 2013.12 - 2021.04

#include <cassert>
#include <cmath>
#include "math.hpp"
#include "View.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <string>
namespace example
{

    View::View(unsigned width, unsigned height)
        :
        width(width),
        height(height),
        color_buffer(width, height),
        rasterizer(color_buffer),
        camera(60.0f, 0.1f, 100.0f, Vector3f(0.0f, -1.0f, 3.0f), Vector3f(0.0f, 0.0f, -1.0f), Vector3f(0.0f, 1.0f, 0.0f)),
        light({ 0.0f, 0.0f, 10.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, 10.0f),
        ambient_color(1.0f, 1.0f, 1.0f)
    {
        root = std::make_shared<Node>();
        // Define file paths for the objects
        std::vector<std::string> objectFiles = {
            "../../shared/assets/stanford-bunny.obj",
            "../../shared/assets/rocks.obj",
            "../../shared/assets/barrel.obj",
            "../../shared/assets/character-human.obj",
            "../../shared/assets/character-orc.obj",
            "../../shared/assets/floor.obj"
        };


        // Define transformations for the objects
        std::vector<glm::mat4> objectTransforms = {
            glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.8f, -1)), glm::vec3(1.0f, 1.0f, 1.0f)),  // Stanford bunny
            glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -0.5f, -1)),                                             // Rocks
            glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, -0.5f, -1)),                                            // Barrel
            glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, -1)),                                             // Human character
            glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -1.0f, -1)),                                             // Orc character
            glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(5.0f, 1.0f, 5.0f))      // Floor
        };

        // Load and add the objects to the scene
        for (size_t i = 0; i < objectFiles.size(); ++i) {
            auto mesh = std::make_shared<Mesh>(objectFiles[i], camera, color_buffer, rasterizer, width, height);

            mesh->transformation = objectTransforms[i];
            root->addChild(mesh);
            meshes.push_back(mesh);
        }
    }



    void View::update ()
    {
        static float rotation_angle = 0.0f;

        // For each Mesh Node call the update
        for (auto& mesh : meshes)
        {
            mesh->update();
        }

        if (!meshes.empty()) {
            // Increment the rotation angle
            rotation_angle += 0.01f; // ACCUMULATIVE!!! THE BUNNY IS GOING TO FLY!!!
            glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), rotation_angle, glm::vec3(0.0f, 1.0f, 0.0f));
            // Get the bunny for demonstration purposes...
            meshes[0]->transformation = rotation * meshes[0]->transformation;
        }
    }
    
    Camera& View::get_camera()
    {
        return camera;
    }
    
    void View::render ()
    {
        rasterizer.clear();

        // For each Mesh Node call the render
        for (auto& mesh : meshes)
        {
            mesh->render(light, ambient_color);
        }
        // Se copia el framebúffer oculto en el framebúffer de la ventana:
        color_buffer.blit_to_window();
    }

    

}
