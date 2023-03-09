#pragma once

#include <SFML/System/Vector3.hpp>
#include "math.hpp"

using namespace example;


struct Transform {
    Vector3f position;
    Vector3f rotation;
    Vector3f scale;

    Matrix44 get_transform_matrix() {
        Matrix44 translation_matrix = translate(Matrix44(1.0f), position);
        Matrix44 rotation_matrix = rotation_z() * rotation_y() * rotation_x();
        Matrix44 scale_matrix = glm::scale(Matrix44(1.0f), scale);

        return translation_matrix * rotation_matrix * scale_matrix;
    }

    // Add a function to handle translation
    void translate_local(Vector3f delta_position) {
        position += rotation_matrix() * delta_position;
    }

    // Add functions to handle rotation around each axis
    Matrix44 rotation_x() {
        return rotate_around_x(Matrix44(1.0f), rotation.x);
    }

    Matrix44 rotation_y() {
        return rotate_around_y(Matrix44(1.0f), rotation.y);
    }

    Matrix44 rotation_z() {
        return rotate_around_z(Matrix44(1.0f), rotation.z);
    }

    // Helper function to get the rotation matrix
    Matrix44 rotation_matrix() {
        return rotation_z() * rotation_y() * rotation_x();
    }

    // Add functions to handle scaling
    void scale_x(float factor) {
        scale.x *= factor;
    }

    void scale_y(float factor) {
        scale.y *= factor;
    }

    void scale_z(float factor) {
        scale.z *= factor;
    }
};

class Camera {
	
	Matrix44 get_inverse_transform_matrix() {
		return inverse(transform.get_transform_matrix());
	}

private:
    
    float fov;
    float near;
    float far;
    float move_speed_ = 5.0f;
    float rotation_speed_ = 0.1f;
    Transform transform;
    Vector3f view_direction;
    Vector3f up_direction;
    bool mouse_captured = false;
    Vector2f last_mouse_position;
};