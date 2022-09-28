#pragma once

namespace Comphi {

	class IFileRef
	{
	public:
		IFileRef(std::string filePath) : m_filePath(filePath) {};
		IFileRef() = default;
		virtual ~IFileRef() = default;

		const std::string getFilePath() { return m_filePath; };
		const std::string getData() { return m_byte_fileContent.data(); };
		const std::vector<char> getByteData() { return m_byte_fileContent; };
		void setFileByteData(const std::vector<char> data) { m_byte_fileContent = data; };
		void setFileData(const std::string in) { m_byte_fileContent = std::vector<char>(in.begin(), in.end()); };
		
		virtual void reload(std::string filePath) = 0;
		virtual bool writeToFile(const std::string in) = 0;
		virtual const std::string getFilename() = 0;
		virtual const std::string getBaseFolder() = 0;

	protected:
		virtual const bool load() = 0;
		virtual const bool unload()  = 0;
		std::string m_filePath;
	private:
		std::vector<char> m_byte_fileContent;
	};
}