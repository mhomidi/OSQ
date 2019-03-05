#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros


#define n_array (sizeof (array) / sizeof (const char *))

int main ()
{
    for (int i = 0; i < 10; ++i) {
        write(1, "salam hala khobi sha?\n", strlen("salam hala khobi sha?\n") + 1);
    }
    return 0;
}