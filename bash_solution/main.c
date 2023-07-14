//imre@thinkpad:~/ballonwheel$ eval "$(ssh-agent -s)"
//Agent pid 1298
//imre@thinkpad:~/ballonwheel$ ssh-add ../.ssh/id_ed25519.rsa
//Enter passphrase for ../.ssh/id_ed25519.rsa: 
//Identity added: ../.ssh/id_ed25519.rsa (dobany.hu@gmail.com)

//imre@imiEee:~/ballonwheel$ git remote set-url origin git@github.com:ballonwheel/ballonwheel.git


//imre@imiEee:~/bow$ tar -xf arduino-cli_0.24.0_Linux_64bit.tar.gz

//../../bow/arduino-cli compile --verbose --fqbn arduino:avr:nano arduino.ino
//../../bow/arduino-cli upload -v -p /dev/ttyUSB1 --fqbn arduino:avr:nano arduino.ino

//sudo nice --20 ./a.out

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

#define BAUDRATE B115200
#define MODEMDEVICE "/dev/ttyUSB1"
#define _POSIX_SOURCE 1 /* POSIX compliant source */








struct sched_param param;
pthread_attr_t attr;
pthread_t thread;
int ret;
int fd,c, res;
FILE* fd_pos; 
FILE* fd_motor;
struct termios oldtio,newtio;
char buf[255];
char bufo[20] = "XYZ01234901234567890";
char bufo_last[20]="000\n";
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
int wait;

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

int chk(void)
{
int r=-1;
  if(buf[0]==bufo[0])
    if(buf[1]==bufo[1])
      if(buf[2]==bufo[2])
	r=0; 
 return r;
}

void *thread_func(void *data)
{

	//https://tldp.org/HOWTO/RTLinux-HOWTO-4.html
	//pthread_make_periodic_np(pthread_self(), gethrtime(), 1000000);
        //nekem ez itt tokmindegy, mert while(1) van, az arduino idozit, tehat tonkre van vagva az rt a linux oldalrol nezve

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
		memset(buf, 0x00, 5);
		while((res=read(fd,&buf[0],5)) < 5)
			;
		if((dbg_) || (chk()))
			printf("%i.%i:<--%02x %02x %02x %02x\n", i,res, (u_int8_t)buf[0], (u_int8_t)buf[1], (u_int8_t)buf[2], (u_int8_t)buf[3]);
		//if(buf[0] =! 'A')printf("err 'A'\n");
             	//if(buf[1] =! 'B')printf("err 'B'\n");
             	//if(buf[2] =! 'C')printf("err 'C'\n");
             	//if(buf[3] =! 'D')printf("err 'D'\n");
		buf[3]='\n';
		buf[4]=0;
		//sleep(0.005);


        	/*file oepn*/
        	fd_pos=fopen("./tmp/bow_pos","w+");
        	if(fd_pos == NULL){
          	  perror("Open bow_pos Failed\r\n");
        	}
		fprintf(fd_pos, "%s", buf);
		fclose(fd_pos);

		wait=1;
		while(wait){
		  memset(bufo, 0x00, 5);
	          fd_motor=fopen("./tmp/bow_motor","r");
                  if(fd_motor == NULL){
                    perror("Open bow_motor Failed\r\n");
                  }
                  if(res=fread(bufo,1, 4,fd_motor) > 0)
			wait = 0;
		  else
			printf("x");
                  //bufo[0]='0';
                  //bufo[1]='0';
                  //bufo[2]='0';
                  bufo[3]='\n';
                  bufo[4]=0;
		  //strcpy(bufo_last, bufo);
		  fclose(fd_motor);
		}

		if(dbg_)
			printf("%i.%i:-->%02x %02x %02x %02x\n", i,res, (u_int8_t)bufo[0], (u_int8_t)bufo[1], (u_int8_t)bufo[2], (u_int8_t)bufo[3]);
		write(fd, &bufo[0], 5);



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
