// crt_localtime_s.c
/* This program uses _time64 to get the current time 
 * and then uses _localtime64_s() to convert this time to a structure 
 * representing the local time. The program converts the result 
 * from a 24-hour clock to a 12-hour clock and determines the 
 * proper extension (AM or PM).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

int main( void )
{
    struct tm newtime;
    char am_pm[] = "AM";
    __time64_t long_time;
    char timebuf[26];

    // Get time as 64-bit integer.
    _time64( &long_time ); 
    // Convert to local time.
    //newtime = (struct tm *)_localtime64( &long_time ); 
    memcpy( &newtime, (void *)_localtime64( &long_time ), sizeof(newtime) ); 

    if( newtime.tm_hour > 12 )        // Set up extension. 
            strcpy( am_pm, "PM" );
    if( newtime.tm_hour > 12 )        // Convert from 24-hour 
            newtime.tm_hour -= 12;    // to 12-hour clock. 
    if( newtime.tm_hour == 0 )        // Set hour to 12 if midnight.
            newtime.tm_hour = 12;

    // Convert to an ASCII representation. 
    strcpy( timebuf, asctime(&newtime) );
    printf( "%.19s %s\n", timebuf, am_pm );
    printf( "Press Enter...");
    getch();
}
