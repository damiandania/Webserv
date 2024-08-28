#ifndef Request_HPP
#define Request_HPP

#include "Webserv.hpp"
#include "Socket.hpp"

#define MAX_URI_LENGTH 2000
#define MAX_HEADER_LENGTH 4096

class Socket;

class Request
{
	private:
		Request();

		Socket *							_server;
		bool								_error;

		std::string							_method;
		std::string							_uri;
		std::string							_version;
		std::map<std::string, std::string>	_headers;
		std::string							_body;

		std::string							_path;
		std::vector<std::string>			_methodAllowed;
		bool								_autoindex;
		std::string							_root;
		std::string							_index;
		std::string							_redirection;
		std::vector<std::string>			_cgi;
		std::vector<std::string>			_cgiext;
		std::string							_upload;
		std::map<std::string, std::string>	_errorPages;
		std::string 						_location;

		char								_buf[BUF_SIZE];
		std::string							_code;

		void		_setRoute();
		bool		_getRouteValues(std::string const& location);
		bool		_isIp(std::string const& str);
		void		_processGet();
		void		_processPost();
		void		_processDelete();
		void		_createCodeTable();
		void		_checkParserRequest();
		void		_printValues() const;


	public:
		Request(Socket & Server);
		~Request();

		bool								getError() const;
		std::string							getMethod() const;
		std::string							getUri() const;
		std::string							getVersion() const;
		std::string							getHeader(std::string const& header) const;
		bool								hasHeader(std::string const& header) const;
		std::string							getBody() const;
		std::string 						getPath() const;
		std::vector<std::string> 			getMethodAllowed() const;
		bool								getAutoindex() const;
		std::string							getRoot() const;
		std::string							getIndex() const;
		std::string							getRedirection() const;
		std::vector<std::string>			getCgi() const;
		std::vector<std::string>			getCgiext() const;
		std::string							getUpload() const;
		std::map<std::string, std::string>	getErrorPages() const;
		std::string							getLocation() const;
		std::string							getCode() const;

		void	setPath(std::string const& path);
		void	setLocation(std::string const& location);
		void	setCode(std::string const& code);

		int			store(char buf[BUF_SIZE], struct pollfd *poll_fd, int bytes_read);
		bool		storeFinished();
		int			ci_find_substr( const std::string& str1, const std::string& str2 );
		void		parser();
		void		process();
		void		printRequest() const;
		void		cleanRequestData();

		struct my_equal {
			bool operator()(char ch1, char ch2) {
				return std::toupper(ch1) == std::toupper(ch2);
			}
		};
};

#endif

