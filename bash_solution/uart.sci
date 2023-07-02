

clear

/*here the bow initialization */
    global g;
    g=9.81;
    //disp(g,"g=");

    global Ra;
    Ra=0.3;
    //disp(Ra, "Ra=")

    global La;
    La=18e-3;
    //disp(La, "La=")

    global km;
    km=2.518;
    //disp(km, "km=")

    global ku;
    ku=2.093;
    //disp(ku, "ku=")

    global rw;
    rw=0.27;
    //disp(rw, "rw=")

    mwheel = 6.6; //mass of wheel [kg]
    Town = 87.5; ///* ms */                                     /* time of speedup w/o load*/
    mload = 1.08; ///* kg */                                    /* mass of load (tried to be a equally divided around the circle) */
    Tload = 110; ///* ms */                                     /* time of speedup w/ load */
    global Iw;
    Iw = Town * ((mload * rw^2) / (Tload - Town));      /* inertia of wheel */
    //disp(Iw,"Iw=")


    Tm=(Iw*3*Ra)/(km*ku);

    Te=(La)/(3*Ra);

    global rv;
    rv=2.3e-3;
    //disp(rv, "rv=")


    global mb;
    mb=0.203;
    //disp(mb, "mb=")


    //radius of ball as a sphere
    global rbf;
    rbf=30e-3;
    //disp(rbf, "rbf=")

    //radius of circle of ball ON EDGE
    global rb;
    edge=30e-3;
    rb=sqrt(rbf^2-(edge/2)^2);
    //disp(rb, "rb=")

    global Ib;
    Ib=(2/5) * mb * rbf^2;
    //disp(Ib, "Ib=")


//**************************************************************
//PI controller design
//**************************************************************
//the motor a PIT1 system
//s=poly(0,'s');
//Big_gamma = km/(ku*km+Ra*rv);
//A_motor=(1/(Ra+L*s)*km*1/(Iw*s));
//B_motor=ku;
//W_motor=syslin('c', (A_motor)/(1+A_motor*B_motor));
//bode(W_motor);
//clf;nyquist(W_motor);

//s=poly(0,'s')
//G=syslin('c', (1/km)/(Tm*Te*s^2+Tm*s+1))
//show_margins(G)
//bode(G)
//K_PID_DCmotorLoad=0.6351
//Ti_PID_DCmotorLoad=0.01

//**************************************************************
//BoW system - space state equations
//**************************************************************
//the system below calculated with Maxima --> file: Bow.mc
nev=(Ib*Iw+mb*rb^2*Iw+mb*rw^2*Ib);
alfaw=(Ib*rw*mb*g)/nev;
betaw=(Ib+mb*rb^2)*(ku*km+rv*Ra)/(Ra*nev);
gammaw=km*(Ib+mb*rb^2)/(Ra*nev);
alfa2=(Iw*rb^2+Ib*rw^2)/((rw+rb)*nev);
beta2=(Ib*rw*(ku*km+rv*Ra))/(Ra*(rw+rb)*nev);
gamma2=(Ib*rw*km)/(Ra*(rw+rb)*nev);


A=[-betaw, 0, alfaw;
-beta2, 0, alfa2;
0, 1, 0];

B=[gammaw; gamma2; 0];

C = [0, 0, 1;];

D = [0;];


// Check contollability
AB = [B, A*B, A*A*B;];
rank(AB);
spec(AB);

BoW_ss = syslin('c', A,B,C);
BoW_tf = ss2tf(BoW_ss);
//disp(BoW_tf_manual); //checking both tf equal
//scf(1); clf(1);
//bode(BoW_tf)
//scf(2); clf(2);
//nyquist(BoW_tf);
//scf(3); clf(3);
//plzr(BoW_tf);
//zpk(BoW_tf);

BoW_ssd=dscr(BoW_ss, 0.02);
F=BoW_ssd(2);
g=BoW_ssd(3);
c=BoW_ssd(4);
d=BoW_ssd(5);


/*
F=ppol(A,B,[-1,-1,-1]);
spec(A-B*F);
BoW_ssF = syslin('c', A-B*F,B,C);
BoW_tfF = ss2tf(BoW_ssF);
scf(11); clf(11);
bode(BoW_tfF);
scf(12); clf(12);
nyquist(BoW_tfF)
scf(13); clf(13);
plzr(BoW_tfF);
zpk(BoW_tfF);
*/



