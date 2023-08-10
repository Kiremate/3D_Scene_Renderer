#pragma once
#include "math.hpp"
#include <SFML/Window.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
using namespace example;
/**
 * @struct Transform
 * @brief Represents a 3D transformation including position, rotation and scale.
 */
struct Transform {
	Vector3f position; ///< Position of the object
	Quaternion rotation; ///< Rotation of the object represented as a quaternion
	Vector3f scale; ///< Scale factors in 3 dimensions

	/**
   * @brief Get the transformation matrix
   * @return 4x4 transformation matrix representing this transform
   */
	Matrix44 get_transform_matrix() {
		Matrix44 scale_matrix = glm::scale(example::Matrix44(1.0f), scale);
		Matrix44 rotation_matrix = glm::mat4_cast(rotation);
		Matrix44 translation_matrix = glm::translate(example::Matrix44(1.0f), position);
		return translation_matrix * rotation_matrix * scale_matrix;
	}

	void move(const example::Vector3f& offset) {
		position += offset;
	}

	void rotate(const example::Quaternion& q) {
		rotation = glm::normalize(rotation * q);
	}

	void set_rotation(const example::Quaternion& q) {
		rotation = q;
	}

	void scale_up(const example::Vector3f& factor) {
		scale *= factor;
	}

	void scale_down(const example::Vector3f& factor) {
		scale /= factor;
	}

	void translate_local(const example::Vector3f& offset) {
		// Get the rotation matrix from the quaternion
		Matrix44 rotation_matrix = glm::mat4_cast(rotation);

		// Transform the offset vector by the rotation matrix
		Vector4f transformed_offset = glm::vec4(offset, 0.0f) * rotation_matrix;

		// Add the transformed offset to the position
		position += example::Vector3f(transformed_offset);
	}

	void rotate(float pitch, float yaw) {
		// Update the rotation quaternion with the new pitch and yaw angles
		Quaternion q_yaw = glm::angleAxis(glm::radians(yaw), example::Vector3f(0, 1, 0));
		Quaternion q_pitch = glm::angleAxis(glm::radians(pitch), example::Vector3f(1, 0, 0));
		rotation = glm::normalize(rotation * q_yaw * q_pitch);
	}

};

/**
 * @class Camera
 * @brief Represents a camera in 3D space.
 */
class Camera {

public:
	/**
	* @brief Get the view matrix for this camera
	* @return 4x4 view matrix
	*/
	Matrix44 get_view_matrix();
	/**
	  * @brief Get the inverse of the transform matrix
	  * @return Inverse of the transform matrix
	  */
	Matrix44 get_inverse_transform_matrix();
	void handle_input(sf::Window& window, float delta_time);
	Camera() = default;
	Camera(float fov, float near_plane, float far_plane, Vector3f position, Vector3f target, Vector3f up)
		: fov(fov),
		near_plane(near_plane),
		far_plane(far_plane),
		move_speed_(5.0f),
		rotation_speed_(0.1f),
		transform{},
		view_direction(normalize(target - position)),
		up_direction(normalize(up)),
		mouse_captured(false)
	{
		transform.position = position;

		Vector3f forward = normalize(target - position);
		Vector3f right = normalize(cross(up, forward));
		Vector3f new_up = cross(forward, right);
		Matrix33 rotation_matrix(right, new_up, forward);
		transform.rotation = Quaternion(rotation_matrix);
	}
	void move_camera(float x, float y, float z);
	void update_directions();

private:
	float fov; ///< Field of view
	float near_plane; ///< Near clipping plane distance
	float far_plane; ///< Far clipping plane distance
	float move_speed_ = 5.0f; ///< Speed at which camera moves
	float rotation_speed_ = 0.1f; ///< Speed at which camera rotates
	float current_pitch = 0.0f; ///< Current pitch angle
	float current_yaw = 0.0f; ///< Current yaw angle
	float look_speed = 0.1f; ///< Speed at which camera looks around
	float min_mouse_delta = 0.5f; ///< Minimum mouse movement to capture
	Transform transform; ///< Transformation of the camera
	Vector3f view_direction; ///< Direction that the camera is looking
	Vector3f up_direction;  ///< Up direction for the camera
	Vector3f right_direction; ///< Right direction for the camera
	bool mouse_captured = false;  ///< Whether the mouse cursor is captured
	Vector2f last_mouse_position;///< Last recorded mouse position
};