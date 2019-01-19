#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>

#include"cgic.h"

// 对应cgi响应动作
extern int cgi_login(int argc, char* argv[]);
extern int cgi_upload(int argc, char* argv[]);

#define ARRAY_SIZE(x) ((unsigned)(sizeof(x) / sizeof((x)[0])))

struct bb_applet
{
    const char *name; // 程序名称
    int (* applet_main)(int argc, char* argv[]); // 仿main函数
};

// 全局结构体数组，保存程序名称与对应的执行函数
static struct bb_applet g_applets[] =
{
    {"login.cgi", cgi_login},
    {"upload.cgi", cgi_upload},
};

static int cmp_name(const void *a, const void *b)
{
    const struct bb_applet *aa = a;
    const struct bb_applet *bb = b;
    return strcmp(aa->name, bb->name);
}

static int applet_name_compare(const void *name, const void *idx)
{
    int i = (int)(ptrdiff_t)idx - 1;
    return strcmp(name, g_applets[i].name);
}

int find_applet_by_name(int size, const char *name)
{
    if (size > 8)
    {
        const char *p;
        p = bsearch(name, (void*)(ptrdiff_t)1, size, 1, applet_name_compare);

        return (int)(ptrdiff_t)p - 1;
    }
    else
    {
        for (int i = 0; i < size; i++)
        {
            if (strcmp(name, g_applets[i].name) == 0)
                return i;
        }
        return -1;
    }
}

int execute(int argc, char *argv[])
{
    char* p = NULL;
    char* applet_name = argv[0];
    int size = ARRAY_SIZE(g_applets);
    
    int applet_no = -1;

    if ((p = strrchr (applet_name, '/')) != NULL) {
        applet_name = p + 1;
    }

    qsort(g_applets, size, sizeof(g_applets[0]), cmp_name);

    applet_no = find_applet_by_name(size, applet_name);

    if (applet_no != -1)
        return g_applets[applet_no].applet_main(argc, argv);

    return 0;
}

void setHtmlHead(void)
{
    cgiHeaderContentType("text/html", "utf-8");

    fprintf(cgiOut, "<html><head>\n");
    fprintf(cgiOut, "<title>字符叠加器管理软件</title></head>\n");
    fprintf(cgiOut, "<body>\n");
}

void setHtmlFoot(void)
{
    fprintf(cgiOut, "</body></html>\n");
}

int main(int argc, char *argv[])
{
    int ret = 0;

    cgiInitEnv();

    setHtmlHead();

    ret = execute(argc, argv);

    setHtmlFoot();

    return ret;
}
