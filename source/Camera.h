#pragma once
#include "math.hpp"
#include <SFML/Window/Event.hpp>

using namespace example;

struct Transform {
    Vector3f position;
    Quaternion rotation;
    Vector3f scale;

    Matrix44 get_transform_matrix() {
        Matrix44 scale_matrix = glm::scale(Matrix44(1.0f), scale);
        Matrix44 rotation_matrix = glm::mat4_cast(rotation);
        Matrix44 translation_matrix = glm::translate(Matrix44(1.0f), position);
        return translation_matrix * rotation_matrix * scale_matrix;
    }

    void move(const Vector3f& offset) {
        position += offset;
    }

    void rotate(const Quaternion& q) {
        rotation = glm::normalize(rotation * q);
    }

    void set_rotation(const Quaternion& q) {
        rotation = q;
    }

    void scale_up(const Vector3f& factor) {
        scale *= factor;
    }

    void scale_down(const Vector3f& factor) {
        scale /= factor;
    }

    void translate_local(const Vector3f& offset) {
        // Get the rotation matrix from the quaternion
        Matrix44 rotation_matrix = glm::mat4_cast(rotation);

        // Transform the offset vector by the rotation matrix
        Vector4f transformed_offset = glm::vec4(offset, 0.0f) * rotation_matrix;

        // Add the transformed offset to the position
        position += Vector3f(transformed_offset);
    }

    void rotate(float pitch, float yaw) {
        // Update the rotation quaternion with the new pitch and yaw angles
        Quaternion q_yaw = glm::angleAxis(glm::radians(yaw), Vector3f(0, 1, 0));
        Quaternion q_pitch = glm::angleAxis(glm::radians(pitch), Vector3f(1, 0, 0));
        rotation = glm::normalize(rotation * q_yaw * q_pitch);
    }
};

class Camera {
    
public:
    Matrix44 get_view_matrix();
    Matrix44 get_inverse_transform_matrix();
    void handle_event(const sf::Event& event, float delta_time);
    Camera() = default;
    Camera(float fov, float near, float far, Vector3f position, Vector3f target, Vector3f up) :
        fov(fov), near(near), far(far), transform({ position, Vector3f(0.0f), Vector3f(1.0f) }), view_direction(normalize(target - position)), up_direction(normalize(up))
    {
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