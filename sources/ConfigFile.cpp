#include "ConfigFile.hpp"

ConfigFile::ConfigFile()
{}

ConfigFile::ConfigFile(std::string const& path) : _path(path)
{
	if (access(path.c_str(), R_OK) != 0)
		throw ConfigFileException("Config file error : File is not accessible");
	std::ifstream inFile(path.c_str());
	if (!inFile.is_open())
		throw ConfigFileException("Config file error : Failed to open file");
	if (inFile.peek() == std::ifstream::traits_type::eof())
		throw ConfigFileException("Config file error : File is empty");
	inFile.close();
}


ConfigFile::ConfigFile(ConfigFile const& src) : _path(src._path)
{}

ConfigFile::~ConfigFile()
{}

ConfigFile & ConfigFile::operator=(ConfigFile const& src)
{
	if (this != &src)
		_path = src._path;
	return *this;
}
