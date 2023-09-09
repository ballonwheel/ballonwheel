
# not a function file:
1;


function [Q, M, rk] = fullrf(A)
    #//[Q,M,rk]=fullrf(A)
    #//Full rank factorization : A=Q.M
    #//with range(Q)=range(A) and ker(M)=ker(A),
    #//Q full column rank , M full row rank
    #// rk = rank(A) = #columns(Q) = #rows(M)
    #//F.D.
    #//!
    [U,s,V]=svd(A);
    rk=rank(A);
    sq=sqrt(s);
    Q=U*sq;M=sq*V';
    Q=Q(:,1:rk);M=M(1:rk,:);
endfunction



#https://wiki.octave.org/Control_package
#https://wiki.octave.org/Category:Octave_Forge
#https://gnu-octave.github.io/packages/

clear
pkg load control
pkg load signal

#/*here the bow initialization */
#    //global g;
    g=9.81;
#    //disp(g,"g=");

#    //global Ra;
    Ra=0.3;
#    //disp(Ra, "Ra=")

#    //global La;
    La=18e-3;
#    //disp(La, "La=")

#    //global km;
    km=2.518;
#    //disp(km, "km=")

#    global ku;
    ku=2.093;
#    //disp(ku, "ku=")

#    global rw;
    rw=0.27;
#    //disp(rw, "rw=")

    mwheel = 6.6; #//mass of wheel [kg]
    Town = 87.5; #///* ms */                                     /* time of speedup w/o load*/
    mload = 1.08; #///* kg */                                    /* mass of load (tried to be a equally divided around the circle) */
    Tload = 110; #///* ms */                                     /* time of speedup w/ load */
#    global Iw;
    Iw = Town * ((mload * rw^2) / (Tload - Town));     # /* inertia of wheel */
#    //disp(Iw,"Iw=")


    Tm=(Iw*3*Ra)/(km*ku);

    Te=(La)/(3*Ra);

#    global rv;
    rv=2.3e-3;
#    //disp(rv, "rv=")


#    global mb;
    mb=0.203;
#    //disp(mb, "mb=")


#//radius of ball as a sphere
rbf=30e-3;
#//disp(rbf, "rbf=")

#radius of circle of ball ON EDGE
edge=30e-3;
rb=sqrt(rbf^2-(edge/2)^2);
#//disp(rb, "rb=")

Ib=(2/5) * mb * rbf^2;

vdc=24;#[V]
drive_cwmax=255;#+24V
drive_zero=127;#0V
drive_ccwmax=0;#-24V
posx=[24;40;64;100;130;160;190;210;245];#[ADC8bit]
posx_min=24;#[ADC8bit]
posx_max=245;#[ADC8bit]
posy=[-17;-15;-10;-5;0;5;10;15;17];#[fok]






#**************************************************************
#PI controller design
#**************************************************************
#//the motor a PIT1 system
#//s=poly(0,'s');
#//Big_gamma = km/(ku*km+Ra*rv);
#//A_motor=(1/(Ra+L*s)*km*1/(Iw*s));
#//B_motor=ku;
#//W_motor=syslin('c', (A_motor)/(1+A_motor*B_motor));
#//bode(W_motor);
#//clf;nyquist(W_motor);

#//s=poly(0,'s')
#//G=syslin('c', (1/km)/(Tm*Te*s^2+Tm*s+1))
#//show_margins(G)
#//bode(G)
#//K_PID_DCmotorLoad=0.6351
#//Ti_PID_DCmotorLoad=0.01

#**************************************************************
#BoW system - space state equations
#**************************************************************
#the system below calculated with Maxima --> file: Bow.mc
nev=(Ib*Iw+mb*rb^2*Iw+mb*rw^2*Ib);
alfaw=(Ib*rw*mb*g)/nev;
betaw=(Ib+mb*rb^2)*(ku*km+rv*Ra)/(Ra*nev);
gammaw=km*(Ib+mb*rb^2)/(Ra*nev);
alfa2=(Iw*rb^2+Ib*rw^2)/((rw+rb)*nev);
beta2=(Ib*rw*(ku*km+rv*Ra))/(Ra*(rw+rb)*nev);
gamma2=(Ib*rw*km)/(Ra*(rw+rb)*nev);

disp("------- continous --------");

A=[-betaw, 0, alfaw;
-beta2, 0, alfa2;
0, 1, 0];

B=[gammaw; gamma2; 0];

C = [0, 0, 1;];

D = [0;];
disp("A")
disp(A);
disp("B")
disp(B);
disp("C")
disp(C);
disp("D")
disp(D);


#// Check contollability
AB = [B, A*B, A*A*B;];
disp("AB")
disp(AB);

rAB=rank(AB);
disp("rank AB")
disp(rAB);

eAB=eig(AB);#spec(AB);
disp("eigen AB")
disp(eAB);

