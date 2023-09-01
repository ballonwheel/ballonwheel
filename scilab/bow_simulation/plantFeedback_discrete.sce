clear
g=9.81;
Ra=0.3;
La=18e-3;
km=2.518;
ku=2.093;
rw=0.27;
mwheel = 6.6; //mass of wheel [kg]
Town = 87.5; ///* ms */                                     /* time of speedup w/o load*/
mload = 1.08; ///* kg */                                    /* mass of load (tried to be a equally divided around the circle) */
Tload = 110; ///* ms */                                     /* time of speedup w/ load */
Iw = Town * ((mload * rw^2) / (Tload - Town));      /* inertia of wheel */
Tm=(Iw*3*Ra)/(km*ku);
Te=(La)/(3*Ra);
rv=2.3e-3;
mb=0.203;
rbf=30e-3;
edge=30e-3;
rb=sqrt(rbf^2-(edge/2)^2);
Ib=(2/5) * mb * rbf^2;


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
disp("BoW_ss")
disp(BoW_ss)



BoW_ssd=dscr(BoW_ss, 0.02);
F=BoW_ssd(2);
g=BoW_ssd(3);
c=BoW_ssd(4);
d=BoW_ssd(5);
disp("BoW_ssd")
disp(BoW_ssd)


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
disp("P")
disp(P)
[K,X] = lqr(P);
disp(K)
disp(X)
spec(A+B*K);                                     // check stability

Pd = syslin('d',F,g,C1,D12);     // The plant (continuous-time)
disp("Pd")
disp(Pd)
[Kd,X] = lqr(Pd);
disp(Kd)
disp(X)
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

disp("---------------")

x = [0;//ww
  0;//w2
  0.4];//fi2
u = 0;
sum_zn1=[0;0;0];
i=0;
while i<500
 i=i+1;
 if((x(3)<%pi/2) && (x(3)>-%pi/2))then //vegallas
   disp(i)
   
   //controller
   //u=Kd*x;
   
   //estimator
   sum_zn1=Fd*sum_zn1+Gd*x(3)+Hd*u;
   u=Kd*sum_zn1;
   if(u>24)then
       u=24;
   end  
   if(u<-24)then
       u=-24;
   end  
   disp("u")
   disp(u)
   meas.values(i,1)=u;
   
   //plant
   xp=F*x+g*u;
   x=xp;
   disp("x")
   disp(x)
   meas.values(i,2)=x(3);
   
   meas.time(i)=(i-1)*0.02;
   
   
   
   //y=C*x+D*u;
   //disp("y") 
   //disp(y) 
 end     
end

subplot(2,1,1)
plot(meas.time, meas.values(:,1))
subplot(2,1,2)
plot(meas.time, meas.values(:,2))

csvWrite(meas.values, '/home/imre/ballonwheel/scilab/bow_simulation/meas2_values.csv')


csvWrite(meas.time, '/home/imre/ballonwheel/scilab/bow_simulation/meas2_time.csv')



