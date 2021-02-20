#pragma once

namespace Comphi {

	class FileManager
	{
	public:
		FileManager(std::string filePath) : m_filePath(filePath) { load(); };
		~FileManager() { unload(); };

		virtual const bool getData(std::string& out) = 0;
		virtual bool setData(const std::string in) = 0;

	protected:
		virtual const bool load() { return false; };
		virtual const bool unload() { return false; };
		std::string m_filePath;
		std::string m_fileContent;
	};
}