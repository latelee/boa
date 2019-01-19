#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>

#include"cgic.h"

int cgi_upload(int argc, char* argv[])
{
    cgiFilePtr file;

    mode_t mode;
    char localFileName[64];
    char serverFileName[64];
    char contentType[1024];
    char buffer[1024];
    char *tmpPtr = NULL;
    int saveFd = 0; // 保存文件的句柄
    int size = 0;
    int got = 0;
    int t = 0;
    int ret = 0;

    cgiHeaderContentType("text/html");
    
    //取得html页面中file元素的值，应该是文件在客户机上的路径名
    if (cgiFormFileName("file", localFileName, sizeof(localFileName)) != cgiFormSuccess)
    {
        fprintf(cgiOut, "could not retrieve filename<p>\n");
        ret = -1;
        goto END;
    }

    cgiFormFileSize("file", &size);

    //取得文件类型，不过本例中并未使用
    cgiFormFileContentType("file", contentType, sizeof(contentType));
    
    fprintf(cgiOut, "file name: %s size: %d type: %s<p>\n", localFileName, size, contentType);
    
    //目前文件存在于系统临时文件夹中，通常为/tmp，通过该命令打开临时文件。
    // 临时文件的名字与用户文件的名字不同，所以不能通过路径/tmp/userfilename的方式获得文件
    if (cgiFormFileOpen("file", &file) != cgiFormSuccess)
    {
        fprintf(cgiOut, "could not open the file: %s<p>\n", localFileName);
        ret = -1;
        goto END;
    }
    
    t=-1;
    //从路径名解析出用户文件名
    while(1)
    {
        tmpPtr=strstr(localFileName+t+1,"//");
        if(NULL==tmpPtr)
            tmpPtr=strstr(localFileName+t+1,"/");//if "//" is not path separator, try "/"
        if(NULL!=tmpPtr)
            t=(int)(tmpPtr-localFileName);
        else
            break;
    }
    strcpy(serverFileName, localFileName+t+1);
    
    mode=S_IRWXU|S_IRGRP|S_IROTH;
    //在当前目录下建立新的文件，第一个参数实际上是路径名，
    // 此处的含义是在cgi程序所在的目录（当前目录））建立新文件
    saveFd=open(serverFileName, O_RDWR|O_CREAT|O_TRUNC|O_APPEND, mode);
    if(saveFd < 0)
    {
        fprintf(cgiOut, "could not create the new file: %s<p>\n", serverFileName);
        ret = -1;
        goto END;
    }
    //从系统临时文件中读出文件内容，并放到刚创建的目标文件中
    while (cgiFormFileRead(file, buffer, sizeof(buffer), &got) == cgiFormSuccess)
    {
        if(got > 0)
            write(saveFd, buffer, got);
    }

//END:
    cgiFormFileClose(file);
    close(saveFd);
END:
    if (ret)
        fprintf(cgiOut, "Error to upload.<p>\n");
    else
        fprintf(cgiOut, "File %s has been uploaded.<p>\n",serverFileName);

    return ret;
}
