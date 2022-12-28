//imre@thinkpad:~/ballonwheel$ eval "$(ssh-agent -s)"
//Agent pid 1298
//imre@thinkpad:~/ballonwheel$ ssh-add ../.ssh/id_ed25519.rsa
//Enter passphrase for ../.ssh/id_ed25519.rsa: 
//Identity added: ../.ssh/id_ed25519.rsa (dobany.hu@gmail.com)
 
//imre@imiEee:~/ballonwheel$ git remote set-url origin git@github.com:ballonwheel/ballonwheel.git


//imre@imiEee:~/bow$ tar -xf arduino-cli_0.24.0_Linux_64bit.tar.gz

//../../bow/arduino-cli compile --verbose --fqbn arduino:avr:nano arduino.ino
//../../bow/arduino-cli upload -v -p /dev/ttyUSB1 --fqbn arduino:avr:nano arduino.ino

 

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
#include <unistd.h>
#include <sys/mman.h>


#include <fcntl.h>
#include <termios.h>
#include <string.h>

#include <time.h>
#include <poll.h>


#include <errno.h>
extern int errno ;

#define BAUDRATE B2000000
#define MODEMDEVICE "/dev/ttyUSB1"
#define _POSIX_SOURCE 1 /* POSIX compliant source */








struct sched_param param;
pthread_attr_t attr;
pthread_t thread;
int ret;
int fd,c, res;
struct termios oldtio,newtio;
char buf[255];
char bufo[20] = "ABCDEFGH\r\nIJK";
char ref, ref_last;
char chprev;
int i,j, k, m;
int dbg_;
int inta=1, intb=0;
float floata=1.0, floatb=1.0;
struct timespec begin, end;
long seconds;
long nanoseconds;
double elapsed;
int rxok;

char command[32];



#if  1
//https://stackoverflow.com/questions/2917881/how-to-implement-a-timeout-in-read-function-call
size_t TimeoutRead (int port, void*buf, size_t size, int mlsec_timeout)
{
    struct pollfd fd = { .fd = port, .events = POLLIN };

    size_t      bytesread = 0;

    while (poll (&fd, 1, mlsec_timeout) == 1)
    {
        int chunksize = read (port, buf + bytesread, size);
        if (chunksize == -1)
            return -1;

        bytesread += chunksize;
        size -= chunksize;

        if (size == 0)
            return bytesread;
    }

    // TODO: IsTimeout = true;
    return bytesread = 99;
}
#endif


void *thread_func(void *data)
{
        /* Do RT specific stuff here */
 	printf("Do RT specific stuff here\n");

        fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY );
        if (fd <0) {perror(MODEMDEVICE); exit(-1); }
        tcgetattr(fd,&oldtio); /* save current port settings */
        bzero(&newtio, sizeof(newtio));
        newtio.c_cflag = BAUDRATE /*| CRTSCTS*/ | CS8 /*| CLOCAL*/ | CREAD;
        newtio.c_iflag = IGNPAR;
        //newtio.c_oflag = 0;
        /* set input mode (non−canonical, no echo,...) */
        newtio.c_lflag = 0;
        newtio.c_cc[VTIME] = 0; /* inter−character timer unused */
        newtio.c_cc[VMIN] = 1; /* blocking read until 5 chars received */
        tcflush(fd, TCIFLUSH);
        tcsetattr(fd,TCSANOW,&newtio);
        
	/* #setserial /dev/ttyUSB1 low_latency */
        //ioctl(fd, TIOCGSERIAL, &serial);
        //serial.flags |= ASYNC_LOW_LATENCY;
        //ioctl(fd, TIOCSSERIAL, &serial);
	//avagy
	fcntl(fd, F_SETFL, FNDELAY);


	//select() vagy poll() --> read(), hogy legyen TO
	//https://stackoverflow.com/questions/2917881/how-to-implement-a-timeout-in-read-function-call
	//scanf("%10s",command);
	//printf("%s",command);
	//p = getcmd(command);
    	//memset(buf, 0x00, 10);
	//write(fd, p, 10);
        //res=TimeoutRead (fd, &buf[0], 10, 100);

	

	printf("int:%i ",inta+intb);


	//write(fd, bufo, 8);
	clock_gettime(CLOCK_REALTIME, &begin);

/*
ez a verzio:
timer inditja ADCt
ADC inditja tx-et
tx inditja rx-et

ez jol megy, de szkoppal be kell huzni a min idokre....
*/


	while (1) { /* loop for input */
		dbg_=!(i++%10000);
		if(dbg_)
		{
			clock_gettime(CLOCK_REALTIME, &end);
    			seconds = end.tv_sec - begin.tv_sec;
			nanoseconds = end.tv_nsec - begin.tv_nsec;
    			elapsed = seconds + nanoseconds*1e-9;

			printf("err:%i /Time measured: %.6f seconds.\n", m, elapsed);

			clock_gettime(CLOCK_REALTIME, &begin);

		}
		memset(buf, 0x00, 10);
		while((res=read(fd,&buf[0],1)) < 1)
			;
		//if(dbg_)
		//	printf("%i.  %i:<--%02x %02x %02x\n", m++, res, buf[0], ref, ref_last);
		if(ref != buf[0]){m++;}
		//ref_last = ref;
		if(buf[0] == ref)ref++;
		if(ref >'9' || ref < '0'){ref='0';}
		bufo[0]=ref;
		write(fd, &bufo[0],1);
		
	}

	tcsetattr(fd,TCSANOW,&oldtio);

        return NULL;
}

int main(void)
{

#if 0
	fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY );
	if (fd <0) {perror(MODEMDEVICE); exit(-1); }
	tcgetattr(fd,&oldtio); /* save current port settings */
	bzero(&newtio, sizeof(newtio));
	newtio.c_cflag = BAUDRATE /*| CRTSCTS*/ | CS8 | CLOCAL | CREAD;
	newtio.c_iflag = IGNPAR;
	newtio.c_oflag = 0;
	/* set input mode (non−canonical, no echo,...) */
	newtio.c_lflag = 0;
	newtio.c_cc[VTIME] = 0; /* inter−character timer unused */
	newtio.c_cc[VMIN] = 4; /* blocking read until 5 chars received */
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd,TCSANOW,&newtio);
	/* setserial lowlatency */
	ioctl(fd, TIOCGSERIAL, &serial);
	serial.flags |= ASYNC_LOW_LATENCY;
	ioctl(fd, TIOCSSERIAL, &serial);

#endif


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
		sleep(5);
		printf(".");
	}

        /* Join the thread and wait until it is done */
        ret = pthread_join(thread, NULL);
        if (ret)
                printf("join pthread failed: %m\n");

out:

	//tcsetattr(fd,TCSANOW,&oldtio);

        return ret;
}



