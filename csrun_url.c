#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

char toHex(char x)
{
    return x > 9 ? x -10 + 'A': x + '0';
}
int urlencode(const char* sIn,char* sOut)
{
    memset(sOut,100,0);
    for( int ix = 0; ix < strlen(sIn); ix++ )
    {
        unsigned char buf[4];
        memset( buf, 0, 4 );
        if( isalnum( (unsigned char)sIn[ix] ) )
        {
            buf[0] = sIn[ix];
            buf[1] = (unsigned char)'\0';
        }
        else
        {
            buf[0] = '%';
            buf[1] = toHex( (unsigned char)sIn[ix] >> 4 );
            buf[2] = toHex( (unsigned char)sIn[ix] % 16);
            buf[3] = (unsigned char)'\0';
        }
        strcat(sOut,(char *)buf);
    }
    return strlen(sOut);
}
