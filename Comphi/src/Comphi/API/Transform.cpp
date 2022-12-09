#include "cphipch.h"
#include "Transform.h"

namespace Comphi {

	glm::vec3 Transform::getForwardVector()
	{
		//TODO: Does not account parent rotation ?
		if (parent != nullptr) {
			return quaternionRotation * parent->getForwardVector();
		}
		return quaternionRotation * Coordinates::forward;
	}

	glm::vec3 Transform::getUpVector()
	{
		if (parent != nullptr) {
			return  quaternionRotation * parent->getUpVector();
		}
		return quaternionRotation * Coordinates::up;
	}

	glm::vec3 Transform::getRightVector()
	{
		if (parent != nullptr) {
			return  quaternionRotation * parent->getRightVector();
		}
		return quaternionRotation * Coordinates::right;
	}
	
	glm::vec3 Transform::getLookVector()
	{
		if (parent != nullptr) {
			return  getRelativePosition() + getForwardVector();
		}
		return position + getForwardVector();
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
		return quaternionRotation *= glm::quat(glm::radians(pitchYawRoll));
	}

	glm::quat Transform::lookAt(glm::vec3 point)
	{
		return quaternionRotation = glm::quatLookAt(glm::normalize(point - getRelativePosition()), getUpVector());
	}

	glm::mat4 Transform::getModelMatrix()
	{
		glm::mat4 ModelMatrix = glm::mat4(1.0f);

		ModelMatrix = glm::translate(ModelMatrix, getRelativePosition());
		ModelMatrix *= glm::toMat4(getRelativeRotation());
		ModelMatrix = glm::scale(ModelMatrix, getRelativeScale());
		return ModelMatrix;
		
	}

	glm::vec3 Transform::getRelativePosition()
	{
		//TODO: fix recursive relative pos
		if (parent != nullptr) {
			return parent->getRelativePosition() + (parent->quaternionRotation * position);
		}
		return position;
	}

	glm::vec3 Transform::getRelativeScale()
	{
		if (parent != nullptr) {
			return parent->getRelativeScale() * scale;
		}
		return scale;
	}

	glm::quat Transform::getRelativeRotation()
	{
		if (parent != nullptr) {
			return parent->getRelativeRotation() * quaternionRotation;
		}
		return quaternionRotation;
	}

}
