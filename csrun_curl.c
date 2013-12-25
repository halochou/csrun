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

int request(char *host,unsigned short port, char *api, char *parameters, char* message)
{
    //Socket setup
    struct sockaddr_in sin;
    int sock = socket (AF_INET, SOCK_STREAM, 0);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    struct hostent * host_addr = gethostbyname(host);
    sin.sin_addr.s_addr = *((int*)*host_addr->h_addr_list) ;
    if(connect(sock,(struct sockaddr*)&sin, sizeof(struct sockaddr_in)) == -1)
    {
        return 0;
    }
    //Send POST data

    char msg[1024];
    memset(msg,1024,0);
    strcpy(msg,"POST ");
    strcat(msg,api);
    strcat(msg," HTTP/1.1\r\nHost: ");
    strcat(msg,host);
    strcat(msg,":");
    char tmpmsg[32]="";
    sprintf(tmpmsg,"%d",port);
    strcat(msg,tmpmsg);
    strcat(msg,"\r\n");
    strcat(msg,"Accept: */*\r\nContent-Length: ");
    sprintf(tmpmsg,"%d\r\n",strlen(parameters));
    strcat(msg,tmpmsg);
    strcat(msg,"Content-Type: application/x-www-form-urlencoded\r\n\r\n");
    strcat(msg,parameters);
    send(sock,msg,strlen(msg),0);
    //Receive response message
/*
    char ch[2]=" ",recvmsg[2048]="";
    memset(recvmsg,1024,0);
    memset(message,1024,0);
    while(1){
        ch[0]=0;ch[1]=0;
        int len = recv(sock,ch,1,0);
        strcat(recvmsg,ch);
        if(ch[0] == 0) break;
    }
    int i=0,counter=0;
    for(i=0;i<=strlen(recvmsg)-1;i++){
        if(counter == 7)break;
        if(recvmsg[i]=='\r'){
            counter++;
        }
    }
    for(int j=i,k=0;j<strlen(recvmsg);j++,k++){
        message[k]=recvmsg[j];
        message[k+1]=0;
    }*/
 
 char c1[2]="";
 int l,line_length;
 int loop = 1;
 int bHeader = 0;

 while(loop) {
   l = recv(sock, c1, 1, 0);
   if(l<0) loop = 0;
   if(c1[0]=='\n') {
       if(line_length == 0) loop = 0;

       line_length = 0;
       if(strstr(message, "200") != NULL)
           bHeader = 1;
   }
   else if(c1[0]!='\r') line_length++;
   //_DEBUG_PRINT( cout<<c1[0]);
   //printf("%c",c1[0]);
   strcat(message,c1);
 }

 strcpy(message,"");
 if(bHeader) {
     //printf("####BODY####");
     char p[1024];
     while((l = recv(sock,p,1023,0)) > 0)  {
         //_DEBUG_PRINT( cout.write(p,l));
         p[l] = '\0';
         strcat(message, p);
     }

     //_DEBUG_PRINT( cout << message.c_str());
     //printf("%s",message);
 } else {
     return -102;
 }





    close(sock);
    return 1;
}
