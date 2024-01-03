




pkg load signal
pkg load control
pkg load symbolic

graphics_toolkit("gnuplot")


s = tf ('s');

A = 100*1/s
B = 1
G = A/(1+A*B)
#G = 1/(s+1)
#figure(1)
#bode(G)

#z = tf ('z', 0.2);
#H = 0.095/(z-0.9)


A1 = 1/s
B1 = 1/s
G1 = A1/(1+A1*B1)
figure(2)
bode(G1)


