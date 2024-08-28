#ifndef Response_HPP
#define Response_HPP

#include "Webserv.hpp"
#include "Response.hpp"
#include "HtmlMaker.hpp"
#include "CGI.hpp"

class Request;
class CGI;

class Response
{
	private:
		std::string							_contentType;
		std::string							_code;
		bool								_error;
		std::string							_body;
		std::string							_response;
		std::map<std::string , std::string>	_codeTable;

		Response();

		void		_composeResponse(Request & request);
		std::string	_getDate();
		void		_printHeaderResponse();
		void 		_createCodeTable();
		void		_parseError(Request & request);
		void		_checkAndSetPath(Request & request);
		bool		_openFile(std::string const& root, std::string const& path);
		bool		_isDirectory(std::string const& path);
		bool		_isImage(std::string const& path);
		std::string	_getBinaryContent(std::string const& path);

	public:
		Response(Request & request, struct pollfd *_poll_fd, std::string contentType);
		~Response();

		int			sendResponse(struct pollfd *_poll_fd);
};

#endif

