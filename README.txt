Ball on Wheel
Golyó a keréken
Ver2026Feb26a

A rendszer osszeallitas: 
https://github.com/ballonwheel/ballonwheel/images/setup

A rendszer a Kando Konferencian szerepelt 2023ban:
https://konf2023.kvk.uni-obuda.hu/publikacio

A rendszer videoja mukodes kozben:
http://dobany.hu/VID_20230913_210931.3gp

***********
> Bevezetes
***********
A rendszer lenyege egy egyensulyozo golyo a biciklikereken.
Konkretabban egy elektromos kerékpár 22 col-os tömlő nélküli felni peremein gurul 
egy biliárdgolyó. A kereket HUB (BLDC) motor hajtja, hall szenzorokkal 
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
FEJEZET PID


