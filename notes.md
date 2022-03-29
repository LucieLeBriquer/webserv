# Webserv

## HTTP server
At the most basic level, whenever a browser needs a file that is hosted on a web server, the browser requests the file via HTTP. When the request reaches the correct (hardware) web server, the (software) HTTP server accepts the request, finds the requested document, and sends it back to the browser, also through HTTP.

HTTP server usually use TCP for communications.

## TCP socket programming
Socket : mechanism to give programs access to the network
- **create** a socket with `socket()`
- **identify** the socket with `bind()`
- **wait** for a connection with `listen()` and `accept()` (or `send()` and `recv()`)
- **send** and **receive** messages with `read()` and `write()`
- **close** the socket with `close()`

There are some explanations about those functions in the following section.

## Authorized functions

### htons, htonl, ntohs, ntohl -- convert values between host and network byte order
```c++
#include <arpa/inet.h>

uint32_t htonl(uint32_t hostlong);
uint16_t htons(uint16_t hostshort);
uint32_t ntohl(uint32_t netlong);
uint16_t ntohs(uint16_t netshort);
```
- `htons()` : converts a short integer (e.g. address) to a network representation 
- etc.

### select, poll, epoll -- synchronous I/O multiplexing
```c++
int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)
```

Some macro available to deal with `fd_set` :
```c++
void FD_CLR(int fd, fd_set *set)        # remove fd from set
void FD_ZERO(fd_set *set)               # clear set
void FD_SET(int fd, fd_set *set)        # add fd in set
int  FD_ISSET(int fd, fd_set *set)      # check if fd is in set
```

`select()` waits until one of `readfds` is ready for reading, or until one of `writefds` is ready for writing, or until one of `exceptfds` has encoutered an exception (cf. test.cpp) or until `timeout` is reached.

### kqueue, kevent -- kernel event notification mechanism
The `kqueue()` system call creates a new kernel event queue and returns a descriptor.

The `EV_SET()` macro is provided for ease of initializing a `kevent` structure.

### socket -- create a socket
```c++
#include <sys/socket.h>

int socket(int domain, int type, int protocol);
```
- return value : socket descriptor (like file descriptor)
- `domain` : specifies communication domain (local `AF_LOCAL`, through an internet protocol `AF_INET`, etc.)
- `type` : specifies the semantics of communication over the socket (`SOCK_STREAM`, `SOCK_DGRAM`, ...)
- `protocol` : specifies a protocol to use, it should be consistant with the `domain`, you can see protocol's values in `/etc/protocols`

### accept -- accept a connection on a socket
```c++
int accept(int sockfd, struct sockaddr *restrict addr, socklen_t *restrict addrlen)
```
`accept` grabs the first connection request and create a new socket for communication (the listening socket should be used only for listening purpose). `addr` and `addrlen` are filled by the function.

### listen -- listen for connections on a socket
```c++
int listen(int sockfd, int backlog)
```
marks the socket `sockfd` as a listening socket. The `backlog` argument defines the maximum lenght of the queue of pending connection requests.

### send -- send a message on a socket
```c++
ssize_t send(int sockfd, const void *buf, size_t len, int flags)
```
The only difference between `write()` and `send()` is the presence of flags.

### recv -- receive a message from a socket
```c++
ssize_t recv(int sockfd, void *buf, size_t len, int flags)
```
The only difference between `read()` and `recv()` is the presence of flags.

### bind -- identify a socket
Almost like assigning an address to a mailbox
```c++
int bind(int sockfd, const struct sockaddr *address, socklen_t address_len)

struct sockaddr_in 
{ 
    __uint8_t         sin_len; 
    sa_family_t       sin_family; 
    in_port_t         sin_port; 
    struct in_addr    sin_addr; 
    char              sin_zero[8]; 
};
```
with :
- `sin_family` = `domain`
- `sin_port` = a port number
- `sin_addr` = address for the socket (for example `inet_addr("127.0.0.1")` or const like `INADDR_ANY`)

### connect

