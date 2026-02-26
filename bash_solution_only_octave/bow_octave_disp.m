




fdout=fopen("./tmp/bow_dispout", "r");
pos = [1:1:1000];
mot = [1:1:1000];
x=[1:1:1000];
i=1;

while(1)
 if(i > 1000)
  i = 1;
  fclose(fdout);
  subplot(2,1,1);
  plot(x,pos);
  subplot(2,1,2);
  plot(x,mot);
  #disp(".");

  pos = pos * 0;
  mot = mot * 0;

  pause(1);
  copyfile("./tmp/bow_dispout", "./tmp/bow_dispget");
  fdout=fopen("./tmp/bow_dispget", "r");
 endif

  pos(i)=fread(fdout,1, "uint8");
  mot(i)=fread(fdout,1, "uint8");

 i = i + 1;
endwhile



#graphics_toolkit ("gnuplot")
#x = -2*pi:2*pi;
#y = sin(x);
#figure(1,'Visible','off');
#plot(y)
#print (1, "-ddumb", "dummy");
#type -q dummy.txt
#pause(2);

