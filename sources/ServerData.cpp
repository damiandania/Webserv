#include "ServerData.hpp"
#include <string>

ServerData::ServerData() : _ip("0.0.0.0"), _port(8080), _serverName(""),
	_host(""), _root(""), _index(""), _maxBodySize(0),
	_isPortSet(false), _isServerNameSet(false), _isHostSet(false),
	_isRootSet(false),_isIndexSet(false), _isMaxBodySize(false)
{}

ServerData::~ServerData()
{}

ServerData::ServerData(ServerData const& src) : _ip(src._ip), _port(src._port),
	_serverName(src._serverName), _host(src._host), _root(src._root),
	_index(src._index), _maxBodySize(src._maxBodySize),
	_errorPages(src._errorPages), _routes(src._routes)
{}

ServerData & ServerData::operator=(ServerData const& src)
{
	if (this != &src)
	{
		_ip = src._ip;
		_port = src._port;
		_serverName = src._serverName;
		_host = src._host;
		_root = src._root;
		_index = src._index;
		_maxBodySize = src._maxBodySize;
		_errorPages = src._errorPages;
		_routes = src._routes;
	}
	return *this;
}

void ServerData::setPort(std::string const& address)
{
	if (_isPortSet == true)
		throw ServerDataException("Duplicate port");
	_isPortSet = true;
	size_t pos = address.find(':');
	if (pos != std::string::npos)
	{
		_ip = address.substr(0, pos);
		std::string port_str = address.substr(pos + 1);
		std::istringstream(port_str) >> _port;
	}
	else
	{
		if (address.find('.') != std::string::npos)
		{
			_ip = address;
		}
		else
		{
			std::istringstream(address) >> _port;
		}
	}
}
void ServerData::setServerName(std::string const& serverName)
{
	if (_isServerNameSet == true)
		throw ServerDataException("Duplicate server name");
	_isServerNameSet = true;
	_serverName = serverName;
}

void ServerData::setRoot(std::string const& root)
{
	if (_isRootSet == true)
		throw ServerDataException("Duplicate root");
	_isRootSet = true;
	_root = root;
}

void ServerData::setIndex(std::string const& index)
{
	if (_isIndexSet == true)
		throw ServerDataException("Duplicate index");
	_isIndexSet = true;
	_index = index;
}

void ServerData::setHost(std::string const& host)
{
	if (_isHostSet == true)
		throw ServerDataException("Duplicate host");
	_isHostSet = true;
	_host = host;
}

void ServerData::setMaxBodySize(int mbz)
{
	if (_isMaxBodySize == true)
		throw ServerDataException("Duplicate Client Max Body Size");
	_isMaxBodySize = true;
	_maxBodySize = mbz;
}

void ServerData::setErrorPages(std::string const& errorCode, std::string const& path)
{
	if (_errorPages.find(errorCode) != _errorPages.end())
	{
		std::ostringstream oss;
		oss << errorCode;
		throw ServerDataException("Duplicate error page for error code " + oss.str());
	}
	_errorPages[errorCode] = path;
}

void ServerData::setRoutes(RouteData const& route)
{
	_routes.push_back(route);
}

std::string ServerData::getIp() const
{
	return _ip;
}

int ServerData::getPort() const
{
	return _port;
}

std::string ServerData::getServerName() const
{
	return _serverName;
}

std::string ServerData::getRoot() const
{
	return _root;
}

std::string ServerData::getIndex() const
{
	return _index;
}

std::string ServerData::getHost() const
{
	return _host;
}

int ServerData::getMaxBodySize() const
{
	return _maxBodySize;
}

std::map<std::string, std::string> ServerData::getErrorPages() const
{
	return _errorPages;
}

std::vector<RouteData> ServerData::getRoutes() const
{
	return _routes;
}


std::ostream &operator<<(std::ostream &o, ServerData const& src)
{
	o << "Ip: " << src.getIp() << "\n"
		<< "Port: " << src.getPort() << "\n"
		<< "Server Name: " << src.getServerName() << "\n"
		<< "Host: " << src.getHost() << "\n"
		<< "Root: " << src.getRoot() << "\n"
		<< "Index: " << src.getIndex() << "\n";
	std::map<std::string, std::string> ErrorPages = src.getErrorPages();
	for (std::map<std::string, std::string>::const_iterator it = ErrorPages.begin(); it != ErrorPages.end(); ++it)
		o << "Error Page " << it->first << " : " << it->second << "\n";
	o << "Max Body Size: " << src.getMaxBodySize() << "\n";
	for (size_t i = 0; i < src.getRoutes().size(); ++i)
		o << "Route " << i + 1 << ": " << src.getRoutes()[i] << "\n";
	return o;
}

// bool ServerData::isValidIp(std::string const& ip)
// {
// 	std::istringstream iss(ip);

// 	int octet;
// 	char dot;
// 	for (int i = 0; i < 4; ++i) {
// 		if (!(iss >> octet) || octet < 0 || octet > 255)
// 			return false;
// 		if (i < 3 && (!(iss >> dot) || dot != '.'))
// 			return false;
// 	}
// 	return true;
// }


// bool ServerData::isValidPort(std::string const& port)
// {
// 	std::istringstream iss(port);
// 	int p;
// 	if (!(iss >> p) || p < 0 || p > 65535)
// 		return false;
// 	return true;
// }


// void ServerData::setIp(std::string const& ip)
// {
// 	_ip = ip;
// }
