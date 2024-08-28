#include "Configuration.hpp"

Configuration::Configuration()
{}

Configuration::Configuration(std::string const& path)
{
	// printStream(path);
	parseConfig(path);
	// printServer();
}


Configuration::Configuration(Configuration const& src) : _server(src._server), _configFile(src._configFile)
{}

Configuration::~Configuration()
{}

Configuration & Configuration::operator=(Configuration const& src)
{
	if (this != &src)
	{
		_server = src._server;
		_configFile = src._configFile;
	}
	return *this;
}

void Configuration::printStream(std::string const& path)
{
	std::string line;
	std::ifstream inFile(path.c_str());
	std::cout << MAG << "File content : \n" << RES << std::endl;
	while (std::getline(inFile, line))
		std::cout << line << std::endl;
}

void Configuration::printServer() const
{
	for (size_t i = 0; i < _server.size(); ++i)
		std::cout << "Server " << i + 1 << "\n" << _server[i] << std::endl;
}

void Configuration::removeComments(std::string & line)
{
	size_t pos = line.find('#');
	if (pos != std::string::npos)
		line = line.substr(0, pos);
	size_t pos2 = line.find(';');
	if (pos2 != std::string::npos)
		line = line.substr(1, pos2);
}


void Configuration::removeSpaces(std::string const& path)
{
	std::ifstream inFile(path.c_str());
	std::string line;

	while (std::getline(inFile, line))
	{
		removeComments(line);
		line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));
		line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);

		// Remove spaces before semicolons
		size_t pos = 0;
		while ((pos = line.find(';', pos)) != std::string::npos)
		{
			while (pos > 0 && isspace(line[pos - 1]))
			{
				line.erase(pos - 1, 1);
				pos--;
			}
			pos++;
		}

		std::istringstream iss(line);
		std::string word;
		std::string noSpaceLine;
		while (iss >> word)
		{
			if (!noSpaceLine.empty())
				noSpaceLine += " ";
			noSpaceLine += word;
		}
		if (!noSpaceLine.empty())
			_configFile += noSpaceLine + "\n";
	}
	// std::cout << MAG << "Config file without comments and inutils spaces :" << std::endl;
	// std::cout << _configFile << RES << std::endl;
}

