#include "../includes/Socket.hpp"

// Constructor..................................................................
Socket::Socket(void) {
	return ;
}

Socket::Socket(ServerData const& serverData) :
	_serverData(serverData),
	_closeConnection(false),
	_compressArray(false),
	_cgi_status(false),
	_contentType("text/html") {
	_initSocket();
	// _checkCgiExt();
}


Socket::~Socket(void) {
	return ;
}



// Getters......................................................................
int Socket::getServerFd() {
	return _serverFd;
}

std::vector<int> & Socket::getClientFd() {
	return _clientFd;
}

ServerData Socket::getServerData() {
	return _serverData;
}

char * Socket::getBufRequest() {
	return _buf;
}



// Create an AF_INET stream socket..............................................
void Socket::_initSocket()
{
	_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	_testResult(_serverFd, "Socket()");

	// Reuse the socket
	int reuse = 1;
	_result = setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
	_testResult(_result, "Setsockopt()");

	// Set socket to be nonblocking
	_result = fcntl(_serverFd, F_SETFL, O_NONBLOCK);
	_testResult(_result, "Fcntl()");

	// Bind the socket
	_address.sin_family = AF_INET;
	_address.sin_port = htons(_serverData.getPort());
	_address.sin_addr.s_addr = _convertToIp(_serverData.getIp());

	_result = bind(_serverFd, (struct sockaddr *)&_address, sizeof(_address));
	_testResult(_result, "Bind()");

	// Set the listen back log
	_result = listen(_serverFd, BACKLOG);
	_testResult(_result, "Listen()");
}



// Convert ip address from string to binary.....................................
unsigned long Socket::_convertToIp(const std::string & ip)
{
	unsigned long result = 0;
	unsigned long temp = 0;
	int count = 0;

	for (size_t i = 0; i < ip.size(); ++i)
	{
		if (ip[i] == '.')
		{
			result = (result << 8) | temp;
			temp = 0;
			++count;
			++i;
		}
		if (ip[i] >= '0' && ip[i] <= '9')
			temp = temp * 10 + (ip[i] - '0');
		else
			count += 4;
	}

	if (count != 3)
	{
		std::stringstream ss;
		ss << "IP: " << _serverData.getIp() << " Invalid address";
		throw std::runtime_error(ss.str());
	}

	result = (result << 8) | temp;

	return htonl(result);
}



// Accept all income connection.................................................
void Socket::acceptConnection()
{
	socklen_t	address_size = sizeof(_address);
	int			new_client = 0;

	new_client = accept(_serverFd, (sockaddr *)&_address, &address_size);
	if (new_client < 0)
	{
		if (errno != EWOULDBLOCK)
			_testResult(new_client, "Accept()");
	}
	else
	{
		_clientFd.push_back(new_client);
		std::cout << GRE "+++ Connected: Client: " RES << new_client
			<< GRE " IP: " RES << _serverData.getIp() << GRE " Port: " RES
				<< _serverData.getPort() << std::endl;
	}
}



// Manage the different connection, recepetion and delivery of data.............
bool Socket::handleConnection(struct pollfd * _poll_fd, int client_fd, Socket & server, char **envp)
{
	Request request(server);
	int bytes_read;
	_closeConnection = false;

	if ((bytes_read = _receiveData(_poll_fd, client_fd)) > 0)
	{
		request.store(_buf, _poll_fd, bytes_read);

		if (request.storeFinished())
		{
			_poll_fd->revents = POLLOUT;
			request.printRequest();
			request.parser();
			if (request.getError() == true)
				_closeConnection = true;
			request.process();

			if ((request.hasHeader("Content-Type") || request.getUri().find("?") != std::string::npos)
					&& _closeConnection == false)
				CGI cgi(_serverData, request, envp);
		}
	}
	if (bytes_read == 0)
	{
		_closeConnection = true;
	}
	if (_poll_fd->revents & POLLOUT)
	{
		Response response(request, _poll_fd, _contentType);
		request.cleanRequestData();
	}
	if (_closeConnection)
	{
		close(_poll_fd->fd);
		_compressVecClient(_poll_fd->fd);
		_poll_fd->fd = -1;
	}

	return _closeConnection;
}




// Receive the data from the client.............................................
int Socket::_receiveData(struct pollfd *_poll_fd, int client_fd)
{
	_result = recv(_poll_fd->fd, _buf, BUF_SIZE, 0);
	if (_result < 0)
	{
		_closeConnection = true;
		std::cout << "Recv() failed" << std::endl;
		return _result;
	}
	if (_result == 0)
	{
		_closeConnection = true;
		std::cout << RED "--- Disconnected: Client: " RES << client_fd
			<< RED " Port: " RES << _serverData.getPort() << std::endl;
		return _result;
	}
	_buf[_result] = '\0';

	return _result;
}



// Delete the closed element of the vector......................................
void Socket::_compressVecClient(int fd)
{
	std::vector<int>::iterator it = std::find(_clientFd.begin(),
		_clientFd.end(), fd);
	if (it != _clientFd.end())
	{
		_clientFd.erase(it);
	}
}



// Test all the results and check the port......................................
void Socket::_testResult(int iten_to_test, std::string msg)
{
	if (iten_to_test < 0 || _serverData.getPort() > 65535)
	{
		std::stringstream ss;
		if (_serverData.getPort() > 65535)
			ss << "Port: " << _serverData.getPort() << ": " << "invalid argument";
		else
			ss << "Port: " << _serverData.getPort() << ": " << msg << ": " << strerror(errno);
		throw std::runtime_error(ss.str());

		close(_serverFd);
	}
}



// Check the cgi extension.......................................................
void Socket::_checkCgiExt()
{
	std::vector<RouteData> currentRoute = _serverData.getRoutes();

	for (size_t i = 0; i < currentRoute.size(); i++)
	{
		if (currentRoute[i].getPath() == "/cgi-bin")
		{
			std::vector<std::string> cgi = currentRoute[i].getCgi();
			std::vector<std::string> cgiExt = currentRoute[i].getCgiExt();
			for (size_t j = 0; j < cgi.size(); j++)
			{
				bool isValidExtension = false;
				for (size_t k = 0; k < cgiExt.size(); k++)
				{
					if (cgi[j].substr(cgi[j].length() - cgiExt[k].length()) == cgiExt[k])
					{
						isValidExtension = true;
						break;
					}
				}
				if (!isValidExtension)
				{
					std::stringstream ss;
					ss << "Invalid CGI extension for file: " << cgi[j];
					throw std::runtime_error(ss.str());
				}
			}
			_cgi_status = true;
		}
	}
}
