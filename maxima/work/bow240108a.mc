/* >batch("bow230108.mc"); */

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
betab: %th(1)[1];
s: %th(2)[2];
betaw: %th(3)[3];
beta2: %th(4)[4];

/*  ------------------------------------- */

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
express(%);
L2: %th(1)[1];




diaplay("L2");
display(L2);
expand(diff(L2, omegaw));
express(%);

/*    
eqLw_a: expand(diff(diff(L,omegaw),t));
eqLw_b: diff(L,fiw);
eqLw: eqLw_a + eqLw_b;

eqL2_a: expand(diff(diff(L,omega2),t));
eqL2_b: diff(L,fi2);
eqL2: eqL2_a + eqL2_b;
*/

fiw : fiw(t);                           /* angular displacement of wheel */
fi2 : fi2(t);                           /* angular displacement of position of ball on wheel */
fib : fib(t);                           /* angular displacement of ball */
omegaw : diff(fiw,t);                   /* angular velocity of wheel */
omega2 : diff(fi2,t);                   /* angular velocity of position of ball on wheel */
omegab : diff(fib,t);                   /* angular velocity of ball */
betab : diff(diff(fib,t),t);            /* angular acceleration of ball */
beta2 : diff(diff(fi2,t),t);            /* angular acceleration of position of ball on wheel */
betaw : diff(diff(fiw,t),t);            /* angular acceleration of wheel */


