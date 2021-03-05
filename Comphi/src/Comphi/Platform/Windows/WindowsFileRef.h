#pragma once
#include "../IFileRef.h"

namespace Comphi::Windows{
	class FileRef : public IFileRef
	{
	public:
		FileRef(std::string filePath);
		~FileRef() = default;

		virtual bool setFileData(const std::string in) override;
		virtual const std::string getFilename() override;
		virtual const std::string getBaseFolder() override;

	protected:
		const bool load() override;
		const bool unload() override;
	};
}