
//setpoint
n = input("put the n: ")
Vd = input("put the Vd: ");
Vq = input("put the Vq: ");
Vs = [Vd Vq];
T = input("put the T in [%]: ");

//gep adottdagok
//r, Ld, Lq, Tload(w), jrot
r = 20e-3;
Ld=43e-6;
Lq=43e-6;
flux=0.0002;
Jrot=0.0007;
Tload=0.1;
npp = 5;
 

//gep egyenletek
//bemf = fi * w
w=2*%pi*n*npp;
bemf=[0 w*flux];
xq=w*Lq;
xd=w*Ld;
//Te = 3 / 2 * flux * iq
//iq = 2 / 3 * Te / flux
//iq = 2 / 3 * Treq 
//Vs = bemf + xq*iq + xd*id + r*sqrt(id^2 + iq^2)


o=[0 0]

a=[3 3]

z=[o' bemf']

plot2d(z(1,:),z(2,:))


//Te = Tload --> nem gyorsit

//Te > Tm --> goyrsit

//Te = Tload + FF*w + dw/dt * jrot

 

//megadom Vs, w,

//Vs-sel adott w-nal T-hez kepest valtozik az id, iq

//tehat Vs(vd, Vq) val kell jatszani es megmutatni, h adott w-nal valtozik id, iq

 

//kikopi:

//bemf, id, iq, terhelesi szoget

//vabc, iabc, theta, bemfabc(theta-90fok)

 
