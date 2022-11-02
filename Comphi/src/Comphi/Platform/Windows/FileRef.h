#pragma once
#include "Comphi/Platform/IFileRef.h"

namespace Comphi::Windows{
	class FileRef : public IFileRef
	{
	public:
		FileRef(std::string filePath);
		FileRef() = default;
		~FileRef() = default;
		virtual void reload(std::string filePath) override;

		virtual bool writeToFile(const std::string in) override;
		virtual const std::string getFilename() override;
		virtual const std::string getBaseFolder() override;

	protected:
		const bool load() override;
		const bool unload() override;
	};
}