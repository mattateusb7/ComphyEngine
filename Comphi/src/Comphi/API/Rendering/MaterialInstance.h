#pragma once
#include "Comphi/API/Rendering/Material.h"
#include "Comphi/API/Rendering/ShaderBinding.h"

namespace Comphi {

	class MaterialInstance : public IObject, public ShaderBinding
	{
	public:
		MaterialInstance(MaterialPtr& parent);
		virtual void cleanUp() override {};

		MaterialPtr parent;
	};
	
	typedef std::shared_ptr<MaterialInstance> MaterialInstancePtr;

}

//template<> struct std::hash<Comphi::MaterialInstancePtr> {
//	size_t operator()(Comphi::MaterialInstancePtr const& materialInst) const {
//		return materialInst->UID;
//	}
//};