




fdin=-1;
position=1;
motor=1;
motor_last=1;
positionStr="000\n";
i=0;
j=0;

while(1) #1
  positionStr="255";
 # two=2;
 #while(two)
  while(fdin < 0) #2
    #disp(':');
    fdin = fopen ("./tmp/bow_pos", "r");
    if(fdin != -1)
      if((result = fgetl (fdin))  != -1)
	positionStr = sprintf("%s",result);
        fclose(fdin);
        #fflush(fdin);
        delete("./tmp/bow_pos");
        #disp(";");
      endif
      #disp(".")
     endif
    #disp("positionstr");
    #disp(positionStr);
    position=str2num(positionStr);
  endwhile
  fdin=-1;

  #disp("position");
  #disp(position);
  #if(position ==  motor-1)
   if((position != motor-1) && (motor != 1 ))
   disp("cnt error");
   j=j+1;
   disp(i);
   disp(j);
   disp(position);
   disp(motor);
   disp("---");
   #position=motor-1;
   error=1;
  # two=0;
  else
  # two=two-1;
   error=0;
  endif
  #motor = position+1;
 #endwhile

  pause(0.0008);


 #if(error==0)
  motor=motor+1;

  if((motor>250) || (motor < 1))
   motor = 1;
  endif
  #disp("motor: ");
  #disp(motor);
  motorStr=sprintf("%03ui\n",motor); 

  fdout=fopen("./tmp/bow_motor", "w+");
  fputs(fdout,motorStr);
  fclose(fdout);
  #fflush(fdout);
 #endif

 i=i+1;
 #disp(i);
 #disp("--loop end--");

endwhile #2


