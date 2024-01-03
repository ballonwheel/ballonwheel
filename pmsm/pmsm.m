




pkg load control
pkg load symbolic

#https://www.mathworks.com/help/sps/ref/pmsm.html

syms Rs Fl Ls Ms Lm N jrot

Ld=Ls+Ms+3/2*Lm
Lq=Ls+Ms-3/2*Lm
L0=Ls-2*Ms


#----
#Vd = Rs * id + Ld * did/dt - N*w*iq*Lq
#Vq = Rs * iq + Lq * diq/dt + N*w*(id*Ld+Fl)
#V0 = Rs * i0 + L0 * di0/dt
#dw/dt * jrot = 3/2*N*(iq*(id*Ld+Fl)-id*iq*Lq)
#dth/dt = w

syms id iq i0 w did diq di0 dw
Vd = Rs * id + Ld * did - N*w*iq*Lq
Vq = Rs * iq + Lq * diq + N*w*(id*Ld+Fl)
V0 = Rs * i0 + L0 * di0
dw = 1/jrot * 3/2*N*(iq*(id*Ld+Fl)-id*iq*Lq)
dth = w

solve(Vd, did)
solve(Vq, diq)
solve(V0, di0)
dw
dth





