
/*
1, abrazolni a nyomatek tenyezot
Mmax = 3/2 * U*Up /(w * xd), Halaszi 291.o
Mmax = 3/2 * Vs*bemf /(w * xLs)
Mmax = 3/2 * Vs*flux*w /(w * xLs)

Ls=43e-6
Vs=12
flux=0.0026
Npp=5
n=10:10:6000
Mmax=Vs*flux*(n/60*2*%pi*Npp*Ls)^-1

iqmax=30
Mmax2=3/2*flux*iqmax

Pmax=300
wmax=4500/60*2*%pi*Npp
Mmax3=Pmax/wmax

M=Mmax*sindelta
delta=arcsin(M/Mmx) | adott Vs mellett



2, ha tudom a delta, terhelesi szoget adott Vs mellett,
akkor fel tudom rajzolni a vektor abrat

3, ha tudom a fazisaramot,m ert pl megmerem, akkor is fel tudom rajzolni a vektor abrat


??????ha tudom ia pillanatnyit, es tudom merre forog, akkor ib, ic adott, nem?
??????a battery arambol tdok poziciot szamolni, ha ismerem a nyomatek tenyezot|Vs mellett

??????mit erdemes megcsinalni?
1, 
kimerem, hogy
 - n, iabc, ibatt Vs flux Ld Lq rs 
 - csinalok egy nyomatek tablat: T=3/2*Npp*iq
get input 'n'
es modellezem a valos rendszert: ecu a scrollban
vektor abra
iabc id iq Vs Vd Vq theta powerfactor delta SkomplexTelj eta

2, merni az aramot es soroson frissiteni iabc-t
vektorabra rajz






/*





