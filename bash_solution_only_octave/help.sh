


echo "a kandos raspberry jelszo iqhomesetup "
echo "run/stop like this:"
echo " "
echo ">sudo ./run.sh"
echo ">sudo ./stop.sh"
echo " "
echo "-------------------------- o ---------------------------------"
echo " "
echo "szenzor kalibralas"
echo " "
echo "antiwindup"
echo "  ha a 5sec-ig a pozicio nagyobb mint +-13 fok es a motor fesz 24V, akkor ini van es ujra probalkozik 5sec mulva"
echo " "
echo "kommunikacio"
echo " linux<--arduino  [frame_number]       [ADC]      [notused]		#arduino adja a schedule-t, Ts"
echo " linux-->arduino  [frame_number] [motor_voltage]    [Ts]			#linux mondja meg a Ts-t"
echo " "
echo "arduino-nano-motor-drive"
echo " - ha nem jon rx a linux-tol(10darab), akkor a default Ts = 16msec"
echo " - ha a Ts < 20msec vagy nagybb 100msec, akkor a default Ts=19msec lesz"
echo " - ha default Ts van, akkor leall a motor"




