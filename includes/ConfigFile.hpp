#ifndef DEF_CONFIGFILE
#define DEF_CONFIGFILE

#include "Webserv.hpp"

class ConfigFile
{
	public:
		ConfigFile(std::string const& path);
		ConfigFile(ConfigFile const& src);
		~ConfigFile();
		ConfigFile &operator=(ConfigFile const& src);

		class ConfigFileException : public std::exception
		{
			private:
				std::string _msg;
			public:
				ConfigFileException(std::string msg) : _msg(msg) {}

				virtual const char* what() const throw()
				{
					return (_msg.c_str());
				}
				virtual ~ConfigFileException() throw() {}
		};


	private:
		std::string _path;

		ConfigFile();

};


#endif

