<h1 id="title" align="center">
	WEBSERVER
</h1>

<h3 align="center">
	<a href="#socketpair">socketpair()</a>
	<span> · </span>
	<a href="#htons">htons()</a>
	<span> · </span>
	<a href="#htonl">htonl()</a>
	<span> · </span>
	<a href="#ntohs">ntohs()</a>
	<span> · </span>
	<a href="#ntohl">ntohl()</a>
	<span> · </span>
	<a href="#select">select()</a>
	<span> · </span>
	<a href="#poll">poll()</a>
	<span> · </span>
	<a href="#epoll">epoll()</a>
	<span> · </span>
	<a href="#kqueue">kqueue()</a>
	<span> · </span>
	<a href="#socket">socket()</a>
	<span> · </span>
    <a href="#setsockopt">socket()</a>
	<span> · </span>
	<a href="#bind">bind()</a>
	<span> · </span>
	<a href="#listen">listen()</a>
	<span> · </span>
	<a href="#accept">accept()</a>
	<span> · </span>
	<a href="#send">send()</a>
	<span> · </span>
	<a href="#recv">recv()</a>
	<span> · </span>
	<a href="#connect">connect()</a>
	<span> · </span>
	<a href="#getaddrinfo">getaddrinfo()</a>
	<span> · </span>
	<a href="#freeaddrinfo">freeaddrinfo()</a>
	<span> · </span>
	<a href="#getsockopt">getsockopt()</a>
	<span> · </span>
	<a href="#getsockname">getsockname()</a>
	<span> · </span>
	<a href="#getprotobyname">getprotobyname()</a>
	<span> · </span>
</h3>

<h1 id="title" align="center">
	Old Project
</h1>

<h3 align="center">
	<a href="#signal">Signal()</a>
    <span> · </span>
    <a href="#kill">Kill()</a>
    <span> · </span>
    <a href="#stat">Stat()</a>
    <span> · </span>
    <a href="#opendir">Opendir()</a>
    <span> · </span>
    <a href="#readdir">Readdir()</a>
    <span> · </span>
    <a href="#closedir">Closedir()</a>
    <span> · </span>
    <a href="#read">Read()</a>
	<span> · </span>
    <a href="#open">Open()</a>
	<span> · </span>
    <a href="#close">Close()</a>
	<span> · </span>
    <a href="#access">Access()</a>
	<span> · </span>
    <a href="#dup">Dup()/Dup2()</a>
	<span> · </span>
    <a href="#exec">Execve()</a>
	<span> · </span>
    <a href="#exit">Exit()</a>
	<span> · </span>
    <a href="#fork">Fork()</a>
	<span> · </span>
    <a href="#wait">Wait() and Waitpid()</a>
	<span> · </span>
    <a href="#pipe">Pipe()</a>
	<span> · </span>
    <a href="#unlink">Unlink()</a>
	<span> · </span>
    <a href="#perror">Perror()</a>
	<span> · </span>
    <a href="#strerror">Strerror()</a>
	<span> · </span>
</h3>


<div id="socketpair">

# Socketpair() function

The socketpair() function in the C language is used to create a pair of connected sockets for inter-process communication. These sockets are typically of the same type, either SOCK_STREAM for reliable, two-way, connection-based byte streams, or SOCK_DGRAM for unreliable datagrams.

The signature of the socketpair() function is:
```bash
#include <sys/socket.h>

int socketpair(int domain, int type, int protocol, int sv[2]);
```
The parameters of the socketpair() function are:

- `domain`: This specifies the communication domain in which the socket should be created. Commonly used domains include AF_UNIX for local communication and AF_INET for IPv4 communication.
type: This specifies the type of socket to be created, such as SOCK_STREAM for a stream socket or SOCK_DGRAM for a datagram socket.
- `protocol`: This parameter specifies the protocol to be used with the sockets. For most applications, a protocol value of 0 can be used, and the system will choose the appropriate protocol based on the specified domain and type.
- `sv[2]`: This is an array of two integers that will be filled in by the function with the file descriptors for the newly created sockets.
The socketpair() function creates a pair of connected sockets and stores their file descriptors in the array sv. These sockets are connected to each other, allowing bidirectional communication between processes.

Here's an example of how socketpair() might be used to create a pair of connected stream sockets:
```bash
#include <sys/socket.h>
#include <stdio.h>

int main() {
    int sv[2];

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == -1) {
        perror("socketpair");
        return 1;
    }

    printf("Socket pair created with file descriptors: %d and %d\n", sv[0], sv[1]);

    // Now you can use sv[0] and sv[1] as connected sockets for communication.

    return 0;
}
```
<br>
<br>
<div id="htons">

# htons() function

The htons() function in the C language converts a 16-bit unsigned integer from host byte order to network byte order (big-endian).

The signature of the htons() function is:
```bash
#include <arpa/inet.h>

uint16_t htons(uint16_t hostshort);
```
The parameter of the htons() function is:

- `hostshort`: This is the 16-bit unsigned integer value to be converted from host byte order to network byte order.
The htons() function ensures that the integer value is represented in the standard network byte order, which is big-endian. This is important for portability and interoperability between different systems with different byte orders.

Here's an example of how htons() might be used to convert a port number from host byte order to network byte order:
```bash
#include <arpa/inet.h>
#include <stdio.h>

int main() {
    uint16_t host_port = 12345;
    uint16_t network_port = htons(host_port);

    printf("Host port: %d\n", host_port);
    printf("Network port (in big-endian): %d\n", network_port);

    return 0;
}
```
<br>
<br>
<div id="htonl">

# htonl() function

The htonl() function in the C language converts a 32-bit unsigned integer from host byte order to network byte order (big-endian).

The signature of the htonl() function is:
```bash
#include <arpa/inet.h>

uint32_t htonl(uint32_t hostlong);
```
The parameter of the htonl() function is:

- `hostlong`: This is the 32-bit unsigned integer value to be converted from host byte order to network byte order.
The htonl() function ensures that the integer value is represented in the standard network byte order, which is big-endian. This is important for portability and interoperability between different systems with different byte orders.

