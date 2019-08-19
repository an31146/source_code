#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_DIGITS 10000000

_CRTIMP char* __cdecl __MINGW_NOTHROW	strrev (char*);

bool mem_alloc(void **ptr, size_t n, bool bRealloc)
{
    if (bRealloc && *ptr != NULL)
        *ptr = realloc(*ptr, n);
    else
        *ptr = malloc(n);
    
    if (*ptr == NULL)
    {
        printf("Failed to allocate memory.\n");
        return false;
    }
    else
    {
        printf("Allocated %u bytes at 0x%"PRIxPTR"\n", (uint32_t)n, (uint64_t)*ptr);
        return true;
    }
}

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
        char *t = NULL;
        if ( !mem_alloc((void *)&t, MAX_DIGITS, false) );
        strcpy(t, a);
        strcpy(a, b);
        strcpy(b, t);
    }
#ifdef _DEBUG
    printf("multiplicand: %s\nmultiplier: %s\n", a, b);
#endif
    
    for (int i = 0; i < strlen(a)+strlen(b); i++)
        c[i] = '0';
    // terminating null
    c[strlen(a)+strlen(b)] = '\0';
#ifdef _DEBUG
    printf("c: %s\nstrlen(c): %d\n", c, (int)strlen(c));
#endif
        
    for (int i = 0; i < strlen(a); i++)
    {
        char carry;
        for (int j = 0; j < strlen(b); j++)
        {
            char digit_i = a[i] - '0'; //putchar(a[i]);
            char digit_j = b[j] - '0'; //putchar(b[j]);

#ifdef _DEBUG
            printf("%s\n", c);
#endif
            carry = (c[i+j] - '0' + digit_i * digit_j) / 10;
#ifdef _DEBUG
            putchar(carry + '0');
            printf("%d\n", carry);
            putchar(c[i+j]);
#endif
            c[i+j] = (c[i+j] - '0' + digit_i * digit_j) % 10 + '0';
#ifdef _DEBUG
            putchar(c[i+j]);
#endif
            
            if (carry > 0)
                for (int k = i+j+1; k < strlen(a)+strlen(b); k++)
                {
                    char newcarry = (c[k] - '0' + carry) / 10;
#ifdef _DEBUG
                    putchar(newcarry + '0');
#endif
                    c[k] = (c[k] - '0' + carry) % 10 + '0';
#ifdef _DEBUG
                    putchar(c[k]);
                    printf("%c\n", c[k]);
#endif                    
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
        char t[MAX_DIGITS];
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
    char *a = NULL, *b = NULL, *c = NULL;
    int num_cases, start, stop;
    
    if ( !mem_alloc((void *)&a, MAX_DIGITS, false) )
        return EXIT_FAILURE;
    //a = malloc(MAX_DIGITS);
    //printf("%X\n", a);
    if ( !mem_alloc((void *)&b, MAX_DIGITS, false) )
        return EXIT_FAILURE;
    if ( !mem_alloc((void *)&c, 2 * MAX_DIGITS, false) )
        return EXIT_FAILURE;

    FILE *f_in = fopen("long_multiplication.txt", "r");
    FILE *f_out = fopen("long_multiplication_results.txt", "w");

    fscanf(f_in, "%d\n", &num_cases);
        
    while (num_cases--) 
    { 
        if ( !mem_alloc((void *)&a, MAX_DIGITS, true) ) 
            return EXIT_FAILURE;

        if ( !mem_alloc((void *)&b, MAX_DIGITS, true) )
            return EXIT_FAILURE;

        //scanf("%s %s", a, b);
        fgets(a, MAX_DIGITS, f_in);
        *strrchr(a, '\n') = '\0';                 // remove trailing newline!
        printf("strlen(a): %d\n", (int)strlen(a));

        fgets(b, MAX_DIGITS, f_in);
        *strrchr(b, '\n') = '\0';                 // remove trailing newline!
        printf("strlen(b): %d\n", (int)strlen(b));
        
        if ( !mem_alloc((void *)&c, 2 * MAX_DIGITS, true) )
            return EXIT_FAILURE;

        start = clock();
        long_mult(c, a, b);
        stop = clock();
        
        //addition(c, a, b);
        
        
        //printf("%s\n", c);
        fputs(c, f_out);
        fputc('\n', f_out);
        printf("wrote strlen(c) = %d bytes to file.\n", (int)strlen(c));
        printf("\ntime: %d ms\n", (stop-start));
        
    }
    fclose(f_in);
    fclose(f_out);
    free(a);
    free(b);
    free(c);
    
    return 0;
}
