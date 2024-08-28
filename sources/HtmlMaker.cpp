#include "HtmlMaker.hpp"

// Default error page...........................................................
std::string defaultError(std::string const& code, std::string const& codeDescription)
{
	std::cout << YEL "Using default error page" RES << std::endl;
	std::string body =
		"<!DOCTYPE html>\n"
		"<html lang=\"en\">\n"
		"<head>\n"
		"    <meta charset=\"UTF-8\">\n"
		"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
		"    <title>Error</title>\n"
		"    <style>\n"
		"        /* Reset some default browser styles */\n"
		"        body, h1, p, a {\n"
		"            margin: 0;\n"
		"            padding: 0;\n"
		"            font-family: 'Arial', sans-serif;\n"
		"            color: #333;\n"
		"        }\n"
		"\n"
		"        body {\n"
		"            background-color: #f4f4f4;\n"
		"            display: flex;\n"
		"            justify-content: center;\n"
		"            align-items: center;\n"
		"            height: 100vh;\n"
		"        }\n"
		"\n"
		"        .container {\n"
		"            text-align: center;\n"
		"            padding: 20px;\n"
		"            background: #fff;\n"
		"            box-shadow: 0 0 10px rgba(0,0,0,0.1);\n"
		"            border-radius: 10px;\n"
		"        }\n"
		"\n"
		"        .error-page {\n"
		"            max-width: 500px;\n"
		"            margin: auto;\n"
		"        }\n"
		"\n"
		"        .error-page h1 {\n"
		"            font-size: 5em;\n"
		"            margin-bottom: 0.5em;\n"
		"        }\n"
		"\n"
		"        .error-code {\n"
		"            font-size: 2em;\n"
		"            color: #e74c3c;\n"
		"        }\n"
		"\n"
		"        .error-message {\n"
		"            margin: 20px 0;\n"
		"            font-size: 1.2em;\n"
		"        }\n"
		"\n"
		"        .home-button {\n"
		"            display: inline-block;\n"
		"            padding: 10px 20px;\n"
		"            color: #fff;\n"
		"            background-color: #3498db;\n"
		"            border-radius: 5px;\n"
		"            text-decoration: none;\n"
		"            transition: background-color 0.3s;\n"
		"        }\n"
		"\n"
		"        .home-button:hover {\n"
		"            background-color: #2980b9;\n"
		"        }\n"
		"    </style>\n"
		"</head>\n"
		"<body>\n"
		"    <div class=\"container\">\n"
		"        <div class=\"error-page\">\n"
		"            <h1>Oops!</h1>\n"
		"            <p class=\"error-code\">Error <span id=\"error-code\"> " + code + "</span></p>\n"
		"            <p class=\"error-message\">" + codeDescription + "</p>\n"
		"            <a href=\"/\" class=\"home-button\">Go to Homepage</a>\n"
		"        </div>\n"
		"    </div>\n"
		"</body>\n"
		"</html>\n";

	return body;
}



// Make the autoindex...........................................................
std::string makeAutoindex(std::string const& path, std::string const& root, Request & request)
{
	std::string relativePath;
	if (path == request.getIndex())
		relativePath = root;
	else
		relativePath = root + path + "/";

	std::string tmpDir = root + "/tmp";
	struct stat info;
	if (stat(tmpDir.c_str(), &info) != 0)
	{
		if (mkdir(tmpDir.c_str(), 0755) != 0)
		{
			std::cerr << "Error: Autoindex: Creating directory" << std::endl;
			return "";
		}
	}
	else if (!(info.st_mode & S_IFDIR))
	{
		std::cerr << "Error: Autoindex: The path is not a directory" << std::endl;
		return "";
	}

	std::ofstream file((tmpDir + "/autoindex.html").c_str(), std::ios::out | std::ios::trunc);

	file << "<!DOCTYPE html>" << std::endl;
	file << "<html lang=\"en\">" << std::endl;
	file << "<head>" << std::endl;
	file << "<link rel=\"shortcut icon\" href=\"favicon.png\"/>" << std::endl;
	file << "    <meta charset=\"UTF-8\">" << std::endl;
	file << "    <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">" << std::endl;
	file << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">" << std::endl;
	file << "    <title>Webserv Autoindex</title>" << std::endl;
	file << "</head>" << std::endl;
	file << "<body>" << std::endl;
	file << "    <h1>Index of " + relativePath + "</h1>" << std::endl;

	getDirectories(file, relativePath, request.getLocation());
	getFiles(file, relativePath, relativePath.erase(0, root.size()));

	if (path != request.getIndex())
		file << "    <a href=\"" + request.getLocation() + request.getIndex() + "\"> Go Back</a>\n";

	file << "</body>" << std::endl;
	file << "</html>" << std::endl;
	file.close();

	return "/tmp/autoindex.html";
}



