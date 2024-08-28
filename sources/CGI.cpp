#include "CGI.hpp"
#include <string.h>

CGI::CGI(CGI const& src) : _env(src._env), _vecCGI(src._vecCGI),_vecCGIExt(src._vecCGIExt), _exitStatus(src._exitStatus),
	_script(src._script), _cgiInterpreter(src._cgiInterpreter), _upload(src._upload), _pid(src._pid), _req(src._req)
{
}

CGI::~CGI()
{
	if (_envp)
	{
		for (size_t i = 0; _envp[i]; ++i)
		{
			if (_envp[i])
				free(_envp[i]); // Utilizar free en lugar de delete[] para strdup
		}
		delete[] _envp; // Liberar el arreglo de punteros _envp
	}
}

CGI::CGI(ServerData const& serv, Request &req, char **envp) : _envp(NULL), _exitStatus(0),
	_script(""), _cgiInterpreter(""), _upload(""), _pid(0), _req(req)
{
	std::string uri = req.getUri();
	std::size_t pos = uri.find("/forms");
	if (pos != std::string::npos)
		uri.replace(pos, std::string("/forms").length(), "/cgi-bin");
	if (uri.find("?") != std::string::npos)
		uri = uri.substr(0, uri.find("?"));
	_script = serv.getRoot() + uri;
	// std::cout << "Script: " << _script << std::endl;
	execute(serv, req, envp);
}

CGI & CGI::operator=(CGI const& src)
{
	if (this != &src)
	{
		_env = src._env;
		_vecCGI = src._vecCGI;
		_vecCGIExt = src._vecCGIExt;
		_exitStatus = src._exitStatus;
		_script = src._script;
		_cgiInterpreter = src._cgiInterpreter;
		_upload = src._upload;
		_pid = src._pid;
		_req = src._req;
		convertEnvToEnvp();
	}
	return *this;
}

int CGI::getExitStatus() const
{
	return _exitStatus;
}

pid_t CGI::getPid() const
{
	return _pid;
}

std::string CGI::getScript() const
{
	return _script;
}


void CGI::addVar(std::string const& key, std::string const& value)
{
	std::string var = key + "=" + value;
	_env.push_back(var);
}

