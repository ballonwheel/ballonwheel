

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#define BAUDRATE B2000000
#define MODEMDEVICE "/dev/ttyUSB0"
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1
volatile int STOP=FALSE;
int i;
main()
{
int fd,c, res;
struct termios oldtio,newtio;
char buf[255];
char bufo[4] = "ABCD";
	fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY );
	if (fd <0) {perror(MODEMDEVICE); exit(-1); }
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
	while (1) { /* loop for input */
		i++;
		res = read(fd,buf,255); /* returns after 5 chars have been input */
		printf("%i:%i:<--%0x %0x  %0x  %0x\n", i, res, buf[0], buf[1], buf[2], buf[3]);
		res = write(fd, bufo, 4);
		printf("%i:%i:-->%0x %0x  %0x  %0x\n", i, res, bufo[0], bufo[1], bufo[2], bufo[3]);
	}
	tcsetattr(fd,TCSANOW,&oldtio);
}

