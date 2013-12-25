/*  SRUN3000 Client in C        */
/*  Coded by BUAA38024118 ZhouYang  */
/*  This Version is programmed in pure C language */
/*  and can be cross-compiled to other arch like ARM/MIPS*/
/*  So you can run srun on a router which support linux */

/*  Only for research purpose, no commercial use */

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

#include "csrun_encrypt.c"
#include "csrun_curl.c"
#include "csrun_url.c"


int login(char* username,char* password,char* host,unsigned short port,char* mac,long delta,char* buffer);


int main(int argc, char **argv){
    //csrun username password host port mac
    char username[50]="",password[50]="",host[50]="202.112.136.131",mac[50]="11:22:33:44:55:66";
    unsigned short port=3333;
    char buf[1024]="";

    switch(argc)
    {
        case 5:
            strcpy(username, *(argv+1));
            strcpy(password, *(argv+2));
            strcpy(host, *(argv+3));
            port = atoi(*(argv+4));
            break;
        default:
            printf("csrun username password host port mac\n");
            exit(1);
    }

    memset(buf,1023,0);
    long dt = 0;
    int res = 0;
    while(1){
        res = login(username,password,host,port,mac,dt,buf);
        time_t srvtime = atoi(buf);
        time_t localtime=time(NULL);
        dt = srvtime - localtime;
        printf("%d::%ld,%ld,%ld\n",res,srvtime,localtime,dt);
        if(res) break;
    }
    return 0;
}

int login(char* username,char* password,char* host,unsigned short port,char* mac,long delta,char* buffer)
{
    char login_str[512]="";
    char tmpstr[512]="";

    strcpy(login_str,"username=");
    urlencode(username,tmpstr);
    strcat(login_str,tmpstr);
    strcpy(tmpstr,"");
    strcat(login_str,"&password=");
    char passcoded[50]="";
    encrypt(password,delta,passcoded);
    urlencode(passcoded,tmpstr);
    strcat(login_str,tmpstr);
    strcpy(tmpstr,"");
    strcat(login_str,"&drop=0&type=2&n=99&mac=");
    strcpy(tmpstr,"");
    urlencode(mac,tmpstr);
    strcat(login_str,tmpstr);
    strcpy(tmpstr,"");

    request(host,(unsigned short)port,"/cgi-bin/do_login",login_str,buffer);
    char* bg = strstr(buffer,"password_error@");
    if(bg == NULL){
        return 1;
    }
    //printf("%s",buffer);
    int i = 0;
    while((bg+i)[0]!='\n'){
        i++;
    }
    char second[50];
    int k;
    for(k = 0;k<=i;k++){
        second[k]=(bg+k)[0];
    }
    second[k] = 0;
    //printf("%s",second);
    memset(buffer,100,0);
    strcpy(buffer,second+15);
    //printf("%s\n",second);

    return 0;
}
