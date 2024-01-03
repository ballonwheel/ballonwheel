



#diff egynletek
# v0 = R i + L di / dt + ui
# ui = w ke
# T = B * jrot - w FF - Tload
# T = kt * i
# Tload = 0
#
# v0 = R i + L (di / dt) + w ke
# kt * i = jrot (dw / dt) - w FF
#
# dw / dt = - w FF / jrot + i kt / jrot
# di / dt   = - w ke / L     - i R / L      + v0 / L
#
# x_potty = A x + b u
# x = [ w ;
#       i ]
# A = [-(FF / jrot) (kt / jrot) ;
#      -(ke / L)     -(R / L)     ]
# b = [ 0 ;
#     1 / L]
# c = [ 1 0], w a kimenet
# d = 0
# u = V0

clear
graphics_toolkit("gnuplot")
pkg load signal
pkg load control

J = 0.01;
b = 0.1;
K = 0.01; # ke == kt
R = 1;
L = 0.5;
s = tf('s');
P_motor = K/((J*s+b)*(L*s+R)+K^2)
A = [-b/J   K/J
    -K/L   -R/L];
B = [0
    1/L];
C = [1   0];
D = 0;
motor_ss = ss(A,B,C,D)
motor_ss = ss(P_motor);
#bode(P_motor)
#nyquist(P_motor)
step(P_motor)








