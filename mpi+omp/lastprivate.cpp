<<<<<<< HEAD
#include <stdio.h> 
int main() 
{    
    int done = 4, done2 = 5;    
     
#pragma omp parallel for lastprivate(done, done2) num_threads(2) schedule(static)
    for(int a=0; a<8; ++a)
    {
        if(a==2) done=done2=0;
        if(a==3) done=done2=1;
    }
    printf("%d,%d\n", done,done2);
}
=======
#include <stdio.h> 
int main() 
{    
    int done = 4, done2 = 5;    
     
#pragma omp parallel for lastprivate(done, done2) num_threads(2) schedule(static)
    for(int a=0; a<8; ++a)
    {
        if(a==2) done=done2=0;
        if(a==3) done=done2=1;
    }
    printf("%d,%d\n", done,done2);
}
>>>>>>> c9927b723f2cb399085be2360f2ae56e793e3691
