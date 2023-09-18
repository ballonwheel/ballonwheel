
clear




clear

global g
g=9.81
disp(g,"g=")

    global Ra
    Ra=0.3
    disp(Ra, "Ra=")
    
    global La
    La=18e-3
    disp(La, "La=")
    
    global km
    km=2.518
    disp(km, "km=")
    
    global ku
    ku=2.093
    disp(ku, "ku=")

    global rw 
    rw=0.27
    disp(rw, "rw=")
   
    mwheel = 6.6 //mass of wheel [kg]
    Town = 87.5; ///* ms */					/* time of speedup w/o load*/
    mload = 1.08; ///* kg */ 					/* mass of load (tried to be a equally divided around the circle) */
    Tload = 110; ///* ms */					/* time of speedup w/ load */
    global Iw
    Iw = Town * ((mload * rw^2) / (Tload - Town));	//* inertia of wheel */
    disp(Iw,"Iw=")
    
    Tm=(Iw*3*Ra)/(km*ku)

    Te=(La)/(3*Ra)

    global rv
    rv=2.3e-3
    disp(rv, "rv=")


    global mb
    mb=0.203
    disp(mb, "mb=")
    
    //radius of ball as a sphere
    global rbf
    rbf=30e-3
    disp(rbf, "rbf=")

    //radius of circle of ball ON EDGE
    global rb
    edge=30e-3;
    rb=sqrt(rbf^2-(edge/2)^2);
    disp(rb, "rb=")

    global Ib
    Ib=(2/5) * mb * rbf^2
    disp(Ib, "Ib=")



//**************************************************************
//PI controller design
//**************************************************************
//the motor a PIT1 system
//s=poly(0,'s');
//A_motor=(1/(Ra+L*s)*km*1/(Iw*s));
//B_motor=ku;
//W_motor=syslin('c', (A_motor)/(1+A_motor*B_motor));
//bode(W_motor);
//clf;nyquist(W_motor);

s=poly(0,'s')
G=syslin('c', (1/km)/(Tm*Te*s^2+Tm*s+1))
//show_margins(G)
//bode(G)
//K_PID_DCmotorLoad=0.6351
//Ti_PID_DCmotorLoad=0.01


//**************************************************************
//BoW system - space state equations
//**************************************************************

//the system below calculated with Maxima --> file: Bow.mc

nev=(Ib*Iw+mb*rb^2*Iw+mb*rw^2*Ib)

alfaw=(Ib*rw*mb*g)/nev

betaw=(Ib+mb*rb^2)*(ku*km+rv*Ra)/(Ra*nev)

gammaw=km*(Ib+mb*rb^2)/(Ra*nev)

alfa2=(Iw*rb^2+Ib*rw^2)/((rw+rb)*nev)

beta2=(Ib*rw*(ku*km+rv*Ra))/(Ra*(rw+rb)*nev)

gamma2=(Ib*rw*km)/(Ra*(rw+rb)*nev)



A=[-betaw, 0, alfaw;
-beta2, 0, alfa2;
0, 1, 0]

B=[gammaw; gamma2; 0]

C = [0, 0, 1;]

D = [0;]






Ts = 0.02 
Kp = 534
Ki = 5.14e+03 
Kd = 12.1 
