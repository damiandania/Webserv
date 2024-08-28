#include "Request.hpp"
#include "Socket.hpp"

// Global variables..............................................................
std::string requestData;


// Constructor..................................................................
Request::Request(Socket & server) :
	_server(&server),
	_error(false),
	_method(""),
	_uri(""),
	_version(""),
	_body(""),
	_path(""),
	_autoindex(false),
	_root(server.getServerData().getRoot()),
	_index(server.getServerData().getIndex()),
	_redirection(""),
	_upload(""),
	_location(""),
	_code("0")
{
	_methodAllowed.clear();
	_methodAllowed.push_back("GET");
	_methodAllowed.push_back("POST");
	_methodAllowed.push_back("DELETE");
	_cgi.clear();
	_cgiext.clear();
	_errorPages = (server.getServerData().getErrorPages());
	_headers.clear();
	return;
}

Request::~Request() {
	return;
}



// Store all the request in _request............................................
int Request::store(char buf[BUF_SIZE], struct pollfd *ptr_tab_poll, int bytes_read)
{
	int ret = 0;
	(void)ptr_tab_poll;
	for (int i = 0; i < bytes_read; i++)
	{
		requestData += buf[i];
		ret++;
	}
	return ret;
}



// Parse the request............................................................
void Request::parser()
{
	try {
		std::istringstream request_stream(requestData);
		std::string line;

		if (std::getline(request_stream, line))
		{
			std::istringstream line_stream(line);
			line_stream >> _method >> _uri >> _version;
		}
		if (_method.empty() || _uri.empty() || _version.empty())
			throw std::runtime_error("400"); // Bad request
		if (_method != "GET" && _method != "POST" && _method != "DELETE")
			throw std::runtime_error("405"); // Method not allowed
		if (_version != "HTTP/1.1")
			throw std::runtime_error("505"); // HTTP version not supported
		if (_uri.length() > MAX_URI_LENGTH)
			throw std::runtime_error("414"); // URI too long

		bool header_finished = false;
		while (std::getline(request_stream, line))
		{
			if (!header_finished)
			{
				if (line == "\r" || line == "\n" || line == "\r\n")
				{
					header_finished = true;
					continue;
				}

				size_t pos = line.find(':');
				if (pos != std::string::npos)
				{
					std::string header_name = line.substr(0, pos);
					std::string header_value = line.substr(pos + 1);
					header_value.erase(header_value.find_last_not_of(" \t\n\r\f\v") + 1);
					header_value.erase(0, header_value.find_first_not_of(" \t\n\r\f\v"));
					_headers[header_name] = header_value;
				}
			}
			else
				_body += line + "\n";
		}
		if (_headers.size() > MAX_HEADER_LENGTH)
			throw std::runtime_error("431"); // Request Header Fields Too Large
		// Check if the server name is the same as the host name
		if (_headers.find("Host") != _headers.end())
		{
			std::string host = _headers["Host"];

			if (_isIp(host) == true)
			{
				std::string ip = _server->getServerData().getIp();
				if (ip != "0.0.0.0")
				{
					if (host.find(ip) == std::string::npos)
						throw std::runtime_error("400"); // Bad request
				}
			}
			// else if (host != "localhost:8080")
			else if (host.find(_server->getServerData().getServerName()) == std::string::npos
				&& host.find("localhost") == std::string::npos)
				throw std::runtime_error("400"); // Bad request
		}
		if (_body.length() > static_cast<size_t>(_server->getServerData().getMaxBodySize()))
			throw std::runtime_error("413"); // Request entity too large
	}
	catch (std::exception & e)
	{
		_code = e.what();
		_error = true;
	};
}


bool Request::_isIp(std::string const& ip)
{
	std::istringstream iss(ip);

	int octet;
	char dot;
	for (int i = 0; i < 4; ++i) {
		if (!(iss >> octet) || octet < 0 || octet > 255)
			return false;
		if (i < 3 && (!(iss >> dot) || dot != '.'))
			return false;
	}
	return true;
}



