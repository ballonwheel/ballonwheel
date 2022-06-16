/*
 * main.c
 *
 * @date 2019/08/09
 * @author Cosmin Tanislav
 * @author Cristian Fatu
 */

#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include "uart.h"

#include <time.h>




#define LOOPBACK_FORMAT "%i\r\n"
#define LOOPBACK_FORMAT_LEN strlen(LOOPBACK_FORMAT)
#define MAX_READ_SIZE 235
#define MAX_LOOPBACK_SIZE MAX_READ_SIZE + LOOPBACK_FORMAT_LEN

int main() {
	struct UartDevice dev;
	int rc;
	int i=0;
   	int cnt=0;
	unsigned char  sdata=0, rdata;

	dev.filename = "/dev/ttyUSB0";
	dev.rate = B2000000;

	rc = uart_start(&dev, false);
	if (rc) {
		return rc;
	}

	char read_data[MAX_READ_SIZE];
	char loopback_data[MAX_LOOPBACK_SIZE];
	size_t read_data_len;

	printf("UART DEMO\r\n");
	//uart_writes(&dev, "UART DEMO\r\n");

	while (1) {
		cnt++;
		printf("%i: ",cnt);
		if(rc = uart_read(&dev, read_data)){
		 printf("%c\n", read_data[0]);
		 if(read_data[0] == 'D' || read_data[0] == 'd' || read_data[0] == '4'){
		  if(i){
			sprintf(loopback_data, "ABCD");
			i = 0;
		  }
		  else{
                        sprintf(loopback_data, "1234");
                        i = 1;
		  }	

		  uart_writes(&dev, loopback_data);
		 }
		}
		//rc = read(*dev.filename, read_data, 4);
		//read_data[rc] = 0;
		//printf("%s\n", read_data);

		//getchar();
		//150usec
		//nanosleep((const struct timespec[]){{0, 150000L}}, NULL);
#if 0
		if (read_data_len > 0) {
			printf("rd:%s", read_data);
			snprintf(loopback_data, MAX_LOOPBACK_SIZE, LOOPBACK_FORMAT, i);
			printf("wr:%s",loopback_data);
			uart_writes(&dev, loopback_data);
			i++;
			if(i>9)i=0;
		}
#endif
	}

	uart_stop(&dev);

    return 0;
}
