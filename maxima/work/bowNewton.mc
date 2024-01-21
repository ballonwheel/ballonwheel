
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

