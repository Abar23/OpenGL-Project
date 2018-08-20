#define _USE_MATH_DEFINES

#include <cmath>
#include "Quaternion.h"

Quaternion::Quaternion(float angleOfRotationInDegrees, glm::vec3 vector)
{
	//convert degrees to radians
	angleOfRotationInDegrees = angleOfRotationInDegrees * (float)(M_PI / 180);

	//Represent the quaternion as (angle, vector) using (cos(theta/2), v * sin(theta/2))
	this->angleOfRotationInRadians = cos(angleOfRotationInDegrees / 2);
	this->vector = vector * sin(angleOfRotationInRadians / 2);
}

Quaternion::Quaternion()
{
	this->angleOfRotationInRadians = 1;
	this->vector = glm::vec3(0.0f, 0.0f, 0.0f);
}


Quaternion::~Quaternion()
{
}

Quaternion Quaternion::conjugate()
{
	glm::vec3 inverse;

	inverse.x = -this->vector.x;
	inverse.y = -this->vector.y;
	inverse.z = -this->vector.z;

	return Quaternion(this->angleOfRotationInRadians, inverse);
}

Quaternion Quaternion::normalize()
{
	float length = this->length();

	this->angleOfRotationInRadians /= length;
	this->vector.x /= length;
	this->vector.y /= length;
	this->vector.z /= length;

	return *this;
}

Quaternion Quaternion::multiply(const Quaternion *quaternion)
{
	Quaternion quaternionProduct;

	quaternionProduct.angleOfRotationInRadians = (this->angleOfRotationInRadians * quaternion->angleOfRotationInRadians) - glm::dot(this->vector, quaternion->vector);
	quaternionProduct.vector = (this->vector * quaternion->vector) + (quaternion->vector * this->angleOfRotationInRadians) + glm::cross(this->vector, quaternion->vector);

	return quaternionProduct;
}

glm::mat4 Quaternion::QuaternionToMatrix()
{
	return glm::mat4(this->angleOfRotationInRadians, -this->vector.z, this->vector.y, this->vector.x,
					 -this->vector.z, this->angleOfRotationInRadians, -this->vector.x, this->vector.y,
					 -this->vector.y, this->vector.x, this->angleOfRotationInRadians, this->vector.z,
					 -this->vector.x, -this->vector.y, -this->vector.z, this->angleOfRotationInRadians);
}

float Quaternion::length()
{
	return (float)sqrt(this->angleOfRotationInRadians * this->angleOfRotationInRadians + this->vector.x * this->vector.x + this->vector.y * this->vector.y + this->vector.z * this->vector.z);
}

inline float Quaternion::getAngleOfRotation()
{
	return this->angleOfRotationInRadians;
}

inline float Quaternion::getB()
{
	return this->vector.x;
}

inline float Quaternion::getC()
{
	return this->vector.y;
}

inline float Quaternion::getD()
{
	return this->vector.z;
}
