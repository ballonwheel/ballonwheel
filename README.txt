Ball on Wheel
Golyó a keréken
Ver2026Feb27a

A rendszer osszeallitas: 
https://github.com/ballonwheel/ballonwheel/blob/main/images/setup.jpg

A rendszer a Kando Konferencian szerepelt 2023ban:
https://konf2023.kvk.uni-obuda.hu/absztraktok

A rendszer videoja mukodes kozben:
http://dobany.hu/images/VID_20230913_210931.3gp

Az elkeszult rendszer mukodtetesehez szukseges fajlok:
https://github.com/ballonwheel/ballonwheel/tree/main/bash_solution_only_octave
avagy
git clone https://github.com/ballonwheel/ballonwheel.git

***********
> Bevezetes
***********
A rendszer lenyege egy egyensulyozo golyo a biciklikereken.
Konkretabban egy elektromos kerékpár 22 col-os tömlő nélküli felni peremein 
gurul egy biliárdgolyó. A kereket HUB (BLDC) motor hajtja, hall szenzorokkal 
ellátva. A golyó pozícióját lézeres távolságmérővel mérem. Az így kialakult 
rendszer kényszerpályán mozgó golyó, aminek gyakorlatilag nincs
egyensúlyi pontja, ezért csak instabil pozícióra tudunk szabályozni vagy egy 
trajektória mentén irányítani a rendszert. 
A dokumentum celja lenyegileg a kulonbozo szabalyozasi lehetosegek vizsgalata.

PID -> allapotvisszacsatolas -> LQR -> observer -> nemlinearis -> energia -> NN

Ahhoz azonban, hogy szabalyozot alkossak modellt kell kesziteni.
A modell alkotast elvegzem Newton, Lagrange es Hamilton szerint is.

Ezutan allapotteres modellt keszitek, majd linearizalast vegzek.
A szakaszt gyokhely, Bode es Nyquist diagrammokkal vizsgalom.
A szakaszhoz diszkret szimulacios modellt is keszitek c++ programnyelvben,
amit minden szabalyozohoz hasznalok.

Ezutan kezdodhet a szabalyozok tervezese a fenti sorrendben.

A dokumentumban szandekosan magyar nyelvu ekezet nelkuli karaktereket hasznalok
a legegyszerubb plain text struktura megtartasa miatt.
A dokumentumban Maxima es Octave szintaktikat kovetek a matematikai formulakhoz.
A kepeket az images, a szkripteket a scripts konyvtarban tarolom.


*******************************
> FEJEZET Tartalomjegyzek
*******************************
FEJEZET Bevezetes        
FEJEZET Tartalomjegyzek
FEJEZET Terminológia és konvenció                
FEJEZET Kinematika
FEJEZET Newton
FEJEZET Lagrange        
FEJEZET Hamilton
FEJEZET Allapotter es linearizalas
FEJEZET Szakaszvizsgalat es szimulacioja
FEJEZET Szabalyozo tervezes altalaban
FEJEZET PID


*********************************
> FEJEZET Terminológia és konvenció
*********************************

--------------------------
>> FEJEZET GEOMETRIAI PARAMETEREK 
--------------------------
rw : kerek sugara
rb : golyo sugara
O : kerek kozeppontja
K : golyo kozeppontja

--------------------------
>> FEJEZET GEOMETRIAI PARAMETEREK
--------------------------
rw : kerek sugara
rb : golyo sugara
O : kerek kozeppontja
K : golyo kozeppontja

--------------------------
>> FEJEZET GEOMETRIAI PARAMETEREK
--------------------------
rw : kerek sugara
rb : golyo sugara
O : kerek kozeppontja
K : golyo kozeppontja
A : kontaktpont (kerek es golyo erintkezesi pontja)
r_OA : O -> A helyvektor, |r_OA| = rw
r_AK : A -> K helyvektor, |r_AK| = rb

--------------------
>> FEJEZET SZOGEK
-------------------
phi2 : golyo helyzetszoge a kereken (palya menten)
phib : golyo sajat forgas szoge
phiw : kerek forgas szoge

------------------------------
>> FEJEZET ELOJELES ES IRANYKONVENCIOK
------------------------------
Koordinata rendszerek:
(1) Globalis (x, y):
fix inertiarendszer (csak geometriahoz / abrahoz)
(2) Kiserotrider a golyon (x', y'):
x' : tangencialis irany, a phi2 novekedesenek iranya
y' : radial irany, a golyo kozéppontjatol kifele
z = z’ : a papirbol kifele
Megjegyzes:
A transzlacios dinamikai egyenletek az (x', y') rendszerben
vannak felirva (a_cp a y' iranyban, a_tg az x' iranyban).
Forgasi mennyisegek:
phi2 pozitiv, ha a golyo a +x' iranyba mozog a kereken.
phib pozitiv, ha a golyo CCW forog (+z’ irany).
phiw pozitiv, ha a kerek CCW forog (+z irany).

-----------------
>> FEJEZET SZOGSEBESSEGEK
-----------------
omega_2 := phi2_dot (golyo palyamenti szogsebesseg)
omega_b := phib_dot (golyo sajat szogsebesseg)
omega_w := phiw_dot (kerek szogsebesseg)

------------------
>> FEJEZET SZOGGYORSULASOK
------------------
beta2 := phi2_ddot (golyo palyamenti szoggyorsulas)
betab := phib_ddot (golyo sajat szoggyorsulas)
betaw := phiw_ddot (kerek szoggyorsulas)

-------------
>> FEJEZET SEBESSEGEK
-------------
vK : golyo kozeppont sebessege (abszolut)
vA_ball : kontaktpont sebessege a golyon
vA_wheel : kontaktpont sebessege a kereken
v_rel := vA_wheel - vA_ball (relativ sebesseg kontaktpontban)
Gordulesi feltetel (no slip):
v_rel = 0

---------------
>> FEJEZET GYORSULASOK
---------------
a_cp : centripetalis gyorsulas
a_cp = vK^2 / (rw + rb)
= (rw + rb) * omega_2^2
a_tg : tangencialis gyorsulas
a_tg = (rw + rb) * beta2

------------------
>> FEJEZET EROK ES NYOMATEKOK
------------------
m : golyo tomege
Ib : golyo tehetetlensegi nyomateka
Iw : kerek tehetetlensegi nyomateka
N : normal ero
S : tapadasi (erintkezesi) ero

Ero elojelek:
N pozitiv, ha a golyora +y' iranyban (kozeppont fele) hat.
S pozitiv, ha a golyot +x' iranyban gyorsitja.
M : kerekre hato ossznyomatek
Mw : motor nyomatek
ML : csapagy surlodasi nyomatek

Nyomatek:
M pozitiv, ha a kereket a pozitiv phiw iranyba gyorsitja.

----------------------
>> FEJEZET TAPADASI FELTETEL
----------------------
|S| <= mu_s * N (tiszta gordules)
csuszas eseten:
|S| = mu_k * N
MOTOR MODELL
Mw = k2 * (VA - k1 * omega_w) / RA
ML = rr * omega_w
rr : viszkozus surlodasi egyutthato
mu_s : statikus surlodasi egyutthato
mu_k : kinetikus surlodasi egyutthato



