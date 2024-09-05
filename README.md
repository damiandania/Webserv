<p>
<img src="https://github.com/damiandania/damiandania/blob/main/Pics/Webserv.png"
	alt="Project pic" width="150" height="150"/>
</p>

# Webserv

Webserv is a lightweight web server implemented in C++ and C++98. It is designed to handle HTTP requests and serve static files, as well as execute CGI scripts. This project aims to provide a simple yet functional web server with support for various configurations.

## Features

- **HTTP/1.1 Support**: Handles basic HTTP methods such as GET, POST, and DELETE.
- **CGI Execution**: Supports execution of CGI scripts for dynamic content generation.
- **Configurable**: Allows configuration through external configuration files.
- **Autoindex**: Generates directory listings automatically when no index file is present.
- **Error Handling**: Customizable error pages for different HTTP error codes.

### Key Components

- **Configuration**: The [`ConfigFile`](command:_github.copilot.openSymbolInFile?%5B%7B%22scheme%22%3A%22file%22%2C%22authority%22%3A%22%22%2C%22path%22%3A%22%2FUsers%2Fdamiandania%2FGitFolder%2FWebserv%2Fincludes%2FConfigFile.hpp%22%2C%22query%22%3A%22%22%2C%22fragment%22%3A%22%22%7D%2C%22ConfigFile%22%5D "/Users/damiandania/GitFolder/Webserv/includes/ConfigFile.hpp") class handles reading and validating configuration files.
- **HTTP Request Handling**: The [`Request`](command:_github.copilot.openSymbolInFile?%5B%7B%22scheme%22%3A%22file%22%2C%22authority%22%3A%22%22%2C%22path%22%3A%22%2FUsers%2Fdamiandania%2FGitFolder%2FWebserv%2Fincludes%2FRequest.hpp%22%2C%22query%22%3A%22%22%2C%22fragment%22%3A%22%22%7D%2C%22Request%22%5D "/Users/damiandania/GitFolder/Webserv/includes/Request.hpp") and [`Response`](command:_github.copilot.openSymbolInFile?%5B%7B%22scheme%22%3A%22file%22%2C%22authority%22%3A%22%22%2C%22path%22%3A%22%2FUsers%2Fdamiandania%2FGitFolder%2FWebserv%2Fincludes%2FResponse.hpp%22%2C%22query%22%3A%22%22%2C%22fragment%22%3A%22%22%7D%2C%22Response%22%5D "/Users/damiandania/GitFolder/Webserv/includes/Response.hpp") classes manage incoming HTTP requests and outgoing responses.
- **CGI Support**: The [`CGI`](command:_github.copilot.openSymbolInFile?%5B%7B%22scheme%22%3A%22file%22%2C%22authority%22%3A%22%22%2C%22path%22%3A%22%2FUsers%2Fdamiandania%2FGitFolder%2FWebserv%2Fincludes%2FCGI.hpp%22%2C%22query%22%3A%22%22%2C%22fragment%22%3A%22%22%7D%2C%22CGI%22%5D "/Users/damiandania/GitFolder/Webserv/includes/CGI.hpp") class manages the execution of CGI scripts.
- **Autoindex**: The [`HtmlMaker`](command:_github.copilot.openSymbolInFile?%5B%7B%22scheme%22%3A%22file%22%2C%22authority%22%3A%22%22%2C%22path%22%3A%22%2FUsers%2Fdamiandania%2FGitFolder%2FWebserv%2Fincludes%2FHtmlMaker.hpp%22%2C%22query%22%3A%22%22%2C%22fragment%22%3A%22%22%7D%2C%22HtmlMaker%22%5D "/Users/damiandania/GitFolder/Webserv/includes/HtmlMaker.hpp") class generates directory listings when no index file is found.

### Building the Project

1. **Clone this repository:**
	```bash
	git clone https://github.com/damiandania/Webserv.git

2. **Navigate to the project directory:**
	```bash
	cd Webserv

3. **To build the project, run the following command:**
```bash
	make

4. **To build the project, run the following command:**
```bash
	./Webserv <config_file>
