


pkg load symbolic



#******************************************

# symbolic calc

#************************************************/

% reduce needless whitespace

format compact

% reduce irritations

more off

% start a diary

%diary lectureN.txt

clear

syms t

#syms fib(t)
#syms fiw(t)
#syms fi2(t)
#omegab = diff(fib,t);
#betab = diff(omegab,t);
#omegaw = diff(fiw,t);
#betaw = diff(omegaw,t);
#omega2 = diff(fi2,t);
#beta2 = diff(omega2,t);
#betabc_m = diff(vbc_m(1), t)

syms fib fiw fi2 omegab betab omegaw betaw omega2 beta2
syms mb g N s rb rw rv km ku Ua Ra thetab thetaw murelative
syms x y z
#kinematika
vbc_m = [[(rw + rb) * omega2]; [0]; [0]]
vp_m = [[rw * omegaw]; [0]; [0]]
omegab_m = [[0]; [0]; [omegab]]
rb_m = [[0]; [rb]; [0]]

#s <= murelative * N # not used

#dinamika
#dinamika golyo
acp = omega2^2*(rw+rb) # not used
mb * g * cos(fi2) - N == mb * acp # not used
atg = beta2*(rw+rb)

#dinamika kerek
Mm = km*(Ua-ku*omegaw)/Ra
Ms = rv*omegaw

#---------------------
kzzz=(vp_m + cross(omegab_m, rb_m))
sol_omegab = solve(vbc_m(1) == kzzz(1), omegab)
betab = (-beta2*rb - beta2*rw + betaw*rw) / rb

sol_s = solve(s * rb == thetab * betab, s)

equs=[
mb * g  + sol_s == mb * atg;
Mm - Ms - sol_s * rw == thetaw * betaw;
]

equs_sol=solve(equs, beta2, betaw)
equs_sol


