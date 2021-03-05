#include "cphipch.h"
#include "WindowsFileRef.h"

namespace Comphi::Windows {

	FileRef::FileRef(std::string filePath) : IFileRef(filePath)
	{
		load();
	}

	const std::string FileRef::getFilename() {

		const auto& filePath = getFilePath();

		auto n = filePath.find_last_of('/');
		if (n == std::string::npos) {
			n = filePath.find_last_of('\\');
			if (n == std::string::npos) return filePath;
		}
		return filePath.substr(n + 1);
	}

	const std::string FileRef::getBaseFolder()
	{
		TCHAR Path[512];
		GetModuleFileName(NULL, Path, 512);
		std::wstring path = Path;
		std::string path_str(path.begin(), path.end());
		return path_str;
	}

	bool FileRef::setFileData(const std::string in)
	{
		setFileContent(in);

		//Add AT_pos in the future
		std::ofstream ofs;
		ofs.open(getFilePath());

		if (!ofs.good()) {
			COMPHILOG_CORE_INFO("Creating File: \"" + getFilename() + "\"");
		}

		ofs << getFileContent();
		ofs.close();
		return true;
	}

	const bool FileRef::load()
	{
		/*std::ifstream myfile;
		myfile.open(m_filePath);

		while (myfile.good()) {
			To += myfile.get();
		}

		myfile.close();
		*/
		/*std::ifstream Input(m_filePath);
		if (!Input.good()) {
			COMPHILOG_CORE_ERROR("Failed To Read: %s", m_filePath);
			Input.close();
			return false;
		}
		To = std::string{ std::istreambuf_iterator<char>(Input), {} };
		Input.close();
		*/

		std::ifstream ifs(getFilePath().c_str(), std::ios::ate); //set to end
		if (!ifs.good()) {
			COMPHILOG_CORE_ERROR("Failed To Read: \"" + getFilePath() + "\"");
			ifs.close();
			return false;
		}
		std::ifstream::pos_type fileSize = ifs.tellg();
		ifs.seekg(0, std::ios::beg); //seek beginning

		std::vector<char> bytes(fileSize);
		ifs.read(bytes.data(), fileSize); //read

		setFileContent(std::string(bytes.data(), fileSize));

		COMPHILOG_CORE_INFO("Successfuly Read: \"" + getFilename() + "\"");
		return true;
	}

	const bool FileRef::unload()
	{
		return true;
	}
}