void Configuration::parseLocationBlock(std::stringstream & ss, ServerData & server)
{
	RouteData route(server);
	std::string line;
	while (std::getline(ss, line))
	{
		std::istringstream iss(line);
		std::string token;
		iss >> token;
		if (token == "location")
		{
			std::string path;
			std::string lastToken;
			std::string moreToken;
			iss >> path;
			if (path.empty() || path == "{")
				throw ParseConfigException("Parsing error: No value in path directive");
			iss >> lastToken;
			if (lastToken.empty() || lastToken != "{")
				throw ParseConfigException("Parsing error: No left brace in path directive");
			iss >> moreToken;
			if (!moreToken.empty())
				throw ParseConfigException("Parsing error: Too many arguments in path directive");
			route.setPath(path);
		}
		else if (token == "root")
		{
			std::string root;
			iss >> root;
			if (root.empty() || root == ";")
				throw ParseConfigException("Parsing error: No value in root directive");
			if (root[root.size() - 1] != ';')
				throw ParseConfigException("Parsing error: Missing semicolon in root directive");
			root.erase(root.size() - 1);
			if (root[root.size() - 1] == '/')
				root.erase(root.size() - 1);
			route.setRoot(root);
		}
		else if (token == "autoindex")
		{
			std::string autoIndex;
			iss >> autoIndex;
			if (autoIndex.empty() || autoIndex == ";")
				throw ParseConfigException("Parsing error: No value in autoindex directive");
			if (autoIndex[autoIndex.size() - 1] != ';')
				throw ParseConfigException("Parsing error: Missing semicolon in autoindex directive");
			autoIndex.erase(autoIndex.size() - 1);
			if (autoIndex == "on")
				route.setAutoIndex(true);
			else if (autoIndex == "off")
				route.setAutoIndex(false);
			else
				throw ParseConfigException("Parsing error: Autoindex must be on/off");
		}
		else if (token == "index")
		{
			std::string index;
			iss >> index;
			if (index.empty() || index == ";")
				throw ParseConfigException("Parsing error: No value in index directive");
			if (index[index.size() - 1] != ';')
				throw ParseConfigException("Parsing error: Missing semicolon in index directive");
			index.erase(index.size() - 1);
			if (index[0] != '/')
				index = "/" + index;
			if (index[index.size() - 1] == '/')
				index.erase(index.size() - 1);
			route.setIndex(index);
		}
		else if (token == "allow_methods")
		{
			std::vector<std::string> methods;
			std::string method;
			int semicolon = 0;
			while (iss >> method)
			{
				if (method[method.size() - 1] == ';')
				{
					semicolon = 1;
					method.erase(method.size() - 1);
				}
				methods.push_back(method);
			}
			if (semicolon == 0)
				throw ParseConfigException("Parsing error: Missing semicolon in allow_methods directive");
			if (methods.empty())
				throw ParseConfigException("Parsing error: No value in allow_methods directive");
			if (method != "GET" && method != "POST" && method != "DELETE")
				throw ParseConfigException("Parsing error: Allow_methods must be GET/POST/DELETE");
			route.setHttpMethods(methods);
		}
		else if (token == "cgi_path")
		{
			std::vector<std::string> cgiPaths;
			std::string cgi;
			int semicolon = 0;
			while (iss >> cgi)
			{
				if (cgi[cgi.size() - 1] == ';')
				{
					semicolon = 1;
					cgi.erase(cgi.size() - 1);
				}
				cgiPaths.push_back(cgi);
			}
			if (semicolon == 0)
				throw ParseConfigException("Parsing error: Missing semicolon in allow_methods directive");
			if (cgiPaths.empty())
				throw ParseConfigException("Parsing error: No value in allow_methods directive");
			route.setCgi(cgiPaths);
		}
		else if (token == "cgi_ext")
		{
			std::vector<std::string> cgiExts;
			std::string ext;
			int semicolon = 0;
			while (iss >> ext)
			{
				if (ext[ext.size() - 1] == ';')
				{
					semicolon = 1;
					ext.erase(ext.size() - 1);
				}
				cgiExts.push_back(ext);
			}
			if (semicolon == 0)
				throw ParseConfigException("Parsing error: Missing semicolon in allow_methods directive");
			if (cgiExts.empty())
				throw ParseConfigException("Parsing error: No value in allow_methods directive");
			route.setCgiExt(cgiExts);
		}
		else if (token == "upload")
		{
			std::string upload;
			iss >> upload;
			if (upload.empty() || upload == ";")
				throw ParseConfigException("Parsing error: No value in upload directive");
			if (upload[upload.size() - 1] != ';')
				throw ParseConfigException("Parsing error: Missing semicolon in upload directive");
			upload.erase(upload.size() - 1);
			if (upload[upload.size() - 1] != '/')
				upload = upload + "/";
			if (upload[0] == '/')
				upload = "." + upload;
			if (upload[0] != '/' && upload[0] != '.')
				upload = "./" + upload;
			route.setUpload(upload);
		}
		else if (token == "return")
		{
			std::string redir;
			std::string errorCode;
			iss >> errorCode >> redir;
			if (redir.empty() || redir == ";")
				throw ParseConfigException("Parsing error: No value in redirection directive");
			if (redir[redir.size() - 1] != ';')
				throw ParseConfigException("Parsing error: Missing semicolon in redirection directive");
			redir.erase(redir.size() - 1);
			route.setRedirection(errorCode, redir);
		}
		else if (token == "error_page")
		{
			std::string errorCode;
			std::string path;
			iss >> errorCode >> path;
			if (path.empty() || path == ";")
				throw ParseConfigException("Parsing error: No value in error_page directive");
			if (path[path.size() - 1] != ';')
				throw ParseConfigException("Parsing error: Missing semicolon in error_page directive");
			path.erase(path.size() - 1);
			if (path[0] != '/')
				path = "/" + path;
			route.setErrorPages(errorCode, path);
		}
		else if (token == "}")
			continue;
		else if (!line.empty())
		{
			throw ParseConfigException("Parsing error: Unknown directive in location block: " + line);
		}
	}
	server.setRoutes(route);
}


