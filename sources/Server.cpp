#include "../includes/Server.hpp"

//TODO:
// Close the server if an error occur and delete it from the list


// Constructor..................................................................
Server::Server(std::string file, char **envp)
{
	try
	{
		ConfigFile myConfigFile(file);
		Configuration myConfig(file);
		_initServers(myConfig.getServer());
		_runServers(envp);
	}
	catch(const std::exception& e)
	{
		std::cerr << RED << e.what() << RES << std::endl;
		_closeInheritedFds();
	}
}

Server::Server(Server & src)
{
	*this = src;
}

Server::~Server(void)
{
	return ;
}



// Assignment operator..........................................................
Server & Server::operator=(Server & src)
{
	if (this != &src)
	{
		_serverList.clear();
		std::list<Socket>::iterator it;
		for (it = src._serverList.begin(); it != src._serverList.end(); it++)
		{
			_serverList.push_back(*it);
		}
		for (int i = 0; i < src._numFd; i++)
			_pollFds[i] = src._pollFds[i];
		_numFd = src._numFd;
		_closeConnection = src._closeConnection;
	}
	return *this;
}



// Create all the servers.......................................................
void Server::_initServers(std::vector<ServerData> vec_server_data)
{
	_closeConnection = false;
	_numFd = 0;

	for (int i = 0; i < (int)vec_server_data.size(); i++)
	{
		try
		{
			Socket new_server(vec_server_data[i]);
			std::cout << GRE "Server is ready: " RES << new_server.getServerFd()
				<< GRE " IP: " RES << new_server.getServerData().getIp()
				<< GRE " Port: " RES << new_server.getServerData().getPort() << std::endl;

			_pollFds[_numFd].fd = new_server.getServerFd();
			_pollFds[_numFd].events = POLLIN;
			_pollFds[_numFd].revents = 0;

			_serverList.push_back(new_server);
			_numFd++;
		}
		catch (std::exception & e)
		{
			std::cerr << RED "Error: " << e.what() << RES << std::endl;
		};
	}
	if (_serverList.size() <= 0)
		throw std::runtime_error("No server avalible");
}



// Handle the signal (ctrl + c).................................................
void handle_signal(int signum)
{
	(void)signum;
	std::cout << RED "\nClossing the server..." RES << std::endl;
}



// Initialize the loop of program wwith poll()..................................
void Server::_runServers(char **envp)
{
	while (true)
	{
		signal(SIGINT, handle_signal);
		try
		{
			std::cout << YEL "\nWaiting for activity..." RES << std::endl;
			_result = poll(_pollFds, _numFd, TIMEOUT);
			if (_result == -1)
			{
				if (errno != EINTR)
					std::cerr << RED "Poll()" RES << std::endl;
				_cleanServers();
				return;
			}
			if (_result == 0)
			{
				std::cerr << RED "Timeout" RES << std::endl;
				_cleanServers();
				return;
			}
			for (int i = 0; i < _numFd; i++)
			{
				if (_pollFds[i].revents != 0)
				{
					_handle_events(i, envp);
				}
			}
		}
		catch (std::exception & e)
		{
			std::cerr << RED "Error: " << e.what() << RES << std::endl;
		};
		_compressPollArray();
	}
}



// Close all the fds before exiting.............................................
void Server::_cleanServers()
{
	// Close the server fds
	for (int i = _numFd - 1; i != -1; i--)
	{
		close(_pollFds[i].fd);
		_pollFds[i].fd = -1;
		_pollFds[i].events = 0;
		_pollFds[i].revents = 0;
		// std::cout << RED "Clossing fd: " RES << i + 1 << std::endl;
		_numFd--;
	}
	// Close the client fds
	_closeInheritedFds();
}



// Handle all the incoming events...............................................
void Server::_handle_events(int index, char **envp)
{
	for (std::list<Socket>::iterator it = _serverList.begin(); it != _serverList.end(); it++)
	{
		Socket & current_server = *it;
		std::vector<int> & vec_client = current_server.getClientFd();
		size_t old_size = vec_client.size();

		if (_pollFds[index].fd == current_server.getServerFd())
		{
			current_server.acceptConnection();
			_addPollFd(vec_client, old_size);
			// std::cout << MAG "Creating fd: " RES << _numFd << std::endl;////////////
		}
		else
		{
			std::vector<int>::iterator it = std::find(vec_client.begin(),
				vec_client.end(), _pollFds[index].fd);
			if (it != vec_client.end())
			{
				_closeConnection = current_server.handleConnection(&_pollFds[index],
					*it, current_server, envp);
			}
		}
	}
}



// Add the new FDS for the new income connection................................
void Server::_addPollFd(std::vector<int> vec_client, size_t old_size)
{
	for (size_t i = old_size; i < vec_client.size(); i++)
	{
		_pollFds[_numFd].fd = vec_client[i];
		_pollFds[_numFd].events = POLLIN;
		_pollFds[_numFd].revents = 0;
		_numFd++;
	}
}



// Compress the poll array if it had less connections...........................
void Server::_compressPollArray()
{
	if (_closeConnection == true)
	{
		_closeConnection = false;
		for (int i = 0; i < _numFd; i++)
		{
			if (_pollFds[i].fd == -1)
			{
				for(int j = i; j < _numFd - 1; j++)
				{
					_pollFds[j].fd = _pollFds[j+1].fd;
				}
				i--;
				_numFd--;
				// std::cout << MAG "Quantity of fd: " RES << _numFd << std::endl;
			}
		}
	}
}



// Close inherited fds..........................................................
void Server::_closeInheritedFds()
{
	for (int fd = 3; fd < 104; ++fd)
	{
		fcntl(fd, F_SETFD, FD_CLOEXEC);
		close(fd);
	}
}
