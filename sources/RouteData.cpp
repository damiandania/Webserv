#include "RouteData.hpp"

RouteData::RouteData(ServerData const& server) : _path(""),
	_autoIndex(false), _root(""), _index(""),
	_upload(""),_isAutoIndexSet(false), _isRootSet(false),
	_isIndexSet(false), _isUploadSet(false)
{
	(void)server;
	// _httpMethods.push_back("GET");
	// _httpMethods.push_back("POST");
	// _httpMethods.push_back("DELETE");
}

RouteData::~RouteData()
{}

RouteData::RouteData(RouteData const& src) : _path(src._path), _httpMethods(src._httpMethods),
	_autoIndex(src._autoIndex), _root(src._root), _redirection(src._redirection), _index(src._index),
	_cgi(src._cgi), _cgiExt(src._cgiExt), _upload(src._upload), _errorPages(src._errorPages),
	_isAutoIndexSet(src._isAutoIndexSet), _isRootSet(src._isRootSet), _isIndexSet(src._isIndexSet),
	_isUploadSet(src._isUploadSet)
{}

RouteData &RouteData::operator=(RouteData const& src)
{
	if (this != &src)
	{
		_path = src._path;
		_httpMethods = src._httpMethods;
		_autoIndex = src._autoIndex;
		_root = src._root;
		_redirection = src._redirection;
		_index = src._index;
		_cgi = src._cgi;
		_cgiExt = src._cgiExt;
		_upload = src._upload;
		_isAutoIndexSet = src._isAutoIndexSet;
		_isRootSet = src._isRootSet;
		_isIndexSet = src._isIndexSet;
		_isUploadSet = src._isUploadSet;
		_errorPages = src._errorPages;
	}
	return *this;
}


void RouteData::setPath(std::string const& path)
{
	int count = std::count(path.begin(), path.end(), '/');
	if (count >= 2)
		throw RouteDataException("Route Path contains more than two '/' characters");
	_path = path;
}

void RouteData::setHttpMethods(std::vector<std::string> const& httpMethods)
{
	for (std::vector<std::string>::const_iterator it = httpMethods.begin(); it != httpMethods.end(); ++it)
	{
		if (std::count(httpMethods.begin(), httpMethods.end(), *it) > 1)
			throw RouteData::RouteDataException("Duplicate values found in allow_methods");
	}
	_httpMethods = httpMethods;
}

void RouteData::setAutoIndex(bool autoIndex)
{
	if (_isAutoIndexSet == true)
		throw RouteDataException("AutoIndex already set");
	_isAutoIndexSet = true;
	_autoIndex = autoIndex;
}

void RouteData::setRoot(std::string const& root)
{
	if (_isRootSet == true)
		throw RouteDataException("Root already set");
	_isRootSet = true;
	_root = root;
}

void RouteData::setIndex(std::string const& index)
{
	if (_isIndexSet == true)
		throw RouteDataException("Index already set");
	_isIndexSet = true;
	_index = index;
}

void RouteData::setRedirection(std::string const& errorCode, std::string const& redirection)
{
	if (_errorPages.find(errorCode) != _errorPages.end())
	{
		std::ostringstream oss;
		oss << errorCode;
		throw RouteDataException("Duplicate error page for error code " + oss.str());
	}
	_redirection[errorCode] = redirection;
}

void RouteData::setCgi(std::vector<std::string> const& cgi)
{
	for (std::vector<std::string>::const_iterator it = cgi.begin(); it != cgi.end(); ++it)
	{
		if (std::count(cgi.begin(), cgi.end(), *it) > 1)
			throw RouteData::RouteDataException("Duplicate values found in cgi_path");
	}
	_cgi = cgi;
}

void RouteData::setCgiExt(std::vector<std::string> const& cgiExt)
{
	for (std::vector<std::string>::const_iterator it = cgiExt.begin(); it != cgiExt.end(); ++it)
	{
		if (std::count(cgiExt.begin(), cgiExt.end(), *it) > 1)
			throw RouteData::RouteDataException("Duplicate values found in cgi_ext");
	}
	_cgiExt = cgiExt;
}

void RouteData::setUpload(std::string const& upload)
{
	if (_isUploadSet == true)
		throw RouteDataException("Upload already set");
	_isUploadSet = true;
	_upload = upload;
}

void RouteData::setErrorPages(std::string const& errorCode, std::string const& path)
{
	_errorPages[errorCode] = path;
}

std::string RouteData::getPath() const
{
	return _path;
}

std::vector<std::string> RouteData::getHttpMethods() const
{
	return _httpMethods;
}

bool RouteData::getAutoIndex() const
{
	return _autoIndex;
}

std::string RouteData::getRoot() const
{
	return _root;
}

std::string RouteData::getIndex() const
{
	return _index;
}

std::map<std::string, std::string> RouteData::getRedirection() const
{
	return _redirection;
}

std::vector<std::string> RouteData::getCgi() const
{
	return _cgi;
}

std::vector<std::string> RouteData::getCgiExt() const
{
	return _cgiExt;
}

std::string RouteData::getUpload() const
{
	return _upload;
}

std::map<std::string, std::string> RouteData::getErrorPages() const
{
	return _errorPages;
}

std::ostream &operator<<(std::ostream &o, RouteData const& src)
{
	o << "\n\tpath = [" << src.getPath() << "]\n";

	// Debug: Check HttpMethods
	std::vector<std::string> httpMethods = src.getHttpMethods();
	o << "\thttpMethods = [";
	for (size_t i = 0; i < httpMethods.size(); ++i)
	{
		o << httpMethods[i];
		if (i < httpMethods.size() - 1) o << ", ";
	}
	o << "]\n";

	// Debug: Check AutoIndex
	o << "\tautoIndex = [" << (src.getAutoIndex() ? "true" : "false") << "]\n";

	// Debug: Check Root
	o << "\troot = [" << src.getRoot() << "]\n";

	// Debug: Check Index
	o << "\tindex = [" << src.getIndex() << "]\n";

	// Debug: Check Redirection
	o << "\tredirection  = [" ;
	std::map<std::string, std::string> redir = src.getRedirection();
	std::map<std::string, std::string>::const_iterator it2;
	for (it2 = redir.begin(); it2 != redir.end(); ++it2)
	{
		o << it2->first << ": " << it2->second;
		if (it2 != --redir.end()) o << ", ";
	}
	o << "]\n";

	// Debug: Check CGI
	std::vector<std::string> cgi = src.getCgi();
	o << "\tcgi = [";
	for (size_t i = 0; i < cgi.size(); ++i)
	{
		o << cgi[i];
		if (i < cgi.size() - 1) o << ", ";
	}
	o << "]\n";

	// Debug: Check CGI Extensions
	std::vector<std::string> cgiExt = src.getCgiExt();
	o << "\tcgiExt = [";
	for (size_t i = 0; i < cgiExt.size(); ++i)
	{
		o << cgiExt[i];
		if (i < cgiExt.size() - 1) o << ", ";
	}
	o << "]\n";

	// Debug: Check Upload
	o << "\tupload = [" << src.getUpload() << "]\n";

	// Debug: Check ErrorPages
	std::map<std::string, std::string> errorPages = src.getErrorPages();
	o << "\terrorPages = [";
	std::map<std::string, std::string>::const_iterator it;
	for (it = errorPages.begin(); it != errorPages.end(); ++it)
	{
		o << it->first << ": " << it->second;
		if (it != --errorPages.end()) o << ", ";
	}
	o << "]";

	return o;
}
