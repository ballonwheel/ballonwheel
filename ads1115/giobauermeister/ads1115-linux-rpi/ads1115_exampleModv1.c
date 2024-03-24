/*
 ============================================================================
 Name        : ads1115_example.c
 Author      : Giovanni Bauermeister
 Description : Read analog values from potentiometer using ADS1115 and prints to terminal
 https://github.com/giobauermeister/ads1115-linux-rpi/tree/master
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "ads1115_rpi.h"


#include <time.h>
#include <poll.h>


struct timespec begin, end;
long seconds;
long nanoseconds;
double elapsed;
unsigned int i, dbg_;


int main(void) {





	if(openI2CBus("/dev/i2c-1") == -1)
	{
		return EXIT_FAILURE;
	}
	setI2CSlave(0x48);

	clock_gettime(CLOCK_REALTIME, &begin);
	i = 0;

	while(1)
	{
		dbg_=!(i++%100);
		if(dbg_)
		{
			clock_gettime(CLOCK_REALTIME, &end);
    			seconds = end.tv_sec - begin.tv_sec;
			nanoseconds = end.tv_nsec - begin.tv_nsec;
    			elapsed = seconds + nanoseconds*1e-9;

			printf("Time measured: %.6f seconds.\n", elapsed);

			clock_gettime(CLOCK_REALTIME, &begin);

		}


		readVoltage(0);
		//printf("CH_0 = %.2f V\n", readVoltage(0));
		//printf("CH_1 = %.2f V\n", readVoltage(1));
		//printf("CH_2 = %.2f V\n", readVoltage(2));
	}

	return EXIT_SUCCESS;
}