Here's an example of how htonl() might be used to convert an IP address from host byte order to network byte order:
```bash
#include <arpa/inet.h>
#include <stdio.h>

int main() {
    uint32_t host_ip = 0xC0A80101; // 192.168.1.1 in hexadecimal
    uint32_t network_ip = htonl(host_ip);

    printf("Host IP address: %d.%d.%d.%d\n", (host_ip >> 24) & 0xFF, (host_ip >> 16) & 0xFF, (host_ip >> 8) & 0xFF, host_ip & 0xFF);
    printf("Network IP address (in big-endian): %d.%d.%d.%d\n", (network_ip >> 24) & 0xFF, (network_ip >> 16) & 0xFF, (network_ip >> 8) & 0xFF, network_ip & 0xFF);

    return 0;
}
```
<br>
<br>
<div id="ntohs">

# ntohs() function
The ntohs() function in the C language converts a 16-bit unsigned integer from network byte order (big-endian) to host byte order.

The signature of the ntohs() function is:
```bash
#include <arpa/inet.h>

uint16_t ntohs(uint16_t netshort);
```
The parameter of the ntohs() function is:

- `netshort`: This is the 16-bit unsigned integer value to be converted from network byte order to host byte order.
The ntohs() function converts the integer value from the standard network byte order, which is big-endian, to the byte order of the host system. This is important for portability and interoperability between different systems with different byte orders.

Here's an example of how ntohs() might be used to convert a port number from network byte order to host byte order:
```bash
#include <arpa/inet.h>
#include <stdio.h>

int main() {
    uint16_t network_port = 0x3039; // Port number 12345 in hexadecimal
    uint16_t host_port = ntohs(network_port);

    printf("Network port (in big-endian): %d\n", network_port);
    printf("Host port: %d\n", host_port);

    return 0;
}
```
<br>
<br>
<div id="select">

# Select() function
The select() function in the C language is used for monitoring file descriptors (such as those representing sockets, files, or pipes) to see if they are ready for reading, writing, or if an exception occurred.

The signature of the select() function is:
```bash
#include <sys/select.h>

int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
```
The parameters of the select() function are:

- `nfds`: This is the highest-numbered file descriptor in any of the sets, plus 1.
- `readfds`: This is a pointer to a set of file descriptors to monitor for reading.
- `writefds`: This is a pointer to a set of file descriptors to monitor for writing.
- `exceptfds`: This is a pointer to a set of file descriptors to monitor for exceptions.
- `timeout`: This is a pointer to a struct timeval specifying the maximum time to wait for any of the descriptors to become ready. If NULL, select() will block indefinitely until a descriptor is ready.

The select() function allows a program to simultaneously check multiple file descriptors to see if they are ready for I/O operations without blocking. It can be used for multiplexing I/O operations in network programming or for handling input from multiple sources in a graphical user interface.

Here's a simple example of how select() might be used to monitor file descriptors for reading:
```bash
#include <sys/select.h>
#include <stdio.h>

int main() {
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds); // Monitor standard input (stdin) for reading

    int ready = select(STDIN_FILENO + 1, &readfds, NULL, NULL, NULL);
    if (ready == -1) {
        perror("select");
        return 1;
    }

    if (FD_ISSET(STDIN_FILENO, &readfds)) {
        printf("Data is available for reading on stdin.\n");
        // Perform read operation on stdin
    }

    return 0;
}
```
<br>
<br>
<div id="poll">

# Poll() function
The poll() function in the C language is used for monitoring multiple file descriptors for events, similar to the select() function, but with a different interface.

The signature of the poll() function is:
```bash
#include <poll.h>

int poll(struct pollfd *fds, nfds_t nfds, int timeout);
```
The parameters of the poll() function are:

- `fds`: This is a pointer to an array of struct pollfd elements, each representing a file descriptor to monitor and the events to monitor for.
- `nfds`: This is the number of elements in the fds array.
- `timeout`: This is the maximum time to wait for an event to occur, in milliseconds. A timeout of -1 blocks indefinitely, while a timeout of 0 returns immediately.

The poll() function monitors the file descriptors specified in the fds array for the events specified in each struct pollfd element. It can monitor for events such as readability, writability, or errors.

Here's an example of how poll() might be used to monitor multiple file descriptors for reading:
```bash
#include <poll.h>
#include <stdio.h>

int main() {
    struct pollfd fds[1];
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;

    int ready = poll(fds, 1, -1);
    if (ready == -1) {
        perror("poll");
        return 1;
    }

    if (fds[0].revents & POLLIN) {
        printf("Data is available for reading on stdin.\n");
        // Perform read operation on stdin
    }

    return 0;
}
```
<br>
<br>
<div id="epoll">

# Epoll() function

The epoll() function in the C language is used for scalable I/O event notification, particularly for managing large numbers of file descriptors efficiently.

The signature of the epoll() function is:
```bash
#include <sys/epoll.h>

int epoll_create(int size);
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
```
The parameters of the epoll() function are:

- `size`: This is a hint for the kernel about the size of the event backing store. This parameter is ignored since Linux 2.6.8.
- `epfd`: This is the file descriptor returned by epoll_create().
- `op`: This specifies the operation to perform, such as adding, removing, or modifying a file descriptor in the epoll instance.
- `fd`: This is the file descriptor to add, remove, or modify in the epoll instance.
- `event`: This is a pointer to an epoll_event structure containing information about the event to monitor.
- `events`: This is a pointer to an array of epoll_event events where detected events will be stored.
- `maxevents`: This is the maximum number of events to return in the events array.
- `timeout`: This specifies the maximum wait time in milliseconds. A negative value means wait indefinitely, 0 means return immediately if no events are available.

The epoll() interface involves three main system calls: epoll_create(), epoll_ctl(), and epoll_wait().

epoll_create(): Creates an epoll instance and returns a file descriptor representing the instance.
epoll_ctl(): Modifies the epoll instance by adding, modifying, or removing file descriptors from the interest list.
epoll_wait(): Waits for events on the file descriptors registered with the epoll instance.
The epoll interface is generally more efficient than select() and poll() for large numbers of file descriptors, especially when dealing with thousands or more.

