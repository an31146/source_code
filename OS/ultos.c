#include <stdio.h>
#include <stdlib.h>
int main (void) 
{
    unsigned long long input[] = {1,45,0xf001,0xffffffffffffffff};
    int i = 0;
    char buf[5] = {0};
    for(i=0;i<_countof(input);i++)
    {
        _ultoa_s(input[i],buf,sizeof(buf),16);
        printf("%s\n",buf);
    }
    return 1;   
}
