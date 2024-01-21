
/* **************************** */
/* MAXIMA SYMBOLIC CALCULATIONS */
/* **************************** */

/* ******************* */
/* Content			   */
/* ******************* */

/* Variable definition */
/* Newton modell */
/* Lagrange modell */
/* Parameter definition */
/* Showing both gives same result */
/* System equations */
/* Linearization around equilibrium */

/*
 i_end : length(A);
j_end : length(transpose(row(A,1)));
with_stdout("maxima_try.txt", 
 for i:1 thru i_end do
  for j:1 thru j_end do
   printf (true,"A=[~,4f,~,4f;~%~,4f,~,4f]~%",B[i,j],B[i,j],B[i,j]));
*/   
		

/* ******************* */
/* Variable definition */
/* *******************
************** comment start *************************************** 
g :  								/* gravity acceleration */

N : 								/* force of weight of ball */
mu : 								/* coefficient of friction between ball and wheel for clear rolling */
acp : 								/* centripetal acceleration of ball */
atg : 								/* tangential acceleration of ball */
s : 								/* force of friction between ball and wheel */

/* ball */
rbf : 								/* radius of ball (full ball, the whole one) */
edge : 
rb : 								/* radius of ball secant on edge of wheel */
mb : 	/* kg */					/* mass of ball */
thetab : 							/* inertia of ball */
fib : 								/* angular displacement of ball */
fi2 : 								/* angular of position of ball on wheel */
omegab : 							/* angular velocity of ball */
betab :  							/* angular acceleration of ball */
/* ball - kinetic matices */
vbc_m : 							/* an Euclid matrix(xyz) for velocity of mass center of ball */
vp_m :								/* an Euclid matrix(xyz) for velocity of point of contact of ball and wheel */
omegab_m : 							/* an Euclid matrix(xyz) for angular velocity of ball */
rb_m :								/* an Euclid matrix(xyz) for radius of ball of edge on wheel */

/* wheel */
rw : 								/* radius of wheel */
mwheel :  /* kg */					/* mass of wheel */
Town :  /* ms */					/* time of speedup w/o load*/
mload :  /* kg */ 					/* mass of load (tried to be a equally divided around the circle) */
Tload :  /* ms */					/* time of speedup w/ load */
thetaw : 							/* inertia of wheel */
fiw :								/* angular displacement of wheel */
omegaw :							/* angular velocity of wheel */
betaw :								/* angular acceleration of wheel */

/* motor */
Mm : 								/* moment of motor */
Ms : 								/* moment of viscous fraction */
********** comment end *******************************/



