#include <stdio.h>
#include <string.h>
#include <time.h>

_CRTIMP char* __cdecl __MINGW_NOTHROW	strrev (char*);

void long_mult(char *c, char *a, char *b)
{
    if (strcmp(a, "0") == 0 || strcmp(b, "0") == 0)
    {
        strcpy(c, "0");
        return;
    }

    strrev(a); strrev(b);
    
    // the following swap doesn't make any difference
    if (strlen(a) > strlen(b))
    {
        char t[100];
        strcpy(t, a);
        strcpy(a, b);
        strcpy(b, t);
    }
    
    for (int i=0; i<strlen(a)+strlen(b); i++)
        c[i] = '0';
    // terminating null
    c[strlen(a)+strlen(b)] = '\0';
        
    for (int i=0; i<strlen(a); i++)
    {
        char carry;
        for (int j=0; j<strlen(b); j++)
        {
            char digit_i = a[i] - '0';
            char digit_j = b[j] - '0';

            //printf("%s\n", c);
            carry = (c[i+j] - '0' + digit_i * digit_j) / 10;
            //printf("%d\n", carry);

            c[i+j] = (c[i+j] - '0' + digit_i * digit_j) % 10 + '0';
            
            if (carry > 0)
                for (int k=i+j+1; k<strlen(a)+strlen(b); k++)
                {
                    char newcarry = (c[k] - '0' + carry) / 10;
                    c[k] = (c[k] - '0' + carry) % 10 + '0';
                    //printf("%c\n", c[k]);
                    
                    if (newcarry == 0)
                        break;
                    else
                        carry = newcarry;
                }
        }
    }   // for
    if (c[strlen(c)-1] == '0')
        c[strlen(c)-1] = '\0';
    strrev(c);
    
    return;
}

void addition(char *c, char *a, char *b)
{
    strrev(a); strrev(b);

    if (strlen(a) > strlen(b))          // make a the shorter string
    {
        char t[100];
        strcpy(t, a);
        strcpy(a, b);
        strcpy(b, t);
    }

    for (int i=0; i<=strlen(b); i++)
        c[i] = '0';
    // terminating null
    c[strlen(b)+1] = '\0';
    
    for (int i=0; i<strlen(a); i++)
    {
        char carry;
        char digit_a = a[i] - '0';
        char digit_b = b[i] - '0';
        
        carry = (c[i] - '0' + digit_a + digit_b) / 10;
        c[i] = (c[i] - '0' + digit_a + digit_b) % 10 + '0';
        
        if (carry > 0)
            c[i+1] = carry + '0';
    }
    if (c[strlen(c)-1] == '0')
        c[strlen(c)-1] = '\0';
    strrev(c);
    
    return;
}

int main(int argc, char *argv[])
{
    char a[10000], b[10000], c[20000];
    int cases, start, stop;
    
    scanf("%d", &cases);
    
    while (cases--)
    {
        scanf("%s %s", a, b);
        
        start = clock();
        long_mult(c, a, b);
        stop = clock();
        
        //addition(c, a, b);
        
        printf("%s\n", c);
        printf("\ntime: %d ms\n", (stop-start));
    }
    
    return 0;
}
