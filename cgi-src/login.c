#include<stdio.h>
#include<stdlib.h>

void setHtmlHead(char* title)
{
    char titlebuf[32] = {0};
    //printf("<head>\n");
    printf("content-type:text/html;charset=gb2312\n");
    //printf("<meta http-equiv=X-UA-Compatible content=IE=edge>\n");
    //printf("<meta name=viewport content=width=device-width, initial-scale=1>\n");

    sprintf(titlebuf, "<title>%s</title>", title);
    printf("%s\n", titlebuf);
    //printf("</head>\n");
}

void setHtmlTitle(char* title)
{
    char titlebuf[32] = {0};
    printf("content-type:text/html;charset=utf-8\n\n");
    sprintf(titlebuf, "<title>%s</title>", title);
    printf("%s\n", titlebuf);
}

int cgi_login(int argc, char* argv[])
{
    char *querystr = NULL;
    char name[16],pwd[16];

    setHtmlTitle("登陆结果");
    //return 0;
    printf("<H3>登陆结果</h3>");
    querystr=getenv("QUERY_STRING");
    if(querystr==NULL)
    {
        printf("<p>error, no input</p>\n");
    }
    else
    {
        printf("query string: %s\n",querystr);
        sscanf(querystr,"name=%[^&]&pwd=%s",name,pwd);
        printf("<p>name=%s</p>",name);
        printf("<p>pwd=%s</p>",pwd);
    }
    return 0;
}