BoW_ss = ss(A,B,C,D);
disp("BoW_ss");
disp(BoW_ss);
BoW_tf = ss2tf(BoW_ss);
disp("BoW_tf");
disp(BoW_tf);
#//disp(BoW_tf_manual); //checking both tf equal
#//scf(1); clf(1);
#//bode(BoW_tf)
#//scf(2); clf(2);
#//nyquist(BoW_tf);
#//scf(3); clf(3);
#//plzr(BoW_tf);
#//zpk(BoW_tf);

disp("------- discrete --------");
Ts=0.02;
disp("Ts");
disp(Ts);
BoW_ssd=c2d(BoW_ss, Ts);
F=BoW_ssd.a;
g=BoW_ssd.b;
c=BoW_ssd.c;
d=BoW_ssd.d;
disp("Bow_ssd");
display(BoW_ssd);
disp("F");
disp(F);
disp("g");
disp(g);
disp("c");
disp(c);
disp("d");
disp(d);


#/*
#F=ppol(A,B,[-1,-1,-1]);
#spec(A-B*F);
#BoW_ssF = syslin('c', A-B*F,B,C);
#BoW_tfF = ss2tf(BoW_ssF);
#scf(11); clf(11);
#bode(BoW_tfF);
#scf(12); clf(12);
#nyquist(BoW_tfF)
#scf(13); clf(13);
#plzr(BoW_tfF);
#zpk(BoW_tfF);
#*/


disp("------- LQ reglator --------");

#**************************************************************
# LQ project --> K
#**************************************************************
#the system below calculated with Maxima --> file: Bow.mc

#//omegaw suly
q1 = 0.009118906527810399;
#//q1 = 1

#//omega2 suly
q2 = 0.0914991210900477;
#//q2 = 1

#//fi2 suly
q3 = 131.3122540004697;
#//q3 = 1

#//Uk suly
R = 0.25;
#//R = 1

Q = diag([q1, q2,q3]);
Big = blkdiag(Q,R);             #  //create big common Q R matrix
#// Now we calculate C1 and D12
nstates = 3;
#// full rank factirazition : egy matrixot tobb matrix szorzatara bontok: a tenyezok rangja azonos legyen az eredeti matrix rangjaval
# https://www.mathworks.com/matlabcentral/fileexchange/19197-full-rank-factorization
[w,wp] = fullrf(Big);
C1=wp(:,1:nstates); #// kivalasztjuk az osszes sort es az 1->nstates oszlopokat
#D12=wp(:,$:$);   #//[C1,D12]'*[C1,D12]=Big //$ means last element: mindegyik sor utolso eleme-->oszlop matrix
D12=wp(:,end);   #//[C1,D12]'*[C1,D12]=Big //$ means last element: mindegyik sor utolso eleme-->oszlop matrix

disp("A");
disp(A);
disp("B");
disp(B);
disp("C1");
disp(C1);
disp("D12");
disp(D12);

#P = syslin('c',A,B,C1,D12);   #   // The plant (continuous-time)
P = ss(A,B,C1,D12);   #   // The plant (continuous-time)
disp("P");
display(P);
[K,X] = lqr(P, Q, R);
disp("K");
disp(K);
disp("X");
disp(X);

#spec(A+B*K);                   #                  // check stability
eABK=eig(A+B*K);                   #                  // check stability
disp("eABK");
disp(eABK);

#Pd = syslin('d',F,g,C1,D12);    # // The plant (continuous-time)
Pd = ss(F,g,C1,D12, Ts);    # // The plant (continuous-time)
disp("Pd");
display(Pd);
[Kd,Xd] = lqr(Pd, Q, R);
Kd=-Kd;
disp("Kd");
disp(Kd);
disp("Xd");
disp(Xd);
eABKd=eig(F+g*Kd);                    #                        // check stability
disp("eABKd");
disp(eABKd);


disp("---------- estimator ---------");
#**************************************************************
# estimator discrete
#**************************************************************
#same LQ metode and q used

FT = F';
disp("FT");
disp(FT);

cT = c';
disp("cT");
disp(cT);

FTcT = [cT, FT * cT, FT *FT * cT];
disp("FTcT");
disp(FTcT);

rFTcT=rank(FTcT);
disp("rFTcT");
disp(rFTcT);

eFTcT=eig(FTcT);
disp("eFTcT");
disp(eFTcT);


#// LQ project for estimator
#//Q = diag([131.3122540004697,2.05175396875734,0.009118906527810399])           // comes from maxima
#//R = 0.25

#Big_est = sysdiag(Q,R);           #       //create big common Q R matrix
Big_est = blkdiag(Q,R);           #       //create big common Q R matrix
#// Now we calculate C1 and D12
#//nstates = 3;
#// full rank factirazition : egy matrixot tobb matrix szorzatara bontok: a tenyezok rangja azonos legyen az eredeti matrix rangjaval
[w_est, wp_est] = fullrf(Big_est);
c1_est = wp_est(:,1:nstates); #// kivalasztjuk az osszes sort es az 1->nstates oszlopokat
d12_est = wp_est(:,end);   #//[C1,D12]'*[C1,D12]=Big //$ means last element: mindegyik sor utolso eleme-->oszlop matrix

