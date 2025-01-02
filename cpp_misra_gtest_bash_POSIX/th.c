/*
http://linuxprograms.wordpress.com/2007/12/29/threads-programming-in-linux-examples/
http://linuxprograms.wordpress.com/2007/12/29/threads-programming-in-linux-examples/
http://linuxprograms.wordpress.com/2007/12/19/linux-kernel-support-for-threads-light-weight-processe/
http://linuxprograms.wordpress.com/2008/01/23/piping-in-threads/
http://www.thegeekstuff.com/2012/04/create-threads-in-linux/
http://www.thegeekstuff.com/2010/11/50-linux-commands/



*/



#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void * thread1()
{
        while(1){
                printf("Hello!!\n");
		sleep(5);
        }
}

void * thread2()
{
        while(1){
                printf("How are you?\n");
		sleep(5);
        }
}

int main()
{
        int status;
        pthread_t tid1,tid2;

        pthread_create(&tid1,NULL,thread1,NULL);
        pthread_create(&tid2,NULL,thread2,NULL);
        pthread_join(tid1,NULL);
        pthread_join(tid2,NULL);
        return 0;
}

