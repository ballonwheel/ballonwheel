

clear

/*here the bow initialization */


/* here the UART rx-tx ini*/
warning('off')

POSITION='/home/imre/ballonwheel/bash_solution/tmp/bow_pos'
MOTOR='/home/imre/ballonwheel/bash_solution/tmp/bow_motor'
 
position=0; 
i=0;
j=0;

deletefile(POSITION);

/* here starting the rx-tx loop */
/* start with this script runing, then 'uart_rxtx.sh' from bash*/
while 1
 j=j+1;
 
 err3=-2;
 while err3 ~= 0
  [fd3, err3]=mopen(POSITION,'r');
  /*disp("xxxxxxxxxxxxxxxxx",fd3)*/
  /*disp(err3,"err3")*/
 end
 position_str=mgetl(fd3,1);
 mclose(fd3);
 deletefile(POSITION);

 //disp(position_str);
 //disp(isempty(position_str), "isempty");
 if(isempty(position_str)==%F) then
   position=strtod(position_str);
 else
   disp(j, i,"position_str false");
   i=i+1;   
 end
 

 /* here the bow control algorithm /*


 motor = position;


 disp(motor);
 */ end of bow control */

 /*create file of 'bow_motor', that will be sent by bash*/
 //disp(motor);
 motor_str=string(motor);
 disp(motor_str);


 fd4=mopen(MOTOR,'w'); 
 mputl(motor_str, fd4);
 mclose(fd4);
  
 //sleep(20); no sleep, but need to be finished before next sent by arduino (10msec)  !!!!
 
end

mclose(fd2);


