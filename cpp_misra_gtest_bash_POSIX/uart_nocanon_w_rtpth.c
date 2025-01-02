

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>


#include <limits.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>


#include <fcntl.h>
#include <termios.h>
#include <string.h>

#include <time.h>
#include <poll.h>

#define BAUDRATE B2000000
#define MODEMDEVICE "/dev/ttyUSB0"
#define _POSIX_SOURCE 1 /* POSIX compliant source */


struct sched_param param;
pthread_attr_t attr;
pthread_t thread;
int ret;
int fd,c, res;
struct termios oldtio,newtio;
char buf[255];
char bufo[4] = "ABCD";
int i;

void *thread_func(void *data)
{
        /* Do RT specific stuff here */
 	printf("Do RT specific stuff here\n");

	write(fd, bufo, 4);
	while (1) { /* loop for input */
		i++;
		res = read(fd,buf,255); /* returns after 5 chars have been input */
		printf("%i:%i:<--%0x %0x  %0x  %0x\n", i, res, buf[0], buf[1], buf[2], buf[3]);
		res = write(fd, bufo, 4);
		printf("%i:%i:-->%0x %0x  %0x  %0x\n", i, res bufo[0], bufo[1], bufo[2], bufo[3]);
	}


        return NULL;
}

int main(void)
{
	fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY );
	if (fd <0) {perror(MODEMDEVICE); exit(−1); }
	tcgetattr(fd,&oldtio); /* save current port settings */
	bzero(&newtio, sizeof(newtio));
	newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
	newtio.c_iflag = IGNPAR;
	newtio.c_oflag = 0;
	/* set input mode (non−canonical, no echo,...) */
	newtio.c_lflag = 0;
	newtio.c_cc[VTIME] = 0; /* inter−character timer unused */
	newtio.c_cc[VMIN] = 4; /* blocking read until 5 chars received */
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd,TCSANOW,&newtio);


        /* Lock memory */
        if(mlockall(MCL_CURRENT|MCL_FUTURE) == -1) {
                printf("mlockall failed: %m\n");
                exit(-2);
        }

        /* Initialize pthread attributes (default values) */
        ret = pthread_attr_init(&attr);
        if (ret) {
                printf("init pthread attributes failed\n");
                goto out;
        }

        /* Set a specific stack size  */
        ret = pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN);
        if (ret) {
            printf("pthread setstacksize failed\n");
            goto out;
        }

        /* Set scheduler policy and priority of pthread */
        ret = pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
        if (ret) {
                printf("pthread setschedpolicy failed\n");
                goto out;
        }
        param.sched_priority = 80;
        ret = pthread_attr_setschedparam(&attr, &param);
        if (ret) {
                printf("pthread setschedparam failed\n");
                goto out;
        }
        /* Use scheduling parameters of attr */
        ret = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
        if (ret) {
                printf("pthread setinheritsched failed\n");
                goto out;
        }

        /* Create a pthread with specified attributes */
	printf("create pth\n");
        ret = pthread_create(&thread, &attr, thread_func, NULL);
        if (ret) {
                printf("create pthread failed: %i\n", ret);
                goto out;
        }

	printf(":");
	while(1)
	{
		//nanosleep((const struct timespec[]){{0, 1000000000L}}, NULL);
		sleep(0.5);
		printf(".");
	}

        /* Join the thread and wait until it is done */
        ret = pthread_join(thread, NULL);
        if (ret)
                printf("join pthread failed: %m\n");

out:

	tcsetattr(fd,TCSANOW,&oldtio);

        return ret;
}



