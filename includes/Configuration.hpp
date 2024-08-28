#ifndef DEF_CONFIGURATION
#define DEF_CONFIGURATION

#include "Webserv.hpp"
#include "ServerData.hpp"

class ServerData;

class Configuration
{
	public:
		Configuration(std::string const& path);
		Configuration(Configuration const& src);
		~Configuration();
		Configuration &operator=(Configuration const& src);
		std::vector<ServerData> getServer() const;

		class ParseConfigException : public std::exception
		{
			private:
				std::string _msg;
			public:
				ParseConfigException(std::string msg) : _msg(msg) {}

				virtual const char* what() const throw()
				{
					return (_msg.c_str());
				}
				virtual ~ParseConfigException() throw() {}
		};

	private:
		Configuration();
		std::vector<ServerData> _server;
		std::string _configFile;

		void parseConfig(std::string const& path);
		void parseServerBlock(std::stringstream & ss, ServerData & server);
		void parseLocationBlock(std::stringstream & ss, ServerData & server);
		void removeComments(std::string & line);
		void removeSpaces(std::string const& path);
		void printStream(std::string const& path);
		void printServer() const;

};

std::ostream &operator<<(std::ostream &o, Configuration const& src);

#endif

