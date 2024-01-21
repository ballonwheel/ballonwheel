

# imre@raspberrypi:~/ballonwheel/sympy $ uname -a
# Linux raspberrypi 5.15.36-rt41-v8+ #1 SMP PREEMPT Fri May 6 21:32:06 CEST 2022 aarch64 GNU/Linux

# sudo apt-get install python
# imre@raspberrypi:~/ballonwheel/sympy $ python
# Python 3.9.2 (default, Feb 28 2021, 17:03:44)
# [GCC 10.2.1 20210110] on linux

# sudo apt-get install python3-pip
# git clone https://github.com/sympy/sympy.git
# git pull origin master
# python -m pip install -e .

# python .\bowSymbol.py



from sympy import *
import sympy as sp
import sympy

print("Hello, World!")
print(sympy.__version__)


#golyo kinematika
rw, rb, omega2, omegaw, omegab = symbols('rw rb omega2 omegaw omegab')

vbc_m = Matrix([[(rw + rb) * omega2], [0], [0]])
#print(vbc_m[0])
vp_m = Matrix([[rw * omegaw], [0], [0]])
omegab_m = Matrix([[0], [0], [omegab]])
rb_m = Matrix([[0], [rb], [0]])
eq_kin1 = Eq(vbc_m[0], (vp_m + omegab_m.cross(rb_m))[0])
eq_kin1 = simplify(eq_kin1)
print(eq_kin1)
print("ezt kene automatan kifejezni")
#Eq(omega2*(rb + rw), -omegab*rb + omegaw*rw)
beta2, betaw, betab = symbols('beta2 betaw betab')
eq_kin2 = Eq(beta2*(rb + rw), -betab*rb + betaw*rw)
print(eq_kin2)

#sol_beta2 = solve(eq_kin2, beta2)
#print(sol_beta2)

#golyo dinamika
m, g, fi2, N, S, jb = symbols('m g fi2 N S jb')
acp = omega2**2*(rw+rb)
eq_dinb1 = Eq(m*g*cos(fi2)-N, m*acp)
print(eq_dinb1) 
atg = beta2*(rw+rb)
eq_dinb2 = Eq(m*g*sin(fi2)+S, m*atg) 
print(eq_dinb2) 
eq_dinb3 = Eq(S * rb, jb * betab) 
print(eq_dinb3) 

#kerek dinamika
u, ke, ki, Ra = symbols('u ke ki Ra')
Mw, Ml, jw, FF = symbols('Mw Ml jw FF')
Mw = ki*(u-ke*omegaw)/Ra
Ml = FF*omegaw
eq_dinw1 = Eq(Mw - Ml - S*rw, jw*betaw)
print(eq_dinw1)

#https://docs.sympy.org/latest/guides/solving/solve-system-of-equations-algebraically.html
#Equations Which Have a Closed-Form Solution, and SymPy Cannot Solve
#https://stackoverflow.com/questions/31547657/how-can-i-solve-system-of-linear-equations-in-sympy
#sol = solve([eq_kin1, eq_kin2, eq_dinb1, eq_dinb2, eq_dinb3, eq_dinw1], [beta2], dict=True)
sol = sp.solve((eq_kin1, eq_kin2, eq_dinb2, eq_dinb3, eq_dinw1), beta2, dict=True)
print(sol)
#answer: [] <----csak egy ures struktura...ugyenez jon octave-ban

#log:
#Hello, World!
#1.9
#Eq(omega2*(rb + rw), -omegab*rb + omegaw*rw)
#ezt kene automatan kifejezni
#Eq(beta2*(rb + rw), -betab*rb + betaw*rw)
#Eq(-N + g*m*cos(fi2), m*omega2**2*(rb + rw))
#Eq(S + g*m*sin(fi2), beta2*m*(rb + rw))
#Eq(S*rb, betab*jb)
#Eq(-FF*omegaw - S*rw + ki*(-ke*omegaw + u)/Ra, betaw*jw)
#[]


