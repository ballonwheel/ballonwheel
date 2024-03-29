

/* maxima install */
/* $ uname -a */
/* Linux raspberrypi 5.15.36-rt41-v8+ #1 SMP PREEMPT Fri May 6 21:32:06 CEST 2022 aarch64 GNU/Linux */
/* sudo apt-get install maxima */
/* $ maxima  */
/* (%i2) version: ?\*autoconf\-version\*; */
/* (%o2)                               5.44.0 */


/* igy futtasd */
/*parancssorbol: */
/* $ maxima -batch=bowSymbol.mc */

/* maxima inditasa utan */ 
/* $ batch("bowSymbol.mc"); */


/* maxima doc */
/* https://maxima.sourceforge.io/docs/manual/maxima_424.html#Category_003a-Declarations-and-inferences */
/* https://maxima.sourceforge.io/docs/manual/maxima_toc.html#SEC_Contents */

print("hello world");

/* debugmode(true); */

/* Newton */
acp : omega2^2*(rw+rb);
atg : beta2*(rw+rb);
eq2 : mb * g * cos(fi2) - N = mb * acp; /* not used, only compulsion forses */
eq4 : mb * g * sin(fi2) + s = mb * atg;
eq5 : s * rb = thetab * betab;

Mm : km*(Ua-ku*omegaw)/Ra;
Ms : rv*omegaw;
eq6 : Mm - Ms - s * rw = thetaw * betaw;

vbc_m : [((rw + rb) * omega2), 0, 0]$
vp_m : [(rw * omegaw), 0, 0]$
omegab_m : [0, 0, omegab]$
rb_m : [0, rb, 0]$
load("vect")$
eq7_0 : vbc_m = vp_m + (omegab_m ~ rb_m); 
eq7_1:  express(%);
vbc : vbc_m[1]$
vbc;
/* ezt alabb megcsinalja, ha  fiw : fiw(t);  definialva van, de utana az algsys nem kezeli....
eq7_10 :  diff(vbc_m, t) = diff((vp_m + (omegab_m ~ rb_m), t));
express(%);
*/
eq7_11 :  (rw + rb) * omega2 = rw * omegaw - rb * omegab;
/* ide kene automatan eljutni, mert beta2 kell */
eq7 : (rw + rb) * beta2 = rw * betaw - rb * betab;

eq8 : s <= mu * N;

algsys([eq4, eq5, eq6, eq7], [betab, s, betaw, beta2]);
%th(1)[1];
betab_newton: %th(1)[1];
s: %th(2)[2];
betaw_newton: %th(3)[3];
beta2_newton: %th(4)[4];

/* manual ellenorzes */
print(" ");
print(" ");
print(" ");
print(" ");
print(" ");
betaw_check: (Ra*thetab*rw*mb*g*sin(fi2)-(thetab+mb*rb^2)*(km*ku+rv*Ra)*omegaw+(thetab+mb*rb^2)*km*Ua)/(Ra*(thetab*thetaw+mb*rb^2*thetaw+mb*thetab*rw^2));
betaw_0 : betaw_check - betaw_newton;
print("************ betaw ellenorezes ***************************");
expand(betaw_0);
print("************    vege    ***************************");
print(" ");
print(" ");
print(" ");
print(" ");
print(" ");
print(" ");

/* nem tudom egyszeru resztortekre bontani, de vmit csinal */
expand(betaw_newton);

print(" ");
print(" ");
print(" ");
print(" ");
print(" ");
print(" ");








/*  ------------------------------------- */
print("----------Lagrange -----------------");
/* Lagrange */

Tw: 1/2*thetaw*omegaw^2;

algsys([eq7_11], [omegab]);
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


print("------ Lagrange derivative --------");


fiw : fiw(t);                           /* angular displacement of wheel */
fi2 : fi2(t);                           /* angular displacement of position of ball on wheel */
fib : fib(t);                           /* angular displacement of ball */
omegaw : diff(fiw,t);                   /* angular velocity of wheel */
omega2 : diff(fi2,t);                   /* angular velocity of position of ball on wheel */
omegab : diff(fib,t);                   /* angular velocity of ball */

betab : diff(diff(fib,t),t);            /* angular acceleration of ball */
beta2 : diff(diff(fi2,t),t);            /* angular acceleration of position of ball on wheel */
betaw : diff(diff(fiw,t),t);            /* angular acceleration of wheel */


/* az alabbi egyenletbol kell az omegab */
eq7_diff : (rw + rb) * omega2 = rw * omegaw - rb * omegab;
algsys([eq7_diff], [omegab]);
%th(1)[1];
/* aazt gondolnam, h omegab-t kitalalja, h fent definialtam es egyenlove teszi, de nem...
ezert ujra def 
*/
omegab_diff: %th(1)[1];

