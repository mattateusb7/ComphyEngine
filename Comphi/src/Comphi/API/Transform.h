#pragma once
namespace Comphi {

	class Transform 
	{
	public:
		Transform() = default;
		~Transform() = default;

		glm::vec3 getForwardVector();
		glm::vec3 getLookVector();
		glm::vec3 getUpVector();
		glm::vec3 getRightVector();

		glm::vec3 getEulerAngles();
		glm::quat setEulerAngles(glm::vec3 pitchRollYaw);
		glm::quat eulerRotation(glm::vec3 pitchRollYaw);
		glm::quat lookAt(glm::vec3 point);

		glm::mat4 getModelMatrix();

		glm::quat quaternionRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 scale = glm::vec3(1.0f);
		//glm::vec3 skew;
		//glm::vec3 prespective;

	protected:
		glm::mat4 ModelMatrix;

	};

	namespace Coordinates {

	const glm::vec3 up = glm::vec3(0, 0, 1);
	const glm::vec3 right = glm::vec3(1, 0, 0);
	const glm::vec3 forward = glm::vec3(0, 1, 0);
	
	}

	typedef std::shared_ptr<Transform> TransformInstance;

}
