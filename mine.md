**socket(domain, type, protocole) = creation d'un socket (la valeur de retour fait usite a la requete d'e l'ouverture d;une ligne de telephone chez sfr)
    -> domaine : AF_INET, AF_INET6 .. famille d'address
    -> type : type de service = SOCK_STREAM (virtual circuit service) = TCP  SOCK_DGRAM (datagram service) = UDP
    -> protocole : 0 car pas de variation, une seule forme de type de service pour sock_stream (tcp).

**Binding = naming a socket = assinging a transport address to the socket = port number. (obtention du numero de telephone par sfr)

For IP networking, we use struct sockaddr_in :

sin_family = the address family
sin_port = port nu;ber (transport address)
sin_addr = the address for this socket,machine ip addr. 

** The listen system call tells a socket that it should be capable of accepting incoming connections.
    Backlog, defines the maximum number of pending connections that can be queued up before connections are refused.

**The accept system call grabs the first connection request on the queue of pending connections (set up in listen) and creates a new socket for that connection.
-> By default, socket operations are synchronous, or blocking, and accept will block until a connection is present on the queue.

-----------------------------

There are 9 HTTP methods.

Some of them are:

    GET — Fetch a URL
    HEAD — Fetch information about a URL
    PUT — Store to an URL
    POST — Send form data to a URL and get a response back
    DELETE — Delete a URL GET and POST (forms) are commonly used

REST APIs use GET, PUT, POST, and DELETE.
