#pragma once
#include "Component.h"

namespace Comphi {

	class Transform;
	typedef std::shared_ptr<Transform> TransformPtr;

	class Transform : public Component
	{
	public:
		Transform(TransformPtr& parent);
		Transform() = default;

		TransformPtr parent;

		glm::vec3 getForwardVector();
		glm::vec3 getLookVector();
		glm::vec3 getUpVector();
		glm::vec3 getRightVector();

		glm::vec3 getEulerAngles();
		glm::quat setEulerAngles(glm::vec3 pitchRollYaw);
		glm::quat eulerRotation(glm::vec3 pitchRollYaw);
		glm::quat lookAt(glm::vec3 point);

		glm::mat4 getModelMatrix();
		glm::mat4 getViewMatrix();
		glm::mat4 getModelViewMatrix();

		glm::vec3 getRelativePosition();
		glm::vec3 getRelativeScale();
		glm::quat getRelativeRotation();

		glm::quat quaternionRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 scale = glm::vec3(1.0f);
		//glm::vec3 skew; //in the 4th dimension
		//glm::vec3 prespective;

	};

	namespace Coordinates {

		const glm::vec3 up = glm::vec3(0, 0, 1);
		const glm::vec3 right = glm::vec3(1, 0, 0);
		const glm::vec3 forward = glm::vec3(0, 1, 0);
	
	}

}
