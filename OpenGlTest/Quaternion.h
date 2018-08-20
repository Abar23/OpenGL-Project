#ifndef QUATERNION_H
#define QUATERNION_H

#include "glm\glm.hpp"

using namespace std;

class Quaternion
{
public:
	Quaternion(float angleOfRotation, glm::vec3 vector);

	Quaternion();

	~Quaternion();

	Quaternion conjugate();

	Quaternion normalize();

	Quaternion multiply(const Quaternion *quaternion);

	glm::mat4 QuaternionToMatrix();

	inline float length();

	inline float getAngleOfRotation();

	inline float getB();

	inline float getC();

	inline float getD();

private:
	float angleOfRotationInRadians;

	glm::vec3 vector;
};

#endif //QUATERNION_H