Here's a basic example of how epoll might be used to monitor file descriptors for reading:
```bash
#include <sys/epoll.h>
#include <stdio.h>

#define MAX_EVENTS 10

int main() {
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        return 1;
    }

    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = STDIN_FILENO;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &event) == -1) {
        perror("epoll_ctl");
        return 1;
    }

    struct epoll_event events[MAX_EVENTS];
    int ready = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
    if (ready == -1) {
        perror("epoll_wait");
        return 1;
    }

    for (int i = 0; i < ready; ++i) {
        if (events[i].events & EPOLLIN) {
            printf("Data is available for reading on stdin.\n");
            // Perform read operation on stdin
        }
    }

    return 0;
}
```
<br>
<br>
<div id="kqueue">

# Kqueue() function

The kqueue() function is used in BSD-based systems (such as macOS, FreeBSD, and others) for event notification. It is similar to epoll() on Linux and provides efficient I/O event monitoring.

The signature of the kqueue() function is:
```bash
#include <sys/event.h>

int kqueue(void);
int kevent(int kq, const struct kevent *changelist, int nchanges, struct kevent *eventlist, int nevents, const struct timespec *timeout);
```
The parameters of the kqueue() function are:

- `kq`: This is the file descriptor returned by the kqueue() function.
- `changelist`: This is a pointer to an array of kevent events to add, delete, or modify in the event queue.
- `nchanges`: This is the number of events in the changelist array.
- `eventlist`: This is a pointer to an array of kevent events where detected events will be stored.
- `nevents`: This is the maximum number of events to return in the eventlist array.
- `timeout`: This specifies the maximum wait time before returning from kevent().

The kqueue() interface involves three main system calls: kqueue(), kevent(), and close().

kqueue(): Creates a new kernel event queue and returns a descriptor representing the queue.
kevent(): Modifies the kernel event queue by adding, deleting, or modifying events.
close(): Closes the kernel event queue.
Here's a basic example of how kqueue might be used to monitor file descriptors for reading:
```bash
#include <sys/event.h>
#include <stdio.h>

#define MAX_EVENTS 10

int main() {
    int kq = kqueue();
    if (kq == -1) {
        perror("kqueue");
        return 1;
    }

    struct kevent event;
    EV_SET(&event, STDIN_FILENO, EVFILT_READ, EV_ADD, 0, 0, NULL);

    if (kevent(kq, &event, 1, NULL, 0, NULL) == -1) {
        perror("kevent");
        return 1;
    }

    struct kevent events[MAX_EVENTS];
    int ready = kevent(kq, NULL, 0, events, MAX_EVENTS, NULL);
```
<br>
<br>
<div id="socket">

# Socket() function

The socket() function in C language is used to create a new socket, which is an endpoint for communication between processes. Sockets can be of various types, such as stream sockets (for TCP) or datagram sockets (for UDP).

The signature of the socket() function is:
```bash
#include <sys/socket.h>

int socket(int domain, int type, int protocol);
```
The parameters of the socket() function are:

- `domain`: This specifies the communication domain in which the socket should be created, such as AF_INET for IPv4 communication or AF_INET6 for IPv6 communication.
- `type`: This specifies the type of socket to be created, such as SOCK_STREAM for a stream socket (TCP) or SOCK_DGRAM for a datagram socket (UDP).
- `protocol`: This parameter specifies the protocol to be used with the socket. Typically, a protocol value of 0 can be used, and the system will choose the appropriate protocol based on the specified domain and type.

The socket() function returns a new socket descriptor if successful, which can be used for subsequent socket operations. If an error occurs, it returns -1.

Here is a basic exemple:
```bash
#include <sys/socket.h>
#include <stdio.h>

int main() {
    // Create a TCP socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return 1;
    }

    printf("Socket created successfully.\n");

    // Close the socket
    close(sockfd);

    return 0;
}
```
<br>
<br>
<div id="setsockopt">

# Setsockopt() function

The setsockopt() function in C is used to set options associated with a socket. It allows you to control various aspects of socket behavior, such as setting socket options like SO_REUSEADDR or TCP_NODELAY.

The signature of the setsockopt() function is:
```bash
#include <sys/socket.h>

int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
```
The parameters of the setsockopt() function are:

- `sockfd`: The file descriptor of the socket.
- `level`: The level at which the option is defined (e.g., SOL_SOCKET).
- `optname`: The socket option to be set.
- `optval`: A pointer to the buffer containing the value to be set for the option.
- `optlen`: The size of the buffer pointed to by optval.

The setsockopt() function returns 0 on success, or -1 if an error occurs.

Example:
```bash
#include <sys/socket.h>
#include <stdio.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return 1;
    }

    // Set socket option SO_REUSEADDR
    int optval = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
        perror("setsockopt");
        close(sockfd);
        return 1;
    }

    printf("SO_REUSEADDR option set successfully.\n");

    // Close the socket
    close(sockfd);

    return 0;
}
In this example, setsockopt() is used to set the SO_REUSEADDR option for a socket, allowing the socket to bind to an address that is already in use.
```

</div>
</div>
<div id="bind">

# Bind() function

The bind() function in C language is used to associate a socket with a specific local address and port number.

The signature of the bind() function is:
```bash
#include <sys/socket.h>

int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```
The parameters of the bind() function are:

- `sockfd`: This is the file descriptor of the socket returned by the socket() function.
- `addr`: This is a pointer to a sockaddr structure containing the local address and port to bind the socket to.
- `addrlen`: This is the size of the sockaddr structure.

The bind() function associates the socket specified by sockfd with the address and port specified by addr. This is necessary for servers to listen for incoming connections on a specific port.

Here's an example :
```bash
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

int main() {
    // Create a TCP socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return 1;
    }

    // Bind the socket to port 8080
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("bind");
        return 1;
    }

    printf("Socket bound successfully.\n");

    // Close the socket
    close(sockfd);

    return 0;
}
```

<br>
<br>
<div id="listen">

# Listen() function

The listen() function in C language is used on a socket that has been bound with bind() to set it in a passive mode, where it can accept incoming connection requests.

