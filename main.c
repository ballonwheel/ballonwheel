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
#define MODEMDEVICE "/dev/ttyUSB0"
#define _POSIX_SOURCE 1 /* POSIX compliant source */








struct sched_param param;
pthread_attr_t attr;
pthread_t thread;
int ret;
int fd,c, res;
struct termios oldtio,newtio;
char buf[255];
char bufo[20] = "XYZ01234901234567890";
char ref;
char chprev;
int i,j, k;
int dbg_;

struct timespec begin, end;
long seconds;
long nanoseconds;
double elapsed;
int rxok;

char command[32];
float pos, pos0, pos1, pos2, vel, vel1, vel2, acc, velw;
float distance;

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

	




	//write(fd, bufo, 8);
	clock_gettime(CLOCK_REALTIME, &begin);

	while (1) { /* loop for input */
		dbg_=!(i++%1000);
		if(dbg_)
		{
			clock_gettime(CLOCK_REALTIME, &end);
    			seconds = end.tv_sec - begin.tv_sec;
			nanoseconds = end.tv_nsec - begin.tv_nsec;
    			elapsed = seconds + nanoseconds*1e-9;

			printf("Time measured: %.6f seconds.\n", elapsed);

			clock_gettime(CLOCK_REALTIME, &begin);

		}
		memset(buf, 0x00, 10);
		while((res=read(fd,&buf[0],4)) < 4)
			;
		//if(dbg_)
			//printf("%i:<--%02x %02x %02x %02x\n", res, (u_int8_t)buf[0], (u_int8_t)buf[1], (u_int8_t)buf[2], (u_int8_t)buf[3]);
		//if(buf[0] =! 'A')printf("err 'A'\n");
             	//if(buf[1] =! 'B')printf("err 'B'\n");
             	//if(buf[2] =! 'C')printf("err 'C'\n");
             	//if(buf[3] =! 'D')printf("err 'D'\n");


//V          mm   adc
//2,30 1,56  91   86   --> 1.0581
//2,91 4,28  205  159  --> 1.2893
//4,08 6,07  296  220  --> 1.3409
#define MIDDLE_ADC 159
#define MM1 91.0
#define MM2 205.0
#define MM3 296.0
#define ADC1 86.0
#define ADC2 159.0
#define ADC3 220.0
#define RATE1 (MM1/ADC1)
#define RATE2 (MM2/ADC2)
#define RATE3 (MM3/ADC3)
#define DELTA_ADC1 (ADC2-ADC1)
#define DELTA_ADC2 (ADC3-ADC2)
#define DELTA_RATE1 (RATE2-RATE1)
#define DELTA_RATE2 (RATE3-RATE2)
#define DELTA_ADC1_PER_DELTA_RATE1 (DELTA_RATE1 / DELTA_ADC1)
#define DELTA_ADC2_PER_DELTA_RATE2 (DELTA_RATE2 / DELTA_ADC2)
#define OFFSET1 (RATE1-(ADC1*DELTA_ADC1_PER_DELTA_RATE1))
#define OFFSET2 (RATE2-(ADC2*DELTA_ADC2_PER_DELTA_RATE2))


 		//buf[0] 3.3e-3
		//buf[1] 6.6e-3
		//buf[2] 10e-3
                pos0 = (u_int8_t)buf[0];
                pos1 = (u_int8_t)buf[1];
                pos2 = (u_int8_t)buf[2];
                pos = pos2;

                if(pos < MIDDLE_ADC)
                  distance = pos * (pos * DELTA_ADC1_PER_DELTA_RATE1 + OFFSET1);
                else
                  distance = pos * (pos * DELTA_ADC2_PER_DELTA_RATE2 + OFFSET2);

		//pos0 = postable[(u_int8_t)buf[0]];
                //pos1 = postable[(u_int8_t)buf[1]];
                //pos2 = postable[(u_int8_t)buf[2]];
		//pos = pos2;

		vel2=(pos2-pos1)/3.3e-3;
		vel1=(pos1-pos0)/3.3e-3;
		vel = vel2;

		acc=(vel2-vel1)/3.3e-3;


		//max speed: 2,5msec / 60degree
 		//ADC: 10khz->100usec-->2,5ms/0.1ms = 25
		velw = (u_int8_t)buf[3];

                //printf("%012.4f  %012.4f  %012.4f  %012.4f  %012.4f  %012.4f  %012.4f  %012.4f\n", OFFSET1, DELTA_ADC1_PER_DELTA_RATE1, DELTA_RATE1, DELTA_ADC1, RATE1, RATE2, RATE3, MIDDLE_ADC);
		printf("%012.4f  %012.4f  %012.4f  %012.4f  %012.4f  %012.4f  %012.4f  %012.4f\n", distance, pos0, pos1, pos2, vel1, vel2, acc, velw);

		//if(ref != buf[0]){printf("error %i:<--%0x\n", res, buf[0]); ref = buf[0];}
		//if(ref =='9'){ref='0';}
		//else ref++;




		/*****************************
		process
		******************************/
		//pos, velocity, acc, wheel_vel, wheel_acc
		//-1, +1, +1, +1
		//-1, +1,  0,
		//-1,  0,  0,

		//middle,







		write(fd, &bufo[0], 4);



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



u_int8_t postable[256]=
{

};
