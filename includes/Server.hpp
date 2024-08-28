#ifndef Server_HPP
#define Server_HPP

#include "Webserv.hpp"
#include "Socket.hpp"
#include "Configuration.hpp"
#include "ConfigFile.hpp"

#define POLL_SIZE	3000
#define TIMEOUT		(60 * 3 * 1000) // 3 minutes

class Socket;
class Configuration;

class Server {
	private:
		Server(void);

		std::list<Socket>	_serverList;
		pollfd				_pollFds[POLL_SIZE];
		int					_numFd;
		bool				_closeConnection;
		int					_result;

		void	_initServers(std::vector<ServerData> _vec_server_data);
		void	_runServers(char **envp);
		void	_initLoop();
		void	_compressPollArray();
		void	_handle_events(int index, char **envp);
		void	_addPollFd(std::vector<int> vec_client, size_t old_size);
		void	_cleanServers();
		void	_closeInheritedFds();


	public:
		Server(std::string config, char **envp);
		Server(Server & src);
		~Server(void);

		Server & operator=(Server & src);
};

#endif

