#ifndef DEF_SERVER
#define DEF_SERVER

#include "Webserv.hpp"
#include "RouteData.hpp"

class RouteData;

/**
 * @class ServerData
 * @brief Stores configuration data for a server block from a configuration file.
 */
class ServerData
{
	public:
		ServerData();
		ServerData(ServerData const& server);
		~ServerData();

		ServerData &operator=(ServerData const& src);

		// bool isValidPort(std::string const& port);
		// bool isValidIp(std::string const& ip);
		// void setIp(std::string const& ip);

		// Setters

		/**
		 * @brief Sets the IP address and port for the server.
		 * @param address The IP address and port in the format "IP:Port".
		 */
		void setPort(std::string const& address);

		/**
		 * @brief Sets the server name.
		 * @param serverName The server name.
		 */
		void setServerName(std::string const& serverName);

		 /**
		 * @brief Sets the root directory for the server.
		 * @param root The root directory path.
		 */
		void setRoot(std::string const& root);

		/**
		 * @brief Sets the index file for the server.
		 * @param index The index file.
		 */
		void setIndex(std::string const& index);

		/**
		 * @brief Sets the host for the server.
		 * @param host The host address.
		 */
		void setHost(std::string const& host);

		/**
		 * @brief Sets the maximum body size for client requests.
		 * @param mbz The maximum body size in bytes.
		 */
		void setMaxBodySize(int mbz);

		/**
		 * @brief Sets the error page for a specific HTTP error code.
		 * @param errorCode The HTTP error code.
		 * @param path The path to the error page.
		 */
		void setErrorPages(std::string const& errorCode, std::string const& path);

		/**
		 * @brief Adds a route to the server.
		 * @param route The route data.
		 */
		void setRoutes(RouteData const& route);

		// Getters
		std::string getIp() const;
		int getPort() const;
		std::string getServerName() const;
		std::string getRoot() const;
		std::string getIndex() const;
		std::string getHost() const;
		int getMaxBodySize() const;
		std::map<std::string, std::string> getErrorPages() const;
		std::vector<RouteData> getRoutes() const;

		/**
		 * @class ServerDataException
		 * @brief Exception class for server data errors.
		 */
		class ServerDataException : public std::exception
		{
			private:
				std::string _msg;
			public:
				ServerDataException(std::string msg) : _msg(msg) {}

				virtual const char* what() const throw()
				{
					return (_msg.c_str());
				}
				virtual ~ServerDataException() throw() {}
		};


	private:
		std::string _ip; // IP address of the server
		int _port; // Port number of the server
		std::string _serverName; // Server name
		std::string _host; // Host address
		std::string _root; // Root directory
		std::string _index; // Index file
		int _maxBodySize; // Maximum body size for client requests
		std::map<std::string, std::string> _errorPages; // Error pages for specific HTTP error codes
		std::vector<RouteData> _routes; // List of routes

		// bool _isIpset;
		bool _isPortSet;
		bool _isServerNameSet;
		bool _isHostSet;
		bool _isRootSet;
		bool _isIndexSet;
		bool _isMaxBodySize;
};

std::ostream &operator<<(std::ostream &o, ServerData const& src);

#endif