The signature of the listen() function is:
```bash
#include <sys/socket.h>

int listen(int sockfd, int backlog);
```
The parameters of the listen() function are:

- `sockfd`: This is the file descriptor of the socket returned by the socket() function and bound with bind().
- `backlog`: This is the maximum length of the queue of pending connections.

The listen() function marks the socket referred to by sockfd as a passive socket, ready to accept incoming connection requests. The backlog parameter specifies the maximum number of connections that can be waiting to be accepted. If the backlog is reached, new connection requests may be refused.

Here is an exemple:
```bash
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

int main() {
    // Create a TCP socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return 1;
    }

    // Bind the socket to port 8080
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("bind");
        return 1;
    }

    // Set the socket to listening mode with a maximum backlog of 5
    if (listen(sockfd, 5) == -1) {
        perror("listen");
        return 1;
    }

    printf("Socket listening for incoming connections.\n");

    // Close the socket
    close(sockfd);

    return 0;
}
```
<br>
<br>
<div id="accept">

# Accept() function

The accept() function in C language is used with a listening socket to accept a new incoming connection request, creating a new socket for communication with the client.

The signature of the accept() function is:
```bash
#include <sys/socket.h>

int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```
The parameters of the accept() function are:

- `sockfd`: This is the file descriptor of the listening socket returned by the socket() function and bound with bind() and listen().
- `addr`: This is a pointer to a sockaddr structure that will contain the address of the client connecting to the server.
- `addrlen`: This is a pointer to a socklen_t variable that will contain the size of the sockaddr structure.

The accept() function blocks until a new connection is established. It then creates a new socket with the same properties as sockfd (such as type and protocol), and returns a new file descriptor for this socket. The address and address length parameters are used to retrieve information about the client connecting to the server.

Here is an exemple:
```bash
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

int main() {
    // Create a TCP socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return 1;
    }

    // Bind the socket to port 8080
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("bind");
        return 1;
    }

    // Set the socket to listening mode with a maximum backlog of 5
    if (listen(sockfd, 5) == -1) {
        perror("listen");
        return 1;
    }

    printf("Socket listening for incoming connections.\n");

    // Accept incoming connection
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
    if (client_sockfd == -1) {
        perror("accept");
        return 1;
    }

    printf("Accepted incoming connection from %s:%d.\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    // Close the sockets
    close(client_sockfd);
    close(sockfd);

    return 0;
}
```

<br>
<br>
<div id="send">

# Send() function

The send() function in C language is used to transmit data on a connected socket.

The signature of the send() function is:
```bash
#include <sys/socket.h>

ssize_t send(int sockfd, const void *buf, size_t len, int flags);
```
The parameters of the send() function are:

- `sockfd`: This is the file descriptor of the connected socket.
- `buf`: This is a pointer to the buffer containing the data to be transmitted.
- `len`: This is the length of the data in bytes.
- `flags`: This parameter is used to modify the behavior of the send operation (usually set to 0).

The send() function sends data from the buffer pointed to by buf through the connected socket specified by sockfd. It returns the number of bytes sent, or -1 if an error occurs.

Here is an exemple:
```bash
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

int main() {
    // Create a TCP socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return 1;
    }

    // Connect to a server
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8080);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        return 1;
    }

    printf("Connected to server.\n");

    // Send data to the server
    const char *message = "Hello, server!";
    ssize_t bytes_sent = send(sockfd, message, strlen(message), 0);
    if (bytes_sent == -1) {
        perror("send");
        return 1;
    }

    printf("Sent %zd bytes to server.\n", bytes_sent);

    // Close the socket
    close(sockfd);

    return 0;
}
```
<br>
<br>
<div id="recv">

# Recv() function

The recv() function is used to receive data from a connected socket. This function is typically used in client-server communication where a socket is in a connected state.

The signature of the recv() function is:
```bash
#include <sys/socket.h>

ssize_t recv(int sockfd, void *buf, size_t len, int flags);
```
The parameters of the recv() function are:

- `sockfd`: The file descriptor of the connected socket.
- `buf`: Pointer to the buffer where the received data will be stored.
- `len`: The length in bytes of the buffer.
- `flags`: Options that modify the behavior of the recv() operation. Commonly set to 0.

The recv() function returns the number of bytes received, or -1 if an error occurs.

Example:
```bash
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // Assume socket is connected to a server
    char buffer[1024];

    ssize_t bytes_received = recv(sockfd, buffer, sizeof(buffer), 0);
    if (bytes_received == -1) {
        perror("recv");
        close(sockfd);
        return 1;
    }

    buffer[bytes_received] = '\0';
    printf("Received: %s\n", buffer);

    close(sockfd);
    return 0;
}
```

</br>
</br>
<div id="connect">

# Connect() function

The connect() function is used to establish a connection to a specified socket address.

The signature of the connect() function is:
```bash
#include <sys/socket.h>

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```
The parameters of the connect() function are:

- `sockfd`: The file descriptor of the socket.
- `addr`: A pointer to a sockaddr structure that specifies the destination address.
- `addrlen`: The size of the sockaddr structure.

The connect() function returns 0 on success, or -1 if an error occurs.

Example:
```bash
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        return 1;
    }

    printf("Connected to the server.\n");

    // Close the socket
    close(sockfd);

    return 0;
}
```

</br>
</br>
<div id="getaddrinfo">

# Getaddrinfo() function

The getaddrinfo() function is used to resolve a host name and a service name to a list of address structures.

The signature of the getaddrinfo() function is:
```bash
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);
```

The parameters of the getaddrinfo() function are:

- `node`: A pointer to a null-terminated string specifying the host name to resolve.
- `service`: A pointer to a null-terminated string specifying the service (like "http" or port number).
- `hints`: A pointer to an addrinfo structure that specifies criteria for selecting the socket address structures returned.
- `res`: A pointer to a pointer that, on success, points to a linked list of one or more addrinfo structures containing response information.
The getaddrinfo() function returns 0 on success or a non-zero error code on failure.

