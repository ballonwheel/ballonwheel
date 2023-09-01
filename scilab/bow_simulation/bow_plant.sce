

clear

g=9.81;
Ra=0.3;
La=18e-3;
km=2.518;
ku=2.093;
rw=0.27;
mwheel = 6.6; //mass of wheel [kg]
Town = 87.5; ///* ms */                                 	/* time of speedup w/o load*/
mload = 1.08; ///* kg */                                	/* mass of load (tried to be a equally divided around the circle) */
Tload = 110; ///* ms */                                 	/* time of speedup w/ load */
Iw = Town * ((mload * rw^2) / (Tload - Town));  	/* inertia of wheel */
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

bow_ss = syslin('c', A, B, C, D);
disp(bow_ss)

/*--------------*/
//allapot valtozok
//x1: w_wheel
//x2: w_golyo
//x3: fi_golyo
x=[0;0;0];
//x potty --> x derivalt ido szerint
xp=A*x+B*Uk;
Xpos=C*x+D*Uk;


//while(10 sec / 20msec)


//endwhile








