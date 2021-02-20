#include "cphipch.h"
#include "FileRef.h"

Comphi::FileRef::FileRef(std::string filePath) : FileManager(filePath) {}

const bool Comphi::FileRef::getData(std::string& out)
{
	out = m_fileContent;
	return true;
}

bool Comphi::FileRef::setData(const std::string in)
{
	m_fileContent = in;

	//Add AT_pos in the future
	std::ofstream ofs;
	ofs.open(m_filePath);
	//if (!ofs.good()) {
	//	COMPHILOG_CORE_ERROR("Failed To Read: %s", m_filePath);
	//	ofs.close();
	//	return false;
	//}
	ofs << m_fileContent;
	ofs.close();
	return true;
}

const bool Comphi::FileRef::load()
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
	
	std::ifstream ifs(m_filePath.c_str(), std::ios::ate); //set to end
	//if (!ifs.good()) {
	//	COMPHILOG_CORE_ERROR("Failed To Read: %s", m_filePath);
	//	ifs.close();
	//	return false;
	//}
	std::ifstream::pos_type fileSize = ifs.tellg();
	ifs.seekg(0, std::ios::beg); //seek beginning
	
	std::vector<char> bytes(fileSize);
	ifs.read(bytes.data(), fileSize); //read
	
	m_fileContent = std::string(bytes.data(), fileSize);
	return true;
}

const bool Comphi::FileRef::unload()
{
	return true;
}