### inet_addr

### setsockopt

### getsockname

### fcnt

-----------------
## HTTP requests methods

HTTP defines a set of request methods(or *verbs*) to indicate the desired action to be performed for a given resource.

The HTTP/1.0 specification defined the GET, HEAD, and POST methods, and the HTTP/1.1 specification added five new methods: PUT, DELETE, CONNECT, OPTIONS, and TRACE.

- `GET` = requests that the target resource transfer a representation of its state (**HTTP status codes**). Requests using GET should only retrieve data without making changes.
- `HEAD` = asks for a response identical to a GET request, but without the response body (only the header). 
- `POST` = submits an entity to the specified resource, often causing a change in state or side effects on the server.
- `PUT` = requests that the target resource create or update its state with the state defined by the submitted request. A distinction from POST is that the client specifies the target location on the server.
- `DELETE` = deletes the specified resource.
- `CONNECT` =  establishes a tunnel to the server identified by the target resource.(?)
- `OPTIONS` =  requests that the target resource transfer the HTTP methods that it supports. This can be used to check the functionality of a web server by requesting '*' instead of a specific resource.
- `TRACE` =  requests that the target resource transfer the received request in the response body. That way a client can see what (if any) changes or additions have been made by intermediaries.
- `PATCH` = applies partial modifications to a resource.
```
GET / HTTP/1.1
Host: www.example.com
User-Agent: Mozilla/5.0
Accept: *
Accept-Language: en-GB,en;q=0.5
Accept-Encoding: gzip, deflate, br
Connection: keep-alive
```

## HTTP/1.1 response
A response message is sent by a server to a client as a reply to its former request message. They define how information sent/received, the session verification and identification of the client (cookies, IP, user-agent) or their anonymity (VPN or proxy), how the server should handle data (Do-Not-Track)... 
```
HTTP/1.1 200 OK
Date: Mon, 23 May 2005 22:38:34 GMT
Content-Type: text/html; charset=UTF-8
Content-Length: 155
Last-Modified: Wed, 08 Jan 2003 23:11:55 GMT
Server: BestServ (Unix) (Red-Hat/Linux)
ETag: "3f80f-1b6-3e1cb03b"
Accept-Ranges: bytes
Connection: close

<html>
<head>
    <title>An Example Page</title>
</head>
<body>
    <p>Exemple of a server response.</p>
</body>
</html>
```
---------------

## Configuration file
Example of a complete configuration file (`nginx.conf` inspiration):
```
server {
	listen			HOST:PORT
	server_name		SERVER_NAME
	root			ROOT
	index			INDEX
	client_body		MAX_CLIENT_BODY
	methods			METHOD1 METHOD2 ...
	error_page		NUM_ERROR ERROR_FILE

	location *.php {
		cgi_pass	CGI
	}
}

server {
	listen			HOST:PORT
	server_name		SERVER_NAME
	root			ROOT
	index			INDEX
	client_body		MAX_CLIENT_BODY
	methods			METHOD1 METHOD2 ...

	location /DIRECTORY1 {
		root			ROOT
		index			INDEX
		client_body		MAX_CLIENT_BODY
		methods			METHOD1 METHOD2 ...
	}

	location /DIRECTORY2 {
		root			ROOT
		index			INDEX
		client_body		MAX_CLIENT_BODY
		methods			METHOD1 METHOD2 ...
		autoindex
	}

	location /TOREDIR {
		return REDIR_URL
	}
}
```
no regexp for location routes

# To Do List
- parsing of configuration file
- server part, with one socket per server block in conf file
- parsing of requests
- handling easy requests
- handling php CGIss

# Ressources

- [Basic explanations on how a web server works](https://developer.mozilla.org/fr/docs/Learn/Common_questions/What_is_a_web_server)
- [RFC documentation](https://datatracker.ietf.org/doc/html/rfc2616)
- [Socket programming](https://www.geeksforgeeks.org/socket-programming-cc/)
- [How to build a simple HTTP server from scratch](https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa)