std::string CGI::intToString(int value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

std::string CGI::ExtractIpClient(Request const& req)
{
	std::string ipClient = req.getHeader("Host");
	size_t pos = ipClient.find(':');
	return ipClient.substr(0, pos);
}

std::string CGI::ExtractQuery(Request const& req)
{
	std::string query = req.getUri();
	size_t pos = query.find('?');
	return query.substr(pos + 1);
}


void CGI::getEnvp(char **envp)
{
	for (char **current = envp; *current; ++current)
		_env.push_back(std::string(*current));
}

bool CGI::getCGIandExt(ServerData const& serv)
{
	std::vector<RouteData> currentRoute = serv.getRoutes();

	for (size_t i = 0; i < currentRoute.size(); i++)
	{
		if (currentRoute[i].getPath() == "/cgi-bin")
		{
			_vecCGI = currentRoute[i].getCgi();
			_vecCGIExt = currentRoute[i].getCgiExt();
			_upload = currentRoute[i].getUpload();
			// std::cout << "upload is here : " << _upload << std::endl;
			return true;
		}
	}
	return false;
}

void CGI::addCgiVar(ServerData const& serv, Request const& req)
{
	std::string ipClient = ExtractIpClient(req);
	std::string query = ExtractQuery(req);
	addVar("REDIRECT_STATUS", "200");
	// addVar("AUTH_TYPE", req.getAuthentification()); // Type d'authentification (Basic, Digest, etc.)/(Request)
	if (req.getMethod() == "POST")
	{
		addVar("CONTENT_LENGTH", req.getHeader("Content-Length")); // only for POST(Request)
		if (req.getHeader("Content-Type").length())
			addVar("CONTENT_TYPE", req.getHeader("Content-Type")); // only for POST(Request)
	}
	else if (req.getMethod() == "GET")
	{
		addVar("QUERY_STRING", query); // after the ? in the URL(Request)
	}
	addVar("GATEWAY_INTERFACE", "CGI/1.1");
	addVar("PATH_INFO", req.getPath()); // path asked by the client(Request)
	addVar("PATH_TRANSLATED", serv.getRoot() + req.getPath()); // full path to the file in the server
	addVar("REMOTE_ADDR", ipClient); // IP address of the client(Request)
	addVar("REQUEST_METHOD", req.getMethod()); // method of the request
	addVar("SCRIPT_NAME", _script); // path to the CGI script
	addVar("SCRIPT_FILENAME", _script); // full path to the CGI script
	addVar("SERVER_NAME", serv.getServerName()); // name of the server
	addVar("SERVER_PORT", intToString(serv.getPort())); // port of the server
	addVar("SERVER_PROTOCOL", "HTTP/1.1");
	addVar("SERVER_SOFTWARE", serv.getServerName()); // name of the server software
	addVar("UPLOAD_DIR", _upload); // directory where the files will be uploaded
}

void CGI::convertEnvToEnvp()
{
	_envp = new char*[_env.size() + 1];
	for (size_t i = 0; i < _env.size(); ++i)
		_envp[i] = strdup(_env[i].c_str());
	_envp[_env.size()] = NULL;
}

bool CGI::interpreter()
{
	std::string scriptExtension = _script.substr(_script.find_last_of(".") + 1);

	if (scriptExtension == "php")
	{
		if (std::find(_vecCGIExt.begin(), _vecCGIExt.end(), ".php") != _vecCGIExt.end()
			&& std::find(_vecCGI.begin(), _vecCGI.end(), "/usr/bin/php-cgi") != _vecCGI.end())
		{
			_cgiInterpreter = "/usr/bin/php-cgi";
			return true;
		}
		else
		{
			std::cerr << "Error: PHP interpreter not found or extension" << std::endl;
			return false;
		}
	}
	else if (scriptExtension == "py")
	{
		if (std::find(_vecCGIExt.begin(), _vecCGIExt.end(), ".py") != _vecCGIExt.end()
			&& std::find(_vecCGI.begin(), _vecCGI.end(), "/usr/bin/python3") != _vecCGI.end())
		{
			_cgiInterpreter = "/usr/bin/python3";
			return true;
		}
		else
		{
			std::cerr << "Error: Python interpreter not found or extension" << std::endl;
			return false;
		}
	}
	else
	{
		std::cerr << "Error: Unsupported script extension" << std::endl;
		return false;
	}
}

void CGI::execute(ServerData const& serv, Request &req, char **envp)
{
	if (getCGIandExt(serv) == false || interpreter() == false)
	{
		req.setCode("500");
		return;
	}
	addCgiVar(serv, _req);
	getEnvp(envp);
	// printEnv();
	convertEnvToEnvp();
	int pipe_in[2];
	int pipe_out[2];
	if (pipe(pipe_in) == -1 || pipe(pipe_out) == -1)
	{
		perror("pipe");
		return;
	}
	_pid = fork();
	if (_pid == -1)
	{
		perror("fork");
		return;
	}

	if (_pid == 0)
	{
		close(pipe_in[1]);
		close(pipe_out[0]);
		dup2(pipe_in[0], STDIN_FILENO);
		dup2(pipe_out[1], STDOUT_FILENO);
		char *args[] = { const_cast<char*>(_cgiInterpreter.c_str()), const_cast<char*>(_script.c_str()), NULL };
		execve(_cgiInterpreter.c_str(), args, _envp);
		perror("Execve failed");
		req.setCode("500");
		return ;
	}
	else
	{
		close(pipe_in[0]);
		close(pipe_out[1]);
		if (_req.getMethod() == "POST")
		{
			std::string body = _req.getBody();
			write(pipe_in[1], body.c_str(), body.size());
		}
		close(pipe_in[1]);
		char buffer[1024];
		std::string cgi_output;
		ssize_t bytes_read;
		while ((bytes_read = read(pipe_out[0], buffer, sizeof(buffer))) > 0)
			cgi_output.append(buffer, bytes_read);
		close(pipe_out[0]);
		int status;
		waitpid(_pid, &status, 0);
		_exitStatus = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
		if (cgi_output.find("Contact deleted successfully") != std::string::npos)
			req.setPath("/forms/delete_ok.html");
		else if (cgi_output.find("The file has been uploaded successfully") != std::string::npos)
			req.setPath("/forms/upload_ok.html");
		else if (cgi_output.find("Unsupported Media Type") != std::string::npos)
			req.setCode("415"); // Unsupported Media Type
		else if (cgi_output.find("Form submitted successfully") != std::string::npos)
			req.setPath("/forms/contact_ok.html");
		// std::cout << "Raw Output: " << cgi_output << std::endl;
	}
}

void CGI::printArgs(char *args[])
{
	std::cout << "Arguments for execve:" << std::endl;
	for (int i = 0; args[i] != NULL; ++i)
		std::cout << "args[" << i << "]: " << args[i] << std::endl;
}

void CGI::printEnv() const
{
	std::cout << "Environment variables for execve:" << std::endl;
	for (size_t i = 0; i < _env.size() - 1; ++i)
		std::cout << _env[i] << std::endl;
}

std::ostream &operator<<(std::ostream &o, CGI const& src)
{
	o << "CGI script path: " << src.getScript() << ", PID: " << src.getPid() << ", Exit status: " << src.getExitStatus();
	return o;
}