/* ******************* 
* Equations of motion by Newton 
*
* notes: some equations just defined for whole physical view but not used 
*
* ******************* */
printf(true, "XXXXXXXXXXXXXXXXXXXXXXX  Newton model XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
/* ball - translation movement eq */
/* kinematic eq (desc. of motions (no forces here)) */
acp : omega2^2*(rw+rb);
atg : beta2*(rw+rb);
/* force eq */
eq2 : mb * g * cos(fi2) - N = mb * acp; /* not used, only compulsion forses */
eq4 : mb * g * sin(fi2) + s = mb * atg; 

/* ball  - rotation eq */
eq5 : s * rb = thetab * betab;


/* wheel - translation movement eq */
/* there is no translation movement */

/* wheel - rotation movement eq */
Mm : km*(Ua-ku*omegaw)/Ra;
Ms : rv*omegaw;
eq6 : Mm - Ms - s * rw = thetaw * betaw;


/* kinetic conditions (causes of motions by forces) */
/* the ball must not slip on wheel; */
/* connection between translation and rotation movement of mass center of ball; */
/* vbc_m = vp_m + omegab_m * rb_m */
vbc_m : matrix([(rw + rb) * omega2], [0], [0]);
vp_m : matrix([rw * omegaw], [0], [0]);
omegab_m : matrix([0], [0], [omegaw]);
rb_m : matrix([0], [rb], [0]);
eq7_0 : vbc_m = vp_m + omegab_m * rb_m; /* to do */
/* (rw + rb) * omegaw = rw * omegaw + rb * omeagb */
eq7_1 : (rw + rb) * omega2 = rw * omegaw - rb * omegab;
eq7 : (rw + rb) * beta2 = rw * betaw - rb * betab;

/* kinetic condition also for clear rolling (not slipping) */
eq8 : s <= mu * N;


/* **** */
/* Solving equation system getting betaw & beta2 */
/* (betaw & beta2: the highest order of phasespace variables) */
/* **** */

/* try1 - linsolve, solve
sol1: solve([eq7,eq5],[betab,s]);
betab: sol1[1];
*/
/* try2 - funcsolve 
s: sol1[2];
sol3: funcsolve([eq4,eq6],[betaw,beta2]);
betaw: sol3[1];
beta2: sol3[2];
*/

/* try3 - algsys */
/* http://maxima.sourceforge.net/docs/manual/maxima_20.html#SEC104 */
algsys([eq4, eq5, eq6, eq7], [betab, s, betaw, beta2]);
%th(1)[1];
betab: %th(1)[1];
s: %th(2)[2];
betaw: %th(3)[3];
beta2: %th(4)[4];

betaw_manual: (Ra*thetab*rw*mb*g*sin(fi2)-(thetab+mb*rb^2)*(km*ku+rv*Ra)*omegaw+(thetab+mb*rb^2)*km*Ua)/(Ra*(thetab*thetaw+mb*rb^2*thetaw+mb*thetab*rw^2));
betaw_0 : betaw_manual - betaw;
expand(betaw_0); /* --> result 0, means manual and maxima have same result */

beta2_manual: (Ra*(thetaw*rb^2+thetab*rw^2)*mb*g*sin(fi2)-thetab*rw*(ku*km+Ra*rv)*omegaw+thetab*rw*km*Ua)/(Ra*(rw+rb)*(thetab*thetaw+mb*rb^2*thetaw+mb*thetab*rw^2));
beta2_0 : beta2_manual - beta2;
expand(beta2_0); /* --> result 0, means manual and maxima have same result */


/* ******************* */
/* Equations of motion by Lagrange*/
/* ******************* */
printf(true, "XXXXXXXXXXXXXXXXXXXXXXX  Lagrangian model XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");

Tw: 1/2*thetaw*omegaw^2;

algsys([eq7_1], [omegab]);
%th(1)[1];
omegab: %th(1)[1];
vb: (rw+rb)*omega2;
Tb: 1/2*thetab*omegab^2 + 1/2*mb*vb^2;

Uw: 0;
Ub: mb*g*(rw+rb)*cos(fi2);

L: (Tw+Tb)-(Uw+Ub);
L_manual: 1/2*thetaw*omegaw^2+1/2*thetab*1/(rb^2)*(rw*omegaw-(rw+rb)*omega2)^2+1/2*mb*(rw+rb)^2*omega2^2-mb*g*(rw+rb)*cos(fi2);
L_0: L_manual - L;
expand(L_0); /* --> result 0, means manual and maxima have same result */


fiw : fiw(t);							/* angular displacement of wheel */
fi2 : fi2(t);							/* angular displacement of position of ball on wheel */
fib : fib(t);							/* angular displacement of ball */
omegaw : diff(fiw,t);					/* angular velocity of wheel */
omega2 : diff(fi2,t);					/* angular velocity of position of ball on wheel */
omegab : diff(fib,t);					/* angular velocity of ball */
betab : diff(diff(fib,t),t); 			/* angular acceleration of ball */
beta2 : diff(diff(fi2,t),t); 			/* angular acceleration of position of ball on wheel */
betaw : diff(diff(fiw,t),t);			/* angular acceleration of wheel */

/* expand(L); not work, why? */
/* expandwrt([L], [omegaw, omeagb, omega2]); not work, why? */
LX:L;

printf(true, "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXx");

L: 1/2*thetaw*omegaw^2+1/2*thetab*1/(rb^2)*(rw*omegaw-(rw+rb)*omega2)^2+1/2*mb*(rw+rb)^2*omega2^2-mb*g*(rw+rb)*cos(fi2);

eqLw_a: expand(diff(diff(L,omegaw),t));
eqLw_b: diff(L,fiw);
eqLw: eqLw_a + eqLw_b;

eqL2_a: expand(diff(diff(L,omega2),t));
eqL2_b: diff(L,fi2);
eqL2: eqL2_a + eqL2_b;

printf(true, "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXx");

/* expandwrt(beta2, omega2); */

/* ******************* */
/* Variable definition */
/* ******************* */
g : 9.81; 								/* gravity acceleration */

N : N;									/* force of weight of ball */
mu : mu;								/* coefficient of friction between ball and wheel for clear rolling */
acp : acp;								/* centripetal acceleration of ball */
atg : atg;								/* tangential acceleration of ball */
s : s;									/* force of friction between ball and wheel */

/* ball */
rbf : 0.03;								/* radius of ball (full ball, the whole one) */
edge : 0.03;
rb : expand(sqrt(rbf^2-(edge/2)^2));	/* radius of ball secant on edge of wheel */
mb : 0.203;	/* kg */					/* mass of ball */
thetab : expand((2/5) * mb * rbf^2);	/* inertia of ball */
fib : fib(t);							/* angular displacement of ball */
fi2 : fi2(t);							/* angular of position of ball on wheel */
omegab : diff(fib,t);					/* angular velocity of ball */
betab : diff(diff(fib,t),t); 			/* angular acceleration of ball */
/* ball - kinetic matices */
vbc_m : vbc_m;							/* an Euclid matrix(xyz) for velocity of mass center of ball */
vp_m : vp_m;							/* an Euclid matrix(xyz) for velocity of point of contact of ball and wheel */
omegab_m : omegab_m;					/* an Euclid matrix(xyz) for angular velocity of ball */
rb_m : rb_m;							/* an Euclid matrix(xyz) for radius of ball of edge on wheel */

/* wheel */
rw : 0.27;								/* radius of wheel */
mwheel : 6.6; /* kg */					/* mass of wheel */
Town : 87.5; /* ms */					/* time of speedup w/o load*/
mload : 1.08; /* kg */ 					/* mass of load (tried to be a equally divided around the circle) */
Tload : 110; /* ms */					/* time of speedup w/ load */
thetaw : Town * ((mload * rw^2) / (Tload - Town);	/* inertia of wheel */
fiw : fiw(t);							/* angular displacement of wheel */
omegaw : diff(fiw,t);					/* angular velocity of wheel */
betaw : diff(diff(fiw,t),t);			/* angular acceleration of wheel */

/* motor */
Mm : Mm;







/* ******************* */
/* LQRegulator developing
/* ******************* */
rpm : 100;
omegaw_max : 2 * %pi * rpm / 60;
q1 : expand(1 / omegaw_max^2);

omega2_max : sqrt( 10/7*g/(rb+rw)*(1-cos(2*%pi*40/360)) );
q2 : expand(1 / omega2_max^2);

fi2_max_deg : 5;
fi2_max : fi2_max_deg * 2 * %pi / 360;
q3 : expand(1 / fi2_max^2);

/* Uk : 24; --> nagyobb sulyt szeretnenk a szabalyozo bementeti parameternek --> kisebb Uk */
Uk : 2;  
u1 : expand(1 / Uk^2);

disp(float(q1), float(q2), float(q3), float(u1));



/* ******************* */
/* tmp copy
/* ******************* */

t : t;
g : 9.81; 								/* gravity acceleration */

N : N;									/* force of weight of ball */
mu : mu;								/* coefficient of friction between ball and wheel for clear rolling */
acp : acp;								/* centripetal acceleration of ball */
atg : atg;								/* tangential acceleration of ball */
s : s;									/* force of friction between ball and wheel */

/* ball */
rbf : 0.03;								/* radius of ball (full ball, the whole one) */
edge : 0.03;
rb : expand(sqrt(rbf^2-(edge/2)^2));	/* radius of ball secant on edge of wheel */
mb : 0.203;	/* kg */					/* mass of ball */
thetab : expand((2/5) * mb * rbf^2);	/* inertia of ball */
fib : fib(t);							/* angular displacement of ball */
fi2 : fi2(t);							/* angular of position of ball on wheel */
omegab : diff(fib,t);					/* angular velocity of ball */
betab : diff(diff(fib,t),t); 			/* angular acceleration of ball */
beta2 : diff(diff(fi2,t),t); 			/* angular acceleration of position of ball on wheel */

/* ball - kinetic matices */
vbc_m : vbc_m;							/* an Euclid matrix(xyz) for velocity of mass center of ball */
vp_m : vp_m;							/* an Euclid matrix(xyz) for velocity of point of contact of ball and wheel */
omegab_m : omegab_m;					/* an Euclid matrix(xyz) for angular velocity of ball */
rb_m : rb_m;							/* an Euclid matrix(xyz) for radius of ball of edge on wheel */

/* wheel */
rw : 0.27;								/* radius of wheel */
mwheel : 6.6; /* kg */					/* mass of wheel */
Town : 87.5; /* ms */					/* time of speedup w/o load*/
mload : 1.08; /* kg */ 					/* mass of load (tried to be a equally divided around the circle) */
Tload : 110; /* ms */					/* time of speedup w/ load */
thetaw : Town * ((mload * rw^2) / (Tload - Town));	/* inertia of wheel */
fiw : fiw(t);							/* angular displacement of wheel */
omegaw : diff(fiw,t);					/* angular velocity of wheel */
betaw : diff(diff(fiw,t),t);			/* angular acceleration of wheel */

/* motor */
Mm : Mm;





/* ******************* */
/* tmp copy2
/* ******************* */

atg : beta2*(rw+rb);
eq4 : mb * g * sin(fi2) + s = mb * atg; 
eq5 : s * rb = thetab * betab;
Mm : km*(Ua-ku*omegaw)/Ra;
Ms : rv*omegaw;
eq6 : Mm - Ms - s * rw = thetaw * betaw;
eq7 : (rw + rb) * beta2 = rw * betaw - rb * betab;

/*
sol1: funcsolve([eq7,eq5],[betab,s]);
betab: sol1[1];
s: sol1[2];
sol3: funcsolve([eq4,eq6],[betaw,beta2]);
betaw: sol3[1];
beta2: sol3[2];
*/
/* http://maxima.sourceforge.net/docs/manual/maxima_20.html#SEC104 */
algsys([eq4, eq5, eq6, eq7], [betab, s, betaw, beta2]);
%th(1)[1];
betab: %th(1)[1];
s: %th(2)[2];
betaw: %th(3)[3];
beta2: %th(4)[4];

betaw_check: (Ra*thetab*rw*mb*g*sin(fi2)-(thetab+mb*rb^2)*(km*ku+rv*Ra)*omegaw+(thetab+mb*rb^2)*km*Ua)/(Ra*(thetab*thetaw+mb*rb^2*thetaw+mb*thetab*rw^2));
betaw_0 : betaw_check - betaw;
expand(betaw_0);

/* ******************* */
/* Equations of motion by Lagrange*/
/* ******************* */
theta1: theta1(t);
theta2: theta2(t);
omega1: diff(theta1,t);
omega2: diff(theta2,t);

T1: 1/2*J1*omega1^2;
U1: 0;
T2: 1/2*J2*omega2^2 + 1/2*M2*(l1*omega1)^2 + 1/2*M2*l1*l2*omega1*omega2*cos(theta2);
U2: M2*g*l2/2*cos(theta2);
L: (T1+T2) -(U1+U2);











/* ******************* */
/* LQRegulator developing
/* ******************* */
rpm : 100;
omegaw_max : 2 * %pi * rpm / 60;
q1 : expand(1 / omegaw_max^2);

omega2_max : sqrt( 10/7*g/(rb+rw)*(1-cos(2*%pi*40/360)) );
q2 : expand(1 / omega2_max^2);

fi2_max_deg : 5;
fi2_max : fi2_max_deg * 2 * %pi / 360;
q3 : expand(1 / fi2_max^2);

/* Uk : 24; --> nagyobb sulyt szeretnenk a szabalyozo bementeti parameternek --> kisebb Uk */
Uk : 2;  
u1 : expand(1 / Uk^2);

disp(float(q1), float(q2), float(q3), float(u1));










/* ******************* */
/* ******************* */
/* ******************* */
/* ******************* */
/* ******************* */
/* ******************* */





/* ******************* */
/* Equations of motion */
/* ******************* */
theta1: theta1(t);
theta2: theta2(t);
omega1: diff(theta1,t);
omega2: diff(theta2,t);

T1: 1/2*J1*omega1^2;
U1: 0;
T2: 1/2*J2*omega2^2 + 1/2*M2*(l1*omega1)^2 + 1/2*M2*l1*l2*omega1*omega2*cos(theta2);
U2: M2*g*l2/2*cos(theta2);
L: (T1+T2) -(U1+U2);

eq1_a: expand(diff(diff(L,omega1),t));
eq1_b: diff(L,theta1);
eq1: eq1_a + eq1_b;

eq2_a: diff(diff(L,omega2),t);
eq2_b: expand(diff(L,theta2));
eq2: eq2_a + eq2_b;




/* ***************************** */
/* Non-linear system formulation */
/* ***************************** */
eqnl1 : a*theta1_pp + b*theta2_pp*cos(theta2)  - b*theta2_p^2*sin(theta2) = e*V - f*theta1_p;
eqnl2 : c*theta2_pp + b*theta1_pp*cos(theta2) - d*sin(theta2) = 0; 

sol: linsolve([eqnl1,eqnl2],[theta1_pp,theta2_pp]);

display(sol[1]);
display(sol[2]);


/* ********************************************************************************** */
/* Maxima script for obtaining the state representation of an inverted pendulum in tau*/
/* ********************************************************************************** */

/* System */
eq1 : a*theta1_pp + b*theta2_pp = tau;
eq2 : c*theta2_pp + b*theta1_pp - d*theta2 = 0; 

sol: linsolve([eq1,eq2],[theta1_pp,theta2_pp]);

/* Theta1_pp processing */
res1 : expand(rhs(sol[1]));
eq1_theta2 : factor(part(res1,1));
eq1_tau : factor(part(res1,2));

/* Theta2_pp processing */
res2 : expand(rhs(sol[2]));
eq2_theta2 : factor(part(res2,2));
eq2_tau : factor(part(res2,1));


/* ************************************************************************** */
/* Maxima script for obtain state representation of an inverted pendulum in V */
/* ************************************************************************** */
e : N*Kt/R;
f : N^2*Kt^2/R;

/* System */
eq1 : a*theta1_pp + b*theta2_pp = e*V - f*theta1_p;
eq2 : c*theta2_pp + b*theta1_pp - d*theta2 = 0; 

sol: linsolve([eq1,eq2],[theta1_pp,theta2_pp]);

/* Theta1_pp processing */
res1 : expand(rhs(sol[1]));
eq1_theta2 : factor(part(res1,2));
eq1_theta1_p : factor(part(res1,3));
eq1_V : factor(part(res1,1));

/* Theta2_pp processing */
res2 : expand(rhs(sol[2]));
eq2_theta2 : factor(part(res2,2));
eq2_theta1_p : factor(part(res2,3));
eq2_V : factor(part(res2,1));


/* ************************************ */
/* Maxima script for transfer functions */
/* ************************************ */
eq1 : a*s^2*theta1_s + b*s^2*theta2_s = tau_s;
eq2 : b*s^2*theta1_s + c*s^2*theta2_s - d*theta2_s = 0;

sol: linsolve([eq1,eq2],[theta1_s,theta2_s]);

W1_s : rhs(sol[1])/tau_s;
W2_s : rhs(sol[2])/tau_s;


/* ********************* */
/* Controllability check */
/* ********************* */
/*
a0 : Kphi*N;
a1 : b*d/delta;
a2 : a*d/delta;
a5 : c/delta*Kphi*N/R
a3 : a5 * a0;
a6 : b/delta*Kphi*N/R;
a4 : a6 * a0;
*/

A : matrix([0, 0, 1, 0], [0, 0, 0, 1], [0, -a1, -a5*a0, 0], [0, a2, a6*a0, 0]);
B : matrix([0], [0], [a5], [-a6]); 
R : addcol(B,A.B,A.(A.B),A.(A.(A.B)));
detR : factor(expand(determinant(R)));