Example:
```bash
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    struct addrinfo hints, *res;
    int status;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 to force version
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo("www.example.com", "http", &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return 2;
    }

    // `res` now points to a linked list of 1 or more struct addrinfos

    // do something with the linked list `res`, such as creating a socket and connecting

    freeaddrinfo(res); // Free the linked list allocated by getaddrinfo

    return 0;
}
```

</br>
</br>
<div id="freeaddrinfo">

# Freeaddrinfo() function

The freeaddrinfo() function is used to free the memory allocated by the getaddrinfo() function.

The signature of the freeaddrinfo() function is:
```bash
#include <netdb.h>

void freeaddrinfo(struct addrinfo *res);
```

The parameter of the freeaddrinfo() function is:

- `res`: A pointer to a addrinfo structure that was allocated by getaddrinfo().

</br>
</br>
<div id="getsockopt">

# Getsockopt() function

The getsockopt() function retrieves the current value of a socket option for a given socket.

The signature of the getsockopt() function is:
```bash
#include <sys/socket.h>

int getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);
```
The parameters of the getsockopt() function are:

- `sockfd`: The file descriptor of the socket.
- `level`: The level at which the option is defined (e.g., SOL_SOCKET).
- `optname`: The socket option for which the value is to be retrieved.
- `optval`: A pointer to a buffer where the value for the requested option will be stored.
- `optlen`: A pointer to the size of the option being retrieved.
The getsockopt() function returns 0 on success, or -1 if an error occurs.

Example:
```bash
#include <sys/socket.h>
#include <stdio.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int optval;
    socklen_t optlen = sizeof(optval);

    if (getsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, &optlen) == -1) {
        perror("getsockopt");
        return 1;
    }

    printf("SO_REUSEADDR is %s\n", (optval ? "enabled" : "disabled"));

    return 0;
}
```

</br>
</br>
<div id="getsockname">

# Getsockname() function

The getsockname() function retrieves the current address to which the socket sockfd is bound.

The signature of the getsockname() function is:
```bash
#include <sys/socket.h>

int getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```
The parameters of the getsockname() function are:

- `sockfd`: The file descriptor of the socket.
- `addr`: A pointer to a sockaddr structure that will receive the address of the socket.
- `addrlen`: A pointer to a socklen_t variable that on input specifies the size of the sockaddr structure and on output specifies the size of the stored address.

The getsockname() function returns 0 on success, or -1 if an error occurs.

Example:
```bash
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    if (getsockname(sockfd, (struct sockaddr *)&addr, &addrlen) == -1) {
        perror("getsockname");
        return 1;
    }

    printf("Local IP: %s, Port: %d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

    return 0;
}
```
</br>
</br>
<div id="getprotobyname">

# Getprotobyname() function

The getprotobyname() function returns protocol information corresponding to a protocol name.

The signature of the getprotobyname() function is:
```bash
#include <netdb.h>

struct protoent *getprotobyname(const char *name);
```
The parameter of the getprotobyname() function is:

- `name`: The name of the protocol (e.g., "tcp" or "udp").

The function returns a pointer to a protoent structure that contains the protocol information, or NULL if the protocol is not found.

Example:
```bash
#include <netdb.h>
#include <stdio.h>

int main() {
    struct protoent *proto;
    proto = getprotobyname("tcp");
    if (proto == NULL) {
        printf("Protocol not found\n");
        return 1;
    }

    printf("Protocol name: %s, Protocol number: %d\n", proto->p_name, proto->p_proto);

    return 0;
}
```

</br>
</br>
<div id="signal">

# Signal() function

The signal() function in the C language is used to establish a signal handler function for a specific signal. Signals are software interrupts that can be sent to a process to notify it of various events or exceptional conditions.

The signature of the signal() function is as follows:
```bash
#include <signal.h>

void (*signal(int signum, void (*handler)(int)))(int);
```
The parameters of the signal() function are:

- `signum`: This is an integer representing the signal number for which you want to set the handler. Common signal numbers include SIGINT (interrupt signal, often generated by pressing Ctrl+C), SIGTERM (termination signal), and SIGUSR1 (user-defined signal).
- `handler`: This is a pointer to a function that will be called when the specified signal is received. The handler function takes an integer argument representing the signal number.
The signal() function allows you to define custom signal handling behavior by specifying a function as the handler. When the specified signal is received by the process, the handler function is invoked. This enables you to perform specific actions or cleanup procedures in response to signals.

The function returns a pointer to the previous signal handler function for the specified signal. It's essential to be cautious when using signal() because its behavior can vary between different operating systems and C libraries. For more consistent and portable signal handling, you may prefer to use the sigaction() function.

Here's an example of how signal() might be used to set a custom signal handler for the SIGINT signal:
```bash
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void customHandler(int signum) {
    printf("Received signal %d (SIGINT)\n", signum);
    // Add custom signal handling code here.
}

int main() {
    // Set customHandler as the handler for the SIGINT signal.
    signal(SIGINT, customHandler);

    while (1) {
        // Your application's main loop.
    }

    return 0;
}
```
In this example, when the process receives the SIGINT signal (e.g., by pressing Ctrl+C), the customHandler() function is called to handle the signal.

<br>
<br>
<div id="kill">

# Kill() function

The kill() function in the C language is used to send a signal to a specific process or group of processes. Signals are software interrupts that can be sent to a process to notify it of various events or exceptional conditions, or to request specific actions.

The signature of the kill() function is as follows:
```bash
#include <signal.h>

int kill(pid_t pid, int sig);
```
The parameters of the kill() function are:

