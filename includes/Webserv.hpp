#ifndef WEBSERV_HPP
#define WEBSERV_HPP

// C
#include <string.h>
#include <poll.h>
#include <fcntl.h>

// C++
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <cerrno>
#include <sstream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <dirent.h>

// System
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

// Containers
#include <vector>
#include <list>
#include <map>

#define BUF_SIZE	4096
#define PORT		8080

#include "Colors.h"

#endif
