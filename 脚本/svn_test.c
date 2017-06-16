#include <stdio.h>
#include <string.h>
#include "version_svn_bulid.h"

void GetSvnRevision()
{
#if __SVN_VERSION__
    char buffer[100];
    strncpy(buffer, BUILD_TIMER, strlen(BUILD_TIMER)+1);
    printf("SVN version:%d, Build time:%s\n", BUILD_SVN_VERSION, buffer);
    snprintf(buffer, sizeof(buffer),  "SVN version:%d, Build time:%s\n", BUILD_SVN_VERSION, BUILD_TIMER);
    printf("%s\n", buffer);
    printf("SVN version:%d, Build time:%s\n", BUILD_SVN_VERSION, (const char *)BUILD_TIMER);
#endif
    
}

int main()
{
    GetSvnRevision();
}

