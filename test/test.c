//#include <stdio.h>
//#include <unistd.h>
//
//void input(char* inputStr)
//{
//    int numBytes;
//    do
//    {
//        numBytes = read(0, inputStr, 128);
//        inputStr[numBytes - 1] = '\0';
//    }while(numBytes < 2);
//
//}
//int main()
//{
//    char buf[1024];
//    int numBytes;
//    input(buf);
//    printf("%s", buf);
//}


#include <stdio.h>
#include <sys/select.h>

int main(void) {
    int len;
    char buff[100];
    fd_set s_rd, s_wr, s_ex;
    while(1) {
        FD_ZERO(&s_rd);
        FD_ZERO(&s_wr);
        FD_ZERO(&s_ex);
        FD_SET(fileno(stdin), &s_rd);
        select(fileno(stdin) + 1, &s_rd, &s_wr, &s_ex, NULL);
        if (FD_ISSET(fileno(stdin), &s_rd)) {
            if (len = read(fileno(stdin), buff, 100)) {
                buff[len] = 0;
                printf("%s", buff);
            }
        }
    }
    return 0;
}