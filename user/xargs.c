#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
    char buf2[512];  // 读取的字符
    char buf[32][32];  
    char *pass[32];  

    for(int i=0;i<32;++i)  
    {
        pass[i] = buf[i]; 
    }
    int i;
    for(i=1;i<argc;++i)
    {
        strcpy(buf[i-1], argv[i]);  
    }
    int n;
    while ((n = read(0, buf2, sizeof(buf2))) > 0) // 从标准输入读取
    {
        int pos = argc - 1;  // 从第一个参数开始
        char *c = buf[pos];  // 指向第一个参数的首地址
        for (char *p = buf2; *p; p++)
        {
            if (*p == ' ' || *p == '\n') // 遇到空格或者换行符
            {
                *c = '\0';
                pos++;
                c = buf[pos];
            }
            else
                *c++ = *p;
        }
        *c = '\0';
        pos++;
        pass[pos] = 0;

        if (fork())
        {
            wait(0);
        }
        else
            exec(pass[0], pass);
    }

    if (n < 0)
    {
        printf("xargs: read error\n");
        exit(0);
    }

    exit(0);
}