- `pid`: This is an integer representing the process ID (PID) of the target process. You can specify the PID of a specific process to send the signal to that process, or you can use special values such as 0 (to send the signal to all processes in the caller's process group) or -1 (to send the signal to all processes for which the caller has permission to send signals).
- `sig`: This is an integer representing the signal number of the signal to be sent. Common signal numbers include SIGTERM (termination signal), SIGINT (interrupt signal), and SIGKILL (kill signal).
The kill() function is a versatile tool for process management and communication in Unix-like operating systems. It can be used to send various signals to processes for different purposes, such as requesting graceful termination (SIGTERM), interrupting a process (SIGINT), or forcefully killing a process (SIGKILL).

The function returns 0 on success and -1 on failure. You can use it in a variety of scenarios, such as sending signals to specific processes or controlling process behavior.

Here's an example of how kill() might be used to send a SIGTERM signal to a specific process with a known PID:
```bash
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main() {
    pid_t target_pid = 12345; // Replace with the PID of the target process.

    // Send a SIGTERM signal to the target process.
    if (kill(target_pid, SIGTERM) == -1) {
        perror("kill");
        return 1;
    }

    return 0;
}
```
In this example, kill() is used to send a SIGTERM signal to the process with the PID 12345.

<br>
<br>
<div id="stat">

# Stat() function

The stat() function in the C language is used to retrieve information about a file specified by its pathname. It provides various details about the file, such as its size, permissions, and timestamps. This function operates on symbolic links by following them and providing information about the linked-to file.

The signature of the stat() function is as follows:
```bash
#include <sys/stat.h>

int stat(const char *pathname, struct stat *buf);
```
The parameters of the stat() function are:

- `pathname`: This is a pointer to a null-terminated string representing the path to the file you want to obtain information about.
- `buf`: This is a pointer to a struct stat where the file information will be stored upon a successful call to stat().
The stat() function fills the provided struct stat with information about the file specified by the pathname. You can access various attributes of the file from the struct stat, such as its size, permissions, timestamps, and more.

Here's an example of how stat() might be used to retrieve information about a file:
```bash
#include <stdio.h>
#include <sys/stat.h>

int main() {
    const char *file_path = "example.txt"; // Replace with the path to your file.
    struct stat file_info;

    if (stat(file_path, &file_info) == 0) {
        printf("File size: %lld bytes\n", (long long)file_info.st_size);
        printf("File permissions: %o\n", file_info.st_mode & 0777);
        // Access more file information from the 'file_info' struct as needed.
    } else {
        perror("stat");
        return 1;
    }

    return 0;
}
```
In this example, stat() is used to retrieve and print information about a file, such as its size and permissions.

<br>
<br>
<div id="opendir">

# Opendir() function

The opendir() function in the C language is used to open and create a directory stream for reading the contents of a directory. It allows you to access the files and subdirectories within a directory.

The signature of the opendir() function is as follows:
```bash
#include <dirent.h>

DIR *opendir(const char *dirname);
```
The parameter of the opendir() function is:

- `dirname`: This is a pointer to a null-terminated string representing the name of the directory you want to open.
The opendir() function opens the directory specified by dirname and returns a pointer to a DIR structure, which is an opaque data type representing the directory stream. You can use this pointer to read the contents of the directory using readdir().

It's important to check the return value of opendir() for errors. If the function fails, it returns NULL, and you can use errno to determine the cause of the error.

Here's an example of how opendir() might be used to open a directory:
```bash
#include <stdio.h>
#include <dirent.h>

int main() {
    const char *directory_path = "/path/to/your/directory"; // Replace with the desired directory path.
    DIR *directory;

    directory = opendir(directory_path);

    if (directory != NULL) {
        printf("Opened directory: %s\n", directory_path);
        // Perform operations with the opened directory.
        closedir(directory); // Don't forget to close the directory when done.
    } else {
        perror("opendir");
        return 1;
    }

    return 0;
}
```
In this example, opendir() is used to open the specified directory, and the opened directory stream is stored in the directory pointer.

<br>
<br>
<div id="readdir">

# Readdir() function

The readdir() function in the C language is used to read the contents of a directory opened with opendir(). It retrieves information about the next entry in the directory, such as the name and file type.

The signature of the readdir() function is as follows:
```bash
#include <dirent.h>

struct dirent *readdir(DIR *dirp);
```
The parameter of the readdir() function is:

- `dirp`: This is a pointer to a DIR structure, representing the directory stream obtained from opendir().
The readdir() function reads the next directory entry from the directory stream pointed to by dirp and returns a pointer to a struct dirent structure, which contains information about the entry. When there are no more entries or an error occurs, it returns NULL.

Here's an example of how readdir() might be used to read and print the contents of a directory:
```bash
#include <stdio.h>
#include <dirent.h>

int main() {
    const char *directory_path = "/path/to/your/directory"; // Replace with the desired directory path.
    DIR *directory;
    struct dirent *entry;

    directory = opendir(directory_path);

    if (directory != NULL) {
        printf("Contents of directory: %s\n", directory_path);
        while ((entry = readdir(directory)) != NULL) {
            printf("Entry name: %s\n", entry->d_name);
        }
        closedir(directory);
    } else {
        perror("opendir");
        return 1;
    }

    return 0;
}
```
In this example, readdir() is used in a loop to read and print the names of all directory entries within the specified directory.

<br>
<br>
<div id="closedir">

# Closedir() function

The closedir() function in the C language is used to close a directory stream previously opened with opendir(). It releases the resources associated with the directory stream.

The signature of the closedir() function is as follows:
```bash
#include <dirent.h>

int closedir(DIR *dirp);
```
The parameter of the closedir() function is:

-`dirp`: This is a pointer to a DIR structure representing the directory stream to be closed.
The closedir() function closes the directory stream pointed to by dirp. It's important to call closedir() to release resources and free memory associated with the directory stream when you're done working with it.

Here's an example of how closedir() might be used to close a directory stream:

#include <stdio.h>
#include <dirent.h>
```bash
int main() {
    const char *directory_path = "/path/to/your/directory"; // Replace with the desired directory path.
    DIR *directory;

    directory = opendir(directory_path);

    if (directory != NULL) {
        printf("Opened directory: %s\n", directory_path);
        // Perform operations with the opened directory.
        closedir(directory); // Close the directory when done.
    } else {
        perror("opendir");
        return 1;
    }

    return 0;
}
```
In this example, closedir() is used to close the directory stream when the program is finished working with it.

<br>
<br>
<div id="read">

# Read() Function

The read() function in the C language is used to read data from a file descriptor. It is typically used with files or sockets, but it can also be used with other types of input data streams.

The signature of the read() function is as follows:
```bash
#include <unistd.h>

ssize_t read(int fd, void *buffer, size_t count);
```

The parameters of the read() function are:

- `fd` : This is the file descriptor from which the data should be read. A file descriptor is an integer that identifies an open file or another type of input data stream. For example, 0 represents standard input (stdin), 1 represents standard output (stdout), and 2 represents standard error (stderr). File descriptors are typically obtained using functions like open() or socket().
- `buffer` : This is a pointer to the buffer where the read data will be stored.
- `count` : It specifies the maximum number of bytes to read from the file descriptor.

The read() function returns the number of bytes read from the file descriptor. If the returned value is -1, it indicates a read error.

<br>
<br>
<div id="open">

# Open() Function

The open() function in the C language is used to open a file and obtain a file descriptor, which is a unique identifier for an open file. It allows you to perform various operations on the file, such as reading, writing, or modifying its attributes.

The signature of the open() function is as follows:
```bash
#include <fcntl.h>

int open(const char *pathname, int flags, mode_t mode);
```

The parameters of the open() function are:

- `pathname` : It is a string that represents the path to the file you want to open.
- `flags` : It specifies the flags that control the opening and behavior of the file. Flags can include O_RDONLY (open for reading), O_WRONLY (open for writing), O_RDWR (open for reading and writing), O_CREAT (create the file if it doesn't exist), O_TRUNC (truncate the file if it exists), and more. Multiple flags can be combined using the bitwise OR operator (|).
- `mode` : It specifies the permissions to set on the file if O_CREAT flag is used. The mode is typically specified using constants defined in the <sys/stat.h> header, such as S_IRUSR (read permission for the owner), S_IWUSR (write permission for the owner), S_IRGRP (read permission for the group), and so on.

The open() function returns a file descriptor on success, which is a non-negative integer. If an error occurs, it returns -1, indicating the failure to open the file.

<br>
<br>
<div id="close">

# Close() Function

The close() function in the C language is used to close a file descriptor. It terminates the connection between the file descriptor and the corresponding open file, releasing any resources associated with it. After closing a file, further operations on that file descriptor are not possible unless the file is opened again.

The signature of the close() function is as follows:
```bash
#include <unistd.h>

int close(int fileDescriptor);
```

The parameter of the close() function is:

- `fileDescriptor` : It is an integer representing the file descriptor of the file to be closed.

The close() function returns 0 on success. If an error occurs while closing the file, it returns -1.

It is important to note that closing a file descriptor does not necessarily imply flushing any buffered data. Therefore, it's recommended to use functions like fsync() or fclose() (for standard library file streams) if you want to ensure that any buffered data is written to the file before closing it.

<br>
<br>
<div id="access">

# Access() Function

The access() function in the C language is used to check the accessibility or permission of a file or directory. It allows you to determine whether a specific file or directory can be accessed in a specified mode.

The signature of the access() function is as follows:

```bash
#include <unistd.h>

int access(const char *pathname, int mode);
```

The parameters of the access() function are:

- `pathname` : It is a string representing the path to the file or directory you want to check.
- `mode` : It specifies the accessibility mode, which can be F_OK to check the existence of the file, R_OK to check read permission, W_OK to check write permission, and X_OK to check execute permission. Multiple modes can be combined using the bitwise OR operator (|).

The access() function returns 0 if the file or directory exists and has the specified permissions according to the mode provided. If the file or directory doesn't exist or doesn't have the specified permissions, or if an error occurs, it returns -1.

Note that the access() function is useful for checking permissions before attempting to perform operations on a file or directory. However, keep in mind that the accessibility status can change between the access() call and subsequent operations, so it's important to handle potential errors or exceptions appropriately.

<br>
<br>
<div id="dup">

# Dup() and Dup2() Functions

The dup() function in C is used to duplicate an existing file descriptor. It creates a new file descriptor that refers to the same file or socket as the original descriptor. The new file descriptor is guaranteed to be the lowest available descriptor.

The signature of the dup() function is as follows:
```bash
#include <unistd.h>

int dup(int oldfd);
```

The dup() function takes the file descriptor oldfd as an argument and returns a new file descriptor on success. If an error occurs, it returns -1.

The dup2() function in C is similar to dup(), but it allows you to specify the desired file descriptor number for the duplicated descriptor. If the specified file descriptor number is already in use, dup2() will close it before duplicating the descriptor.

The signature of the dup2() function is as follows:
```bash
#include <unistd.h>

int dup2(int oldfd, int newfd);
```

The dup2() function takes two arguments: oldfd, the file descriptor to duplicate, and newfd, the desired file descriptor number for the duplicated descriptor. It returns the new file descriptor on success and -1 on failure.

It's important to note that both dup() and dup2() functions are primarily used for redirection of file descriptors, enabling output to be redirected to files or other destinations.

<br>
<br>
<div id="exec">

# Execve() Function

The execve() function in the C language is used to execute a program in place of the currently running process. It is part of the exec family of functions and allows you to replace the current process image with a new program.

The signature of the execve() function is as follows:

```bash
#include <unistd.h>

int execve(const char *pathname, char *const argv[], char *const envp[]);
```

The parameters of the execve() function are:

- `pathname` : It is a string that represents the path to the executable file you want to execute.
- `argv[]` : It is an array of strings that represents the program arguments. The last element of the array must be a NULL pointer.
- `envp[]` : It is an array of strings that represents the environment variables. The last element of the array must be a NULL pointer. If envp is set to NULL, the current environment is used.

The execve() function returns -1 on error, and on success, it does not return at all because the current process image is replaced by the new program.

It's important to note that the execve() function is a powerful tool for process execution, as it allows you to run other programs from within your program. However, it's crucial to handle errors appropriately and ensure that the necessary permissions and file paths are correctly set for the program you want to execute.

<br>
<br>
<div id="exit">

# Exit() Function

The exit() function in the C language is used to terminate the program and return control to the operating system. It allows you to explicitly exit the program at any point during its execution.

The signature of the exit() function is as follows:
```bash
#include <stdlib.h>

void exit(int status);
```

The parameter of the exit() function is:

- `status` : It is an integer value that represents the exit status of the program. A value of 0 typically indicates a successful execution, while a non-zero value indicates an error or abnormal termination.

The exit() function does not return to the calling function. It performs the following actions before terminating the program:

- Calls functions registered with the atexit() function, if any. These functions are typically used for performing cleanup tasks or freeing resources before the program exits.
- Closes open file descriptors, including standard input, standard output, and standard error.
- Flushes buffered output.
- Terminates the program and returns control to the operating system, along with the specified exit status.

The exit() function is commonly used to gracefully terminate a program and communicate its execution status to the calling environment or script. By convention, a value of 0 often represents successful execution, while non-zero values indicate various error conditions or abnormal terminations.

<br>
<br>
<div id="fork">

# Fork() Function

The fork() function in the C language is used to create a new process by duplicating the existing process. It creates a child process that is an exact copy of the parent process, including its code, data, and stack. After the fork() call, two separate processes, the parent process and the child process, are running concurrently.

The signature of the fork() function is as follows:

```bash
#include <unistd.h>

pid_t fork(void);
```

The fork() function does not take any arguments. It returns different values for the parent and child processes:

In the parent process, fork() returns the process ID (PID) of the child process.
In the child process, fork() returns 0.
If an error occurs during the fork() call, it returns -1.

When the program is executed, it will create a child process that starts executing from the point of the fork() call. Both the parent and child processes will continue executing from that point independently, but with separate memory spaces.

The fork() function is commonly used for process creation and parallel execution in C programs. It allows you to perform tasks concurrently and is the basis for many other process-related operations.

<br>
<br>
<div id="wait">

# Wait() and Waitpid() Functions

The wait() and waitpid() functions in the C language are used to wait for the termination of child processes in a parent process.

The wait() function suspends the execution of the calling process until one of its child processes terminates. It allows the parent process to wait for the completion of any child process without specifying a particular child process.
The signature of the wait() function is as follows:

```bash
#include <sys/types.h>
#include <sys/wait.h>

pid_t wait(int *status);
```

The wait() function takes a pointer to an integer status as an argument, which is used to retrieve the termination status of the child process. If the status information is not needed, you can pass NULL.
The wait() function returns the process ID (PID) of the terminated child process, or -1 on error.

<br>

The waitpid() function is similar to wait(), but it allows more control over which child process to wait for. It allows the parent process to wait for the termination of a specific child process or a group of child processes.

The signature of the waitpid() function is as follows:

```bash
#include <sys/types.h>
#include <sys/wait.h>

pid_t waitpid(pid_t pid, int *status, int options);
```

The parameters of the waitpid() function are:

- `pid` : It specifies which child process to wait for. The value of pid can be:
	- -1 to wait for any child process.
	- 0 to wait for any child process in the same process group as the calling process.
	- A positive value representing the process ID of a specific child process to wait for.
- `status` : It is a pointer to an integer used to retrieve the termination status of the child process. If the status information is not needed, you can pass NULL.
- `options` : It specifies additional options for controlling the behavior of waitpid(). Common options include WNOHANG (returns immediately if no child has terminated) and WUNTRACED (returns if a child has stopped, but not terminated).

The waitpid() function returns the process ID (PID) of the terminated child process, 0 if using WNOHANG and no child has terminated, -1 on error, or a negative value to indicate a specific error condition.

<br>
<br>
<div id="pipe">

# Pipe() Function

The pipe() function in the C language is used to create an interprocess communication (IPC) channel between two related processes. It establishes a unidirectional pipe that allows one process to write data to the pipe, while the other process reads the data from it.

The signature of the pipe() function is as follows:

```bash
#include <unistd.h>

int pipe(int pipefd[2]);
```
The pipe() function takes an array pipefd as an argument, which will be populated with two file descriptors: pipefd[0] for reading from the pipe and pipefd[1] for writing to the pipe.

The pipe() function returns 0 on success and -1 on failure.

The pipe acts as a conduit for communication between the parent and child processes. The parent writes a message to the pipe, and the child reads the message from the pipe and prints it.

<br>
<br>
<div id="unlink">

# Unlink() Function

The unlink() function in the C language is used to delete or remove a file from the file system. It allows you to remove a file by specifying its filename or path.

The signature of the unlink() function is as follows:
```bash
#include <unistd.h>

int unlink(const char *pathname);
```

The unlink() function takes a pathname parameter, which is a string representing the path to the file that needs to be deleted.

The unlink() function returns 0 on success and -1 on failure.

It's important to note that the unlink() function permanently removes the file from the file system. Once a file is unlinked, it cannot be recovered, so <strong>caution</strong> should be exercised when using this function.

The unlink() function is commonly used in situations where you need to programmatically delete files in a C program, such as during file management or cleanup operations.

<br>
<br>
<div id="perror">

# Perror() Function

The perror() function in the C language is used to print a descriptive error message to the standard error stream (stderr). It takes a string argument that is typically a custom error message or a label associated with the error. The perror() function then appends a colon and space, followed by the corresponding error message based on the value of the global variable errno.

The signature of the perror() function is as follows:

```bash
#include <stdio.h>

void perror(const char *s);
```

The perror() function takes a const char* argument s, which is the custom error message or label to be printed. If s is not NULL, the error message will be preceded by s:.

The perror() function is a convenient way to display meaningful error messages along with system-specific error descriptions. It helps in diagnosing and debugging errors in C programs by providing more information about the cause of the error.

<br>
<br>
<div id="strerror">

# Strerror() Function

The strerror() function in the C language is used to obtain a human-readable error message corresponding to a given error number. It converts the value of the errno variable into a descriptive error string.

The signature of the strerror() function is as follows:

```bash
#include <string.h>

char *strerror(int errnum);
```

The strerror() function takes an integer argument errnum, which represents the error number for which the error message is to be obtained.

The strerror() function returns a pointer to a string containing the error message associated with the given error number. This string is typically a static string within the implementation, and it should not be modified.

The strerror() function is helpful in scenarios where you want to obtain a human-readable error message based on the error number. It allows you to provide more detailed error information to users or for diagnostic purposes.

<br>
<br>
<a href="#title">Return ↑</a>
<br>
<br>
