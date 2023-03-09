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

void Camera::handle_event(const sf::Event& event, float delta_time)
{
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
        case sf::Keyboard::W:
            transform.translate_local(view_direction * delta_time * move_speed_);
            break;
        case sf::Keyboard::S:
            transform.translate_local(-view_direction * delta_time * move_speed_);
            break;
        case sf::Keyboard::A:
            transform.translate_local(cross(up_direction, view_direction) * delta_time * move_speed_);
            break;
        case sf::Keyboard::D:
            transform.translate_local(cross(view_direction, up_direction) * delta_time * move_speed_);
            break;
        }
    }

    if (event.type == sf::Event::MouseMoved) {
        Vector2f mouse_position(event.mouseMove.x, event.mouseMove.y);

        if (mouse_captured) {
            Vector2f delta = mouse_position - last_mouse_position;
            transform.rotate(delta.y * delta_time * rotation_speed_, delta.x * delta_time * rotation_speed_);
        }

        last_mouse_position = mouse_position;
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        mouse_captured = true;
        last_mouse_position = Vector2f(event.mouseButton.x, event.mouseButton.y);
    }

    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        mouse_captured = false;
    }
}
