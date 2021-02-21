#pragma once

namespace Comphi {

	class FileManager
	{
	public:
		FileManager(std::string filePath) : m_filePath(filePath) {};
		virtual ~FileManager() = default;

		const std::string& getFilePath() { return m_filePath; };
		const std::string& getFileContent() { return m_fileContent; };
		void setFileContent(const std::string in) { m_fileContent = in; };
		
		virtual bool setFileData(const std::string in) = 0;
		virtual const std::string getFilename() = 0;
		virtual const std::string getBaseFolder() = 0;

	protected:
		virtual const bool load() = 0;
		virtual const bool unload()  = 0;

	private:
		std::string m_filePath;
		std::string m_fileContent;
	};
}