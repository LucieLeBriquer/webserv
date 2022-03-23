#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>

int main(void)
{
    fd_set rfds;
    fd_set wfds;
    struct timeval tv;
    int retval;
    int fd = open("test.txt", O_WRONLY);

    /* Surveiller stdin (fd 0) en attente d'entrées */
    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    FD_SET(0, &rfds);
    FD_SET(42, &rfds);
    FD_SET(fd, &wfds);

    /* Pendant 5 secondes maxi */
    tv.tv_sec = 3;
    tv.tv_usec = 0;

    std::cout << FD_ISSET(0, &rfds) << " " << FD_ISSET(42, &rfds) << " " << FD_ISSET(fd, &wfds) << std::endl;
    retval = select(1, &rfds, &wfds, NULL, &tv);
    /* Considérer tv comme indéfini maintenant ! */

    if (retval == -1)
    {
        std::cout << FD_ISSET(0, &rfds) << " " << FD_ISSET(42, &rfds) << " " << FD_ISSET(fd, &wfds) << std::endl;
        perror("select()");
    }
    else if (retval)
    {
        printf("Des données sont disponibles maintenant\n");
        std::cout << FD_ISSET(0, &rfds) << " " << FD_ISSET(42, &rfds) << " " << FD_ISSET(fd, &wfds) << std::endl;
    }
    else
    {
        std::cout << FD_ISSET(0, &rfds) << " " << FD_ISSET(42, &rfds) << " " << FD_ISSET(fd, &wfds) << std::endl;
        printf("Aucune données durant les 5 secondes\n");
    }

    exit(EXIT_SUCCESS);
}

 