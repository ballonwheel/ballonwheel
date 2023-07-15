




fdin=-1;


while(1) #1
  while(fdin < 0) #2
    fdin = fopen ("./tmp/bow_pos", "r");
  endwhile  #2
  txt = fgetl (fdin);
  fclose(fdin);
  fdin=-1;


  #do some math here


 fdout=fopen("./tmp/bow_motor");
 fputs(fdout,"001\n");
 fclose(fdout);


endwhile #2