disp("FT");
disp(FT);
disp("cT");
disp(cT);
disp("c1_est");
disp(c1_est);
disp("d12_est");
disp(d12_est);
P_est = ss(FT,cT,c1_est, d12_est, Ts);     #  // The plant (continuous-time)
display(P_est);
[K_estd,X_estd] = lqr(P_est, Q, R);
K_estd=-K_estd;
disp("K_estd");
disp(K_estd);
disp("X_estd");
disp(X_estd);

eFTcTKestd=eig(FT+cT*K_estd);                             #         // check stability
disp("eFTcTKestd");
disp(eFTcTKestd);

disp("---- estiamtor in feedback ----");
Gd = -K_estd';
Fd = F - Gd * c;
Hd = g;
disp("Gd");
disp(Gd);
disp("Fd");
disp(Fd);
disp("Hd");
disp(Hd);



#/* sensor look up table ADC/degree */
#//https://groups.google.com/g/comp.soft-sys.math.scilab/c/_hUrV8ncBUQ?pli=1
#//->xy=[1 1;2 2;3 3];
#//>save('ff',xy)
#//Volt       mm   adc
#//2,30 1,56  91   86   --> 1.0581
#//2,91 4,28  205  159  --> 1.2893
#//4,08 6,07  296  220  --> 1.3409




x = [0;#ww
  0;#w2
  0];#fi2
u = 0;
sum_zn1=[0;0;0];


#/* ---------- EOF bow ini ----------------------------------- */



simulation=0;

if(simulation==1)
 x(3)=0.4;
 i=0;
 while i<500
   i=i+1;
   disp(i)

   #controller
   #u=Kd*x;

   #controller+estimator
   sum_zn1=Fd*sum_zn1+Gd*x(3)+Hd*u;
   u=Kd*sum_zn1;
   if(u>24)
       u=24;
   end
   if(u<-24)
       u=-24;
   end
   disp("u")
   disp(u)


   xp=F*x+g*u;
   x=xp;
   disp("x")
   disp(x)
   #y=C*x+D*u;
   #disp("y")
   #disp(y)

  meas.values(i,1)=x(3);
  meas.values(i,2)=u;
  meas.time(i,1)=(i-1)*0.02;
 endwhile

 subplot(2,1,1)
 plot(meas.time, meas.values(1:end,1),'b');
 hold on;
 subplot(2,1,2)
 plot(meas.time, meas.values(1:end,2),'b');
 #waitfor(10000)
 pause(20)#xwindow copy to document
 #measAll=[meas.time meas.values];
 #csvwrite('./meas4_values.csv',meas.values)
 #csvwrite('./meas4_time.csv',meas.time)
 #csvwrite('./mAll.csv',measAll)
endif



#--------------------------------------------------------------------------------------------------#


#setup
vdc=24;#[V]
drive_cwmax=255;#+24V
drive_zero=127;#0V
drive_ccwmax=0;#-24V
posx=[24;40;64;100;130;160;190;210;245];#[mm]
posx_min=24;#[mm]
posx_max=245;#[mm]
posy=[-17;-15;-10;-5;0;5;10;15;17];#[fok]


realsetup=1;

if(realsetup==1)

 fdin=-1;
 position=1;
 motor=1;
 motor_last=1;
 positionStr="000\n";
 i=0;
 j=0;
 u = 0;
 sum_zn1=[0;0;0];
 test = 0;#set for debug

 while(1) #0-----------------------------------------------------0

  #get ADC
  positionStr="255";
  #two=2;#getting twice
  #while(two) #--------------------------------------------------1
   while(fdin < 0) #2------------------------------------------->2
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
   endwhile #<-----------------------------------------------------2
   fdin=-1;

   #disp("position");
   #disp(position);
   if(test==1)
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
     #two=0;
    else
     #two=two-1;
     error=0;
    endif
    #motor = position+1;
   endif
  #endwhile #-----------------------------------------------------1




  #control loop
  if(test==1)
   pause(0.0008);
  else
   #position in [ADC8bit] --> x(3)[radian]
   if(position > 245)
     position = 245;
   endif
   if(position < 24)
     position = 24;
   endif
   x(3) = 2 * pi * ((interp1(posx,posy,position)) / 360);

   #controller+estimator
   sum_zn1=Fd*sum_zn1+Gd*x(3)+Hd*u;
   u=Kd*sum_zn1;
   if(u>vdc)
       u=vdc;
   end
   if(u<-vdc)
       u=-vdc;
   end
  endif


  #scale for drive
  #if(error==0)
   if(test == 1)
     motor=motor+1;
   else
     motor = (vdc + u) * drive_zero/vdc;
   endif
  endif

   #safe
   if((motor>250) || (motor < 1))
    motor = 1;
   endif
   #disp("motor: ");
   #disp(motor);

   #send back
   motorStr=sprintf("%03ui\n",motor);

   fdout=fopen("./tmp/bow_motor", "w+");
   fputs(fdout,motorStr);
   fclose(fdout);
   #fflush(fdout);
  #endif

  i=i+1;
  #disp(i);
  #disp("--loop end--");

 endwhile #-------------------------------------------------------0

endif




