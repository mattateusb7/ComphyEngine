#pragma once
#include "../FileManager.h"

namespace Comphi{
	class FileRef : public FileManager
	{
	public:
		FileRef(std::string filePath);
		~FileRef() = default;

		const bool getData(std::string& out) override;
		bool setData(const std::string in) override;

	protected:
		const bool load() override;
		const bool unload() override;
	};
}