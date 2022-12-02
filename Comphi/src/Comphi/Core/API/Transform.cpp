#include "cphipch.h"
#include "Transform.h"

namespace Comphi {

	glm::vec3 Transform::getForwardVector()
	{
		return quaternionRotation * Vector3::forward;
	}

	glm::vec3 Transform::getUpVector()
	{
		return quaternionRotation * Vector3::up;
	}

	glm::vec3 Transform::getRightVector()
	{
		return quaternionRotation * Vector3::right;
	}
	
	glm::vec3 Transform::getLookVector()
	{
		return glm::normalize((position + getForwardVector()) - position);
	}

	glm::vec3 Transform::getEulerAngles()
	{
		return glm::eulerAngles(quaternionRotation);
	}

	glm::quat Transform::setEulerAngles(glm::vec3 pitchRollYaw)
	{
		return quaternionRotation = glm::quat(glm::radians(pitchRollYaw));
	}

	glm::quat Transform::eulerRotation(glm::vec3 pitchYawRoll)
	{
		return quaternionRotation = glm::quat(glm::radians(pitchYawRoll)) * quaternionRotation;
	}

	glm::quat Transform::lookAt(glm::vec3 point)
	{
		return quaternionRotation = glm::quatLookAt(glm::normalize(point-position), Vector3::up);
	}

	glm::mat4 Transform::getModelMatrix()
	{
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
		glm::mat4 rotateMatrix = glm::toMat4(quaternionRotation);
		glm::mat4 translateMatrix = glm::translate(glm::mat4(1.0f), position);

		return translateMatrix * rotateMatrix * scaleMatrix;
	}

	/*glm::quat Transform::LookAt(glm::vec3 target, glm::vec3 Dir)
	{

		glm::vec3 forward = glm::normalize(target - position); //get new Local Z

		glm::vec3 worldForward = Dir; //get World Z
		float dot = glm::dot(worldForward, forward); //getAngle LocalZ > WorldZ
		float rotationAngle = glm::acos(dot); //AngleIn degrees
		glm::vec3 rotationAxis = glm::normalize(glm::cross(forward, worldForward)); //localUP

		return glm::toQuat(glm::rotate(glm::mat4(), rotationAngle, -rotationAxis));

	}*/

}
