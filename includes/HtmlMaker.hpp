#ifndef HtmlMaker_HPP
#define HtmlMaker_HPP

#include "Webserv.hpp"
#include "Request.hpp"

class Request;

std::string	defaultError(std::string const& code, std::string const& codeDescription);
void		getDirectories(std::ofstream& file, std::string root, std::string location);
std::string	makeAutoindex(std::string const& path, std::string const& root, Request & request);
void		getFiles(std::ofstream& file, std::string root, std::string path);
std::string	getDirectoryHtml(std::string const& root, std::string const& path);

#endif