//**************************************************************
// LQ project --> K
//**************************************************************
//the system below calculated with Maxima --> file: Bow.mc

//omegaw suly
q1 = 0.009118906527810399;
//q1 = 1

//omega2 suly
q2 = 0.0914991210900477;
//q2 = 1

//fi2 suly
q3 = 131.3122540004697;
//q3 = 1

//Uk suly
R = 0.25;
//R = 1

Q = diag([q1, q2,q3]);
Big = sysdiag(Q,R);              //create big common Q R matrix
// Now we calculate C1 and D12
nstates = 3;
// full rank factirazition : egy matrixot tobb matrix szorzatara bontok: a tenyezok rangja azonos legyen az eredeti matrix rangjaval
[w,wp] = fullrf(Big);
C1=wp(:,1:nstates); // kivalasztjuk az osszes sort es az 1->nstates oszlopokat
D12=wp(:,$:$);   //[C1,D12]'*[C1,D12]=Big //$ means last element: mindegyik sor utolso eleme-->oszlop matrix

P = syslin('c',A,B,C1,D12);      // The plant (continuous-time)
[K,X] = lqr(P);
spec(A+B*K);                                     // check stability

Pd = syslin('d',F,g,C1,D12);     // The plant (continuous-time)
[Kd,X] = lqr(Pd);
spec(F+g*Kd);                                            // check stability


//**************************************************************
// estimator discrete
//**************************************************************
//same LQ metode and q used

FT = F';

cT = c';

FTcT = [cT, FT * cT, FT *FT * cT];

rank(FTcT);
spec(FTcT);

// LQ project for estimator
//Q = diag([131.3122540004697,2.05175396875734,0.009118906527810399])           // comes from maxima
//R = 0.25

Big_est = sysdiag(Q,R);                  //create big common Q R matrix
// Now we calculate C1 and D12
//nstates = 3;
// full rank factirazition : egy matrixot tobb matrix szorzatara bontok: a tenyezok rangja azonos legyen az eredeti matrix rangjaval
[w_est, wp_est] = fullrf(Big_est);
c1_est = wp_est(:,1:nstates); // kivalasztjuk az osszes sort es az 1->nstates oszlopokat
d12_est = wp_est(:,$:$);   //[C1,D12]'*[C1,D12]=Big //$ means last element: mindegyik sor utolso eleme-->oszlop matrix

P_est = syslin('d',FT,cT,c1_est, d12_est);       // The plant (continuous-time)
[K_estd,X_estd] = lqr(P_est);
spec(FT+cT*K_estd);                                      // check stability


Gd = -K_estd';
Fd = F - Gd * c;
Hd = g;



/* sensor look up table ADC/degree */
//https://groups.google.com/g/comp.soft-sys.math.scilab/c/_hUrV8ncBUQ?pli=1
//->xy=[1 1;2 2;3 3];
//>save('ff',xy)
//Volt       mm   adc
//2,30 1,56  91   86   --> 1.0581
//2,91 4,28  205  159  --> 1.2893
//4,08 6,07  296  220  --> 1.3409

/* ---------- EOF bow ini ----------------------------------- */


/* here the UART rx-tx ini*/
warning('off')

POSITION='/home/imre/ballonwheel/bash_solution/tmp/bow_pos'
MOTOR='/home/imre/ballonwheel/bash_solution/tmp/bow_motor'
 
position=0; 
i=0;
j=0;
motor = 0;
motor_last = 0;

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

 //disp(position_str, "positon");
 //disp(isempty(position_str), "isempty");
 if(isempty(position_str)==%F) then
   position=strtod(position_str);
 else
   disp(j, i,"position_str false");
   i=i+1;   
 end
 

 /* here the bow control algorithm */
 motor = position;

 //disp(motor)
 //disp(motor_last)
 //disp("most")
 if(motor ~=  motor_last+1) then
   disp("************************************** error *************************************");
 end
 motor_last = motor;


 //disp(motor);
 /* eof bow control */

 /*create file of 'bow_motor', that will be sent by bash*/
 //disp(motor);
 motor_str=string(motor);
 //disp(motor_str, "motor");


 fd4=mopen(MOTOR,'w'); 
 mputl(motor_str, fd4);
 mclose(fd4);
  
 //sleep(20); no sleep, but need to be finished before next sent by arduino (10msec)  !!!!
 
end

mclose(fd2);