void Configuration::parseServerBlock(std::stringstream& ss, ServerData& server)
{
	std::string line;
	while (std::getline(ss, line))
	{
		std::istringstream iss(line);
		std::string token;
		iss >> token;

		if (token == "listen")
		{
			std::string listen;
			iss >> listen;
			if (listen.empty() || listen == ";")
				throw ParseConfigException("Parsing error: No value in listen directive");
			if (listen[listen.size() - 1] != ';')
				throw ParseConfigException("Parsing error: Missing semicolon in listen directive");
			listen.erase(listen.size() - 1);
			server.setPort(listen);
		}
		else if (token == "server_name")
		{
			std::string server_name;
			iss >> server_name;
			if (server_name.empty() || server_name == ";")
				throw ParseConfigException("Parsing error: No value in server_name directive");
			if (server_name[server_name.size() - 1] != ';')
				throw ParseConfigException("Parsing error: Missing semicolon in server_name directive");
			server_name.erase(server_name.size() - 1);
			server.setServerName(server_name);
		}
		else if (token == "root")
		{
			std::string root;
			iss >> root;
			if (root.empty() || root == ";")
				throw ParseConfigException("Parsing error: No value in root directive");
			if (root[root.size() - 1] != ';')
				throw ParseConfigException("Parsing error: Missing semicolon in root directive");
			root.erase(root.size() - 1);
			if (root[root.size() - 1] == '/')
				root.erase(root.size() - 1);
			server.setRoot(root);
		}
		else if (token == "index")
		{
			std::string index;
			iss >> index;
			if (index.empty() || index == ";")
				throw ParseConfigException("Parsing error: No value in index directive");
			if (index[index.size() - 1] != ';')
				throw ParseConfigException("Parsing error: Missing semicolon in index directive");
			index.erase(index.size() - 1);
			if (index[0] != '/')
				index = "/" + index;
			if (index[index.size() - 1] == '/')
				index.erase(index.size() - 1);
			server.setIndex(index);
		}
		else if (token == "host")
		{
			std::string host;
			iss >> host;
			if (host.empty() || host == ";")
				throw ParseConfigException("Parsing error: No value in listen directive");
			if (host[host.size() - 1] != ';')
				throw ParseConfigException("Parsing error: Missing semicolon in host directive");
			host.erase(host.size() - 1);
			server.setHost(host);
		}
		else if (token == "error_page")
		{
			std::string errorCode;
			std::string path;
			iss >> errorCode >> path;
			if (path.empty() || path == ";")
				throw ParseConfigException("Parsing error: No value in error_page directive");
			if (path[path.size() - 1] != ';')
				throw ParseConfigException("Parsing error: Missing semicolon in error_page directive");
			path.erase(path.size() - 1);
			if (path[0] != '/')
				path = "/" + path;
			server.setErrorPages(errorCode, path);
		}
		else if (token == "client_max_body_size")
		{
			int maxBodySize;
			if (!(iss >> maxBodySize) || maxBodySize <= 0)
				throw ParseConfigException("Parsing error: Invalid or non-positive value for client_max_body_size");
			std::string semicolon;
			if (!(iss >> semicolon) || semicolon != ";")
				throw ParseConfigException("Parsing error: Missing semicolon after client_max_body_size value");
			server.setMaxBodySize(maxBodySize);
		}
		else if (token == "location")
		{
			int openBraces = 1;
			std::stringstream location_ss;
			std::string location_line;
			location_ss << line << "\n";
			while (std::getline(ss, location_line))
			{
				if (location_line.find('{') != std::string::npos)
					openBraces++;
				if (location_line.find('}') != std::string::npos)
					openBraces--;
				if (openBraces == 0)
					break;
				location_ss << location_line << "\n";
			}
			parseLocationBlock(location_ss, server);
		}
		else if (!line.empty())
		{
			throw ParseConfigException("Parsing error: Unknown directive " + line);
		}
	}
}

void Configuration::parseConfig(std::string const& path)
{
	removeSpaces(path);
	std::stringstream ss(_configFile);
	std::string block;
	std::string line;
	bool serverFound = false;
	while (std::getline(ss, line))
	{
		if (line.find("server {") != std::string::npos)
		{
			serverFound = true;
			int openBraces = 1;
			block.clear();

			while (std::getline(ss, line))
			{
				if (line.find('{') != std::string::npos)
					openBraces++;
				if (line.find('}') != std::string::npos)
					openBraces--;
				if (openBraces == 0)
					break;
				block += line + "\n";
			}
			std::stringstream block_ss(block);
			// std::cout << MAG << "Server Block:\n" << RES << block_ss.str() << std::endl;
			ServerData server;
			parseServerBlock(block_ss, server);
			_server.push_back(server);
		}
		if (serverFound == false)
			throw ParseConfigException("Parsing Error : No server found in the configuration file.");
	}
}


std::vector<ServerData> Configuration::getServer() const
{
	return _server;
}
