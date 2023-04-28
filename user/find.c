#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* fmtname(char *path) // 从路径中获取文件名
{
    char *p;
    for (p = path + strlen(path); p >= path && *p != '/'; p--); // 从后往前找到第一个'/'
    return ++p;
}

void find(char* path, char* name) // 递归查找
{
    char buf[512], *p; 
    int fd;
    struct dirent de;
    struct stat st;
    if ((fd = open(path, 0)) < 0)  // 打开文件失败
    {
        fprintf(2, "find: cannot open %s\n", path);
        close(fd);
        return;
    }
    if (fstat(fd, &st) < 0) // 获取文件信息失败
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }
    switch (st.type) // 判断文件类型
    {
    case T_FILE:   // 文件
        if(strcmp(fmtname(path), name) == 0)
        {
            printf("%s\n", path);
        }
        break;
    case T_DIR:  // 目录
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            printf("find: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de))  // 读取目录下的文件
        {
            if (de.inum == 0)   // 无效的文件
                continue;
            if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)   // "." ".." 跳过
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if(stat(buf, &st) < 0)
            {
                printf("find: cannot stat %s\n", buf);
                continue;
            }
            find(buf, name);
        }
        break;
    
    }
    close(fd);
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        fprintf(2, "usage: find <path> <name>\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}