Tw: 1/2*thetaw*omegaw^2;
vb: (rw+rb)*omega2;
Tb: 1/2*thetab*omegab_diff^2 + 1/2*mb*vb^2;
Uw: 0;
Ub: mb*g*(rw+rb)*cos(fi2);
L: (Tw+Tb)-(Uw+Ub);
L: part(%,2);
/* %th(1)[1]; */
/* ezt kell megoldani
d(dL/d_omwgaw)/dt - dL/dfiw = Mw - Mload
d(dL/d_omega2)/dt - dL/dfi2 = 0
*/

/* ezt is erdemes megnezni : eliminate() */
/* https://maxima.sourceforge.io/docs/manual/maxima_80.html#Item_003a-Polynomials_002fdeffn_002feliminate */
/* subst() */
/* https://stackoverflow.com/questions/42164323/maxima-how-to-replace-variables-in-equations */


    
/* eqLw_a: expand(diff(diff(L,omegaw),t)); */
eqLwa1: diff(L,omegaw);
eqLwa2: diff(eqLwa1, t);
eqLwb: diff(L,fiw);
eqLw: eqLwa2 - eqLwb  = Mm - Ms;

/* ez megy: solve(eqLw, expand(diff(diff(fiw,t),t))); */


eqL2_a1: diff(L,omega2);
eqL2_a2: diff(eqL2_a1,t);
eqL2_b: diff(L,fi2);
eqL2: eqL2_a2 - eqL2_b = 0;


print("--- betaw beta2 kifejez ----");
solve([eqLw, eqL2], [betaw, beta2]);
%th(1)[1];
betaw_lagrange: %th(1)[1];
beta2_lagrange: %th(2)[2];





print("-----betaw----");
expand(betaw_lagrange);
expand(betaw_newton);
expand(betaw_check);

print("-----beta2----");
expand(beta2_lagrange);
expand(beta2_newton);



print(" ");
print(" ");
print(" ");
print(" ");
print(" ");

/* LINEARIZALAS */
/*    diff(    x ,t) =            f(x)             +  g(x)*Ua */
/*         kifejtve manualisan, mert maxima-val nem sikerult  */
/* 1: diff(omegaw,t) = (AAw*sin(fi2) - BBw*omegaw) + (GGw)*Ua */
/* 2: diff(omega2,t) = (AA2*sin(fi2) - BBw*omegaw) + (GG2)*Ua */
/* 3: diff(fi2   ,t) = (     omega2              ) + ( 0 )*Ua */
/* AA es BB konstansok es                                     */ 
/* g(x)-nek nincs x fuggese, tehat a GG is konstans :         */
AAw: Ib*rw*mb*g/(Ib*Iw+mb*rb*rb*Iw+mb*Ib*rw*rw);
BBw: (Ib*mb*rb*rb)*(ku*km+rv*Ra)/(Ra*(Ib*Iw+mb*rb*rb*Iw+mb*Ib*rw*rw));
GGw: km*(Ib+mb*rb*rb)/(Ra*(Ib*Iw+mb*rb*rb*Iw+mb*Ib*rw*rw));
AA2: (Iw*rb*rb+Ib*rw*rw)/((rw+rb)*(Ib*Iw+mb*rb*rb*Iw+mb*Ib*rw*rw));
BB2: (Ib*rw*(ku*km+rv*Ra))/(Ra*(rw+rb)*(Ib*Iw+mb*rb*rb*Iw+mb*Ib*rw*rw));
GG2: (Ib*rw*km)/(Ra*(rw+rb)*(Ib*Iw+mb*rb*rb*Iw+mb*Ib*rw*rw));

/* tvabbiakban omegaw == ww */

/* egyensulyi pont meghatarozasa, ami korul linearizalunk, ott, ahol: */
/* diff(x,t) = 0 */
/* tehat a fentek szerint */
/* 0 =            f(x)             +  g(x)*Ua                 */
eq_lin1: 0 = Aw*sin_fi2-Bw*ww+Gw*Ua;
eq_lin2: 0 = A2*sin_fi2-B2*ww+G2*Ua;
eq_lin12: A2*eq_lin1 - (Aw*eq_lin2); /* megszabadaulni sin_fi2-tol */
eq_lin3: 0 = w2;
/* algsys([eq_lin1, eq_lin2], [ww]); */ /* ez miert nem megy? */
/* %th(1)[1]; */
/* ww: th(1)[1]; */
solve([eq_lin12], ww); 


/* linearizalas utan az alabbi linaris diff egyenlet alakul: */
/* diff(x,t) = A*x + b*Ua */
/* A = diff(f,x)|Xe,Ua */
/* b = diff(g,Ua)|Xe,Ua */
/* kifejtve */
/*  | df1/dww df1/dw2 df1/dfi2 | */
/*  | df2/dww df2/dw2 df2/dfi2 | */
/*  | df3/dww df3/dw2 df3/dfi2 | */




print(" ");
print(" ");
print(" ");
print(" ");
print(" ");










