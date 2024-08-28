#include "Response.hpp"
#include "Request.hpp"
#include "CGI.hpp"

// Constructor..................................................................
Response::Response(Request & request, struct pollfd *_poll_fd, std::string contentType) :
	_contentType(contentType),
	_code(request.getCode()),
	_error(request.getError()),
	_body(""),
	_response(""),
	_codeTable()
{
	_createCodeTable();
	if (_error == false)
		_checkAndSetPath(request);
	if (_error == true)
		_parseError(request);
	_composeResponse(request);
	if (sendResponse(_poll_fd) == -1)
		std::cerr << RED "Error sending response" << RES << std::endl;
	return;
}

Response::~Response() {
	return;
}



// Check if a valid path is given...............................................
void Response::_checkAndSetPath(Request & request)
{

	if (request.getAutoindex() == true && request.getPath() != "favicon.png")
	{
		request.setPath(makeAutoindex(request.getPath(), request.getRoot(), request));
		if (_openFile(request.getRoot(), request.getPath()) == true)
			_code = "200";
		else
			_code = "404";
		return;
	}
	if (request.getPath() == "")
	{
		_code = "404"; // Not found
		_error = true;
		return;
	}
	if (request.getRedirection() != "")
		return;

	if (_openFile(request.getRoot(), request.getPath()) == true)
	{
		_code = "200";
		return;
	}
	if (errno == EACCES)
		_code = "403"; // Forbidden
	else if (_code == "415")
		return;
	else
		_code = "404"; // Not found
	_error = true;
}



// Open the file................................................................
bool Response::_openFile(std::string const& root, std::string const& path)
{
	std::string filePath = root + path;
	if (filePath.find("?") != std::string::npos)
	{
		filePath = filePath.substr(0, filePath.find("?"));
		std::size_t pos = filePath.find("/forms");
		if (pos != std::string::npos)
			filePath.replace(pos, std::string("/forms").length(), "/cgi-bin");
	}
	std::ifstream file(filePath.c_str());

	if (file.is_open() == true)
	{
		if (_isDirectory(filePath))
		{
			_body = getDirectoryHtml(root, path);
			file.close();
			return true;
		}
		else
		{
			std::string ext = path.substr(path.find_last_of(".") + 1);
			if (ext.size() > 0)
			{
				if (_isImage(filePath) == true)
				{
					_body += _getBinaryContent(filePath);
					file.close();
					return true;
				}
				if (ext == "html" || ext == "css" || ext == "js" || ext == "txt" || ext == "ico")
				{
					std::string line;
					_body = "";
					while (std::getline(file, line))
					{
						_body += line + "\r\n";;
					}
					file.close();
					return true;
				}
				else
				{
					_code = "415"; // Unsupported media type
					_error = true;
				}
			}
		}
	}
	return false;
}



// Check if the given path is a directory.......................................
bool Response::_isDirectory(const std::string& path)
{
	struct stat path_stat;
	if (stat(path.c_str(), &path_stat) != 0)
		return false;
	return S_ISDIR(path_stat.st_mode);
}



// Compose the response..........................................................
void Response::_composeResponse(Request & request)
{
	std::stringstream ss;
	ss << _body.size();

	_response =		"HTTP/1.1 " + _code + " " + _codeTable[_code] + "\r\n";
	if (request.getRedirection() != "")
		_response +=	"Location: " + request.getRedirection() + "\r\n";
	_response +=	"Date: " + _getDate() + "\r\n";
	_response +=	"Server: Webserv/1.0 (Unix)\r\n";
	_response +=	"Content-Type: " + _contentType + "\r\n";
	_response +=	"Content-Length: " + ss.str() + "\r\n";
	_response +=	"Connection: Closed\r\n\r\n";
	_response +=	_body;
}



// Get the date..................................................................
std::string Response::_getDate()
{
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, 80, "%a, %d %b %Y %H:%M:%S GMT", timeinfo);

	return buffer;
}



// Send data to the client......................................................
int Response::sendResponse(struct pollfd *_poll_fd)
{
	size_t total_sent = 0;
	char buf[BUF_SIZE];

	_printHeaderResponse();

	size_t i = 0;
	while (i < _response.size() && total_sent < _response.size())
	{
		memset(buf, 0, BUF_SIZE);

		size_t j;
		for (j = 0; j < BUF_SIZE - 1 && i < _response.size(); j++)
			buf[j] = _response[i++];

		int result = send(_poll_fd->fd, buf, j, 0);
		if (result < 0)
		{
			std::cout << "Send() failed" << std::endl;
			return -1;
		}
		if (result == 0)
		{
			std::cout << "No data was sent" << std::endl;
			return -1;
		}

		std::cout << RES "\tBytes sent " << result  << std::endl;
		total_sent += result;
	}

	if (total_sent != _response.size())
	{
		std::cout << "Send() failed" << std::endl;
		return -1;
	}

	_poll_fd->revents = POLLIN;

	return total_sent;
}



