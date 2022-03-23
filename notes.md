# Webserv

## HTTP server


## Authorized functions
### htons, htonl, ntohs, ntohl
- `htons` : converts the unsigned integer hostlong from host byte order to network byte order
- etc.
### select
```
int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)
```

Some macro available to deal with `fd_set` :
- `void FD_CLR(int fd, fd_set *set)` : remove `fd` from `set`
- `void FD_ZERO(fd_set *set)` : clear set
- `void FD_SET(int fd, fd_set *set)` : add `fd` in `set`
- `int FD_ISSET(int fd, fd_set *set)` : check if `fd` is in `set`

`select` waits until one of `readfds` is ready for reading, or until one of `writefds` is ready for writing, or until one of `exceptfds` has encoutered an exception (cf. test.cpp) or until `timeout` is reached.

## Configuration file
nginx.conf inspiration

# Ressources

- https://developer.mozilla.org/fr/docs/Learn/Common_questions/What_is_a_web_server
- https://datatracker.ietf.org/doc/html/rfc2616