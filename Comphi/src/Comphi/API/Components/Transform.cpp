#include "cphipch.h"
#include "Transform.h"

namespace Comphi {
	Transform::Transform(TransformPtr& parent)
	{
		this->parent.reset(parent.get());
	}

	glm::vec3 Transform::getForwardVector()
	{
		if (parent.get() != nullptr) {
			return quaternionRotation * parent->getForwardVector();
		}
		return quaternionRotation * Coordinates::forward;
	}

	glm::vec3 Transform::getUpVector()
	{
		if (parent.get() != nullptr) {
			return  quaternionRotation * parent->getUpVector();
		}
		return quaternionRotation * Coordinates::up;
	}

	glm::vec3 Transform::getRightVector()
	{
		if (parent.get() != nullptr) {
			return  quaternionRotation * parent->getRightVector();
		}
		return quaternionRotation * Coordinates::right;
	}
	
	glm::vec3 Transform::getLookVector()
	{
		if (parent.get() != nullptr) {
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
		return quaternionRotation; // this is not working = glm::quatLookAt(glm::normalize(point - getRelativePosition()), getUpVector());
		
		/*glm::quat LookAt(const glm::vec3 & from, const glm::vec3 & to, const glm::vec3 & up)
		{
			glm::vec3 forward = glm::normalize(to - from);
			glm::vec3 right = glm::normalize(glm::cross(up, forward));
			glm::vec3 newUp = glm::normalize(glm::cross(forward, right));

			glm::mat4 viewMatrix = glm::mat4(
				right.x, newUp.x, -forward.x, 0.0f,
				right.y, newUp.y, -forward.y, 0.0f,
				right.z, newUp.z, -forward.z, 0.0f,
				-glm::dot(right, from), -glm::dot(newUp, from), glm::dot(forward, from), 1.0f);

			return glm::quat_cast(viewMatrix);
		}*/
								   
		//return quaternionRotation = LookAt(getRelativePosition(), point, getUpVector());
	}

	glm::mat4 Transform::getModelMatrix()
	{
		glm::mat4 ModelMatrix = glm::mat4(1.0f);

		ModelMatrix = glm::translate(ModelMatrix, getRelativePosition());
		ModelMatrix *= glm::toMat4(getRelativeRotation());
		ModelMatrix = glm::scale(ModelMatrix, getRelativeScale());
		return ModelMatrix;
		
	}

	glm::mat4 Transform::getViewMatrix()
	{
		return glm::lookAt(getRelativePosition(), getLookVector(), getUpVector());
	}

	glm::vec3 Transform::getRelativePosition()
	{
		if (parent.get() != nullptr) {
			return parent->getRelativePosition() + (parent->quaternionRotation * position);
		}
		return position;
	}

	glm::vec3 Transform::getRelativeScale()
	{
		if (parent.get() != nullptr) {
			return parent->getRelativeScale() * scale;
		}
		return scale;
	}

	glm::quat Transform::getRelativeRotation()
	{
		if (parent.get() != nullptr) {
			return parent->getRelativeRotation() * quaternionRotation;
		}
		return quaternionRotation;
	}

}
