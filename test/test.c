#include <stdio.h>
#include <unistd.h>

void input(char* inputStr)
{
    int numBytes;
    do
    {
        numBytes = read(0, inputStr, 128);
        inputStr[numBytes - 1] = '\0';
    }while(numBytes < 2);

}
int main()
{
    char buf[1024];
    int numBytes;
    input(buf);
    printf("%s", buf);
}