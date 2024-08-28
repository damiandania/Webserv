#ifndef DEF_CGI
#define DEF_CGI

#include "Webserv.hpp"
#include "Request.hpp"
#include "RouteData.hpp"
#include <sys/wait.h>

class Request;

class CGI
{
	public:
		CGI(ServerData const& serv, Request &req, char **envp);
		CGI(CGI const& src);
		~CGI();
		CGI &operator=(CGI const& src);

		std::string getScript() const;
		int getExitStatus() const;
		pid_t getPid() const;

	private:
		CGI();
		std::string intToString(int value);
		std::string ExtractIpClient(Request const& req);
		std::string ExtractQuery(Request const& req);
		void addCgiVar(ServerData const& serv, Request const& req);
		void addVar(std::string const& key, std::string const& value);
		void getEnvp(char **envp);
		void convertEnvToEnvp();
		bool getCGIandExt(ServerData const& serv);
		bool interpreter();
		void extractContentTypeAndBody(std::string const& cgi_output);
		void execute(ServerData const& serv, Request &req, char **envp);

		void printArgs(char *args[]);
		void printEnv() const;

		char **_envp;
		std::vector<std::string> _env;
		std::vector<std::string> _vecCGI;
		std::vector<std::string> _vecCGIExt;
		int _exitStatus;
		std::string _script;
		std::string _cgiInterpreter;
		std::string _upload;
		pid_t _pid;
		Request &_req;

};

std::ostream &operator<<(std::ostream &o, CGI const& src);

#endif

