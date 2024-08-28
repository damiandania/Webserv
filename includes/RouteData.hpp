#ifndef DEF_ROUTEDATA
#define DEF_ROUTEDATA

#include "Webserv.hpp"
#include "ServerData.hpp"

class ServerData;

/**
 * @class RouteData
 * @brief Stores configuration data for a specific route within a server block.
 */
class RouteData
{
	public:
		RouteData(ServerData const& server);
		~RouteData();
		RouteData(RouteData const& src);
		RouteData &operator=(RouteData const& src);

		// Setters

		/**
		 * @brief Sets the path for the route.
		 * @param path The path of the route.
		 */
		void setPath(std::string const& path);

		/**
		 * @brief Sets the root directory for the route.
		 * @param root The root directory path.
		 */
		void setRoot(std::string const& root);

		/**
		 * @brief Enables or disables autoindex for the route.
		 * @param autoIndex True to enable autoindex, false to disable.
		 */
		void setAutoIndex(bool autoIndex);

		/**
		 * @brief Sets the index file for the route.
		 * @param index The index file.
		 */
		void setIndex(std::string const& index);

		/**
		 * @brief Sets the allowed HTTP methods for the route.
		 * @param methods A vector of allowed HTTP methods.
		 */
		void setHttpMethods(std::vector<std::string> const& httpMethods);

		/**
		 * @brief Sets the CGI paths for the route.
		 * @param cgiPaths A vector of CGI paths.
		 */
		void setCgi(std::vector<std::string> const& cgi);

		/**
		 * @brief Sets the CGI extensions for the route.
		 * @param cgiExts A vector of CGI extensions.
		 */
		void setCgiExt(std::vector<std::string> const& cgiExt);

		/**
		 * @brief Sets the upload path for the route.
		 * @param uploadPath The upload path.
		 */
		void setUpload(std::string const& upload);

		/**
		 * @brief Sets the redirection URL for the route.
		 * @param errorCode The HTTP error code.
		 * @param redir The redirection URL.
		 */
		void setRedirection(std::string const& errorCode, std::string const& redirection);

		/**
		 * @brief Sets the error page for a specific HTTP error code.
		 * @param errorCode The HTTP error code.
		 * @param path The path to the error page.
		 */
		void setErrorPages(std::string const& errorCode, std::string const& path);

		// Getters
		std::string getPath() const;
		std::vector<std::string> getHttpMethods() const;
		bool getAutoIndex() const;
		std::string getRoot() const;
		std::string getIndex() const;
		std::map<std::string, std::string> getRedirection() const;
		std::vector<std::string> getCgi() const;
		std::vector<std::string> getCgiExt() const;
		std::string getUpload() const;
		std::map<std::string, std::string> getErrorPages() const;

		/**
		 * @class ServerDataException
		 * @brief Exception class for server data errors.
		 */
		class RouteDataException : public std::exception
		{
			private:
				std::string _msg;
			public:
				RouteDataException(std::string msg) : _msg(msg) {}

				virtual const char* what() const throw()
				{
					return (_msg.c_str());
				}
				virtual ~RouteDataException() throw() {}
		};

	private:
		RouteData();

		std::string _path; // Specifies the URL path for which this route is configured.
		std::vector<std::string> _httpMethods; // List of HTTP methods allowed for this route.
		bool _autoIndex; // Diretory listing enable or no
		std::string _root; // Specifies the filesystem root directory for this route.
		std::map<std::string, std::string> _redirection; // return /.. Specifies a URL to redirect requests to for this route.
		std::string _index; // Default file to return when the route matches a directory.
		std::vector<std::string> _cgi; // List of CGI executable paths to use for handling requests.
		std::vector<std::string> _cgiExt; // List of file extensions for which CGI should be used.
		std::string _upload; // Path to the directory where uploaded files should be stored.
		std::map<std::string, std::string> _errorPages; // Error pages from block server

		bool _isAutoIndexSet;
		bool _isRootSet;
		bool _isIndexSet;
		bool _isUploadSet;
};

std::ostream &operator<<(std::ostream &o, RouteData const& src);

#endif

