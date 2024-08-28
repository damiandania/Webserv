#ifndef Socket_HPP
#define Socket_HPP

#include "Webserv.hpp"
#include "ServerData.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "CGI.hpp"

#define BACKLOG 10

class Socket {
	private:
		ServerData			_serverData;
		int					_serverFd;
		sockaddr_in			_address;
		int					_result;
		std::vector<int>	_clientFd;
		char				_buf[BUF_SIZE + 1];
		bool				_closeConnection;
		bool				_compressArray;
		bool				_cgi_status;
		std::string 		_contentType;

		void			_initSocket();
		void			_testResult(int iten_to_test, std::string msg);
		void			_compressVecClient(int fd);
		unsigned long	_convertToIp(const std::string & ip);
		int				_receiveData(struct pollfd *_poll_fd, int client_fd);
		void			_checkCgiExt();

		Socket(void);

	public:
		Socket(ServerData const& server_data);
		~Socket(void);

		int					getServerFd();
		std::vector<int> &	getClientFd();
		char *				getBufRequest();
		ServerData			getServerData();

		void	acceptConnection();
		bool	handleConnection(struct pollfd *ptr_poll_fd, int client_fd, Socket & server, char **envp);
		void	sendResponse();
};

#endif

