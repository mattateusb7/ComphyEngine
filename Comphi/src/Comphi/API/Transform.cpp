#include "cphipch.h"
#include "Transform.h"

namespace Comphi {

	glm::vec3 Transform::getForwardVector()
	{
		return quaternionRotation * Coordinates::forward;
	}

	glm::vec3 Transform::getUpVector()
	{
		return quaternionRotation * Coordinates::up;
	}

	glm::vec3 Transform::getRightVector()
	{
		return quaternionRotation * Coordinates::right;
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
		return quaternionRotation = glm::quatLookAt(glm::normalize(point-position), Coordinates::up);
	}

	glm::mat4 Transform::getModelMatrix()
	{
		glm::mat4 ModelMatrix = glm::mat4(1.0f);

		if (parent != nullptr) {
			ModelMatrix = glm::translate(ModelMatrix, parent->position);
			ModelMatrix *= glm::toMat4(parent->quaternionRotation);
			ModelMatrix *= glm::toMat4(quaternionRotation);
			ModelMatrix = glm::translate(ModelMatrix, position);
			ModelMatrix = glm::scale(ModelMatrix, parent->scale);
			ModelMatrix = glm::scale(ModelMatrix, scale);
		}
		else {
			ModelMatrix = glm::translate(ModelMatrix, position);
			ModelMatrix *= glm::toMat4(quaternionRotation); //rotate
			ModelMatrix = glm::scale(ModelMatrix, scale);
		}
		return ModelMatrix;
		
	}

}