// Check if the request is finished.............................................
bool	Request::storeFinished()
{
	int		pos;
	size_t len = requestData.size();
	std::string copyRequest(requestData);

	if (ci_find_substr(requestData, "transfer-encoding") != -1 && requestData.find("chunked") != std::string::npos)
	{

		for (size_t i = 0; i < len; i++)
		{
			if (requestData[i] == '\r'
					&& (++i < len && requestData[i] == '\n')
					&& (++i < len && requestData[i] == '0')
					&& (++i < len && requestData[i] == '\r')
					&& (++i < len && requestData[i] == '\n')
					&& (++i < len && requestData[i] == '\r')
					&& (++i < len && requestData[i] == '\n'))
				return true;
		}
	}
	else if ((pos = ci_find_substr(requestData, "content-length")) != -1)
	{
		size_t content_length;
		copyRequest.erase(0, pos + 15);

		if (copyRequest[0] == 13 || copyRequest[0] == 32)
			copyRequest.erase(0, 1);

		int str_len = 0;
		for (size_t i = 0; isdigit(copyRequest[i]); i++)
			str_len++;
		std::istringstream ( copyRequest.substr(0, str_len)) >> content_length;

		for (size_t i = 0; i < len; i++)
		{
			if (requestData[i] == '\r'
					&& (++i < len && requestData[i] == '\n')
					&& (++i < len && requestData[i] == '\r')
					&& (++i < len && requestData[i] == '\n')
					&& (++i < len)) {

				if (requestData.length() - i >= content_length)
					return true;
			}
		}

	}
	else
	{
		for (size_t i = 0; i < len; i++)
		{
			if (requestData[i] == '\r'
					&& (++i < len && requestData[i] == '\n')
					&& (++i < len && requestData[i] == '\r')
					&& (++i < len && requestData[i] == '\n'))
				return true;
		}
	}
	return false;
}


// find substring (case insensitive)
int Request::ci_find_substr( const std::string& str1, const std::string& str2 )
{
	std::string::const_iterator it = std::search( str1.begin(), str1.end(),
			str2.begin(), str2.end(), my_equal() );
	if ( it != str1.end() ) return it - str1.begin();
	else
		return -1; // not found
}



// Process the request depending on the method.................................
void Request::process()
{
	if (_error == true)
		return;

	_setRoute();

	if (_error == false)
	{
		for (size_t i = 0; i < _methodAllowed.size(); i++)
		{
			if (_method == _methodAllowed[i])
				break;
			if (i >= _methodAllowed.size() - 1)
			{
				_code = "405"; // Method not allowed
				_error = true;
			}
		}
	}
}



// Assign the url to the request..............................................
void Request::_setRoute()
{
	std::string fullLocation;
	size_t posBegin = 0;
	size_t posEnd = 1;

	while (posEnd <= _uri.size())
	{
		posBegin = _uri.find('/', posBegin);
		if ((posEnd = _uri.find('/', posEnd)) == std::string::npos)
			posEnd = _uri.size();

		_location = _uri.substr(posBegin, posEnd - posBegin);
		if (_getRouteValues(_location) == false)
		{
			_path = _uri.substr(posBegin);
			if (_path == "")
				_path = _index;
			break;
		}
		posBegin++;
		posEnd++;
		fullLocation += _location;
	}
	if (_path == "")
		_path = _index;

	_location = fullLocation;
	// _printValues();
}



// Print the values of the request..............................................
void Request::_printValues() const
{
	std::cout << MAG "Location: " << _location << std::endl;
	std::cout << "Path: " << _path << std::endl;
	std::cout << "Method allowed: " << std::endl;
	for (size_t i = 0; i < _methodAllowed.size(); i++)
		std::cout << "\t" << _methodAllowed.at(i) << std::endl;
	std::cout << "Autoindex: " << (_autoindex == false ? "false" : "true") << std::endl;
	std::cout << "Root: " << _root << std::endl;
	std::cout << "Index: " << _index << RES << std::endl;
	if (_redirection != "")
		std::cout << MAG "Redirection: " << _redirection << RES << std::endl;
	if (_errorPages.size() > 0)
	{
		std::cout << MAG "Error pages: " << std::endl;
		for (std::map<std::string, std::string>::const_iterator it = _errorPages.begin(); it != _errorPages.end(); ++it)
			std::cout << "\t" << it->first << " : " << it->second << std::endl;
		std::cout << RES << std::endl;
	}
}



