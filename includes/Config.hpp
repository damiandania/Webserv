#ifndef Config_HPP
#define Config_HPP

#include "Webserv.hpp"

class Config
{
	private:
		std::string			_file;
		std::vector<int>	_vec_ports;
	public:
		Config(std::string const & file) : _file(file)
		{
			_vec_ports.push_back(8080);
			_vec_ports.push_back(8081);
			_vec_ports.push_back(8082);
			_vec_ports.push_back(8083);
		};
		~Config(void) {}

		std::vector<int> & get_ports()
		{
			return _vec_ports;
		};
};

#endif
