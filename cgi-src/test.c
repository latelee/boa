#include <stdio.h>
#include <stdlib.h>
#include <time.h>
 
int main(void)
{
    time_t current;
    struct tm *timeinfo;
    time(&current);
    timeinfo = localtime(&current);
    
    //这一句一定要加，否则异步访问会出现页面异常
    printf("Content type: text/html\n\n");
 
    printf("%s", asctime(timeinfo));
}

