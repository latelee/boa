#include<stdio.h>
#include<stdlib.h>

#include "cgic.h"

int cgi_login(int argc, char* argv[])
{
	char name[16] = {0};
    char passwd[16] = {0};

	cgiFormStringNoNewlines("name", name, 16);
	fprintf(cgiOut, "Name: ");
	cgiHtmlEscape(name);
	fprintf(cgiOut, "<BR>\n");

    cgiFormStringNoNewlines("pwd", passwd, 16);
	fprintf(cgiOut, "passwd: ");
	cgiHtmlEscape(passwd);
	fprintf(cgiOut, "<BR>\n");

    return 0;
}
