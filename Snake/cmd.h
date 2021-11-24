#include <Windows.h>
#include <time.h>

using namespace std;

int a,b;
char ca[10],cb[10];
char cmd[101];

void cmd_change_color()///Changes the command line text and background to random colors
{
    srand(time(NULL));
    a=rand()%10;
    b=abs(rand()%10-3);
    itoa(a,ca,10);
    if(b==0) cb[0]='A';
    else if(b==1) cb[0]='B';
    else if(b==2) cb[0]='C';
    else if(b==3) cb[0]='D';
    else if(b==4) cb[0]='E';
    else cb[0]='F';
    strcpy(cmd,"color ");
    strcat(cmd,ca);
    strcat(cmd,cb);
    system(cmd);
}