// Get the directories in the directory.........................................
void getDirectories(std::ofstream& file, std::string root, std::string location)
{
	DIR* dir;
	struct dirent* entry;

	if ((dir = opendir(root.c_str())) != NULL)
	{
		while ((entry = readdir(dir)) != NULL)
		{
			if (entry->d_type == DT_DIR)
			{
				if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
				{
					std::string link = "<a href=\"" + location + "/" + entry->d_name + "\">" + entry->d_name + "</a><br>\n";
					file << link;
				}
			}
		}
		closedir(dir);
	}
}

// Get the files in the directory...............................................
void getFiles(std::ofstream& file, std::string root, std::string path)
{
	DIR* dir;
	struct dirent* entry;

	if ((dir = opendir(root.c_str())) != NULL)
	{
		while ((entry = readdir(dir)) != NULL)
		{
			if (entry->d_type != DT_DIR)
			{
				file << "<a href=\"" << path << entry->d_name << "\">" << entry->d_name << "</a><br>" << std::endl;
			}
		}
		closedir(dir);
	}
}



// Get the default directory html...............................................
std::string getDirectoryHtml(std::string const& root, std::string const& path)
{
	std::string body =
		"<!DOCTYPE html>\n"
		"<html lang=\"en\">\n"
		"<head>\n"
		"    <meta charset=\"UTF-8\">\n"
		"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
		"    <title>Directory</title>\n"
		"    <style>\n"
		"        /* Reset some default browser styles */\n"
		"        body, h1, p, a {\n"
		"            margin: 0;\n"
		"            padding: 0;\n"
		"            font-family: 'Arial', sans-serif;\n"
		"            color: #333;\n"
		"        }\n"
		"\n"
		"        body {\n"
		"            background-color: #f4f4f4;\n"
		"            display: flex;\n"
		"            justify-content: center;\n"
		"            align-items: center;\n"
		"            height: 100vh;\n"
		"        }\n"
		"\n"
		"        .container {\n"
		"            text-align: center;\n"
		"            padding: 20px;\n"
		"            background: #fff;\n"
		"            box-shadow: 0 0 10px rgba(0,0,0,0.1);\n"
		"            border-radius: 10px;\n"
		"        }\n"
		"\n"
		"        .directory-page {\n"
		"            max-width: 500px;\n"
		"            margin: auto;\n"
		"        }\n"
		"\n"
		"        .directory-title {\n"
		"            font-size: 2em;\n"
		"            margin-bottom: 0.5em;\n"
		"        }\n"
		"\n"
		"        .directory-message {\n"
		"            margin: 20px 0;\n"
		"            font-size: 1.2em;\n"
		"        }\n"
		"\n"
		"        .home-button {\n"
		"            display: inline-block;\n"
		"            padding: 10px 20px;\n"
		"            color: #fff;\n"
		"            background-color: #3498db;\n"
		"            border-radius: 5px;\n"
		"            text-decoration: none;\n"
		"            transition: background-color 0.3s;\n"
		"        }\n"
		"\n"
		"        .home-button:hover {\n"
		"            background-color: #2980b9;\n"
		"        }\n"
		"    </style>\n"
		"</head>\n"
		"<body>\n"
		"    <div class=\"container\">\n"
		"        <div class=\"directory-page\">\n"
		"            <h1>Directory</h1>\n"
		"            <p class=\"directory-title\">Path: " + root + path + "</p>\n"
		"            <p class=\"directory-message\">This is a directory.</p>\n"
		"            <a href=\"/\" class=\"home-button\">Go to Homepage</a>\n"
		"        </div>\n"
		"    </div>\n"
		"</body>\n"
		"</html>\n";

	return body;
}