// Get route values.............................................................
bool Request::_getRouteValues(std::string const& location)
{
	for (size_t i = 0; i < _server->getServerData().getRoutes().size(); i++)
	{
		RouteData currentRoute = _server->getServerData().getRoutes()[i];

		if (location == currentRoute.getPath())
		{
			if (currentRoute.getHttpMethods().size() > 0)
				_methodAllowed = currentRoute.getHttpMethods();
			if (currentRoute.getAutoIndex() == true)
				_autoindex = true;
			if (currentRoute.getRoot() != "")
			{
				if (currentRoute.getRoot()[currentRoute.getRoot().size() - 1] == '/')
					_root = currentRoute.getRoot().substr(0, currentRoute.getRoot().size() - 1);
				else
				_root = currentRoute.getRoot();

			}
			if (currentRoute.getIndex() != "")
				_index = currentRoute.getIndex();
			std::map<std::string, std::string> redirection = currentRoute.getRedirection();
			if (!redirection.empty())
			{
				std::map<std::string, std::string>::const_iterator it = redirection.begin();
				_code = it->first;
				_redirection = it->second;
			}
			if (currentRoute.getCgi().size() > 0)
				_cgi = currentRoute.getCgi();
			if (currentRoute.getCgiExt().size() > 0)
				_cgiext = currentRoute.getCgiExt();
			if (currentRoute.getUpload() != "")
				_upload = currentRoute.getUpload();
			if (currentRoute.getErrorPages().size() > 0)
				_errorPages = currentRoute.getErrorPages();
			return true;
		}
	}
	return false;
}



// Print the request............................................................
void Request::printRequest() const
{
	std::cout	<< BLU "\n:::::::::::: REQUEST ::::::::::::" RES << std::endl;

	std::istringstream stream(requestData);
	std::string line;
	int lineCounter = 0;

	while (std::getline(stream, line) && lineCounter < 3)
	{
		std::cout << BLU << line << RES << std::endl;
		lineCounter++;
	}
	std::cout << "\tBytes received " << requestData.size() << std::endl;
	std::cout << std::endl;
}



// Clean the request............................................................
void Request::cleanRequestData()
{
	requestData.clear();
}



// Getters......................................................................
bool Request::getError() const {
	return _error;
}

std::string Request::getMethod() const {
	return _method;
}

std::string Request::getUri() const {
	return _uri;
}

std::string Request::getVersion() const {
	return _version;
}

std::string Request::getHeader(std::string const& header) const {
	return _headers.at(header);
}

bool Request::hasHeader(std::string const& header) const {
    return _headers.find(header) != _headers.end();
}

std::string Request::getBody() const {
	return _body;
}

std::string Request::getPath() const {
	return _path;
}

std::vector<std::string> Request::getMethodAllowed() const {
	return _methodAllowed;
}

bool Request::getAutoindex() const {
	return _autoindex;
}

std::string Request::getRoot() const {
	return _root;
}

std::string Request::getIndex() const {
	return _index;
}

std::string Request::getRedirection() const {
	return _redirection;
}

std::vector<std::string> Request::getCgi() const {
	return _cgi;
}

std::vector<std::string> Request::getCgiext() const {
	return _cgiext;
}

std::string Request::getUpload() const {
	return _upload;
}

std::map<std::string, std::string> Request::getErrorPages() const {
	return _errorPages;
}

std::string Request::getLocation() const {
	return _location;
}

std::string Request::getCode() const {
	return _code;
}



// Setters......................................................................
void Request::setPath(std::string const& path) {
	_path = path;
}

void Request::setLocation(std::string const& location) {
	_location = location	;
}

void Request::setCode(std::string const& code) {
	_code = code;
	_error = true;
}
