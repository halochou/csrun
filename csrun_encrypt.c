#include <time.h>
#include <stdlib.h>

int buildkey(int num,int reverse,char* ret)
{
    int low = num & 0x0f;
    int high = num >> 4;
    high = high & 0x0f;
    if(!reverse){
        ret[0] = low+0x36;
        ret[1] = high+0x63;
        ret[2] = 0;
    }
    else{
        ret[0] = high+0x63;
        ret[1] = low+0x36;
        ret[2] = 0;
    }
    return strlen(ret);
}

int encrypt(char* password,long delta,char* ret)
{
    char key[128] = "";
    memset(ret,100,0);
    int len = strlen(password);
    time_t localtime=time(NULL);
    sprintf(key,"%ld",(localtime+delta)/60);
    strncpy(key,key,16);
    for(int i=0;i<len;i++){
        int keychar = password[i] ^ key[strlen(key)-1-i];
        char tmp[3]="";
        buildkey(keychar,i%2,tmp);
        strcat(ret,tmp);
    }
    strcat(ret,"\0");
    //printf("pass:%s\n",ret);
    return strlen(ret);
}
