

clear
warning('off')

 position_str="0"
 fd1=mopen('/home/imre/bow_pos','w')
 position_str=mputl(position_str, fd1)
 mclose(fd1)

 motor_str ="0"
 fd2=mopen('/home/imre/bow_motor','w') 
 mputl(motor_str, fd2)
 //mclose(fd2)
 
position=0 
i=0
j=0

while 1
 j=j+1
 
 [fd3, err3]=mopen('/home/imre/bow_pos','r')
 //disp(fd3)
 //disp(err3,"err3")
 position_str=mgetl(fd3,1)
 mclose(fd3)

 //disp(position_str)
 //disp(isempty(position_str), "isempty")
 if(isempty(position_str)==%F) then
   position=strtod(position_str)
 else
   disp(j, i,"position_str false")
   i=i+1   
 end
 
 motor = position
 //disp(motor)
 motor_str=string(motor)
 //disp(motor_str)


 fd4=mopen('/home/imre/bow_motor','w') 
 mputl(motor_str, fd4)
 mclose(fd4)
  
 sleep(20)
 
end

mclose(fd2)


