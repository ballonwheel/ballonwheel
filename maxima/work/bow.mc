
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
Ra :
La : 
ia :
iav : 
Ua :
ku :
km :
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
eq6a : Ua = Ra*ia + La*iav + ku*omegaw;
Mm : km*ia;
Ms : rv*omegaw;
eq6b : Mm - Ms - s * rw = thetaw * betaw;



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
algsys([eq4, eq5, eq6a, eq6b, eq7], [betab, s, betaw, beta2, iav]);
%th(1)[1];
betab: %th(1)[1];
s: %th(2)[2];
betaw: %th(3)[3];
beta2: %th(4)[4];
iav:  %th(5)[5];


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






