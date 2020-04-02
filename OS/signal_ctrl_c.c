#include <signal.h>
#include <stdio.h>
#include <stdbool.h>

volatile bool schedule_stopped_flag = 0;

void signals_handler(int sig)
{
	signal(SIGINT, &signals_handler);			// this DOES have an effect!
    printf("Inside signals_handler \n");
    schedule_stopped_flag = true;
	fflush(stdout);
}

void signals_capture(void)
{
    signal(SIGINT, &signals_handler);
}

int main()
{
	signals_capture();
	
	for (int i = 0; i < 10; i++)
	{
		printf("[%d] Press Ctrl-C: ", i);
		getchar();
		printf("\n%d\n", schedule_stopped_flag);
	}
	
	return 0;
}