// check if the path is a image.................................................
bool Response::_isImage(std::string const& path)
{
	std::string ext = path.substr(path.find_last_of(".") + 1);

	if (ext == "jpg" || ext == "jpeg" || ext == "png" || ext == "gif" || ext == "webp" )
	{
		_contentType = "image/" + ext;
		return true;
	}

	return false;
}



// Get binary file content......................................................
std::string Response::_getBinaryContent(std::string const& path)
{
	std::ifstream file(path.c_str(), std::ios::binary);
	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	return content;
}



// Print just the header response, not the body.................................
void Response::_printHeaderResponse()
{
	std::cout << GRE "\n:::::::::::: RESPONSE ::::::::::::" RES << std::endl;

	std::istringstream stream(_response);
	std::string line;

	while (std::getline(stream, line) && line != "\r" && line != "\n" && line != "\r\n")
		std::cout << GRE << line << RES << std::endl;
}



// Parse the error options.....................................................
void Response::_parseError(Request & request)
{
	if (_codeTable.find(_code) == _codeTable.end())
	{
		std::cerr << RED "Unknown error code: " << _code << RES << std::endl;
		_code = "500"; // Internal server error
	}
	std::map<std::string, std::string> errorPages = request.getErrorPages();
	if (!errorPages.empty())
	{
		for (std::map<std::string, std::string>::iterator it = errorPages.begin();
				it != errorPages.end(); ++it) {
			if (_code == it->first) {
				if (_openFile(request.getRoot(), it->second) == true)
					return;
			}
		}
	}
	_body = defaultError(_code, _codeTable[_code]);
}



// Create http code table for the response......................................
void Response::_createCodeTable()
{
	_codeTable.clear();

	_codeTable["0"] = "Error unespected";
	// _codeTable["100"] = "Continue";
	// _codeTable["101"] = "Switching Protocols";
	// _codeTable["102"] = "Processing";
	// _codeTable["103"] = "Checkpoint";

	_codeTable["200"] = "OK";
	// _codeTable["201"] = "Created";
	// _codeTable["202"] = "Accepted;
	// _codeTable[" "203" ] = "Non-Authoritative Information";
	_codeTable["204"] = "No Content";
	// _codeTable["205"] = "Reset Content";
	// _codeTable["206"] = "Partial Content";
	// _codeTable["207"] = "Multi-Status";
	// _codeTable["208"] = "Already Reported";

	// Redirection
	_codeTable["300"] = "Multiple Choices";
	_codeTable["301"] = "Moved Permanently";
	_codeTable["302"] = "Found";
	_codeTable["303"] = "See Other";
	_codeTable["304"] = "Not Modified";
	_codeTable["307"] = "Temporary Redirect";
	_codeTable["308"] = "Permanent Redirect";

	// _codeTable["305"] = "Use Proxy";
	// _codeTable["306"] = "Switch Proxy";

	_codeTable["400"] = "Bad Request";
	// _codeTable["401"] = "Unauthorized";
	// _codeTable["402"] = "Payment Required";
	_codeTable["403"] = "Forbidden";
	_codeTable["404"] = "Not Found";
	_codeTable["405"] = "Method Not Allowed";
	// _codeTable["406"] = "Not Acceptable";
	// _codeTable["407"] = "Proxy Authentication Required";
	// _codeTable["408"] = "Request Timeout";
	// _codeTable["409"] = "Conflict";
	// _codeTable["410"] = "Gone";
	// _codeTable["411"] = "Length Required";
	// _codeTable["412"] = "Precondition Failed";
	_codeTable["413"] = "Payload Too Large";
	_codeTable["414"] = "URI Too Long";
	_codeTable["415"] = "Unsupported Media Type";
	// _codeTable["416"] = "Requested Range Not Satisfiable";
	// _codeTable["417"] = "Expectation Failed";
	// _codeTable["418"] = "I'm a teapot";
	// _codeTable["421"] = "Misdirected Request";
	// _codeTable["422"] = "Unprocessable Entity";
	// _codeTable["423"] = "Locked";
	// _codeTable["424"] = "Failed Dependency";
	// _codeTable["426"] = "Upgrade Required";
	// _codeTable["428"] = "Precondition Required";
	// _codeTable["429"] = "Too Many Request";
	_codeTable["431"] = "Request Header Fields Too Large";
	// _codeTable["451"] = "Unavailable For Legal Reasons";

	_codeTable["500"] = "Internal Server Error";
	// _codeTable["501"] = "Not Implemented";
	// _codeTable["502"] = "Bad Gateway";
	// _codeTable["503"] = "Service Unavailable";
	// _codeTable["504"] = "Gateway Timeout";
	_codeTable["505"] = "HTTP Version Not Supported";
	// _codeTable["506"] = "Variant Also Negotiates";
	// _codeTable["507"] = "Insufficient Storage";
	// _codeTable["508"] = "Loop Detected";
	// _codeTable["510"] = "Not Extended";
	// _codeTable["511"] = "Network Authentication Required";
}
