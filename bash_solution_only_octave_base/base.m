

clear

pkg load control

pkg load signal

pkg load instrument-control

 

 

 

supportedinterfaces = instrhwinfo().SupportedInterfaces

 

if ! isempty(strfind (supportedinterfaces , "serial"))

    disp("Serial: Supported")

else

    disp("Serial: Unsupported")

endif

#similarly with:

# ! isempty(strfind (supportedinterfaces , "parallel"))

# ! isempty(strfind (supportedinterfaces , "i2c"))

#********************************************************

#{

set(s1, "baudrate", 9600) # Change baudrate

set(s1, "bytesize", 5)    # Change byte size (config becomes 5-N-1)

set(s1, "parity", "E")    # Changes parity checking (config becomes 5-E-1),

                          # possible values [E]ven, [O]dd, [N]one.

set(s1, "stopbits", 2)    # Changes stop bits (config becomes 5-E-2), possible

                          # values 1, 2.

set(s1, "dataterminalready", "on")  # Enables DTR line

set(s1, "requesttosend", "on")      # Enables RTS line

                                    # possible values "on", "off".

#}

#{

#********************************************************

# Opens serial port ttyUSB1 with baudrate of 115200 (config defaults to 8-N-1)

s1 = serial("/dev/ttyUSB1", 115200)

# Opens serial port COM10

s1 = serial("\\\\.\\COM10");

# Flush input and output buffers

srl_flush(s1);

# Blocking write call, currently only accepts strings

srl_write(s1, "Hello world!")

# Blocking read call, returns uint8 array of exactly 12 bytes read

data = srl_read(s1, 12)

# Convert uint8 array to string,

char(data)

#or simple you cn do this

s1 = serial("/dev/ttyS0", 9600, 10) # Opens serial port ttyS0 in, 10s timeout

#********************************************************

#}

s = serial("/dev/ttyUSB0", 115200);

#s = serial("\\\\.\\COM4", 115200);

Ts=0.01;

TO = 5*1/Ts;

timeout = 0;

out=[uint8(0) uint8(0) uint8(Ts*1000)];

i=0;

 

run = 1;

while(run)#----------------------------------------------------------------------antiwindup check

  i=i+1

  if (kbhit (1) == 'x')

    break

  endif

  flushinput(s);

  val = srl_read(s, 3);

  #val = fread(s, 3);

  #pc(rx)<-- val(1) val(2) val(3)

  #val(1) - prev frame counter

  #val(2) - adc

  #val(3) - not used

  #pc(tx)-->out(1)out(2)out(3)

  #out(1) - new frame counter

  #out(2) - motor voltage

  #out(3) - Ts in msec

  #disp(".")

 

  #check frame id

  #if(val(1) != out(1) || val(2) != out(2))

  if(val(1) != out(1))                                                        #todo!! do somethig

   disp("frame error");

   disp(i);

 

   disp(val);

   disp(out);

  endif

 

  #control, calc new value

 

 

  #eofControl

 

  #saturation

  #if(u>vdc)

  #    u=vdc;

  #endif

  #if(u<-vdc)

  #    u=-vdc;

  #endif

  #disp(u);

 

 

  #scale for drive

  #out(2) = uint8((vdc + u) * drive_zero/vdc);

  #disp("motor: ");

  #disp(out(2));

 

 

  #timeout of outOfRange

  #chk if in limit and act signal is also on limit for a certain tima

  #if(timeout > TO)

  #  run = 0;

  #endif

 

 

  #frame handling

  if(out(1) == 255)

   out(1) = 0;

  else

   out(1) = out(1) + 1;

  endif

  disp(out)

  #pc(rx)<-- val(1) val(2) val(3)

  #val(1) - prev frame counter

  #val(2) - adc

  #val(3) - not used

  #pc(tx)-->out(1)out(2)out(3)

  #out(1) - new frame counter

  #out(2) - motor voltage

  #out(3) - Ts in msec

  #srl_write(s, out(1:3));

  fwrite(s, out(1:3));

endwhile#--------------------------------------------------------------------------------------antiwindup

 

disp("end of run")

disp("close port...byebye")

fclose(s) # Closes and releases serial interface object



