#include "kernel/fd_types.h"
#include "kernel/types.h"
#include "user/user.h"

int main()
{
    int pfd[2];
    int cfd[2];
    pipe(pfd);
    pipe(cfd);
    char buf[1];
    if(fork() == 0)
    {
        close(pfd[1]); //close write
        close(cfd[0]); //close read
        read(pfd[0], buf, 1);
        printf("%d: received p%sng\n", getpid(), buf);
        write(cfd[1], "o", 1);
        close(cfd[1]);
    }
    else if(fork() > 0)
    {
        close(pfd[0]); 
        close(cfd[1]); 
        write(pfd[1], "i", 1);
        close(pfd[1]);
        read(cfd[0], buf, 1);
        printf("%d: received p%sng\n", getpid(), buf);
    }
    exit(0);
}


    

