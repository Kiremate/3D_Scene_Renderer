#pragma once

#include <SFML/System/Vector3.hpp>
#include "math.hpp"

using namespace example;


struct Transform {
	Vector3f translation;
	Vector3f rotation;
	Vector3f scale;

	Matrix44 get_transform_matrix() {

	}

};


class Camera {
	

	float fov;
	float near;

	Transform transform;

	Matrix44 get_inverse_transform_matrix() {
		return inverse(transform.get_transform_matrix());
	}
	
};