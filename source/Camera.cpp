#include "Camera.h"
Matrix44 Camera::get_view_matrix()
{
    Vector3f center = transform.position + view_direction;
    return look_at(transform.position, center);
}

Matrix44 Camera::get_inverse_transform_matrix()
{
    return inverse(transform.get_transform_matrix());
}

void Camera::move_camera(float x, float y, float z) {
    transform.position += Vector3f(x, y, z);
}

void Camera::handle_input(const sf::Window& window, float delta_time)
{
    // Update the camera's position based on input
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        transform.translate_local(-view_direction * delta_time * move_speed_);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        transform.translate_local(view_direction * delta_time * move_speed_);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        transform.translate_local(cross(up_direction, view_direction) * delta_time * move_speed_);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        transform.translate_local(cross(view_direction, up_direction) * delta_time * move_speed_);
    }

}
