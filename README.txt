

                                ELOSZO

A valasztott instabil pozicioval rendelkezo rendszer a 'golyo a kereken'
demonstralo eszkoz, amelynek az Obudai Egyetem, Kando Kalman
Villamosmernoki Karon oktatott iranyitastechnika tantargy latvanyos
bemutatasa a celja. A 'golyo a kereken' rendszerben a golyo egyensulyoz egy
szabalyozott motorhajtassal ellatott kerek tetejen. 


A berendezes fotoja:
Abra: ../images/setup.jpg

A korabbi fizikai mukodes videoja:
http://dobany.hu/videos/VID_20230913_210931.mp4

Az eredeti berendezes mukodteto fajljai:
https://github.com/ballonwheel/ballonwheel/tree/main/bash_solution_only_octave

A munka 2023-as Kando Konferenciahoz kapcsolodo forrasoldala:
https://konf2023.kvk.uni-obuda.hu/absztraktok

A foto korabbi online forrasa:
https://github.com/ballonwheel/ballonwheel/blob/main/images/setup.jpg


Forras: Diploma_DobanyImre_1705081.pdf
ASCII SZOVEGVALTOZAT

A gorog betuk nevvel szerepelnek: alpha, beta, gamma stb.
A kepletek es tablazatok automatikus szovegkinyeresbol szarmaznak;
elrendezesuk, also es felso indexeik ellenorzest igenyelhetnek.
A kepfelsorolasok az adott PDF-oldal beagyazott raszterkepeire mutatnak.
A vektoros abrakat a kepkinyeres nem feltetlenul tartalmazza.

================================================================================
PDF OLDAL: 9
================================================================================

                                          Tartalomjegyzek
ELOSZO .........................................................................
.............................. 11
OSSZEFOGLALAS ..................................................................
.................... 13
ABSTRACT .......................................................................
........................... 14
BEVEZETES ......................................................................
.......................... 15
1     GOLYO A KEREKEN RENDSZER
.................................................... 17
2     SZABALYOZO VALASZTAS
............................................................ 19
    2.1      PID szabalyozo
...............................................................................
19
    2.2      Fuzzy szabalyozo...................................................
......................... 19
    2.3      Allapotteres szabalyozo
.................................................................. 19
    2.4      Nem linearis szabalyozo, Lie derivalt
............................................ 20
    2.5      Trajektoria kovetes
......................................................................... 21
    2.6      Kornyezeti parameterek
.................................................................. 21
    2.7      Kovetkeztetes .....................................................
............................ 22
3     MODELL ES RENDSZEREGYENLET ...............................................
23
    3.1      Rendszervazlat, parameterek es terminologia
................................ 24
    3.2      Parameterek es
jelolesek................................................................. 25
    3.3      Mozgasegyenletek felirasa (Newton modell)
................................. 26
    3.4      Modell Lagrange szerint
................................................................. 29
    3.5      Newton Lagrange modell azonossag
.............................................. 31
    3.6      Allapotvaltozok kifejezese
............................................................. 31
    3.7      Differencialegyenlet-rendszer
........................................................ 34
    3.8      Linearizalas.......................................................
.............................. 35
    3.9      Osszefoglalas .....................................................
............................. 38
4     SZAKASZ PARAMETEREK
............................................................... 39
    4.1      Golyo es kerek parameterek
........................................................... 39
    4.2      Motor parameterek
......................................................................... 40
    4.3      Golyo a kereken szakasz
jellemzese............................................... 65
5     PID SZABALYOZO TERVEZES
........................................................ 69
6     ALLAPOTVISSZACSATOLAS
........................................................... 73

================================================================================
PDF OLDAL: 10
================================================================================

7      ALLAPOTFIGYELO
............................................................................75
    7.1      Folytonos modell
............................................................................75
    7.2      Diszkret modell....................................................
...........................76
8      SZABALYOZO MEGVALOSITAS
.....................................................77
    8.1      Fejlesztoi rendszer
..........................................................................78
    8.2      Szimulacio ........................................................
..............................80
    8.3      MCU valasztas .....................................................
...........................89
    8.4      Eroforras kiosztas
...........................................................................89
    8.5      X2C forraskod beagyazas
...............................................................92
    8.6      Hajtas megvalositas
........................................................................94
    8.7      Golyo pozicio meres
.......................................................................99
    8.8      Fordulatszam meres
......................................................................102
    8.9      Kod fejlodes ......................................................
............................106
    8.10 Verzio kovetes ........................................................
......................108
9      EREDMENYEK ..............................................................
....................109
10        KOVETKEZTETES ES JAVASLATOK
........................................111
11        CD MELLEKLET
............................................................................112
12        ABRAJEGYZEK
.............................................................................113
13        TABLAJEGYZEK
...........................................................................115
14        IRODALOMJEGYZEK
...................................................................117

================================================================================
PDF OLDAL: 11
================================================================================


================================================================================
PDF OLDAL: 12
================================================================================


================================================================================
PDF OLDAL: 13
================================================================================

                           OSSZEFOGLALAS

A feladatkiiras szerint a valasztott 'golyo a kereken' iranyitastechnikai
hardver es szoftver eszkoz fejleszteset kell elvegezni. A kifejlesztett
szabalyozo a kerek tetejen, az egyensulyi pont korul tartja a golyot. A hardver
kiepites reszben elore meghatarozott. Epito elemei: BLDC motor, Microchip
MCLV kartya. A szoftver kornyezet nyilt forraskodu iranyitastechnikai
programokat hasznaljon. Ajanlott a Scilab numerikus szamitoprogram,
valamint X2C kodgeneralo konyvtar alkalmazasa.
A diplomamunkam kereteben digitalis-jelfeldolgozassal es szabalyozoval
oldom meg egy golyo instabil poziciora valo szabalyozasanak tokeletesiteset.
A rendszerben a beavatkozo nem egy szervomotor, hanem egy altalanos
BLDC hajtas kerekparmotor, ami kulonleges kihivast okozott.
A szabalyozo allapotteres modellel kerult megvalositasra, allapot
visszacsatolas es allapotfigyelo alkalmazasaval, amivel a golyo az egyensulyi
pont korul tarthato. A jovoben tovabbi szabalyozo algoritmusok kifejlesztese
is lehetseges (PID, Lie derivalt) egyszeruen es olcson, ami ily modon kivalo
oktatasi es bemutato eszkoze valhat.
A szabalyozo algoritmus egy beagyazott, mikrokontrolleres, kooperativ-
multitaszk architekturaju rendszeren kerult megvalositasra, amely csupan egy
lezeres tavolsagmero bemenetet hasznal es a BLDC motorral avatkozik be. A
szabalyozo algoritmus a Scilab/XCos nyilt forraskodu, CeCILL licensszel,
szabadon hasznalhato matematikai szamitogepes szoftveren lett kifejlesztve
es az X2C program segitsegevel keszult az iranyitastechnikai hatasvazlatbol
az adott celprocesszoron futo programmodul.
Munkamban megvizsgalom a felhasznalt eszkozoket, megalkotom a rendszer
modellt, felirom annak egyenletrendszeret. Kovetkezokben szabalyozot
tervezek es szimulaciot keszitek hozza, majd megvalositom es hangolom a
szabalyozot. Vegul megvizsgalom az elert eredmenyeket es tovabbi fejlesztesi
lehetosegeket vazolok fel.

================================================================================
PDF OLDAL: 14
================================================================================

                                ABSTRACT

The official definition of the theory is designing a demonstation tool of
control
technique for instabil position. The chosen system is called "ball on wheel".
The designed controller holds the ball in equbilirum. The elements of the
system is already defined partly. This elements are the BLDC motor,
Microchip MCLV board. The software environment shall use open source
programs of control techniqe. Preferably Scilab numerical computing program
and X2C code generating library shall be used.
In my theory I solve the "ball on wheel" problem with digital signal processing
and controlling. The actuator is not a servo motor but a BLDC driving motor
for bicycles which identification had caused a special challenge.
The controller was created through space state model using state feedback and
observer which with the ball can be hold in equibilium. Further developement
could aim different controller implementation (PID, Lie derivative) in simple
and cheap way which makes this tool perfect for education and demonstration.
Those controller may generating trajectory for ball on wheel.
The controller algorithm is implemented in embedded system which uses
cooperative multitask programming architect. The system uses only single
input of the position of the ball and an output for the actuator of BLDC motor.
The controller algorithm was developed in Scilab/Xcos open source with
CeCILL license and also uses X2C library for generating object code for the
selected controller from model.
In my theory I examine the software and hardware tools, creating system
model and its system equations. Next desinging controller and executing
simulations, implementing the controller and tuning that. Finally I evaluate
the results and put further idas for developing the device.

================================================================================
PDF OLDAL: 15
================================================================================

                              BEVEZETES

Az iranyitastechnikai irodalomban nehany jellemzo iskolapeldat szoktak
felhozni, amivel bemutatjak a kulonbozo technikakat. Ilyen peldaul egy golyo
lebegtetese egy elektromagneses terben vagy a forditott inga es a munkamban
bemutatott golyo a kereken problema is. Az elobb leirtakbol is kovetkezik,
hogy ezek a problemak iranyitastechnikai szempontbol mar szeles korben
megoldottak, tehat sok fellelheto irodalmat talalhatunk ezekre. Altalaban is
elmondhato, hogy a manapsag az iparban jelentkezo altalanos
iranyitastechnikai problemak tobbnyire mar megoldottak es sok
professzionalis ceg kinal kidolgozott alkalmazasokat a megoldasukra.
Azonban a kidolgozott fizikai, matematikai modellek es elokeszitett
alkalmazasok ellenere is a felhasznalora harul a feladat, hogy atlatva a sajat
problemarendszeret megfeleloen hangolja a jellemzo iranyitastechnikai
parametereket.
Munkamban eloszor roviden ismertetem a 'golyo a kereken' rendszert, majd
levezetem, a fizikai modellbol fakado differencialegyenleteket Newton es
Lagrange modellel is. Tettem egyszeruen azert, hogy a kepzes kereteben
megismert ket modell hasznalhatosagat megmutassam.
A kovetkezo fejezetben a szabalyozo valasztasrol ertekezek, melyben sorra
veszem a korabbi fuzzy jellegu megoldast, amelyet az elottem dolgozo kollega
valasztott, valamint a tobbi lehetoseget, ami szoba johet, soros PID, allapot
visszacsatolasos szabalyozo, kvadratikus szabalyozo, stb.
A szabalyozo valasztast kovetoen a szabalyozo tervezesevel foglalkozok,
melyben megvizsgalom a PID szabalyozo tervezes lehetoseget, majd allapot
visszacsatolassal es allapotfigyelo tervezessel folytatom.
A szabalyozot megvalosito rendszer felepiteset lepesrol lepesre valositottam
meg. Az elodom altal jol dokumentalt, SVN verziokovetoben tarolt kodot
vettem at. Kesobb attertem a Microchip AN947 kodjara, amit
osszeolvasztottam az X2C 'Blinky_Microchip_dsPIC33Fxxxx_MicrostickPlus'
kodjaval. Ebben a kombinacioban letre hoztam egy egyszeru PID
fordulatszam szabalyozast, azzal a cellal, hogy megvizsgaljam az
alaprendszer tulajdonsagait. Tovabbi lepesekben allapotbecslos szabalyozot
valositottam meg. A feladat megoldasahoz elengedhetetlen volt a kerekpar
motor identifikacio. Az identifikacio ebben az esetben szokatlannak
mondhato, mert egy olyan iranyitastechnikai problema eseteben, ahol
precizios eszkozokre van szukseg nem kezenfekvo valasztas egy kerekpar
hajtas motor, tekintve, hogy szemben a szervomotoroknal katalogusban
adottak a parameterek, a jelen feladathoz adott BLDC motorhoz ilyen adatok
nem alltak rendelkezesre.

================================================================================
PDF OLDAL: 16
================================================================================


================================================================================
PDF OLDAL: 17
================================================================================

              1     GOLYO A KEREKEN RENDSZER

Roviden ismertetem a 'golyo a kereken' rendszert, amely egy altalanosan
ismert iranyitastechnikai iskolapelda. Leegyszerusitve arrol van szo, hogy egy
kerek tetejen kell egy golyot egyensulyozni, ahol beavatkozo szerv egy
kereket hajto motor, lasd az alabbi abra.

               1-1. abra Golyo a kereken rendszer - osszeallitasi vazlat [27]

Esetemben egy elektromos kerekpar 26 col-os tomlo nelkuli felni gumival
bevont peremein gurul egy biliardgolyo. A kereket BLDC motor hajtja, hall
szenzorokkal ellatva. A golyo poziciojat lezeres tavolsagmerovel merem.
Az igy kialakult rendszer egy egydimenzios kenyszerpalyan mozgo golyo,
aminek gyakorlatilag nincs egyensulyi pontja, ezert csak instabil poziciora
tudunk szabalyozni vagy egy trajektoria menten iranyitani a rendszert.

[Kepek ezen a PDF-oldalon:]
[images/oldal_017_kep_001.jpg]

================================================================================
PDF OLDAL: 18
================================================================================


================================================================================
PDF OLDAL: 19
================================================================================

                  2    SZABALYOZO VALASZTAS

A szabalyozo valasztas atgondolasahoz a [2], [5], [12] irodalomjegyzekben
szereplo forrasokat hasznaltam.
A 'golyo a kereken' rendszer szabalyozasahoz az elso feladat eldonteni, hogy
milyen szabalyozot alkalmazok. A dontest elsosorban az befolyasolja, hogy
ismerem a rendszerem belso felepiteset. Annak fizikai modelljet meg tudom
alkotni (feher dobozos modszer), vagy fekete dobozos modszert valasztok.
Esetemben a rendszer fizikai modellje megalkothato. A valasztast a fellelheto
irodalom terjedelme es reszletessege is befolyasolja termeszetesen.
Dolgozatomban feherdobozos,          allapotfigyelot   valasztok   az   alabbiak
megfontolasa miatt.

2.1 PID szabalyozo
A strukturalisan stabil rendszerek eseten elterjedt, a szakasszal sorba
illesztett
PID szabalyozo alkalmazasa. A 'golyo a kereken' rendszer azonban instabil,
ezert ennek a szabalyozonak az alkalmazasa korulmenyesebb, bonyolultabb
tervezesi folyamatot igenyel [12]. Ennek ellenere dolgozatomban
megvizsgalom ennek az alkalmazasi lehetosegeit is.

2.2 Fuzzy szabalyozo
A korabbi megvalositas fuzzy jellegu szabalyozoval keszult, amelyben a
pozicio alapjan fordulatszam szabalyozas tortent. Amennyiben fuzzy
szabalyozot valasztanek, akkor poziciofuggo nyomatekszabalyozast
alkalmaznek.

2.3 Allapotteres szabalyozo
Az allapotteres modell, mint feherdobozos modszer, a 'golyo a kereken'
rendszer eseten mukodokepes lehet, mert megismerhetjuk a rendszer belso
felepiteset es ezaltal pontos modon leirhatoak az allapotvaltozok trajektoriaja
az allapotterben, fazisterben. A rendszer fizikai modellje alapjan a
differencialegyenlet rendszer felirasa, majd allapot-egyenlet felirasa a
feladat.
Mivel a szakasz biztosan labilis, amit igazolni fogok, allapot visszacsatolasra
van szukseg. Allapot visszacsatolas utan a rendszer stabilla valhat az
egyensulyi pont korul.

================================================================================
PDF OLDAL: 20
================================================================================

2.3.1 SISO modell
Az allapot visszacsatolas SISO modelljeben egyetlen bemeneti parameter van,
ami a kapocsfeszultseg es egyetlen kimeneti parameter van, ami a golyo
pozicioja a kereken.
Itt lehetoseg nyilna PID szabalyozo alkalmazasara is, azonban most nem ezt
valasztom, mert ez a modszer utolagosan, a meres utan, kepes beavatkozni a
rendszer allapotaba, ezert nem olyan hatekony, mint a kesobb emlitett
szabalyozok.

2.3.2 SIMO modell
Az allapot visszacsatolas SIMO modelljeben egyetlen bemenet van, ami a
kapocsfeszultseg es tobb kimeneti parameter van, ami a golyo pozicioja, a
golyo pozicio szogsebessege es a kerek szogsebessege.

2.3.3 Allapotfigyelo
Allapotfigyelo vagy allapotbecslo alkalmazasara, akkor lehet szukseg, ha
korrigalni kell a szamolt allapot es a valosagos ertekek kozotti kulonbseget.
Ez a modszer a SISO modell eseten is alkalmazhato a nem megfigyelt
parameterek becslesere.
LQ szabalyozo
A "Linear Quadratic" szabalyozo manapsag nagyon elterjedt modszer, amiben
direktebb modon tudunk hatni a polusathelyezes soran a K parameterre. A
muvelet soran funkcional segitsegevel keressuk a megoldasokat, ezzel
biztositva, hogy a polusok a 0-tol a legtavolabb keruljenek. Ackermann keplet
segitsegevel hasonlo eredmenyre juthatunk, azonban ott proba-ellenorzes
modszerrel szukseges megtalalni a megfelelo polusokat, amit az LQ elvegez
tulajdonkepp matematikai eszkozokkel. [11]
A dolgozatomban ezzel a modszerrel fogom megkeresni a szabalyozo 'K'
parametert, mellette allapotfigyelot alkalmazok es kizarolag a golyo pozicio
erzekelest hasznalok.

2.4 Nem linearis szabalyozo, Lie derivalt
A nem linearis szabalyozo alkalmazasaval (Lie derivalt) a szakaszt nem
linearizaljuk az egyensulyi pont korul, hanem olyan szabalyozot alkalmazunk,
amely segitsegevel a rendszer linearissa es ezaltal szabalyozhatova valik.

================================================================================
PDF OLDAL: 21
================================================================================

2.5 Trajektoria kovetes
Elviekben megoldhato, hogy a fazister elore meghatarozott pontjaiba iranyitva
a golyot, az a kerek tetejen ugymond billeg. Dolgozatomban ennek
megvalositasara nem terek ki, azonban ez az allapot visszacsatolas - alapjel
kovetessel vagy Lie derivalt alkalmazasaval megoldhato lenne.

2.6 Kornyezeti parameterek
Az eszkozzel kapcsolatban elvaras, hogy kevesbe legyen erzekeny a valtozo
kornyezeti parameterekre, mint keret pozicio vagy homerseklet.
A pozicioval kapcsolatban ket irany johet szoba. Az 3-1. abra szerint jelolt 'x'
tengely elterese a vizszintestol a golyo pozicio mereseben jelent problemat,
mert a pozicio szogelfordulasanak szamolasat az erzekelohoz rogzitet
koordinatarendszerhez lehetseges kotni. Megvaltoztatott viszonyitasi ponthoz
egy hangolasi eljaras kereteben lehetseges lenne a 0 deg -os poziciot
beallitani. A
'z' tengely vizszintestol valo elteres nem befolyasolja a rendszert, addig, amig
a golyo le nem billen a kerek peremrol, amennyiben a tapadasi surlodas
paranyi valtozasat elhanyagoljuk es feltetelezzuk a tapadas fennallasat. Ennek
hatarait tapasztalati uton erdemes kitesztelni.
A keszuleket a 0 deg C - 60 deg C homerseklet tartomanyra tervezve annak hatasa
a
szabalyozas minosegere nem elhanyagolhato. A mechanikai parameterek
kismerteku valtozasa elhanyagolhato. Az elektronikara gyakorolt hatasa az
oszcillator frekvenciajanak elallitodasabol adodik, amely elhanyagolhato
merteku, ~1%. [25] A rendszer eleteben a motor parametereit befolyasolja
leginkabb a homerseklet. A motor induktivitasa es ellenallasa ku, km
konstansai is valtoznak a homerseklet hatasara, ezert a motor idoallandoja is
novekszik a homerseklet novekedesevel [26], ami az iranyitastechnikai
jellemzore is hatassal van. A modell megalkotasakor a homersekletvaltozast
figyelembe veve, egy kornyezeti homero segitsegevel, modosithatoak a
szabalyozasi parameterek.

================================================================================
PDF OLDAL: 22
================================================================================

2.7 Kovetkeztetes
A fenti lehetosegeket sorra veve arra kovetkeztetesre jutottam, hogy a
feladatot PID, allapotteres vagy Fuzzy szabalyozoval tudom megoldani.
A Fuzzy szabalyozot ebben az esetben nem valasztom, mert az fekete dobozos
modszerekent, akkor hasznos, ha nem ismerem a rendszer belso felepiteset.
Habar itt is hasznalhato lenne, de viszonylag keves gyakorlati irodalmat
talaltam hozza, valamint szeretnek megismerkedni a hagyomanyos
szabalyozasokkal, mint a PID vagy az allapotteres modell.
A kialakult velemeny alapjan, a PID szabalyozo tervezeset gondolom vegig,
majd az eredmenyek fuggvenyeben az allapot visszacsatolas es allapotfigyelo
szabalyozoval torteno megvalositast valasztom. Ehhez a szabalyozohoz
elegendoen sok gyakorlati irodalmat talaltam. [1][2][16]

================================================================================
PDF OLDAL: 23
================================================================================

            3   MODELL ES RENDSZEREGYENLET

A fejezet megalkotasat a [1], [2], [3], [10], [11], [12] irodalomjegyzekben
szereplo muvek alapjan vegeztem.
Minden feherdobozos iranyitastechnikai feladat, akar elektrodinamikai, akar
mechanikai rendszerrol van szo, a modell alkotasaval kezdodik, melynek
soran felirjuk a rendszeregyenleteket. [10]
A jelen esetben, mechanikai problemarol leven szo, newton-i vagy largrange-
i modell egyenletekkel juthatunk eredmenyre. A ket modell kozott
vegeredmenyeben kulonbseg nincs, de mar ebben az egyszeru modellben is
lathato, hogy bizonyos problemakat sokkal egyszerubb lagrange-i modellel
targyalni. A modellt mindket eljarassal felirom es igazolom, hogy azonos
rendszeregyenletekhez jutok. Mindket modellel csak az iskolapelda kedveert
vizsgaltam meg a problemat, hogy tisztan lathassuk a modellek azonossagat,
elonyeit es kotottsegeit egy adott probleman, egyebkent termeszetesen
elegendo csak egyik vagy masik hasznalata. Az egyenleteket Maxima, nyilt
forraskodu szimbolikus megoldo programmal is ellenorzom, amelynek
forraskodja a CD mellekleten megtalalhato.

================================================================================
PDF OLDAL: 24
================================================================================

3.1   Rendszervazlat, parameterek es terminologia

                   3-1. abra Golyo a kereken rendszermodell [1]

[Kepek ezen a PDF-oldalon:]
[images/oldal_024_kep_002.jpg]

================================================================================
PDF OLDAL: 25
================================================================================

3.2   Parameterek es jelolesek

      Jellemzo          Ertek                Megjegyzes
           g            9,81 m/s^2           Gravitacios gyorsulas
       Ua , Uk          0-24V                Kapocsfeszultseg
         Ra             300mOhm              Armatura ellenallas
         La             30mH                 Armatura induktivitas (1kHz)
         Km             2,093Vs/rad          Mechanikai konstans
         Ku             2,518Nm/A            Villamos konstans
          Iw            0,30618kgm^2         Tehetetlensegi nyomatek -
                                             kerek
          Tv            33,33 * 10^-3        Villamos idoallando
         Tm             263 * 10^-3          Mechanikai idoallando
          rw            0,27m                Kerek sugar
         rwe            0,03m                Kerek perem tavolsag
          rv            2,3*10^-3            Csapagy surlodas, nyomatek
                                             egyutthato
         phiw             -                    Kerek szogelfordulas
         ww             0-120rpm             Kerek      szogsebesseg-Hall
                                             meres
         Mm             -                    Kerek motor nyomatek
         ML             -                    Terhelo nyomatek
          Ib            0.0000731kgm^2       Golyo tehetetlenseg
          rb            0,0254m              Golyo sugar a peremen
         rbf            0,03m                Golyo sugar
         mb             0,203kg              Golyo tomeg
         phi2             -0,45rad -- 0,45 rad Golyo pozicio szogelfordulas
         w2             -                    Golyo pozicio szogsebesseg
                      3-1. tablazat Terminologia es parameterek

A tablazatban talalhato parameterek reszben katalogusadatok alapjan,
reszben a szakasz identifikacio alapjan adottak.

================================================================================
PDF OLDAL: 26
================================================================================

3.3     Mozgasegyenletek felirasa (Newton modell)

3.3.1 Kinetikai feltetelek

A modellhez elengedhetetlen, hogy a golyo a kereken ne csusszon meg, azaz
a tapadas mindvegig fennalljon, ami a tiszta gordules esetet jelenti.

                                        3-2. abra Kinetikai feltetel

A golyo tomegkozeppontjanak transzlacios es rotacios mozgasa kozotti
kapcsolatot az alabbi egyenlettel fejezhetjuk ki. [10]

VK = VA + wb  *  rAK                                                     (AN.1.)
      rw ww         (rw + rb )w2              0            0
VA = ( 0 ) , VK = (      0       ) , Wb = ( 0 ) , rAK = (rb )            (AN.2.)
        0                0                    wb           0
     (A.2.a.)      (A.2.b.)          (A.2.c.)     (A.2.d.)

            i j      k                 -wb rb
Wb  *  rAK = |0 0      wb | = -iwb rb = ( 0 )
(AN.3.)
            0 rb     0                  0

VK = rw ww - wb rb                                                       (AN.4.)

(rw + rb )w2 = rw ww - wb rb                                           (AN.5.)

[Kepek ezen a PDF-oldalon:]
[images/oldal_026_kep_003.png]

================================================================================
PDF OLDAL: 27
================================================================================

A tiszta gordules, tapadas addig all fenn, amig a tapadasi ero kisebb vagy
egyenlo, mint a surlodasi ero.

S  <=  muN                                                               (AN.6)

A mu erteket az anyagok kozotti tulajdonsagok hatarozzak meg. A fenti
azonossagot tapasztalati uton fogom ellenorizni. A differencialegyenlet
felirasahoz, majd, mint latni fogjuk kozvetlenul erre nincs szukseg.

3.3.2   Mozgasegyenletek-golyora

A golyo mozgasegyenleteinek felirasakor kisero-tiederes koordinatarendszert
hasznalok a 3-1. abra szerint (x'; y').
Egy tomegkozeppontra 6 mozgasegyenletet tudunk felirni:
    - 3-at, (x; y; z) iranyban a transzlacios mozgasra es
    - 3-at, (x2; y2; z2) iranyban a rotacios mozgasra.
Jelen esetben, mivel sik problemarol van szo
    - 2 db egyenlet a transzlacios mozgasrol szol (x; y)
    - 1 db egyenlet a rotacios, forgomozgasrol szol (z2)

transzlacios egyenletek
mg * cos phi2 - N = m * acp (AN.7)          acp = (phi2 )2 (rw + rb ) (AN.9)
mg * sin phi2 + S = m * atg (AN.8)            atg = phi2 (rw + rb ) (AN.10)

rotacios egyenlet
S * rb = Ib * phi b                                           (AN.11)

3.3.3   Mozgasegyenletek - kerekre

A kerek koordinatarendszeret az 3-1. abra szerint valasztom. Mivel
transzlacios mozgast nem vegez a kerek, ezert egyenletei sincsenek. Mivel a
kerek forog, ezert rotacios egyenlete van.
M - Srw = Iw * phi w                                          (AN.12)
Az 'M' nyomatekba a motor csapagy surlodast (ML) negativ elojellel a
kesobbiekben bele kell szamolni.
M = Mw - ML                                                  (AN.13)

================================================================================
PDF OLDAL: 28
================================================================================

3.3.4 Osszevont mozgasegyenletek
                   r                r
AN.5  ->  phi b = rw phi w - (1 + rw )phi 2 (AN.14)
                       b                b

AN.14 -> AN.11(S) -> AN.10 -> AN.16
AN.14 -> AN.11(S) -> AN.12 -> AN.15

           r2              r                         k2 (VA -k1 ww )
(Iw + Ib rw2 ) ww - Ib * rw2 * (rw + rb ) * w2 =         RA
                                                                     - rr ww
(AN.15)
           b               b

       1
-Ib * 2 * (rw ww - (rw + rb ) * w2 ) + mb (rw + rb ) * w2 - mb g(rw + rb ) sin
phi2 = 0
      rb
(AN.16)

================================================================================
PDF OLDAL: 29
================================================================================

3.4       Modell Lagrange szerint

A Lagrange modell felirasahoz az [1], [2] irodalomjegyzekek nagyban
segitettek.

L =T-V                                                                    (AL.1)

ahol 'T' a mozgasi energia es 'V' a helyzeti energia, 'L' pedig a Lagrange-
fuggveny.

Esetunkben tehat:
          1
Tw = 2 Iw * ww2                                      (AL.2) - kerek mozgasi
energiaja
          1                1
Tb = 2 Ib * wb2 + 2 mb * Vb2                         (AL.3) - Golyo mozgasi
energiaja
Vb = mb * g(rw + rb ) cos phi2                          (AL.4) - Golyo helyzeti
energiaja

L = Tw + Tb - Vb
(AL.5)
      1                1           1
L = 2 Iw * ww2 + 2 Ib * wb2 + 2 mb * Vb2 - mb * g(rw + rb ) cos phi2
(AL.6)

Ismereteink wb es Vb-re, (AN.4) es (AN.5) alapjan:
           1
wb = r (rw ww - (rw + rb )w2 )
(AL.7)
              b
Vb = (rw + rb )w2
(AL.8)

Ezert az 'L':
      1                1       1                             1
L = 2 Iw * ww2 + 2 Ib * r 2 (rw ww - (rw + rb )w2 )2 + 2 mb (rw + rb )2 * w2 2 -
                               b
mb * g(rw + rb ) cos phi2                (AL.9)

Igy a Lagrange valtozok:

L(phiw ; phi2 ; ww ; w2 )          (AL.10)

A Lagrange-egyenlet zart, konzervativ rendszer eseten:

 d d_partialL             d_partialL
          - d_partialq = 0
(AL.11)
dt d_partialqi            i

================================================================================
PDF OLDAL: 30
================================================================================

ahol 'q' a rendszert jellemzo konfiguracios parameter, itt 'hely', azonban a
'golyo a kereken' rendszer nem konzervativ, tehat disszipativ rendszer, ahol
vesztesegek is vannak. A motort is jellemeznem kell, tehat a rendszerem nem
is tekintheto zartnak.
Az ilyen rendszert a 'altalanos hatas elv' szerint lehet jellemezni a kovetkezok
szerint:

d d_partialL        d_partialL
          - d_partialq = Qi*
(AL.12)
dt d_partialqi        i

ahol Qi* a nem konzervativ erokbol allo altalanos erok eredoje.
Erre azert van szukseg, mert a motort jellemzo altalanos erokre nem lehet
felirni potencialis erot. A potencial fuggveny hely koordinatatol fugg, de a
motor az ww szogsebessegtol. Q * munkajellegu mennyiseg.

Tehat a feladat (AL.9) egyenlet (AL.12) egyeletbe helyettesitese es
megoldasa. Az (AL.12) egy egyenletrendszer, amelyben i(W,2)(kerekre es
golyo poziciora).

 *                             KM (Ua -KU Ww )
Qw = Mw - ML =                                   - rr ww (AL.13), a motor modell
alapjan,
                                          Ra
lasd 4.2 fejezet.
Q2* = 0                                                   (AL.14), a pozicionak
nincs
Q * altalanos ereje.

Lagrange parcialis derivalt phiw ; ww szerint.
 d_partialL                    1   1
       = Iw ww + Ib * 2 * 2(rw ww - (rw + rb )w2 ) * rw
d_partialww                    2   rb
d_partialL
       =0
d_partialphiw
 d  d_partialL                          r
   (      ) = Iw w w + Ib * w2 * (rw w w - (rw + rb )w 2 )
dt d_partialww                          rb
                 rw                                 k (VA -k1 ww )
Iw w w + Ib * 2 * (rw w w - (rw + rb ) * w 2 ) = 2                - rr ww
                  rb                                      RA

             r2                      r                          k2 (VA -k1 ww )
(Iw + Ib w2 ) ww - Ib * w2 * (rw + rb ) * w2 =
- rr ww   (AL.15)
             rb                      rb                               RA

================================================================================
PDF OLDAL: 31
================================================================================

Lagrange parcialis derivalt phi2 ; w2 szerint.
 d_partialL                1
      = -Ib * 2 * (rw ww - (rw + rb ) * w2 ) * (rw + rb ) + mb (rw + rb )2 * w2
d_partialw2               rb
 d_partialL
      = mb g(rw + rb ) * sin phi2
d_partialphi2
 d  d_partialL              1
  ( ) = -Ib * 2 * (rw w w - (rw + rb ) * w 2 ) * (rw + rb ) + mb (rw + rb )2 * w
2
dt d_partialw2             rb
      1
-Ib * 2 * (rw w w - (rw + rb ) * w 2 ) + mb (rw + rb ) * w 2 - mb g(rw + rb ) *
sin phi2 = 0
     r    b
          1
-Ib * 2 * (rw w w - (rw + rb ) * w 2 ) + mb (rw + rb ) * w 2 - mb g * sin phi2 =
0
         rb

           r                           r +r
 -Ib * rw2 * ww + (Ib * wr 2 b + mb (rw + rb )) * w2 = mb g * sin phi2
(AL.16)
              b                            b

3.5       Newton Lagrange modell azonossag

Az (AN15), (AN.16) es (AL15), (AL.16) egyenleteket paronkent
osszehasonlitva lathato, hogy az egyenletek megegyeznek, tehat igazoltam,
hogy a kulonbozo modellek azonos eredmenyt adnak.

3.6       Allapotvaltozok kifejezese

Az (AN.15) es (AN.16) alapjan:
                I
Aw2 + Bww = E             /* CD                                           A =
(rw + rb ) ( b2 + mb )
                rb
      r
Cw2 + Dww = H             /* AB                                           B= -Ib
* w2
      rb
- - - - - - - - - - - - - - - - - - - -- - - - - - - - - - - - - - -
ACw2 + BCww = E * C                                                       E = mg
* sin phi2
     r
ACw2 + ADww = A * H                                                       C =
-Ib * w2 (rw + rb )
      rb
           r2
(BC - AD)ww = E * C - A * H                                                D =
Iw + Ib w2
           rb
        E*C-A*H
ww =                                                                       H=M
        B*C-A*D

ADw2 + BDww = E * D
CBw2 + BDww = H * B
(BC - AD)w2 = H * B - E * D
      H*B-E*D
w2 =
        C*B-A*D
 - - -- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

================================================================================
PDF OLDAL: 32
================================================================================

                 r                                           2                 I
k (VA -k1 ww )
             -Ib * w                                b
                   2 (rw +rb )mg*sin phi2 -(rw +rb )( 2 +mb )(
-rr ww )
                 rb                                                           rb
RA
ww =
I                      r2
                                       r
                                  Ib * w                      b             w
                                       2 (rw +rb )-(rw +rb )( 2 +mb )Iw +Ib 2
                                       rb                          rb
rb
                    rw             Ib        k1 k2              Ib    k U
             (Ib * 2 mg*sin phi2 )+( 2 +mb )(-       -rr )ww +( 2 +mb ) 2 A
                    rb             rb          RA               rb     RA
ww =                                           2
                                                              2
                             I               r          r
                            ( b2 +mb )(Iw +Ib w  )-(Ib * w  )
                             rb              r2b        r2b
Ib *Iw        Ib2 *rw2                    2
                                         rw      2 rw
                                                      2
         +            +m I +m I
                          b w        b b         b                  -I *
 rb2            rb 4                      2
                                         rb         rb4

                 r                                  I
                                            1 2 +r )w +( b +m ) 2 A k k
I             k U
             (Ib * w               b
                   2 mg*sin phi2 )-( 2 +mb )(       r  w   2   b
                 rb                                 rb               RA
rb             RA
ww =                            Ib *Iw                r2
                                        +mb Iw +mb Ib w
                                  r2 b                 r2
                                                        b
             RA Ib rw mg*sin phi2 -(Ib +mb rb )(k1 k2 +rr RA )ww +(Ib +mb rb2
)k2 UA
                                            2
ww =
(AE.1)
                                RA (Ib *Iw +mb rb2 Iw +mb Ib rw
                                                              2)

              r   k2 (VA -k1 ww )                            r
2
         -Ib * w2(                -rr ww )-mg sin phi2 *(Iw +Ib w2)
                 rb               RA
rb
w2 =                          2
                     r                                              I
r2
               (Ib * w                        b              w
                     2 ) (rw +rb )-(rw +rb )( 2 +mb )(Iw +Ib 2 )
                     rb                                             rb
rb

                r        2          rw k2 (VA -k1 ww )
         (Iw +Ib w2 )mg sin phi2 +Ib * 2 (               -rr ww )
                         rb                                 rb            RA
w2 =                                                   2
                                            r                   I
r2
                (rw +rb )[-(Ib * w      b              w
                                 2 ) +( 2 +mb )(Iw +Ib 2 )]
                                               rb           rb
rb

                      r2            w                       r
                                         1 2 -r )w +I * w * 2 A     k k
r    k U
         (Iw +Ib w
                 2 )mg sin phi2 +Ib * 2 (-       r  w b 2
                         rb                                 rb       RA
rb    RA
w2 =
2
                                           I                w            r2
r
                      (rw +rb )[( b2 +mb )(Iw +Ib w
                                                  2 )-(Ib * 2 ) ]
                                           rb                            rb
rb

                 2 +I r 2 )mg sin phi -I r +(k k +R r )w +I r k U
         RA (Iw rw   b w           2 b w    1 2  A r  w b w 2 A
w2 =                             RA (rw +rb )(Ib *Iw +mb rb2 Iw +mb Ib rw
                                                                         2)

----------------------------------------------------
             RA Ib rw mb g sin phi2 -(Ib +mb rb2 )(kU kM +rV RA )ww +(Ib +mb rb2
)kM UA
ww =                                           RA (Ib *Iw +mb rb2 Iw +mb Ib rw
                                                                              2)

         RA (Iw rb2 +Ib rw
                         2 )m g sin phi -I r +(k k +R r )w +I r k U
                             b       2 b w    U M  A r  w b w M A
w2 =                              RA (rw +rb )(Ib *Iw +mb rb2 Iw +mb Ib rw
                                                                          2)

phi 2 = w2
----------------------------------------------------

================================================================================
PDF OLDAL: 33
================================================================================

A konstansokra egyszerubb jelolest bevezetve:
ww = alphaw sin phi2 - betaw ww + gammaw Ua                              (AE.1)
w2 = alpha2 sin phi2 - beta2 ww + gamma2 Ua                              (AE.2)
phi 2 = w2                                                   (AE.3)

ahol
                     I r m g
alphaw = I I +mb rw2 I b+m I r 2
        b w           b b w        b b w

         (I +m r 2 )(k k +r R )
                    U M   V a
betaw = R (Ib *I b+m
                b
         a   b w  r 2 I +m I r 2 )
                            b b w     b b w

             2 b     k (I +m r 2 )
                      b b
gammaw = R (I *I +m
        a    b w r 2 I +m I r 2 )
                           b b w      b b w

                           I r 2 +I r 2
alpha2 = (r +r )(I *Iw +m
                   b  b w
         w       b    r 2 I +m I r 2 )
                          b w     b b w      b b w

                      I r (k k +R r )
beta2 = R (r +r b)(Iw *I U+m
                        M    a r
        A    w        b   r 2 I +m I r 2 )
                            b w      b b w     b b w

                   Ib rw kM
gamma2 =
       Ra w +rb b *Iw +mb rb2 Iw +mb Ib rw
         (r    )(I                       2)

================================================================================
PDF OLDAL: 34
================================================================================

3.7    Differencialegyenlet-rendszer

A (AE.1), (AE.2), (AE.3) alapjan felirhato az allando egyutthatos (LTI), nem
linearis, elsofoku differencialegyenlet rendszer.

x = f(x) + g(x) * Ua                                         (AD.1)

ahol

     ww
x = ( w2 )                                                    (AD.2)
      phi2

         alphaw sin phi2 - betaw ww
f(x) = ( alpha2 sin phi2 - beta2 ww )                                  (AD.3)
                 w2
         gammaw
g(x) = ( gamma2 )                                                 (AD.4)
          0
A 'g'-nek nincsen 'x' fuggese, tehat az egy konstans.

================================================================================
PDF OLDAL: 35
================================================================================

3.8   Linearizalas

A 'golyo a kereken' rendszer egy nem linearis problema, f(x~ sin phi2 ). A
szabalyzas elengedhetetlen feltetele, hogy a rendszer modellunk linearis
legyen annak erdekeben, hogy az allapotter modellnel megismert
szabalyhalmaz alkalmazhato legyen. A linearizalast az [1],[6] alapjan
vegeztem.

A differencialegyenlet rendszer leirasbol a linearizalason keresztul jutunk el
az allapot allapotter modell leirasaig.

x = f(x) + g(x) * UA  ->  linearizalas  ->  x = A(x) + b * UA
ahol
     d_partialf
A = d_partialx |Xe , UA
      d_partialg
b = d_partialU |Xe , UA
         A

ahol Xe az egyensulyi helyzethez tartozo parametereket tartalmazza, A
rendszermatrix es b konstans vektor, ami a bemeneti UA parameteren
keresztul hat. Ennek erdekeben az egyensulyi pont korul linearizalast
vegzunk.
Elso lepeskent az egyensulyi pontot hatarozzuk meg, azaz

x = 0                      (AT.1), ahol a rendszerallapot valtozasa 0.

0 = f(x) + g(x) * Ua         (AT.2),    algebrai  egyenletrendszer  a
                             parameterekre, ami az egyensuly feltetelt
                             fogalmazza meg.

Kifejtve:
0 = alphaw sin phi2 - betaw ww + gammaw Ua                        (AT.3)
0 = alpha2 sin phi2 - beta2 ww + gamma2 Ua                        (AT.4)
0 = w2                                               (AT.5)

A (AT.3), (AT.4), (AT.5) linearis egyenletrendszer megoldasa adja Xe vektort,
ami azokat a parametereket tartalmazza, amelyekre a rendszer egyensulyban
van, ahol a parametervaltozas 0.
x = 0                                                    (AT.100)
f(x) + g(x) * Ua = 0                                      (AT.101)
 - ---------------- -

================================================================================
PDF OLDAL: 36
================================================================================

alphaw sin phi2 - betaw ww + gammaw Ua = 0 /* alpha2
(AT.102)
alpha2 sin phi2 - beta2 ww + gamma2 Ua = 0 /* alphaW
(AT.103)
w2 = 0                                                  (AT.104)
 - ------------------------------
alpha2 alphaw sin phi2 - alpha2 betaw ww + alpha2 gammaw UA = 0
(AT.105)
alphaw alpha2 sin phi2 - alphaw beta2 ww + alphaw gamma2 UA = 0
(AT.106)
(alpha2 betaw - alphaw beta2 )ww + (alphaw gamma2 - alpha2 gammaw )UA = 0
(AT.107)
Kivonom (AT.9) es (AT.10)-et egymasbol, valamint (alpha2 betaw - alphaw beta2 )
biztosan
nem 0, ezert
       alpha gamma -alpha gamma
ww = alpha 2betaw -alphaw beta2 * Ua =* GammaUa
          2 w     w 2
(AT.108)

Ww-t helyettesitsuk vissza (AT.102)-be.
                         alpha gamma -alpha gamma
alphaw sin phi2 - betaw * alpha 2betaw -alphaw beta2 * Ua + gammaw UA
                          2 w      w 2
(AT.109)
             2
(alphaw alpha2 betaw - alphaw beta2 ) sin phi2 - alpha2 gammaw betaw UA + alphaw
betaw gamma2 UA + gammaw alpha2 betaw UA -
alphaw beta2 gammaw = 0                                                 (AT.110)

alphaw (alpha2 betaw - alphaw beta2 ) sin phi2 + alphaw (betaw gamma2 - beta2
gammaw ) = 0
(AT.111)

sin phi2 = 0, akkor van, ha phi2 = 0, ekkor az osszeg elso tagja 0.

Ellenorizzuk a masodik tagban (betaw gamma2 - beta2 gammaw )-t.

                           (Ib +mb rb2 )(kU kM +rV Ra )
(betaw gamma2 - beta2 gammaw ) =                                        *
                           Ra (Ib *Iw +mb rb2 Iw +mb Ib rw2)

                 Ib rw kM                                Ib rw (kU kM +rV Ra )
                                       2 ) - R (r +r )(I *I +m r 2 I +m I r 2 )
*
Ra (rw +rb )(Ib *Iw +mb rb2 Iw +mb Ib rw         a w     b    b w     b b w    b
b w
        kM (Ib +mb rb2 )
                                   =0
(AT.112)
Ra (Ib *Iw +mb rb2 Iw +mb Ib rw
                              2)

Tehat az osszeg masodik tagja 0-t ad, igy, ha
                                                                 GammaUa
x = 0, akkor Ww = GammaUa ; phi2 = 0; phi 2 = w2 = 0, tehat Xe = [ 0 ]
                                                                  0
A fenti Xe vektorbol latszik, hogy vegtelen sok egyensulyi pont letezik Ua
tetszoleges erteke miatt. Ez a gyakorlatban azt jelenti, hogy a kerek foroghat,
mikozben a golyo a kerek tetejen az egyensulyi pontban forog. A cimben
szereplo instabilitas a gyakorlatra vonatkozik, mert a kornyezeti zavaro
hatasok kibillentik a golyot.

================================================================================
PDF OLDAL: 37
================================================================================

Most, hogy ismerjuk az egyensulyi ponthoz tartozo parameter ertekeket
sorfejtest kell vegeznunk f(x)R 3  ->  R 3 vektor fuggvenye, ami szinten egy
matrixot fog eredmenyezni. g(x) eseten erre nincs szukseg, mert annak nincs
x fuggese, az csak egy konstans.

Az allapotvaltozoban szereplo nem linearis sin(phi2 ) tag sorfejtese
                    f'(x )           f''(x )                 f n (x )
 Tn (x ) = f(x0 ) + 1! 0 (x - x0 ) + 2! 0 (x - x0 )2 + ... + n! 0 (x - x0 )n
                                                           (AT.200)

Az elsorendu polinom az erinto egyenes, amely, ha nem ter el tulsagosan a
valtozas linearis kozelitesnek tekintheto.

 T1 (x) = f(x0 ) + f'(x0 )(x - x0 )
(AT.201)
phi0 = 0
(AT.202)
sin' phi = cos phi
(AT.203)
T(phi) = sin 0 deg  + cos 0 deg  * (phi - 0) = 0 + 1 * (phi - 0) = phi
(AT.204)
sin phi ~phi
(AT.205)

A gyakorlatban a fuggveny  +/-  deg 5-ig jo linearis kozelitest ad. Lathato,
hogy a
szabalyozo ebben a tartomanyban tud majd dolgozni, egyebkent a
tartomanyon kivul mas eljarast kell alkalmazni, hogy a linearis tartomanyba
visszahuzza a golyot.

f(x) vizsgalata xe (sin phi2 ~phi2 ) kornyezeteben

                 d_partialf1    d_partialf1    d_partialf1
                d_partialww    d_partialw2     d_partialphi2
d_partialf              d_partialf2    d_partialf2     d_partialf2
      (xe ) = d_partialw       d_partialw2     d_partialphi2
                                     (xe ) =
d_partialx                 w
                 d_partialf3   d_partialf3     d_partialf3
               [d_partialww    d_partialw2 d_partialphi2 ]
    d_partial(alphaw phi2 -betaw ww )   d_partial(alphaw phi2 -betaw ww )
d_partial(alphaw phi2 -betaw ww )
         d_partialww                 d_partialw2                d_partialphi2
                                                                    -betaw    0
alphaw
    d_partial(alpha2 phi2 -beta2 ww )   d_partial(alpha2 phi2 -beta2 ww )
d_partial(alpha2 phi2 -beta2 ww )
         d_partialww                  d_partialw2               d_partialphi2
                                                                 = [ -beta2   0
alpha2 ]
         d_partialw2                  d_partialw2               d_partialw2
0    1    0
[        d_partialww                  d_partialw2               d_partialphi2
]
(AT.300)

================================================================================
PDF OLDAL: 38
================================================================================

Tehat a linearizalt rendszeregyenlet:

x = A(x) + b * UA

 ww     -betaw      0   alphaw ww         gammaw
 w
( 2 ) = [ -beta2     0         w
                      alpha2 ] [ 2 ] + [ gamma2 ] UA
 phi2       0      1    0 phi2          0

3.9   Osszefoglalas

Ezzel meghatarozasra kerult a rendszeregyenlet.
A fenti egyenletek Maxima programmal is ki lettek fejezve, a forraskod CD
mellekleten talalhato.
Kovetkezo lepeskent megvizsgalom a szakasz viselkedeset.

================================================================================
PDF OLDAL: 39
================================================================================

                  4        SZAKASZ PARAMETEREK

A szakasz vizsgalatahoz valamint a szabalyozo megalkotasahoz szukseg van
a szakasz iranyitastechnikai parametereinek meghatarozasara, amit fizikai
modell alkotassal vagy indentifikacioval lehet elvegezni. Az identifikacio
soran a parameterek ugy kerulnek meghatarozasra, hogy a modell es a
rendszer gerjesztes-valasz jelei minel jobban kozelitsek egymast. [16]

4.1   Golyo es kerek parameterek

A golyo es kerek parametereinek merese szukseges az iranyitastechnikai
modellben valo felhasznalashoz.
 Megnevezes           Jeloles      Ertek                        Megjegyzes
 Golyo tomeg          mb           203g
 Golyo sugar          rbf          30mm
 Golyo perem          rb           25,98mm                      sqrt (rbf^2 -
(rwe/2)^)
 sugar
 Golyo                Ib           0,0000731kgm^2 (2/5) * rbf^2 * mb
 tehetetlensegi
 nyomatek
 Kerek sugar          rw           270mm
 Kerek tomeg          mw           6,6kg
 (motorral)
 Kerek perem          rwe          30mm
 Kerek                Iw           Lasd 4.2                     Motorral egyutt
 tehetetlensegi                    fejezetben.                  meghatarozva
 nyomatek
                            4-1. tablazat ID - Golyo es kerek

A fenti tablazatban szereplo parameterek az 3-1. tablazat Terminologia es
parameterek osszefoglalasban feljegyzesre kerultek.

================================================================================
PDF OLDAL: 40
================================================================================

4.2   Motor parameterek

A korabbi fejezetekben kifejtesre kerult, hogy a teljes rendszer
iranyitastechnikai modellje ket fo reszbol all. Az egyik resz termeszetesen a
golyo viselkedeset irja le a kerek tetejen. A masik resz a beavatkozo motor
modellbol all, amelyhez szukseges parameterek azonositasat az alabbiakban
reszletezem. A beavatkozo motor egy "brushless DC" motor (BLDC),
amelynek elonyei a kefe kommutator kopasanak elkerulese ezert elterjedt es
olcso. A kommutaciot elektronikus kapcsolokkal kell megoldani. A BLDC
motor modellje azonos a DC motor modellel.

                                                                     Parameter
identifikacio

          4-1. abra ID-motor, Teljes rendszer hatasvazlat (egy lehetseges
megoldas)

4.2.1 Motor iranyitastechnikai modell
A motor iranyitastechnikai modellezesehez az alabbi tablazatban osszefoglalt
parameterekre van szukseg [3], amelynek mereset az alfejezetekben
reszletezem.

 Jellemzo                  Ertek                        Megjegyzes
 Ra                        300 mOhm                     Armatura ellenallas
 La                        30mH                         Armatura induktivitas
(1kHz)
 Km                        2,093Vs/rad                  Mechanikai konstans
 Ku                        2,518 Nm/A                   Villamos konstans
 Iw                        0,30618 kgm^2                Tehetetlensegi nyomatek
 Tv                        33,33 * 10^-3                Villamos idoallando
 Tm                        263 * 10^-3                  Mechanikai idoallando
                            4-2. tablazat ID - motor, parameterek

[Kepek ezen a PDF-oldalon:]
[images/oldal_040_kep_004.png]

================================================================================
PDF OLDAL: 41
================================================================================

Brush Less DC (BLDC) motor felepites

A BLDC motor tulajdonkeppen egy kiforditott szinkron gep. A tekercseleset
harom fazis alkotja, amely csillag kapcsolasban van kotve. A kulso forgoreszt
46db polus (allando magnes), mig a tengelyhez rogzitett alloreszen 3*17db
tekercs helyezkedik el. A forgoresz poziciojat a beepitett hall szenzorokkal
lehetseges erzekelni.

                                                                  Kulso
                                                                  forgoreszen
allandomagnesek
                                                                  (46db)

                                                                 Tengelyhez
  Hall                                                           rogzitett
alloresz
                                                                 tekercsek
(51db)
  szenzorok

                      4-2. abra ID-motor, BLDC belso felepites

[Kepek ezen a PDF-oldalon:]
[images/oldal_041_kep_005.png]

================================================================================
PDF OLDAL: 42
================================================================================

BLDC motor hajtas

A mereshez a motor hajtasat a Microchip MCLV fejlesztokartyajaval az
AN957 alkalmazassal oldom meg.
 Hajtas tipusa            Unipolaris                           0-24V
 Uk szabalyozas           PWM also oldali                      39kHz
                     4-3. tablazat ID - motor, Hajtas adatok

                  4-3. abra ID - motor, MCLV-hajtas vazlat [23]

[Kepek ezen a PDF-oldalon:]
[images/oldal_042_kep_006.png]

================================================================================
PDF OLDAL: 43
================================================================================

BLDC motor modell

A BLDC motorhoz az egyenaramu motor modell hasznalhato [3] [5].

Uk

                  4-4. abra ID- motor, Egyenaramu motor modell

                 4-5. abra ID-motor, Iranyitastechnikai hatasvazlat

Egyenaramu motor egyenlet:

[Kepek ezen a PDF-oldalon:]
[images/oldal_043_kep_007.png]
[images/oldal_043_kep_008.png]
[images/oldal_043_kep_009.png]

================================================================================
PDF OLDAL: 44
================================================================================

A modellhez szukseges parameterek tehat a kovetkezok:
Ra, armatura ellenallas
Az egy fazistekercs ellenallasa. Mivel a csillagponthoz nem tudok hozzaferni,
ezert ket sorba kapcsolt tekercset lehet megmerni.
La, armatura induktivitas
Az armatura induktivitas meghatarozasa nem egyszeru feladat, mert az fugg a
frekvenciatol, a rotor poziciotol, homerseklettol, tengelyterhelestol, tehat
legres mezotol, stb. Elso kozeliteskent 1kHz-re szinuszos gerjesztesre.
Tv, villamos idoallando [5]
Tv = La / Ra, de a csillagkapcsolas miatt Tv = La / 3Ra
km [Nm/A], motor mechanikai konstans (Lorenz ero)
F_Lorenz = Ba*Ia*La
M = 2*Ba*Ia*La*r_motor_sugar
M = km*Ia, avagy
M = P / omega = Ua*Ia / omega = Km*phia*Ia = km*Ia
Tehat fel kell venni a motor nyomatek-aram fuggvenyet es a meredeksegbol
megallapitani a km nyomatek konstanst.
ku [Vs/rad], motor elektromos konstans
A Biot Savart torveny alapjan belathato:
Ui = 2*Ba*l_motor_hossz*v_keruleti
Ui = 2*Ba*l_motor_hossz*omega*Ra
Ui = 2*Ba*l_motor_hossz*Ra*omega
Ui = ku*omega. Ez az Ui negativan csatol vissza, azaz, ha no a fordulatszam,
csokken a sajat magara juto feszultseg.
---
Uk = Ia*Ra + Ui (allandosult allapotban)
Uk = Ia*Ra + ku*omega
Tehat a kapocsfeszultseg es a tekercsellenallas ismereteben a fordulatszamot
es az aramot megmerve megismerhetjuk a motorra jellemzo villamos
konstanst. A ku konstans a motor tekercseles parameterekbol is
meghatarozhato. [3]

================================================================================
PDF OLDAL: 45
================================================================================

Mechanikai idoallando [5]
Tm = Iw * 3Ra / (ku*km)
Az Iw tehetetlensegi nyomatek felfutasi idomeressel allapithato meg.
M = Iw * betaw

================================================================================
PDF OLDAL: 46
================================================================================

4.2.2 Ra, armatura ellenallas es La, armatura induktivitas

Meres celja
Ra, armatura ellenallas es La armatura induktivitas meghatarozasa.

Meres muszerei
 Nev                       Tipus                            Gyari szam
 Multimeter                MAS830                           S071100407
 Induktivitas mero         Urban Elektronika                DI003
                      4-4. tablazat ID-motor, Ra meres muszerek

Meres leirasa
A muszert a motor ket tekercskivezetesehez csatlakoztatva a muszerek altal
mutatott ertek leolvasasa.
Meresi eredmeny es meres kiertekelese
A meres sikeres, az eredmenyeket az alabbi tablazatban foglalom ossze.
 Armatura            2 Ra = 0,6Ohm         Ra = 0,3Ohm
 ellenallas
 Armatura            2 La = 60mH           La = 30mH              Kolcsonos
 induktivitas                                                     induktivitas
                                                                  elhanyagolva
                     4-5. tablazat ID - motor, Ra,La eredmenyek

================================================================================
PDF OLDAL: 47
================================================================================

4.2.3   Tv, villamos idoallando [5]

Tv = La / Ra, de a csillagkapcsolas miatt
Tv = La / 3Ra = 30mH / (3* 0,3Ohm)
Tv = 33,33 ms

4.2.4   'Km', motor mechanikai konstans meghatarozas

 Meres ideje                                   2016.03.20
 Meres helye                                   Obudai Egyetem- Szolo utcai labor
 Merest vegezte                                Dobany Imre
 Laborvezeto                                   Baba Janos
                   4-6. tablazat ID - motor, Km konstans meres fejlec

Meres celja
Meghatarozni a M = Km * Ia egyenletben szereplo km konstanst.
A meres tovabbi celja, hogy meghatarozza az alabbiakat:
   -    M(I) | n = allando,
   -    M(n) | I=allando,
   -    M(n) | Uk = allando.

================================================================================
PDF OLDAL: 48
================================================================================

Meresi osszeallitas

                      4-6. abra ID - motor, Km meres elrendezes 1

                      4-7. abra ID - motor, Km meres elrendezes 2

[Kepek ezen a PDF-oldalon:]
[images/oldal_048_kep_010.png]
[images/oldal_048_kep_011.jpg]

================================================================================
PDF OLDAL: 49
================================================================================

Meres muszerei

 Nev                   Tipus                                  Gyari szam
 Eromero cella         MOM SP 7926, 4kg,                      750367
                       class A
 Nyomatekmero muszer KVK
 Tapegyseg             TR-9178
                       EMG, 40V, 2A
 Gerjeszto tekercsek,  reztarcsaval szerelt
 fekpad                (nagyobb orvenyaram),
                       szgk. generator
                       ekszijtarcsa
                       dm=54,5mm;
                       dk=545mm
 Arammero (multimeter) MAS830                                 S071100407
 Oszcilloszkop         Tektronix TDS1002B
 Hall cella            BLCD motor sajat
                       cellaja
                     4-7. tablazat ID - motor, Km meres muszerei

Meres leirasa
Feladat: novekvo terhelonyomatek mellett merni a hozzatartozo motoraramot
konstans fordulatszam mellett. Mivel a fordulatszam esik a terhelo
nyomatekkal, ezert a motor kapocsfeszultseget novelni kell a motor
meghajton talalhato P1 potenciometer utan allitasaval.
A merest legalabb harom kulonbozo konstans fordulatszam munkapontban
vizsgalom.
 kerek              n min =                  n kozep =              n max =
 fordulatszam       52,17rpm                 74,53rpm               100,33rpm
 T hall             50ms                     35ms                   26ms
                   4-8. tablazat ID - motor, Km meres munkapontok

================================================================================
PDF OLDAL: 50
================================================================================

Meres lepesei:
   -   nyomatek terhelo  ->  gerjeszto tekercs aram beallit az adott diszkret
       ertekre (~0,1Nm lepesek),
   -   fordulatszam utan allit  ->  Um(pot)  ->  oszcilloszkopon hall elem jele,
       pl. 50ms konstans,
   -   aram leolvas,
   -   ciklus elolrol kezd.
Referencia fordulatszam megallapitasa
Mert ertekek:
 M mero fordulatszama kezi                       1299rpm
 muszerrel merve
 Hall elem jelenek periodus ideje                21ms
                 4-9. tablazat ID - motor, Km meres fordulatszam referencia

d_Mmero / d_ kerek = w_kerek / w_Mmero
50,5mm / 545mm = w_kerek / 1299rpm
n = 2 * (1 / T) * (1 / polus) * 60 [rpm]
n_kerek = 120,44rpm  ->  T_hall = 21ms (hall elem jele)

                       4-8. abra ID - motor, referencia fordulatszam

[Kepek ezen a PDF-oldalon:]
[images/oldal_050_kep_012.jpg]

================================================================================
PDF OLDAL: 51
================================================================================

Nyomatek attetel szamitas:
P_Mmero = P_kerek
M_Mmero * w_Mmero = Mkerek * w_kerek
M_Mmero * w_Mmero / w_kerek = Mkerek
M_Mmero * w_Mmero / ((d_Mmero / d_ kerek) * w_Mmero) = Mkerek
M_Mmero * d_ kerek / d_Mmero = Mkerek
(d_Mmero  ->  itt ~54,5mm, mert a szij felfutott az ekszijtarcsa peremere.)
M_Mmero * 545mm / 54,5mm = Mkerek
M_Mmero * 10 = Mkerek  ->  ezzel a 10-es szorzoval korrigalni kell a meresi
eredmenyt.

================================================================================
PDF OLDAL: 52
================================================================================

Meresi eredmenyek

M(I) | n = allando ->  dUk

                                                      M(I)
             7,00

             6,00                               y = 2,5178x - 0,8192
y = 1,9435x - 0,8872

             5,00

             4,00
                                                                             y =
1,648x - 0,9585
  M [ Nm ]

             3,00

             2,00

             1,00

             0,00
   -0,50             0,00   0,50      1,00     1,50     2,00       2,50    3,00
3,50     4,00      4,50
             -1,00

             -2,00
                                                       I[A]

                                          100rpm           74rpm          52rpm

                                   4-9. abra ID - motor, Km meres M(I)|n=all.

================================================================================
PDF OLDAL: 53
================================================================================

Ia     M mero            M kerek             T_hall            n
[A]    [Nm]              [Nm]                [ms]              [rpm]
4,00   0,555             5,55                26                100,33
3,22   0,440             4,40                26                100,33
2,40   0,309             3,09                26                100,33
1,60   0,165             1,65                26                100,33
1,06   0,082             0,82                26                100,33
0,70   0,013             0,13                26                100,33
0,53   0,003             0,03                35                74,53
0,85   0,082             0,82                35                74,53
1,26   0,158             1,58                35                74,53
1,82   0,272             2,72                35                74,53
2,72   0,445             4,45                35                74,53
3,38   0,560             5,60                35                74,53
0,36   0,000             0,00                50                52,17
0,61   0,069             0,69                50                52,17
1,04   0,184             1,84                50                52,17
1,24   0,240             2,40                50                52,17
1,70   0,351             3,51                50                52,17
2,17   0,465             4,65                50                52,17
2,68   0,585             5,85                50                52,17
         4-10. tablazat ID - motor, Km meres M ( I ) |n=all.

================================================================================
PDF OLDAL: 54
================================================================================

M(n) | I = allando

                                              M(n)
             7,000

             6,000

             5,000

             4,000
  M [ Nm ]

             3,000

             2,000

             1,000

             0,000
                     40   50        60            70       80          90
100   110
                                                   n [ rpm ]

                          M_1A           M_1.6A         M_2.6A
M_3.3A

                           4-10. abra ID- motor, Km meres M(n) | I = all.

================================================================================
PDF OLDAL: 55
================================================================================

Ia     M mero           M kerek              T_hall           n
[A]    [Nm]             [Nm]                 [ms]             [rpm]
0,53   0,003            0,030                35               74
0,36   0,000            0,000                50               52

0,85   0,082            0,820                35               74
0,7    0,013            0,130                26               100
0,61   0,069            0,690                50               52

1,04   0,184            1,840                50               52
1,06   0,082            0,820                26               100

1,26   0,158            1,580                35               74
1,24   0,240            2,400                50               52

1,6    0,165            1,650                26               100
1,82   0,272            2,720                35               74
1,7    0,351            3,510                50               52

2,17   0,465            4,650                50               52

2,4    0,309            3,090                26               100
2,72   0,445            4,450                35               74
2,68   0,585            5,850                50               52

3,22   0,440            4,400                26               100
3,38   0,560            5,600                35               74

4      0,555            5,550                26               100
         4-11. tablazat ID - motor, Km meres M (n) |Ia=all.

================================================================================
PDF OLDAL: 56
================================================================================

M(n) | Uk = allando

                                                M(n)
             7,00

             6,00

             5,00

             4,00
  M [ Nm ]

             3,00

             2,00

             1,00

             0,00
                    0,00   20,00       40,00         60,00        80,00
100,00   120,00
                                                   n [ rpm ]

                                   Pot_max         pot_mid          Pot_min

                            4-11. abra ID - motor, Km meres M(n) | Uk = all.

================================================================================
PDF OLDAL: 57
================================================================================

t_hall   n             M mero          M kerek          Ia          Potmeter
[ms]     [rpm]         [Nm]            [Nm]             [A]         [fok]
26,0     100,33        0,062           0,62             1,00        max
26,0     100,33        0,106           1,06             1,20        max
27,0     96,62         0,173           1,73             1,50        max
27,0     96,62         0,278           2,78             2,00        max
30,0     86,96         0,421           4,21             2,89        max
32,0     81,52         0,566           5,66             3,60        max
32,8     79,53         0,650           6,50             4,02        max

41,0     63,63         0,010           0,10             0,46        middle
44,0     59,29         0,071           0,71             0,75        middle
48,0     54,35         0,247           2,47             1,33        middle
52,0     50,17         0,356           3,56             1,73        middle
55,0     47,43         0,457           4,57             2,04        middle
58,0     44,98         0,537           5,37             2,30        middle
60,0     43,48         0,617           6,17             2,55        middle

56,0     46,58         0,010           0,10             0,35        min
58,0     44,98         0,030           0,30             0,40        min
59,6     43,77         0,075           0,75             0,52        min
62,4     41,81         0,137           1,37             0,68        min
69,0     37,81         0,265           2,65             10,20       min
76,0     34,32         0,380           3,80             1,36        min
82,0     31,81         0,460           4,60             1,56        min
88,0     29,64         0,522           5,22             1,67        min
90,0     28,99         0,568           5,68             1,80        min
94,0     27,75         0,605           6,05             1,86        min
96,0     27,17         0,632           6,32             2,00        min
             4-12. tablazat ID - motor, Km meres M(n) | Uk = all.

================================================================================
PDF OLDAL: 58
================================================================================

Meres kiertekelese

A fenti 'M(I)' karakterisztikabol kitunik, hogy
      -   az eredmenyek linearisak, ami az elvarasnak megfelelo,
      -   a kulonbozo egyenesek nem parhuzamosak, ami kulonbozo konstanst
          jelent, amely nem az elvarasoknak megfelelo,
Az egyenesek kulonbozo meredeksege nem szarmazhat surlodasos jellegu
hatas miatt, mert az az egyenes parhuzamos eltolodasat okozna. A meredekseg
valtozasa inkabb a terhelonyomatek altal okozott hajtas szij megnyulasabol
fakadhat. Feltetelezhetjuk, hogy a legkisebb megnyulas, es ezert a
legpontosabb km eredmeny, a legalacsonyabb fordulatszamnal adodik, mert
ekkor a legkisebb a mechanikai munka (W / t = P = M w).
A fentiek miatt: Km = 2,518 Nm/A

Nagysagrendi becsles:
P = 300W a motor nevleges teljesitmenye
Uk = 24V
n_max = 120rpm
R = 0,3Ohm
---
Ia = 300W / 24V = 12,5A
P=Mw
M = 300W / ((120rpm/60) * 2pi) = 23,79Nm
km = M / I = 23,79 Nm / 12,5A = 1,9 Nm / A

A becsult es a mert km parameterek nagysagrendileg egyeznek.
1,9 Nm / A ~ 2,518 Nm / A

================================================================================
PDF OLDAL: 59
================================================================================

4.2.5   'Ku' motor villamos konstans meghatarozas

 Meres ideje                               2016.09.18
 Meres helye                               1146 Bp. Bazsarozsa utca
 Merest vegezte                            Dobany Imre
                      4-13. tablazat ID - motor, Ku meres fejlec

Meres celja

Meghatarozni az Ui = Ku w egyenletben szereplo Ku konstanst.

Meresi osszeallitas

                      4-12. abra ID - motor, Ku meres elrendezes

[Kepek ezen a PDF-oldalon:]
[images/oldal_059_kep_013.png]

================================================================================
PDF OLDAL: 60
================================================================================

Meres muszerei

 Arammero/multimeter           MAS830           S071100407
 Oszcilloszkop                 Tektronix
                               TDS1002B
 Hall cella                    BLCD motor sajat
                               cellaja

                    4-14. tablazat ID - motor, Ku meres muszerei

Meresi leiras

   -   'Ra' meghatarozasa egyenaramu meressel, figyelembe veve, hogy ket
       kivezetesen ket tekercs van sorba kapcsolva a csillagkapcsolas miatt,
   -   maximalis fordulatszammal hajtani a kereket uresjarasban,
   -   merni a folyo atlag aramot,
   -   merni a hall cella jelet a fordulatszam mereshez,
   -   merni a kapocsfeszultseget.

Meresi eredmenyek

Ra = 300mOhm
Uk = 26,4V
I = 365mA
T_hall = 47Hz (szimmetrikus jel).

================================================================================
PDF OLDAL: 61
================================================================================

Meres kiertekelese

Uk = I 2Ra + Ui
Uk = I 2Ra + Ku w
Uk = I 2Ra + Ku (2pi * f_kerek)
Uk = I 2Ra + Ku (2pi * (f_hall * 2 / polus)
Uk = I 2Ra + Ku (2pi * (f_hall * 2 / 46))
26,4V = 0,365A * 0,6Ohm + Ku * (2pi*(47Hz * 2 / 46))

Ku = 2,039 Vsec/rad

Nagysagrendi becsles:
P = 300W
Uk = 24V
n_max = 136rpm
Ra = 0,3Ohm
---
Ui = Ku*w
Ui = Uk - I*Ra = 24V - 12,5A * (0,3*2)=20,25V
Ku = 20,25V / (136rpm *2Pi/60) =
1,4218 V*sec/rad  <-  ~  ->  2,039 V*sec / rad               Rendben

Tovabbi nagysagrendi ellenorzes lehet, hogy a ku es km ertekek kozel
azonosaknak kell lenniuk. sok esetben nem is szamolnak kulon veluk.
Ku = 2,039 Vs/rad ~ Km =
2,518 Nm/A                                             Rendben

================================================================================
PDF OLDAL: 62
================================================================================

4.2.6 Kerek tehetetlensegi nyomatek merese
A forgokerek sulyabol szarmazo nyomatek befolyassal van az
iranyitastechnikai jellemzokre. Ebbol kifolyolag szukseges a kerek
tehetetlensegi nyomatekat meghatarozni. Ez nem egyszeru feladat, mert a
kereknek nem egyszeru a geometriaja, eppen ezert ezt meressel hatarozhato
meg a legkonnyebben.
Az Iw tehetetlensegi nyomatek felfutasi idomeressel allapithato meg.
SigmaM = Iw * betaw
Azonos motor nyomatek, de kulonbozo terhelonyomatek mellett
termeszetesen kulonbozo felfutasi idok lesznek, melynek idofuggvenyei
exponencialisak azonban az Iw erteketol linearisan fuggnek, tehat:
SigmaM = Iw * betaw1
SigmaM = Iw * betaw2
Tehat meg kell merni a kerek fordulatszam felfutasi idejet allo helyzetbol
maximalis ertekre terheles nelkul, majd ismert terhelessel. A felfutasi idok
valamint a terhelo sulyok ismereteben a kerek tehetetlensegi nyomateka jo
kozelitessel meghatarozhato. A terhelo sulyokat a kerek peremen a leheto
legjobban elosztva kell elhelyezni. [3]
Iw / d Iw = Tures / dT, ahol
d Iw = Iw_terhelt - Iw
dT = Tterhelt - Tures

================================================================================
PDF OLDAL: 63
================================================================================

Meresi elrendezes

                     4-13. abra ID - motor, Iw meres tachomotor

Meres leirasa:
   -   Apro DC motor hozzaszoritas a peremhez,
   -   kerek inditasa maximalis gyorsitassal, mikozben oszcilloszkoppal
       rogziteni a DC motor kapocsfeszultseget,
   -   terhelo sulyok felrogzitese kozel a peremhez 90 fokos elrendezesben,
   -   kerek inditasa maximalis gyorsitassal, mikozben oszcilloszkoppal
       rogziteni a DC motor kapocsfeszultseget.

Mert ertekek:
 T_ures                                      87ms
 T_terhelt                                   110ms
 Sulyok                                      4*270g
 Kerek atmero                                540mm
                    4-15. tablazat ID motor, Iw meres eredmenyek

[Kepek ezen a PDF-oldalon:]
[images/oldal_063_kep_014.png]

================================================================================
PDF OLDAL: 64
================================================================================

Idoallandok leolvasasa oszcilloszkop abrabol:

               4-14. abra ID - motor, Iw meres Terheletlen kerek felfutasi ido
merese

                 4-15. abra ID - motor, Iw meres Terhelt kerek felfutasi ido
merese

Kiertekeles:
Iw = d Iw * Tures / d T
Iw = 1,08kg * (0,54m/2)^2 * 87ms / (110ms - 87ms) = 0,30618 kgm^2

4.2.7 Mechanikai idoallando meghatarozasa
Ahogy azt mar korabban leirtam es a 12.4.1 alapjan:
Tm = Iw*3Ra / (Ku*Km) = 0,3kgm^2 * 3 * 0,3Ohm / (2,039Vs/rad*2,518Nm/A)
Tm = 53ms

[Kepek ezen a PDF-oldalon:]
[images/oldal_064_kep_015.jpg]
[images/oldal_064_kep_016.jpg]

================================================================================
PDF OLDAL: 65
================================================================================

4.3   Golyo a kereken szakasz jellemzese

A 3 fejezetben megalkotasra kerultek a rendszeregyenletek es azokbol a
rendszer allapotteres leirasa. A fejezet elejen identifikalasra kerultek a
rendszer elemei, mint golyo, kerek es motor. A rendszeregyenletek es a
parameterek ismereteben vizsgalhato a rendszer viselkedese ido es
frekvenciatartomanyban.
A Scilab program segitsegevel az allapotteres leirasbol atterhetunk atviteli
fuggvenyes leirasra "ss2tf()" paranccsal. A 'golyo a kereken' (BoW) szakasz
atviteli fuggvenye (BoW_tf = fi2 / Ua):

Elso lepesben az atviteli fuggveny ismereteben a Scilab program segitsegevel
Bode, Nyquist, polus-zero diagramot felrajzoltatom.

                       4-16. abra BoW szakasz - Bode diagram

[Kepek ezen a PDF-oldalon:]
[images/oldal_065_kep_017.png]
[images/oldal_065_kep_018.png]

================================================================================
PDF OLDAL: 66
================================================================================

                      4-17. abra BoW szakasz - Nyquist diagram

                     4-18. abra BoW szakasz - Polus-zerus diagram

Bode, Nyquist, polus-zerus kiertekeles:
A diagramokbol megallapithato, hogy a rendszer nem minimal fazisu, mert a
Bode diagram kezdeti emelkedo amplitudo karakterisztika szakaszhoz tartozo
faziskarakterisztika nem pozitiv erteku. A Nyquist diagramon talalhato olyan
polus, ami a jobb felsikon talalhato. A polus- zerus diagramon talalhato olyan
polus, ami a jobb felsikon talalhato, tehat a szakasz instabil, ahogy vartuk,
hiszen a golyo egysegugras bemeneti kapocsfeszultsegre legurul a kerekrol.
[15]

[Kepek ezen a PDF-oldalon:]
[images/oldal_066_kep_019.png]
[images/oldal_066_kep_020.png]

================================================================================
PDF OLDAL: 67
================================================================================

Golyo legurulasi ideje
Szuksegesnek tartottam megvizsgalni, hogy a golyo mennyi ido alatt gurul le
a kerek tetejerol. Ezt egyszeruen azzal modelleztem, hogy nulla bemenetre,
azaz kapocsfeszultsegre es paranyi kezdeti kiteritesre vizsgalom az 5 deg -os
linearizalt tartomany hatar szogelfordulashoz tartozo idot. A szimulaciot a 8.7
fejezetben ismertetett BALLUFF erzekelobvel vegzett meressel validaltam.

                     4-19. abra BoW szakasz - idoallando szimulacio

                 phi2[rad]

                    Kezdeti 0,06 deg  kb. 2,3sec alatt gurul le 5 deg -ig
                                                                      t[s]

                           4-20. abra BoW szakasz - idoallando

                   5 deg - hoz tartozo
                   feszultseg(tavolsag)

                   1069mV

                                          T=2,2s

                      4-21. abra BoW szakasz - idoallando validalas

[Kepek ezen a PDF-oldalon:]
[images/oldal_067_kep_021.png]
[images/oldal_067_kep_022.png]
[images/oldal_067_kep_023.png]

================================================================================
PDF OLDAL: 68
================================================================================


================================================================================
PDF OLDAL: 69
================================================================================

                5    PID SZABALYOZO TERVEZES

A mernok szamara a legkezenfekvobb megoldas egy iranyitastechnikai
problema megoldasara az elterjedt, a szakasszal sorba illesztendo PID
szabalyozo tervezese, ezert megvizsgalom ennek megvalositasi lehetosegeit.
A feherdobozos modszer eseten a felirt rendszermodell differencial
egyenleteibol az atviteli fuggveny es a Bode diagram konnyen megismerheto,
amint azt a 4.3 fejezetben bemutatasra kerult. A modell hianya eseten a Bode
diagram meresekkel is eloallithato. A szakasz Bode diagram ismereteben mar
elkezdheto a szabalyozo tervezes.

A PID szabalyozo instabil szakaszra valo tervezesenek szabalyszerusegei nem
azonosak a stabil szakaszra valo tervezes szabalyaival. A PID szabalyozot
90 deg -os fazistartalekra tervezem, hogy ne legyenek lengesek.
Cs=Ap * (1+sTi)/ * (1+sTd)/(1+sT1)
fi_n = -180 deg + 90 deg -55 deg  = -145 deg   ->  AdB = -59,52dB - 10dB =
-69,52dB  ->
fc = 13,14Hz
T1 = fc * sqrt(10)
1/Td = fc/sqrt(10)
1/Ti=1/Td*0,1
A szabalyozo es a szakasz nyilt hurku vizsgalata az kovetkezo eredmenyt adja.

[Kepek ezen a PDF-oldalon:]
[images/oldal_069_kep_024.png]

================================================================================
PDF OLDAL: 70
================================================================================

                                              fi_n

 5-1. abra Bow szakasz PID - Bode diagram

5-2. abra BoW szakasz PID - Nyquist diagram

[Kepek ezen a PDF-oldalon:]
[images/oldal_070_kep_025.png]
[images/oldal_070_kep_026.png]

================================================================================
PDF OLDAL: 71
================================================================================

                   5-3. abra BoW szakasz PID - polus zerus diagram

A diagramokbol az latszik, hogy a szakasz a szabalyozoval tovabbra is
instabil, tehat tovabbi hangolast kell vegezni. Az eddigiekbol latszik, hogy a
PID szabalyozo tervezese instabil szakaszra meglehetosen korulmenyes, ezert
a tovabbiakban az allapot visszacsatolasos modszert valasztom a szakasz
stabilizalasara. Ezutan valaszthatom ismet a PID szabalyozast vagy lehetoseg
lesz allapotfigyelovel megoldani a feladatot, amint azt latni fogjuk.

[Kepek ezen a PDF-oldalon:]
[images/oldal_071_kep_027.png]

================================================================================
PDF OLDAL: 72
================================================================================


================================================================================
PDF OLDAL: 73
================================================================================

                 6    ALLAPOTVISSZACSATOLAS

Az allapotteres szabalyozo tervezeset a [2]-es irodalomjegyzek alapjan
keszitettem. Amint azt a PID szabalyozo tervezesenel leirtam, a szakasz
instabil es szabalyozoval sem sikerult stabilissa tenni. Mivel ismerem a
szakasz modelljet, ezert lehetoseg van a rendszer allapotteres leirasara. Az
allapotteres leiras alkalmassa teszi a rendszert arra, hogy allapot
visszacsatolassal ugy modositsam a rendszerjellemzoket, hogy az stabil
legyen.

                     6-1. abra BoW szakasz - Allapot visszacsatolas

Az polusathelyezes soran, a rendszer sajatertekeit helyezzuk at mashova, azzal
a feltetellel, hogy a szakasz iranyithato. A visszacsatolo 'K' matrix szamolasa
Ackermann-keplettel szamolhato, ahol az uj polusok helyet onkenyesen
kijelolve az iranyithatosagi matrix segitsegevel meghatarozhato.

K = (0....0 1) Mc^-1 fi_c(A)

Ahogy azt korabban irtam, az Ackermann-keplet segitsegevel intuitiv es
iterativ hangolassal kell a polusokat meghatarozni. A "Linear Quadratic"
szabalyozoval azonban direktebb modon tudunk hatni a polusathelyezes soran
a K parameterre, melynek soran funkcional segitsegevel keressuk a
megoldasokat, hogy a polusok a 0-tol a legtavolabb keruljenek, amig meg a
szabalyozo kepes beavatkozni. Az LQ technikaval az allapot valtozokhoz (qi)
es a bemenetekhez (rj) sulyozast kell rendelni az alabbiak szerint:

[Kepek ezen a PDF-oldalon:]
[images/oldal_073_kep_028.png]
[images/oldal_073_kep_030.png]

================================================================================
PDF OLDAL: 74
================================================================================

A nagyobb suly eseten kisebb lesz az allapotvaltozo vagy beavatkozo jel
erteke. Az LQ szabalyozo szamolasa a Scilab programban tamogatott. A
sulyokat diagonal matrixba foglalva "full rank factirazition"-t vegzek, amiben
egy matrixot tobb matrix szorzatara bontok ugy, hogy: a tenyezok rangja
azonos legyen az eredeti matrix rangjaval. A Scilab programban az lqr()
fuggveny segitsegevel a 'K' matrix meghatarozhato. Az LQ szabalyozo
tervezeset a [11]-es irodalomjegyzek alapjan keszitettem, melynek Scilab
szkriptje a CD mellekleten megtalalhato.
A 'K' visszacsatolo matrix alkalmazasaval a rendszerem stabilla valik
(szimulacio). Ha minden allapotvaltozot (ww, w2, fi2) mernek, akkor a
szabalyozasi feladat elvileg megoldott. Tovabbi finomhangolassal PID
szabalyozot illeszthetnek a korbe. (rajz) Valojaban viszont nincs lehetosegem
w2-t merni (csak fi2-t derivalni, de akkor valojaban csak fi2 allapotvaltozot
mernem), es az ww-t is csak korlatosan, ezert allapotfigyelot kell terveznem.

================================================================================
PDF OLDAL: 75
================================================================================

                        7    ALLAPOTFIGYELO

7.1   Folytonos modell

Az allapot visszacsatolo tervezesekor lattuk, hogy mivel nincs lehetoseg
minden allapotvaltozot merni, ezert allapotfigyelot kell terveznem, amit a [2]-
es irodalomjegyzek segitsegevel vegeztem.

                        7-1. abra BoW szakasz - Allapotfigyelo

A feladat, hogy az osszes allapotvaltozo erteket eloallitsuk annak ellenere,
hogy csak az egyik all rendelkezesre a meres alapjan. A figyelo matematikaja
szerint:
~x = Fx~ + Gy + Hu
A 'C' matrixban meghatarozhato, hogy mely allapotvaltozot merem. Az
allapotfigyelot a 'H', 'G' oszlopvektorok es 'F' matrix alkotja, ahol 'G'
szamitasi technikaja azonos az allapot visszacsatolasnal leirtakkal, de most a
figyelo bemenete a 'C' transzponalt es az 'A' matrix helyett annak
transzponaltjat kell hasznalni. Tovabba
H = B,
F = A -GC
Az LQ szamolast szinten alkalmazhatom ebben az esetben is.
A alapjel miatti korrekciot nem vettem figyelembe, mert a fi2-t, a golyo
poziciot, nullara szabalyozom. A kesobbiekben lehetseges lenne lassan
valtozo alapjel korrekcioval valamilyen trajektoria menten billegtetni a golyot
a kerek tetejen.
Az allapotfigyelo tervezeshez tartozo Scilab szkript a CD mellekleten
megtalalhato. A kesobbiekben a megfigyelovel ellatott rendszer mukodeset
szimulacioval ellenorzom.

[Kepek ezen a PDF-oldalon:]
[images/oldal_075_kep_031.png]

================================================================================
PDF OLDAL: 76
================================================================================

7.2   Diszkret modell

Mivel a szabalyozot kontrollerrel valositom meg, ezert a szabalyozo diszkret
ideju lesz. A figyelo bemeneten a golyo pozicio A/D atalakitas utan kerul a
szabalyozoba. A szabalyozo kimeneten D/A atalakitas tortenik, de ezt a
beavatkozo motor induktivitasa vegzi el a rakapcsolt negyszogjel kitoltesi
tenyezojenek aranyaban. A szabalyozo mintat vesz idokozonkent a szakasz
allapotabol, amely tulajdonkepp egy holtidos tagot jelent a rendszer szamara.
A holtidos tag atviteli faziskarakterisztikaja exponencialisan negativ elszallo
jellegu a ritkabb mintavetelek eseten. A gyakorlatban elfogadott Nyquist
kriterium otszorose, tehat a szakasz legkisebb idoallandojanak tizedresze
eseten a mintavetel okozott holtido fazistolasa elhanyagolhatova valik [12],
ezert tovabbra is a folytonos modellel dolgoztam tovabb.

                        7-2. abra Diszkret modell - teljes modell

Az alabbi tablazatban osszefoglalom, a mintavetelezesi idot befolyasolo
idoallandokat es a valasztott mintavetelezesi idot is szerepeltetem.
 Megnevezes         Idoallando                Megjegyzes            Fejezetszam
 Golyo a kereken    2s                                              4.3
 szakasz
 Motor felfutas     87ms                                            4.2.6
 Erzekelo           10ms                      2,5ms*4 minta         8.7
                                              futo atlag
 Mintavetelezes     10ms
                      7-1. tablazat Diszkret modell - idoallandok

Amennyiben szukseges a modell a mintaveteles allapotfigyelovel tovabb
tokeletesitheto.[2]

[Kepek ezen a PDF-oldalon:]
[images/oldal_076_kep_033.png]

================================================================================
PDF OLDAL: 77
================================================================================

               8   SZABALYOZO MEGVALOSITAS

A korabbi fejezetben kivalasztott szabalyozo megvalositasa a feladat. A
szabalyozo tulajdonkeppen nehany szorzobol es osszegzobol all, amely a
bemeneti golyo poziciohoz kiszamolja a fazisterben a hozza tartozo kimeneti
motor kapocsfeszultseget, hogy az egyensulyi allapot letrejojjon. A feladat
kiiras szerint beagyazott kornyezetben kell megoldani a feladatot, habar
lehetseges lenne valos ideju szamitogepes kornyezetben is (pl. RTLinux),
megfelelo periferiak alkalmazasaval.
Napjainkban az iranyitastechnikai feladatok megoldasara altalanosan
elterjedtek a numerikus szamito eljarasokat hasznalo es forraskod generalo
szoftverek, mint peldaul a Matlab/Simulink vagy a nyilt forraskodu
Scilab/Xcos [17] es X2C [9] programok. Dolgozatomban Scilab programmal
vegzem el a matrixok szamitasat, Xcos programmal keszitem el a
hatasvazlatot, X2C library-val generalom a C nyelvu forraskodot es a
matematikai szimbolikus szamitasokhoz a Maxima [30], szinten nyilt
forraskodu programot hasznalom fel. A feladatkiirasban szinten
meghatarozott Microchip-MCLV fejlesztokartya alkalmas a feladat
megoldasara, amelyen a motor hajtas elektronika es a szabalyozot
megvalosito kontroller is elhelyezkedik. A kontroller alkalmassagat, sebesseg
es architektura tekinteteben is megvizsgaltam es megfelelonek talaltam, habar
a tovabbi szabalyozo hangolas soran a jelenlegi 40MIPS sebesseg novelese
erdekeben masik, gyorsabb kontrollerre erdemes valtani.
Esetemben egy elektromos kerekpar 26 col-os tomlo nelkuli felni gumival
bevont peremein gurul egy biliardgolyo. A kereket BLDC motor hajtja, hall
szenzorokkal ellatva. Az iranyitastechnikai problemak eseten a feladathoz illo
beavatkozo szervo motort valasztanak jellemzo parameterek alapjan
katalogusbol. Ezek a motorok jellemzoen hurka alakuak, hogy a
tehetetlenseguk kicsi legyen a legresnyomatekhoz kepest. A feladathoz
alkalmazott kerekpar hajtas motor nem szervomotor, az adatlapja sem all
rendelkezesre, igy iranyitastechnikai jellemzoi nem ismertek, ezert az
identifikalni kell, amely kifejtve a 4.2. fejezetben talalhato.
A golyo poziciojanak merese lezeres tavolsagmerovel tortenik, amelynek
analog feszultsegkimenetet AD atalakitoval dolgozza fel a rendszer.

================================================================================
PDF OLDAL: 78
================================================================================

8.1       Fejlesztoi rendszer

A feladatkiiras szerint a nyilt forraskodu Scilab/Xcos es az X2C
programcsomag hasznalata javasolt. A temahoz kotodo Interneten talalhato
irodalmat megvizsgalva [6][9][17] a fent nevezett programokat alkalmasnak
talaltam a szabalyozo megvalositasara.
A Scilab program egy numerikus szamitasok elvegzesere alkalmas
parancssoros program. Ez roviden azt jelenti, hogy a valosagban elofordulo
folytonos matematikai problemak koltseges es idoigenyes megoldasai helyett
megfelelo egyszerusitessel vegzi el a kijelolt szamitasokat. [27] A Scilab
program versenytarsai peldaul az Octave es Matlab programok. Az Xcos
program olyan grafikus feluletet hasznalo program, aminek segitsegevel a
hatasvazlatot megrajzolva vizsgalhato peldaul egy iranyitastechnikai
alkalmazas. A parameterek a Scilab munkakornyezetben kerulhetnek
kiszamitasra. Az X2C programcsomag a Scilab programhoz lett kifejlesztve,
amely alkalmas a kialakitott szabalyozo megvalositasara, es hangolasara.
A Scilab/Xcos es X2C programcsomag egyuttesen alkalmas
fejlesztorendszert alkot rendszervizsgalat, szimulacio-validacio, szabalyozo
megvalositas es hangolas szamara.
A fejlesztoi kornyezet az alabbi elemekbol all:
      -    MPLAB IDE a Microchip MCU futtathato allomany fejlesztesehez
      -    Scilab program a szabalyozo parameterek kiszamitasahoz,
      -    Xcos program a hatasvazlat grafikus osszeallitasahoz,
      -    X2C konyvtar, ami az Xcos programban hasznalva
               o kodgeneralasra es
               o Scope programmal belso allapot megjelenitesre hasznalhato.

 Scilab        Xcosban X2C                            X2C kodgenerator es
 konzol        blokkok                                kommunikator

                                                                     Microchip
                                                      X2C-           IDE: MPLAB
                                                      Szkop

                                8-1. abra Fejlesztoi rendszer

[Kepek ezen a PDF-oldalon:]
[images/oldal_078_kep_034.jpg]

================================================================================
PDF OLDAL: 79
================================================================================

A fejlesztes lepesei soran:
   -   Maxima program segitsegevel a modell egyenletek alapjan a
       differencial egyenletek es a linearizalas utan az allapotteres leiras
       egyutthato konstansai (LTI rendszer) kifejezesre kerulnek. A
       Maxima szkriptek a CD mellekleten talalhatoak.
   -   Scilab program segitsegevel a vektorok es matrixok kiszamitasra
       kerulnek. A Scilab szkriptek a CD mellekleten talalhatoak.
   -   Szimulacio eseten az Xcos program grafikus feluleten osszeallitott
       hatasvazlatot futtatva, a Scilab program "workspace"-ben definialt
       valtozokkal lehet dolgozni. A valos hardverben felhasznalt X2C
       blokkokkal szimulalva a rendszert valoban azt a forraskodot
       tesztelhetem, ami a kontrollerben fog futni. Az Xcos forrasfajlok a
       CD mellekletben megtalalhatoak.
   -   Az X2C kommunikator programjat hasznalva forraskodot kell
       generalni. Ehhez az X2C projektet es az MPLAB projektet azonos
       konyvtarstrukturaba kell helyezni.
   -   MPLAB IDE-ben az uj modell forraskodot le kell forditani
       (compile).
   -   A sikeres kodforditas utan a futtathato allomanyt le kell tolteni az
       MCU-ba. Az MPLAB projektfajlok es futtathato allomanyok a CD
       mellekleten megtalalhatoak.
   -   MPLAB IDE kornyezetben lehetseges a futo kodban hibat keresni,
       "break-point"-kat elhelyezni.
   -   Az X2C csomag lehetove teszi, hogy soros porton keresztul a
       hatasvazlat allapota vizsgalhato legyen, amit a "Scope" alkalmazas
       valosit meg. Ez a lehetoseg a X2C kommunikator programban erheto
       el, miutan az Xcos-ban a "Transform model and push to
       communicator" akcio megtortent.
   -   A stabil allapotokat javasolt a verziokoveto programmal az SVN
       szerverre menteni, melynek importalhato fajljai szinten a CD
       melleleten megtalalhatoak.
   -   Valtoztatas eseten a megfelelo reszre ugorva, modell-Maxima,
       hatasvazlat-Xcos, stb. a valtoztatast elvegezve lephetunk tovabb
       ujabb iteraciot hajtva vegre a tokeletesitesben.

================================================================================
PDF OLDAL: 80
================================================================================

8.2   Szimulacio

Ebben a fejezetben megvizsgalom a rendszert szimulacioval, amely azert
hasznos, mert kisebb idoraforditassal tudom tokeletesiteni a modellemet.
A szimulacio keszitesekor figyelembe kell venni, hogy a szamito
algoritmusok hogyan mukodnek a hatterben. A szamitogep diszkret idokkel
dolgozik, tovabba a Scilab program numerikus (nem analitikus) eljarasokkal.
Dontest kell hozni, hogy a szabalyozot es a szakaszt folytonos vagy diszkret
idoben modellezzem. A kovetkezo tablazatban osszefoglalom az alkalmazott
szimulacios technikakat es azok fontos jellemzoit.
                   Folytonos szakasz                       Diszkret szakasz
 Folytonos         implementalva;                          Nem hasznalt;
 szabalyozo        vizsgalhato a szakasz es a              mintaveteles erzekelo
                   szabalyozott kor jellege;               es analog szabalyozo
                   a host szamitogep aritmetika            lenne;
                   (32,-64bit) eredmenyt
                   befolyasolja; [8]
 Diszkret          Implementalt;                           Nem implementalt;
 szabalyozo        f_sample > 5*Nyquist,                   mintaveteles
erzekelos
                   vagy analog erzekelos                   szabalyozo; a
szakaszt
                   szabalyozo;                             is mintavetelesen
kell
                                                           modellezni;
                        8-1. tablazat Szimulacio lehetosegei

Az alabbi tablazatban osszefoglalom a Scilab/Xcos program altal hasznalt
szamformatumokat. Fontos, hogy a parameterek a kulonbozo gepeken es
program verziokban is azonosan legyenek beallitva. Ennek ellenere a
kulonbozo operacios rendszereken eltero eredmenyek fordulhatnak elo.
 Desktop i3, 64bit                         Laptop i7, 64bit
 Win 8.1, 32bit                            Win 10, 64bit
 radix      =2                             radix                              =2
 digits    =53                             digits                            =53
 huge       =1.79D+308                     huge                      =1.79D+308
 tiny      =2.22D-308                      tiny                      =2.22D-308
 denorm       =T                           denorm                             =T
 tiniest   =4.94D-324                      tiniest                   =4.94D-324
 eps       =1.110D-16                      eps                       =1.110D-16
 minexp       =-1021                       minexp                         =-1021
 maxexp       =1024                        maxexp       =1024
                        8-2. tablazat Scilab szamformatumok

================================================================================
PDF OLDAL: 81
================================================================================

8.2.1 Szimulacio - Xcos
A korabbiakban folytonos idoben irtam fel a modellt. Latszolag a szimulacio
is valos idoben hajtodik vegre, azonban tisztaban kell lenni vele, hogy a
szamitogep diszkret idokkel es numerikus szamitasokkal kepes dolgozni, amit
a "Set Parameter" menuben lehet beallitani. Ugyelni kell arra, hogy a
szamitasi idoallandok joval kisebbek (<<10*), mint a vizsgalt tag idoallandoi.
Ekkor a szimulaciomat tekinthetem folyamatos idejunek.

                        8-2. abra Szimulacio Xcos - beallitasok

Az elozo fejezetben megfontoltak szerint folytonos ideju szakasz es folytonos
ideju szabalyozot alakitok ki az korabbi fejezetben kiszamolt A, B, C szakasz
es a H, G, F megfigyelo vektorok es matrixok felhasznalasaval.

                       8-3. abra Szimulacio, Xcos, folytonos ido

Szimulacio kiertekelese:
A szimulacio sikeres. Az eredmenyek azt mutatjak, hogy a kezdeti kilenditett
allapotbol (fi2 = 0,4 rad) visszaall a rendszer az egyensulyi allapotba.

[Kepek ezen a PDF-oldalon:]
[images/oldal_081_kep_035.png]
[images/oldal_081_kep_036.png]

================================================================================
PDF OLDAL: 82
================================================================================

   Ua[V]
                                            Ellentetes iranyu kapocsfeszultseg

  t[s]

   fi2[rad]
                                     Kibillentett kezdeti allapot

                                                  Beallt egyensulyi allapot

    t[s]

                     8-4. abra Szimulacio - Ua(t); fi2(t), folytonos ido

  dww[rad/s]
                     dww
  dw2[rad/s]

  dfi2[rad]               dfi2, figyelo kezdeti nagy elteres a szakasztol

                                               dw2

                                                                       t[s]
nagyitott

                8-5. abra Szimulacio - Figyelo hibajel, folytonos ido

               fi2
ww[rad/s]
w2[rad/s]
 fi2[rad]

                     w2

               ww

         t[s]

                             8-6. abra Szimulacio, Figyelo kimenet

[Kepek ezen a PDF-oldalon:]
[images/oldal_082_kep_037.png]
[images/oldal_082_kep_038.png]
[images/oldal_082_kep_039.png]

================================================================================
PDF OLDAL: 83
================================================================================

8.2.2         Szimulacio - X2C

Az elozo fejezetben megvalositott folytonos ideju szimulacio azt mutatja,
hogy az elmeleti modell es a szamolt szabalyozo parameterek hozzak az elvart
stabil mukodest es a golyo kepes a kerek tetejen maradni. A szimulaciot
azonban el kell vegezni azokkal az X2C iranyitastechnikai blokkokkal,
amelyekkel a valosagos szabalyozo futni fog. A szakaszt tovabbra is az Xcos
tagokkal szimulalom folytonos idoben, feltetelezve, hogy ez megfelelo
kozelitest ad annak ellenere, hogy diszkret ideju. Tehetem ezt azert, mert
teljesul a tulmintavetelezes, ahogyan azt a fejezet elejen irtam.
Az X2C blokkok egyik korlatja, hogy nincsenek matrix alakot kezelo blokkja,
hanem azokat alapmuveletekkel kell megvalositani. Az aprolekos, faraszto es
ezert konnyen elhibazhato sok ismetelt lepesen kivul ez mas problemat nem
okoz. Az azonos jellegu muveleteket ugynevezett kozos szuperblokkba lehet
foglalni, hogy atlathatobb legyen a modell. A tagok parametereit
betuszimbolumokkal lehetseges azonositani, amelyet a Scilab "workspace"-
ben kell definialni, szinten matrix elemenkent, amint az a forraskodban is
latszik. A szimulacio futtatasa elott tehat le kell futtatni a Scilab szkritptet
ami
a CD mellekleten talalhato.
Szabalyozo kialakitasa elemi matrix muveletekkel:
dx
      = Fx + Gy + Hu
dt
      G11                     H11                 F11 F12 F13 x11
Gy = [G21 ] phi2 ;        Hu = [H21 ] Ua ;   Fx = [F21 F22 F23 ] [x21 ]
      G31                     H31                 F31 F32 F33 x31

dx    F11 F12 F13 x11         G11         H11
    = [F21 F22 F23 ] [x21 ] + [G21 ] phi2 + [H21 ] Ua
dt     F F F          x        G           H
         31   32   33      31      31         31

       F11 x11 + F12 x21 + F13 x31 + G11 phi2 + H11 Ua            F11 x11 + F12
x21 + F13 x31 + G11 phi2 + H11 Ua
dx
    = [F21 x11 + F22 x21 + F23 x31 + G21 phi2 + H21 Ua ]; x =  integral  [F21
x11 + F22 x21 + F23 x31 + G21 phi2 + H21 Ua ] dt
dt
       F31 x11 + F32 x21 + F33 x31 + G31 phi2 + H31 Ua            F31 x11 + F32
x21 + F33 x31 + G31 phi2 + H31 Ua

                            integral (F11 x11 + F12 x21 + F13 x31 + G11 phi2 +
H11 Ua ) dt
Ua = Kx = [K11 K12 K13 ] [ integral (F21 x11 + F22 x21 + F23 x31 + G21 phi2 +
H21 Ua )dt ]
                            integral (F31 x11 + F32 x21 + F33 x31 + G31 phi2 +
H31 Ua )dt

Ua = [K11  integral (F11 x11 + F12 x21 + F13 x31 + G11 phi2 + H11 Ua ) dt] +
[K12  integral (F21 x11 + F22 x21 + F23 x31 + G21 phi2 + H21 Ua ) dt] +
[K13  integral (F31 x11 + F32 x21 + F33 x31 + G31 phi2 + H11 Ua ) dt]

================================================================================
PDF OLDAL: 84
================================================================================

                          8-7. abra Szimulacio, X2C BoW

Szimulacio kiertekelese
A diagramokbol      lathato,   hogy     a    szabalyozasi   minoseg   fugg   a
szamabrazolastol.
A 64 bites float szammal alkotott blokkok helyes eredmenyt hoznak, a
kimenet egyensulyi pontba all es marad. A 32 bites fix pontos
szamabrazolassal alkotott blokkok eseten a kimenet szinten beall es marad az
egyensulyi korul, azonban ezt a motor folyamatos beavatkozasa mellett teszi.
A 16 bites fix pontos szamabrazolassal keszult blokkok nem kepesek az
egyensulyba tartani a kimenetet. Kezdetben ugyan sikerul 0 deg korul tartani a
golyot, de az 6 masodperc utan legurul a kerekrol.
A kisebb felbontasu szamolas eseten az integralas soran halmozodo
pontatlansag lehet az elszallas oka, amelynek okait a tovabbiakban vizsgalni
kell. Az integratorok kimeneteit reszletesen vizsgalva felderitheto, hogy a
kulonbozo esetekben hogyan alakul a hibajel, amelynek a nullahoz kell
konvergalnia, amit nyilvanvaloan nem tortenik meg.
Az azonban megallapithato, hogy a legnagyobb felbontasu szamabrazolast
erdemes hasznalni. A lehetosegeket a kontroller kodmemoria merete es
sebessege befolyasolja. A nagyobb felbontas, nagyobb kodmeretet es lassabb
futasi idot eredmenyez, ami mintavetelezes gyakorisagat csokkentheti.

[Kepek ezen a PDF-oldalon:]
[images/oldal_084_kep_040.png]

================================================================================
PDF OLDAL: 85
================================================================================

Szimulacio eredmenyei

                Ellentetes polaritasu kapocsfeszultseg

                           Kiteritett allapotbol egyensulyba all

                     8-8. abra Szimulacio, X2C - 64bit, float

                      Ellentetes kapocsfeszultseg es allandosult
                      jellegu lenges

                    Kiteritett allapotbol egyensulyba all es marad

                       8-9. abra Szimulacio, X2C - 32bit, fix

                         Ellentetes kapocsfeszultseg, majd lecsengo beallas

                     Kezdeti kiteritett allapotbol egyensulyba, majd elszall

                      8-10. abra Szimulacio, X2C - 16bit, fix

[Kepek ezen a PDF-oldalon:]
[images/oldal_085_kep_041.png]
[images/oldal_085_kep_042.png]
[images/oldal_085_kep_043.png]

================================================================================
PDF OLDAL: 86
================================================================================

Szimulacio megvalositas / Diszkret integrator

                    8-11. abra Szimulacio X2C - Diszkret integrator

               Ti / Ts

                                                                         Ti
                         8-12. abra Szimulacio X2C - Integralasi ido

                                                        telitodes, max. ertek
                                                        1
                                 Integrator ertek valojaban

                                      a megadott orajel duplaja,
                                      ezzel korrigalni kell a
                                      kapcsolasi idot

                    8-13. abra Szimulacio X2C - Integrator felbontas

Kovetkeztetes: amennyiben a folytonos ideju szimulacioban alkalmazott
integratorral azonos egyseg ideju integratort (Ti = 1s) alkalmazok, akkor az
integralasi parametert Ki = 2*Ts/Ti szerint kell valasztani.

[Kepek ezen a PDF-oldalon:]
[images/oldal_086_kep_044.png]
[images/oldal_086_kep_045.png]
[images/oldal_086_kep_046.png]

================================================================================
PDF OLDAL: 87
================================================================================

Szimulacio megvalositas / X2C szamabrazolas es normalizalas
A korabbiakban mar volt arrol szo, hogy Scilab 64bites float szamabrazolast
hasznal. Az X2C blokkok eseteben meg kell hatarozni, hogy milyen
szamabrazolast hasznaljon. A szimulaciot teszteltem 16 es 32 bit fixpontos es
64bit lebegopontos (float) szamabrazolassal. A fixpontos szamabrazolasok
hasznalata eseten az Xcos (szakasz) es X2C (szabalyozo) tagok kozott szam
normalizalasra van szukseg. Az X2C tag a fixpontos -1 es 1 kozotti szamot a
bit szamnak megfelelo felbontassal abrazolja [9].
Szabalyozo bemenet normalizalasa:
fi2_X2C = fi2(rad) * Gain
Gain = fi2_X2C / (fi2 * 2pi)
pl. max.:
Gain = 1 / 6,28 = 0,159235668, tehat ezzel a szabalyozo bemenetet szorozni
kell.
Szabalyozo kimenet normalizalasa:
U = Gain * X2C_kimenet
Gain = U / X2C_kimenet
pl. max.: Gain = 24V / 1V = 24, tehat az X2C kimeneti eredmenyt 24-el
szorozni kell.

================================================================================
PDF OLDAL: 88
================================================================================

8.2.3 Program felepites tervezes

A program felepitesnek ket nagy alkoto resze van
   -   be es kimenetek illesztese, erzekeles, hajtas es monitorozas,
   -   iranyitastechnikai modell.

                         8-14. abra Program felepites reszei

A modell, a szabalyozo parameterek, a szukseges erzekelo bementek es
beavatkozo kimenetek ismereteben szoftver architekturat es eroforras
kiosztast kell valasztani. Az X2C kooperativ multitaszk jellegu rendszerben
lett megvalositva. A main ciklusban az inicializalas utan a soros RS232
kapcsolat fenntartasahoz szukseges fuggveny hivodik vegtelen ciklusban. Az
X2C iranyitastechnikai modellt megszakitasban kell hivni definialt
idokozonkent.
                                                               Interrupt

                                                               mintaveteli ido

                     Soros

                     Kapcsolat

                      8-15. abra Program felepites - X2C igeny

[Kepek ezen a PDF-oldalon:]
[images/oldal_088_kep_047.png]
[images/oldal_088_kep_048.png]

================================================================================
PDF OLDAL: 89
================================================================================

8.3   MCU valasztas

A szabalyozo megvalositasa a kontrollerben tortenik. Az Xcos program
grafikus feluleten megszerkesztett hatasvazlat az X2C kommunikator
programjaban generalodik C forraskodda, majd az MPLAB IDE-ben
leforditott kod a kontrollerbe kerul. A kontroller valasztast a feladatban
szereplo tenyezok eroforrasigenyei hatarozzak meg [18], elsosorban a szakasz
maga. A kontroller valasztast a rendelkezesre allo lehetosegek is
befolyasoljak. Az elodom altal hasznalt MCLV fejlesztokartyan Microchip
dsPIC33FJ32MC204 tipusu kontroller van, amely 40MIPS sebessegre kepes.
A rendelkezesre allo korabbi forraskodok es a Microchip ceg altal publikalt
alkalmazas forraskodok kezenfekvove teszik ennek a tipusnak a hasznalatat.
Kesobbi fejleszteshez rendelkezesre all erosebb, dsPIC33EP256MC506 tipus
is, amely 70MIPS sebessegre kepes. Kesobbi fejezetekben vizsgalom a
sebesseg mellett a kodmemoria es a RAM szukseges meretet. A hardver
eroforrasok, mint peldaul A/D, idozitok, capture bemenetek mindket
kontrollerben rendelkezesre allnak.

8.4   Eroforras kiosztas

Az eroforras kiosztashoz ismerni kell a szakasz, szabalyozo es erzekelok
idoallandoit, amit az alabbi tablazatban foglalok ossze.

 Eszkoz                      Idoallando                        Megjegyzes
 Kontroller                  25ns                              40MIPS a
periferiakra
 X2C                         1kHz                              minimum
 Hall szenzor                500us                             differencia:
max.
betamax=1/(3*pp)[1/s^2]
 Tavolsag mero               2,5ms
 Enkoder attetellel          43us                              23kHz, nem
hasznalt
 Golyo szakasz               2s                                vegallasig
 Motor                       87ms                              Tm felfutas
                      8-3. tablazat Eroforras kiosztas - idoallandok

================================================================================
PDF OLDAL: 90
================================================================================

                            8-16. abra Eroforras kiosztas

A 8-16. abra Eroforras kiosztas szerinti modon osztom ki az eroforrasokat,
amit a kovetkezo tablazatban foglalom ossze.

 Nev       Parameter                Prioritas       Megjegyzes
           period / futasi ido
 A/D       25,4us / 2,8ns           4               [19], Golyo pozicio
 TMR3      6,4us / -                -               [21], ww
 TMR1      500us / 150us            4               [21], X2C
 IC1,2,7   (500us)                  5               [22], Hall, kerek pozicio
 PWM       25us / -                 -               [23], Motor kapocsfeszultseg
                           8-4. tablazat Eroforras kiosztas

Az A/D atalakitas a golyo poziciojanak erzekelesehez hasznalt. Ennek
periodusa legalabb kisebb, mint fele kell, hogy legyen, mint TMR1.
TMR3 szamlalohoz nincs megszakitas rendelve, hanem az a kerek
szogsebesseg meresehez hasznalt. Feladathoz elso megkozelitesben nincs ra
szukseg, azonban a kesobbi finomhangolasnal elofordulhat, hogy
alkalmazasra kerul, tovabba a PID fordulatszam szabalyozas proba
rendszerhez felhasznaltam.
A TMR1 idozito az X2C hatasvazlat hivasat vegzi. Ez a periodusido jelenti a
mintavetelezesi idot a szabalyozo szamara.

[Kepek ezen a PDF-oldalon:]
[images/oldal_090_kep_049.png]

================================================================================
PDF OLDAL: 91
================================================================================

Az IC1,2,7 "capture" megszakitasokhoz a hall szenzorok vannak rendelve,
amelyek a fazisok megfelelo sorrendu kommutaciojahoz hasznalt. Ebben a
megszakitasban a TMR3 szamlalo erteke felhasznalasra kerul, majd
nullazodik.
A PWM parameter a motor kapocsfeszultseg vezerleset vegzi.
Eroforras idozitesek merese:

                                         TMISPS=25ns

                       8-17. abra Eroforras - maximum MIPS

                   TTMR1=500us            Trun=150us

                                     100us

                   8-18. abra Eroforras - TMR1- X2C megszakitas

[Kepek ezen a PDF-oldalon:]
[images/oldal_091_kep_050.png]
[images/oldal_091_kep_051.png]

================================================================================
PDF OLDAL: 92
================================================================================

8.5   X2C forraskod beagyazas

Az X2C program konyvtarnak ket interfesze van. Az egyik a Scilab fele a
hatasvazlat megalkotasahoz, masreszt a megvalositas reszerol a kodolast
tekintve. A Scilab oldali illesztest a szimulacioban megvizsgaltam es
megalkottam a szabalyozot. A forraskod oldali megvalositasban a
kommunikator segit, amivel a kodgeneralas tortenik. Az X2C csapata szamos
alkalmazas pelda kodot bocsajt rendelkezesre kulonbozo kontrollerekhez,
koztuk az altalam valasztott tipushoz is. Az X2C pelda projekt konyvtarban
helyezkedik el az MPLAB IDE projekt is, tehat a fejlesztes ebbol az allapotbol
konnyen elkezdheto.
A Program felepites tervezes fejezetben emlitesre kerult, hogy az X2C
kooperativ multitaszk jellegu program felepitessel rendelkezik. A main
fuggvenyben a soros kommunikacio feladat fut vegtelen ciklusban, mig a
szabalyozo logika megszakitasbol hivodik szigoru idozitessel, ami a
mintavetelezesi ido lesz, amit TMR1 vegez.
A megszakitasban a szabalyozo fuggveny hivas elott a szabalyozo bemenetek,
a hivas utan a kimenetek normalizalasa tortenik. Korabban a szimulacional
szoba kerult a szamformatumok kozotti normalizalas. Ott az Xcos es az X2C
hatasvazlati tagok kellett normalizalni. A kontrollerben az erzekelok es az
X2C be/kimenetei kozott szukseges ezt a normalizalast elvegezni, amit a
kesobbi fejezetekben reszletesen targyalok.
Az X2C programcsomag "Scope" nevezetu alkalmazasa a hatasvazlat
allapotainak vizsgalatara alkalmas. Ahogy az a nevebol is kovetkezik az
alkalmazas olyan, mint egy oszcilloszkop, tehat definialni kell a vizsgalt jelet
a csatornakon, ami lehet I/O vagy belso tagok kozotti kapcsolat. Definialni
kell a trigger tipusat, ami lehet normal, auto, "single shot", kesleltetett,
stb.. A
szkop alkalmazas fontos parametere a memoria nagysaga, amit a forditasi
parameterekben [9] lehet megadni, peldaul:
"#define SCOPE 1000"  ->  az itt meghatarozott meret fele lesz a vizsgalati
ido, ami a szkopban megjelenik, tehat 1ms mintavetelezes eseten 500ms.
Ebbol kovetkezoen a foglalt memoria meretet erdemes a vizsgalat jellegehez
igazitani. Lassabb folyamatokhoz nagyobb memoriara van szukseg, azonban
a tarolt memoria letarolasa, valamint lekuldese a PC-n futo kommunikatornak
hosszabb idot igenyel, ami terheli a kontrollert es zavarja a valos ideju
folyamatokat. Eppen ezert a soros kommunikaciot szukseges atirni alacsony
prioritasu megszakitasba es a gyorsabb kontroller hasznalata is
megfontolando.
Az X2C programcsomagnak vannak meg tokeletesitheto reszei. A szkop
alkalmazasban az ablak reszleteinek nagyitasa, finomhangolasa, valamint,

================================================================================
PDF OLDAL: 93
================================================================================

hogy "single shot" eseten a "delay" beallitas 0 ertekenel nem sikerul elkapni
a tirgget. Elofordul, hogy csak tobbszori probalkozasra indul el az alkalmazas.
Mindezek ellenere a termek tamogatas eros es sok problemat segitettek
megoldani a kollegak, amit ezuton is koszonok.
A szimulacioban targyaltam, hogy a matrix muveleteket elemi eszkozokkel
tudom megvalositani. Az integrator megvalositasaval is hasonloan jartam el,
hogy valoban belassam annak helyes mukodeset, amit a 8.2.2 fejezetben
vegeztem el. Megallapitottam, hogy az orajel ketszerese alatt lep az integrator.
A tovabbiakban is az elemi tagokbol felepitett integratorral dolgoztam.
A hardver interfesz a be es kimeneti portokon keresztul valosul meg. A
portokat a megfelelo cimkevel ellatva a forraskodban megjeleno valtozoba
kell az erzekelo normalizalt erteket beolteni.

         bemeneti cimke:                                       kimeneti cimke:
         golyo pozicio                                         kapocsfeszultseg

                        8-19. abra X2C hatasvazlat beagyazas

[Kepek ezen a PDF-oldalon:]
[images/oldal_093_kep_052.png]

================================================================================
PDF OLDAL: 94
================================================================================

8.6 Hajtas megvalositas
A hajtast a BLDC motoroknal altalanosan alkalmazott "six step" hajtassal
valositom meg, aminek kapcsolasat a 4-3. abra szerint mutattam. A hajtas
megvalositasara mezoorientalt szabalyozas (FOC) is szoba jott, ami a
nyomatek maximalizalas (90fok) erdekeben hasznos, valamint a panel
architektura miatt csak egy tipusu Capture Input lehetseges egy idoben, ami a
hall, BEMF vagy aram merest tesz lehetove, tehat esetleges enkoderes
szogsebesseg erzekeles eseten az FOC johet szoba. Jelenesetben nem
hasznalom, mert SISO modellt hasznalva, csak a golyo szogeltereset merve
nincs szukseg a kerek szogsebesseg meresere.
Hall szenzorok - lepesvezerles
A Microchip dsPIC33FJ32MC204 MCU I/O kontroller leirasa a DS70198
"appication note"-ban talalhato. Az MCLV2 panel kapcsolasi rajzabol
kiderul, hogy a B port 1,2,3 bitjeire vannak a hall szenzorok kotve, amelyeken
rendre az 1,2,7 "Input Capture" funkciok erhetoek el port-map beallitassal. Az
"ICxCON = 1" regiszter beallitasaval a hall szenzorok fel es lefuto elere is
triggereljuk a megszakitasokat. A megszakitas a "default" 4-es prioritasi
szinten marad.
PWM vezerles
Az Uk kapocsfeszultseg vezerlesere egyszeru kapcsolt PWM-et hasznalok, de
lehetoseg lenne trapez vagy szinusz alaku vezerles alkalmazasara is, ami
osszetettebb vezerlo algoritmust igenyelne. A hatasvazlatban a
kapocsfeszultseg a beavatkozo jel, azonban itt PWM jelle kell alakitani,
amelyet aranyos tagkent jelenik meg [28], ahogy azt az alabbi kepen az ISZM
modulatoros szaggato is mutatja. A kapcsolt PWM jel a motor induktivitasan
integralodik feszultsegge.

                                     Aranyos tag
                     8-20. abra Hajtas - PWM mint atviteli tag [28]

A PWM frekvencia valasztasat a motor villamos idoallandoja befolyasolja (>
1kHz), valamint, hogy a valasztott frekvencia ne legyen zavaro az emberi
fulnek (> 20khz), igy ez 40kHz-re adodott. A Microchip dsPIC33FJ32MC204
MCU PWM kontroller leirasa a DS70187 "appication note"-ban talalhato.

[Kepek ezen a PDF-oldalon:]
[images/oldal_094_kep_053.jpg]

================================================================================
PDF OLDAL: 95
================================================================================

Ebben a kontrollerben ket PWM vezerlo kozul a PWM1-et, a beallitasi
lehetosegek kozul a "Free runing mode"-ot hasznalom. P1TMR szamol felfele
amig P1TPER erteket el nem eri, majd P1TMR nullazodik. A P1TPER
maximalis erteke eseten a kitoltesi tenyezo 50%, tehat a 100% eleresehez
ketszeres erteket kell megadnom.
Uk / 24V = PWM_P1DC1 / 2000
        ->  K_PWM = 2000 [db / %] / 24 [V] = 83,33 [db / %V]
Uk / 24V = X2C_Uk / 32767
PWM_P1DCx / 2000 = X2C_Uk / 32767
PWM_P1DCx = 2000 * X2C_Uk / 32767

Az alabbiakban a 8-20. abra szerinti elrendezesben az Uk kapocsfeszultseg
PWM vezerles ellenorzeset vegeztem meressel.

                          8-21. abra Hajtas - hibas PWM

A fenti abran lathato jel nem negyszog alaku, ami hibas vezerlesre utal.
A hibakereses soran kiderult, hogy a P1TPER regiszterbe hibasan nagy ertek
kerul, aminek kovetkezmenyekepp a PWM frekvencia tul nagy, 166kHz, ami
az adott L/R ertekhez nem megfelelo.

[Kepek ezen a PDF-oldalon:]
[images/oldal_095_kep_054.png]

================================================================================
PDF OLDAL: 96
================================================================================

    8-22. abra Hajtas -PWM 10%

8-23. abra Hajtas - PWM 10%, nagyitas

[Kepek ezen a PDF-oldalon:]
[images/oldal_096_kep_055.png]
[images/oldal_096_kep_056.png]

================================================================================
PDF OLDAL: 97
================================================================================

    8-24. abra Hajtas - PWM 50%

8-25. abra Hajtas - PWM 50%, nagyitas

[Kepek ezen a PDF-oldalon:]
[images/oldal_097_kep_057.png]
[images/oldal_097_kep_058.png]

================================================================================
PDF OLDAL: 98
================================================================================

                       8-26. abra Hajtas - PWM 100%

A meresek alapjan a PWM vezerles megfeleloen mukodik.

[Kepek ezen a PDF-oldalon:]
[images/oldal_098_kep_059.png]

================================================================================
PDF OLDAL: 99
================================================================================

8.7   Golyo pozicio meres

                            8-27. abra Golyo pozicio meres

A golyo poziciojanak meghatarozasahoz lezeres tavolsagmerot hasznalok a
fenti elrendezesben. Az erzekelo diszkret mintavetelezessel dolgozza fel a
bemenetet, amelyet a 8-28. abra szemleltet. Az erzekelo altal hasznalt
mintavetelezes holtidos tagot jelent a rendszer szamara, amely a megfeleloen
lassu szakasz dinamika mellett elhanyagolhato fazistolast okoz. A muszer
kimenete muveleti erositon halad keresztul, ami 3,3V-ra illeszti a kimenetet a
kontroller A/D atalakitoja szamara. Az A/D erteke megszakitasban kerul
kiolvasasra, ahol futo atlagolas is tortenik. Erre azert van szukseg, hogy a
zavarasbol fakado hibas eredmenyek hatasa csokkenjen. A szuro eredmenye
hasznalhato a szabalyozo szamara, ami a minimum mintaveteli idot is
meghatarozza. A rendszer idoallandoit a 7.2 fejezetben foglaltam ossze.
                      8-28. abra Golyo pozicio meres- hatasvazlat

Az alabbi tablazatban osszefoglalom a pozicio meres jellemzo parametereit.
 Erzekelo tipus                              BALLUFF BOD 21M-LA04-S92
 Kimenet                                     Feszultseg
 Tavolsagerzekeles                           10mm-5000mm
 Erzekelo mintaveteli ido                    2,5ms
 Szures futo atlaggal                        4 minta
 Szabalyozo min. mintavetel                   >=  10ms
 Golyo a kereken szakasz idoall.             2s
                           8-5. tablazat Golyo pozicio meres

[Kepek ezen a PDF-oldalon:]
[images/oldal_099_kep_060.png]

================================================================================
PDF OLDAL: 100
================================================================================

BALLUFF erzekelo karakterizalas
Az alabbi meressel hataroztam meg a BALLUFF erzekelo gyorsasagat es azt,
hogy a golyo milyen gyorsan gurul le a kerekrol.
Az alabbi kepen lathato meresen az erzekelo elott egy gyufaszalat ejtettem
le, hogy megmerjem, az meresi ido gyakorisagat. Lathato, hogy az erzekelo
megfeleloen gyorsan reagal, ami elegendo a golyo legurulas meresehez az
4-21. abra szerint.

                           8-29. abra Pozicio meres - BALLUFF meresi ido

A/D atalakito kimenetenek felirasahoz a BALLUFF muszer karakterisztikaja
szukseges, amit kimerve, az alabbi eredmenyt kaptam.

                                 BALLUFF tavolsagmero
              1200

              1000
                                                            y = 4,7041x + 213,81
              800
     Ux[mV]

              600

              400

              200

                0
                     0    20      40      60      80     100     120     140
160   180
                                               Tavolsag[mm]

                         8-30. abra Tavolsag meres - BALLUFF karakterisztika

[Kepek ezen a PDF-oldalon:]
[images/oldal_100_kep_061.png]

================================================================================
PDF OLDAL: 101
================================================================================

 x[mm]                      Ux[mV]                          Nad
 0                          213                             66
 38                         390                             121
 81                         590                             183
 90                         640                             198
 99                         690                             214
 168                        952                             295
 244                        1361                            422
                      8-6. tablazat Tavolsag meres - eredmenyek

A tavolsagmero jelebol a golyo szogelfordulasat kell eloallitani, majd azt
normalizalni kell az X2C szamformatumra.
Elso kozelitesben a legegyszerubb, ha feltetelezem, hogy a golyo, sokkal
kissebb atmeroju, mint a kerek, tehat a merheto tavolsag kozel az 'S' ivhosszal
egyenlo az 5 deg -os linearizalt szakaszban. Ennel pontosabb szogelfordulas
meres az [1]-es irodalomjegyzekben talalhato.
A fenti feltetelezessel irhatom, ahol x a merendo tavolsag:
   I.      S = phi2*(rw+rb)
Tovabba feltetelezve, hogy golyo feluletebol fakado elteres elhanyagolhatoan
kicsi.
   II.     xmax = S + rb + x
   III.    Ux = 4.7x + 213
   IV.     Uref=3300mV
   V.      Ux / Nad = Uref / (2^10 - 1), mert az A/D atalakitas 10 bites.
   VI.     phi2 / 2pi = phi2_x2c / 32767, normalizalas.
 ----------
A fenti egyenletrendszert rendezve az alabbi osszefuggesre jutottam.
phi2_x2c = 5930 - 30 * Nad
Ha Nad > 422, akkor 422-vel csokkenteni es elojelet kell valtani.

A szkop programban ellenorizve sikeresen visszamertem a fenti szamolast. A
szkop program +/-0,001 diszkret erteket ugral, ami
phi2 = +/-0,36 deg ; x = +/-1mm; Ux = +/-4,7mV; +/-1digit ugralast jelent.

================================================================================
PDF OLDAL: 102
================================================================================

8.8 Fordulatszam meres
Meres celja:
A SISO modell nem koveteli meg a motor fordulatszam mereset, azonban a
kesobbi fejlesztes erdekeben a SIMO modellhez felhasznalhato lehet. A
fordulatszam meghatarozasahoz a hall szenzorok jeleit fogom felhasznalni.
Ehhez ismernem kell az aranyt a hall jel idozitese es a tengely szogsebessege
kozott, ami a polusok szamatol fugg, ami viszont az adatlapbol nem derul ki,
ezert a szogsebesseg meresre kerul az alabbi elrendezesben.

                    8-31. abra Hajtas - Omegaw meresi elrendezes

[Kepek ezen a PDF-oldalon:]
[images/oldal_102_kep_062.png]

================================================================================
PDF OLDAL: 103
================================================================================

 8-32. abra Hajtas - Omegaw meres Hurst - Hall egy mech. fordulaton

8-33. abra Hajtas - Omegaw meres Kerekpar - Hall egy mech. fordulaton

[Kepek ezen a PDF-oldalon:]
[images/oldal_103_kep_063.png]
[images/oldal_103_kep_064.png]

================================================================================
PDF OLDAL: 104
================================================================================

Gyorsulas meres maximalizalasa
Kihasznalva, hogy a hall szenzorok jele egymashoz kepest rogzitett,
valamint, hogy a szogsebesseget a hall megszakitasokban merem, nagyobb
                                                  mintavetel erheto el, ha
                                                  minden hall szenzor
                                                  megszakitasa uj sebesseg
                                                  meresi eredmenyt ad es a
                                                  mert erteket korrigalni
                                                  kell a harom fazissal,
                                                  tehat:
                                                           ww = 1 / (3 * PP*
Tpp)
                                                           betamax=ww/(Tpp/3)

Korabban emlitettem, hogy a SISO modell miatt csak a golyo poziciojat
merem, ezert nincs szukseg a kerek szogsebesseg meresere, azonban a tovabbi
fejlesztes erdekeben kifejlesztesre kerult, mert ily modon a SIMO modellel
(ww meres) finomithato a rendszer.
Meresi eredmeny:
          P1DC1      DC Ua Thall Tw                    n        PP betamax
betaw
                     [%] [V] [ms] [ms] [rpm]                          [rpm/s]
[rpm/s]
 Hurst    200        10      2,4 18           180      333,3
          1000       50      12 3,8           38       1579 10 139
3750
          2000       100 24 2                 20       3000
 Kerek-   20         10      2,4 120          5520 10,9
 par      1000       50      12 24            1104 54,3         46 7,5
1200
          2000       100 24 12                552      108,7
                  8-7. tablazat Hajtas - Fordulatszam meres eredmenyek

A tablazatbol kiderul, hogy messze nem kepes a hall szenzoros meres a kerek
gyorsulasait merni, azonban erdemes ellenorizni, hogy a szabalyozo
szimulacioban milyen maximalis gyorsulasok fordulnak elo. Ha szukseges,
akkor a motor szogsebesseg mereset enkoderes jeladoval kell megoldani.

[Kepek ezen a PDF-oldalon:]
[images/oldal_104_kep_065.png]

================================================================================
PDF OLDAL: 105
================================================================================

Normalizalas
A szimulacioban es az X2C beagyazasnal is szo volt mar a normalizalas
szuksegessegerol. Itt most megadom, hogy a szogsebesseget hogyan
normalizalom az X2C 16 bites es 32 bites szamformatumra. Az 'N' jeloli a
szamlalo erteket, mig TMR3 az idozito periodusat.
// ww = 2pi / (3 * N * TMR3 * polepair)
// x2c_rad_per_sec / x2c_max = ww / max_rad_per_sec
// x2c_rad_per_sec = 2pi * 32767 /(3*N*6,4*10e-6*46*418)
Inports.Omegaw_16bit = ((long)(284829) / (long)N);
// x2c_rad_per_sec = 2pi * 2147483647 /(3*N*6,4*10e-6*46*418)
Inports.Omegaw_32bit = ((long long)(73060756141) / (long long)N);

================================================================================
PDF OLDAL: 106
================================================================================

8.9       Kod fejlodes

A program fejlesztest az elodom altal keszitett forraskod vizsgalataval
kezdtem. A kodot felhasznalni azonban teljes egeszeben nem tudtam, mert az
fuzzy jellegu szabalyozot valosit meg. A kodban szerepel egy PID jellegu
szabalyozo megvalositas is, azonban szamomra ez sem hasznalhato. A kod
egyeb reszlete, mint peldaul a hardver inicializalas, tavolsag erzekeles
felhasznalasra kerultek.
A Program felepites tervezes fejezetben leirt program reszleteket ket kulon
"application note"-bol fejlesztettem ki. A vegleges format a Microchip ceg
AN957,      valamint      a     X2C      "DemoApplication"      kozul     a
"Blinky_Microchip_dsPIC33Fxxxx_MicrostickPlus"                alkalmazasok
osszeolvasztasaval nyertem. Az igy kialakitott hajtas es iranyitastechnikai
alaprendszert egy egyszeru alkalmazas megvalositasaval kivantam
osszehangolni.
PID teszt kornyezet
A vegleges szabalyozo megvalositas elott egy PID fordulatszam szabalyozast
alakitottam ki. Tettem ezt tobb okbol:
      -    a PID szabalyozo vegzi az egyensulyi pont kozeli helyzetbe allitast,
      -    BLDC motor alaprendszer szimulacioja a motor identifikaciobol nyert
           adatok alapjan,
      -    BLDC motor alaprendszer szimulacio validalasa, ezaltal robosztussag
           vizsgalat a meresi eredmenyek pontossagat tekintve,
      -    diszkret ideju rendszer vizsgalat,
      -    szoftver architektura beinditasa, beleertve a Microchip
fejlesztopanel,
           Scilab/Xcos hatasvazlat szerkeszto es X2C szoftvercsomagot.
Jelenlegi rendszer korlatai
A PID fordulatszam szabalyozoval kialakitott rendszerrel megtapasztaltam a
fejlesztorendszer lehetosegeit es korlatait. Az allapotfigyelos szabalyozohoz
az alaprendszerhez nem kell nyulni, csak az X2C modellt kell ujra generalni
es forditani. Ezzel a technikaval konnyen es gyorsan lehet fejleszteni. Az
modell parameterek futasi ideju modositasa finomhangolast tesz lehetove. A
szkop hasznalat es a forditott kod merete, futasi sebessege azonban eppen
meghaladja a jelenleg hasznalt kontroller eroforrasait. A sebesseg elsosorban
a monitorozaskor okoz gondot, mert a szkop megjeleniteskor megzavarja a
szabalyozo valos ideju beavatkozasat. A 2kbyte adat memoria meretet
meghaladja a 16bitesnel nagyobb hatasvazlat forditasa, ami
elengedhetetlennek tunik.

================================================================================
PDF OLDAL: 107
================================================================================

Tovabbi fejlesztes
A monitorozas altal okozott valosideju beavatkozas problemajat vizsgalni
kell, hogy a programstrukturat atalakitva a jelenlegi kontrollernel maradva,
vagy a sebesseget (MIPS) novelve egy masik kontroller alkalmazasaval
juthatok megoldashoz.
A 32bites hatasvazlat forditasahoz viszont mindkepp kontroller csere
szukseges, azonban a dsPIC33FJ csaladban letezik nagyobb, mint 2Kbyte
memoriaval rendelkezo kontroller. Tehat a PIM kartyan a kontroller
cserejevel lenne megoldhato a problema.
Tovabbi lehetoseg, hogy erosebb kontroller csaladbol valasztok egy tipust,
peldaul a dsPIC33EP256MC506, ami 70MIPS sebessegu, 256kbyte ROM es
32kbyte RAM-mal rendelkezik. Ennek alkalmazasa azert lenne elonyos, mert
ezzel a mezoorientalt szabalyozas (FOC), enkoderes kerekpozicio, nagyobb
sebesseg a szkop kiszolgalashoz, nagyobb adatmemoria, ami szinten a szkop
monitorozasi ablakot tamogatja es a 32bites hatasvazlat forditasa is
lehetsegesse valna. A problema ezzel a megoldassal, hogy a korabbi kod nem
fordul rajta az eltero hardver regiszter kezeles miatt.
Egyeb szelsoseges otletkent felmerult, hogy PIC32 kontroller alkalmazasaval
valositanam meg a hatasvazlat futtatasat, mig a hajtas marad a dsPIC33FJ-
ben. Ebben az esetben a megfeleloen gyors kommunikacio a ket kontroller
kozotti kialakitasa okozza a kihivast.
A fenti esetek mindegyike korulbelul azonos munkaora raforditast igenyel,
azonban az dsPIC33EP alkalmazasa tunik a legoptimalisabbnak.

================================================================================
PDF OLDAL: 108
================================================================================

8.10 Verzio kovetes

Manapsag a szoftverfejlesztes teruleten alapveto eszkoz a verziokoveto
szoftverek hasznalata. Szamos elonyei kozott a forrasfajlok tobb felhasznalos
kozpontositott tarolasa, korabbi verziokra torteno visszaallas, elagazasok
kezelese es riportalt valtozaskovetes szerepel.
Esetleges hatranya, hogy szervert kell fenntartani, bar leteznek lokalis
verziokoveto szoftverek is, de ezek eseten is erdemes gondoskodni az
archivalasrol.
Az elottem fejleszto kollega az SVN programot valasztotta a verzio
koveteshez. A forras fajlokat a szerverrol exportalva a masik szerverre gond
nelkul importalva hozzaferesem van a korabbi kezdetleges verziokhoz is. A
vegleges verziot a GitHub portalon kivanom megosztani.

================================================================================
PDF OLDAL: 109
================================================================================

                           9    EREDMENYEK

Dolgozatomban az elozo fejezetekben megmutattam, hogy hogyan gondoltam
at a szabalyozo valasztast.
A feherdobozos modszer alkalmazasanak szuksege szerint megalkottam a
modellt es felirtam a rendszeregyenleteket.
A szabalyozo tervezeshez szukseges tagok parametereit identifikaltam.
A PID szabalyozo tervezesenel arra kovetkeztetesre jutottam, hogy egyszeru
eszkozokkel nem tudok szabalyozot kesziteni, mert a rendszer parameterei
mellett nem sikerult elerni, hogy a Nyquist diagram megkerulje a -1 pontot,
ami feltetele a tervezesnek. Tovabbi tervezesi iteracioval lehetseges, hogy
sikerulne eredmenyt elernem, de pont a pelda jol mutatja az allapot
visszacsatolas szuksegesseget.
Allapot visszacsatolt rendszert es az allapotfigyelot tervezve sikerult sikeres
szimulaciot kesziteni, melyben a golyo a kerek tetejen egyensulyoz, ehhez
azonban legalabb 32bites szamabrazolast hasznalo megvalositast kell
alkalmazni.
A megvalositott szabalyozoban jelenleg csak 16bites szamabrazolast volt
lehetosegem hasznalni, mert az valasztott kontroller 2kbyte adatmemoriajaba
nem fer bele a 32bites szamabrazolassal keszitet hatasvazlat. A valos
eredmenyek is azt mutatjak, amit a szimulacio hozott. Az errol keszult video
a CD mellekleten megtalalhato. Az eredmenyt az X2C szkop programjaval
lehet monitorozni a be es kimeneteket, illetve a szabalyozo belso allapotait is.

phi2[rad]
                                              Elszallo billeges
          Golyo pozicio

           Kapocsfeszultseg                                              t[ms]

                       9-1. abra Eredmenyek - X2C monitorozas

[Kepek ezen a PDF-oldalon:]
[images/oldal_109_kep_066.png]

================================================================================
PDF OLDAL: 110
================================================================================


================================================================================
PDF OLDAL: 111
================================================================================

            10 KOVETKEZTETES ES JAVASLATOK

A kituzott celt, miszerint iranyitastechnikai demonstralo eszkozt keszitsek
reszben sikerult elerni, annak tovabbfejleszteset folytatni kell, azonban annak
alapjai le vannak fektetve. A modellalkotas megtortent, az egyenletrendszer
felirasra kerult, az szabalyozo megvalositasra kerult, annak hangolasat
folytatni kell. A korabbi fejezetekben a felmerulo problemak kapcsan, mar
reszben irtam arrol, hogy milyen szukseges fejlesztesi lepesekre van szukseg.
A hatasvazlatot legalabb 32bites szamabrazolassal kell megvalositani, amihez
tobb adatmemoriara van szukseg. A szkop program nagyobb idoablak
monitorozasa szinten tobb adat memoriat igenyel. A szabalyozo valosideju
zavartalan futasahoz nagyobb futasi sebessegre van szukseg, amelyet a szkop
megjelenites akadalyoz. Ezen okok miatt a kontroller csere szukseges. A Kod
fejlodes fejezetben kifejtett okokbol a dsPIC33EP tipus megfelelo valasztas
lehet.
A szabalyozo tokeletesitesere is van lehetoseg harom ponton is. A modellben
harom allapotvaltozo szerepel a kerek szogsebesseg, a golyo pozicio sebesseg
es a golyo pozicio maga. Az igy kialakult rendszerben a motor
kapocsfeszultseg a bemenet, amely a kerek forgatonyomatekan keresztul
avatkozik be a szakasz folyamataiba. A motor aram azert nem jelenik meg,
mert felteteleztem, hogy a motor felfutasi, azaz adott fordulatszamra beallasi
ideje elhanyagolhato, tehat nincs dinamikaja es ezert nincs idoallandoja sem.
Ekkor az aram allandosult allapotban szerepel, ezaltal a kapocsfeszultseg altal
meghatarozott, tehat nincs idofuggese es ezert nem jelenik meg az
allapotvaltozok kozott. A valosag azonban az, hogy a motor felfutasi ideje
(87ms) nem pillanatszeru, tehat a nyomatek beavatkozo jelet letrehozo
aramnak van idofuggese. Ebbol kovetkezoen a modellbe be kell vinni a motor
modellben szereplo L di/dt tranziens tagot, igy az aram allapotvaltozo lesz.
Tovabbi fejlesztesi pont a diszkret idejuseg kapcsan kerul elo. A jelenlegi
szabalyozo folytonos idore terveztem. A folytonos ideju modell jol
hasznalhato, ha szakasz idoallando sokkal nagyobb a mintavetelezes idejenel.
Az elobb emlitett legkisebb szakaszidohoz (87ms) a 10ms mintaveteli ido
kozel megfelelonek tunik, esetleg tovabb lehet csokkenteni es az erzekelon
kevesebb mintan vegezni a szurest. A diszkret ideju allapotfigyelo modell
azonban jobban kozeliti a valosagot es masik fontos tenyezoje, hogy a
matematikajabol fakadoan nem tartalmaz integratort, csak osszegzot. Ennek
okan erdemes atgondolni, hogy a hatasvazlat szamabrazolasahoz a 32bites
pontossag valoban szukseges. Harmadreszt a SISO modell bovitheto SIMO
modellre a kerek szogsebesseg meresevel, ehhez azonban enkoderes meres
szukseges.

================================================================================
PDF OLDAL: 112
================================================================================

                               11 CD MELLEKLET

A munkam teljes bemutatasa erdekeben CD mellekletet is csatolok a
nyomtatott forma melle.
A CD melleklet tartalma:
   -   Maxima szkriptek
          o     Drive:\X2C\DemoApplication\BoW_X2C_Space\X2CCode
   -   Scilab szkriptek
          o     Drive:\X2C\DemoApplication\BoW_X2C_Space\X2CCode
   -   Microchip forraskod
          o     Drive:\X2C\DemoApplication\BoW_X2C_Space\X2CCode
   -   Verziokovetes
          o     Drive:\SVN
   -   Irodalomjegyzek
          o     Drive:\Irodalomjegyzek
   -   Eszkozok
          o     Drive:\Eszkozok
   -   Videok
          o     Drive:\Video
   -   Magyarazo
          o     Drive:\Readme.txt
   -   Diplomaterv
          o     Drive:\DobanyImre.pdf

================================================================================
PDF OLDAL: 113
================================================================================

                                    12 ABRAJEGYZEK

1-1. abra Golyo a kereken rendszer - osszeallitasi vazlat [27]
..................... 17
3-1. abra Golyo a kereken rendszermodell [1]
.............................................. 24
3-2. abra Kinetikai feltetel....................................................
......................... 26
4-1. abra ID-motor, Teljes rendszer hatasvazlat (egy lehetseges megoldas) 40
4-2. abra ID-motor, BLDC belso felepites
.................................................... 41
4-3. abra ID - motor, MCLV-hajtas vazlat [23]
............................................ 42
4-4. abra ID- motor, Egyenaramu motor modell
........................................... 43
4-5. abra ID-motor, Iranyitastechnikai hatasvazlat
....................................... 43
4-6. abra ID - motor, Km meres elrendezes 1
............................................... 48
4-7. abra ID - motor, Km meres elrendezes 2
............................................... 48
4-8. abra ID - motor, referencia
fordulatszam............................................... 50
4-9. abra ID - motor, Km meres M(I)|n=all.
................................................. 52
4-10. abra ID- motor, Km meres M(n) | I = all.
............................................ 54
4-11. abra ID - motor, Km meres M(n) | Uk = all.
........................................ 56
4-12. abra ID - motor, Ku meres elrendezes
................................................. 59
4-13. abra ID - motor, Iw meres tachomotor
................................................. 63
4-14. abra ID - motor, Iw meres Terheletlen kerek felfutasi ido merese .......
64
4-15. abra ID - motor, Iw meres Terhelt kerek felfutasi ido merese
.............. 64
4-16. abra BoW szakasz - Bode diagram
...................................................... 65
4-17. abra BoW szakasz - Nyquist diagram
.................................................. 66
4-18. abra BoW szakasz - Polus-zerus diagram
............................................ 66
4-19. abra BoW szakasz - idoallando szimulacio
......................................... 67
4-20. abra BoW szakasz - idoallando
........................................................... 67
4-21. abra BoW szakasz - idoallando validalas
............................................ 67
5-1. abra Bow szakasz PID - Bode diagram
................................................. 70
5-2. abra BoW szakasz PID - Nyquist diagram
............................................ 70
5-3. abra BoW szakasz PID - polus zerus diagram
....................................... 71
6-1. abra BoW szakasz - Allapot visszacsatolas
........................................... 73
7-1. abra BoW szakasz - Allapotfigyelo
....................................................... 75
7-2. abra Diszkret modell - teljes modell
...................................................... 76
8-1. abra Fejlesztoi rendszer
......................................................................... 78
8-2. abra Szimulacio Xcos - beallitasok
........................................................ 81
8-3. abra Szimulacio, Xcos, folytonos ido
.................................................... 81
8-4. abra Szimulacio - Ua(t); fi2(t), folytonos
ido........................................ 82
8-5. abra Szimulacio - Figyelo hibajel, folytonos ido
................................... 82
8-6. abra Szimulacio, Figyelo kimenet
......................................................... 82
8-7. abra Szimulacio, X2C
BoW................................................................... 84
8-8. abra Szimulacio, X2C - 64bit, float
....................................................... 85

================================================================================
PDF OLDAL: 114
================================================================================

8-9. abra Szimulacio, X2C - 32bit, fix
..........................................................85
8-10. abra Szimulacio, X2C - 16bit, fix
........................................................85
8-11. abra Szimulacio X2C - Diszkret integrator
..........................................86
8-12. abra Szimulacio X2C - Integralasi ido
.................................................86
8-13. abra Szimulacio X2C - Integrator felbontas
........................................86
8-14. abra Program felepites reszei
...............................................................88
8-15. abra Program felepites - X2C igeny
....................................................88
8-16. abra Eroforras kiosztas
.........................................................................90
8-17. abra Eroforras - maximum MIPS
.........................................................91
8-18. abra Eroforras - TMR1- X2C megszakitas
..........................................91
8-19. abra X2C hatasvazlat beagyazas
..........................................................93
8-20. abra Hajtas - PWM mint atviteli tag
[28].............................................94
8-21. abra Hajtas - hibas PWM
.....................................................................95
8-22. abra Hajtas -PWM 10%
.......................................................................96
8-23. abra Hajtas - PWM 10%, nagyitas
.......................................................96
8-24. abra Hajtas - PWM
50%......................................................................97
8-25. abra Hajtas - PWM 50%, nagyitas
.......................................................97
8-26. abra Hajtas - PWM 100%
....................................................................98
8-27. abra Golyo pozicio meres
....................................................................99
8-28. abra Golyo pozicio meres- hatasvazlat
................................................99
8-29. abra Pozicio meres - BALLUFF meresi ido
......................................100
8-30. abra Tavolsag meres - BALLUFF karakterisztika
.............................100
8-31. abra Hajtas - Omegaw meresi elrendezes
..........................................102
8-32. abra Hajtas - Omegaw meres Hurst - Hall egy mech. fordulaton ......103
8-33. abra Hajtas - Omegaw meres Kerekpar - Hall egy mech. fordulaton 103
9-1. abra Eredmenyek - X2C monitorozas
..................................................109

================================================================================
PDF OLDAL: 115
================================================================================

                                  13 TABLAJEGYZEK

3-1. tablazat Terminologia es parameterek
................................................... 25
4-1. tablazat ID - Golyo es kerek
.................................................................. 39
4-2. tablazat ID - motor, parameterek
........................................................... 40
4-3. tablazat ID - motor, Hajtas adatok
......................................................... 42
4-4. tablazat ID-motor, Ra meres muszerek
.................................................. 46
4-5. tablazat ID - motor, Ra,La eredmenyek
................................................. 46
4-6. tablazat ID - motor, Km konstans meres
fejlec...................................... 47
4-7. tablazat ID - motor, Km meres muszerei
............................................... 49
4-8. tablazat ID - motor, Km meres munkapontok
....................................... 49
4-9. tablazat ID - motor, Km meres fordulatszam
referencia........................ 50
4-10. tablazat ID - motor, Km meres M ( I ) |n=all.
...................................... 53
4-11. tablazat ID - motor, Km meres M (n) |Ia=all.
...................................... 55
4-12. tablazat ID - motor, Km meres M(n) | Uk = all.
.................................. 57
4-13. tablazat ID - motor, Ku meres fejlec
.................................................... 59
4-14. tablazat ID - motor, Ku meres muszerei
.............................................. 60
4-15. tablazat ID motor, Iw meres eredmenyek
............................................. 63
7-1. tablazat Diszkret modell - idoallandok
................................................. 76
8-1. tablazat Szimulacio lehetosegei
............................................................. 80
8-2. tablazat Scilab szamformatumok
........................................................... 80
8-3. tablazat Eroforras kiosztas - idoallandok
............................................... 89
8-4. tablazat Eroforras kiosztas
..................................................................... 90
8-5. tablazat Golyo pozicio meres
................................................................. 99
8-6. tablazat Tavolsag meres - eredmenyek
................................................ 101
8-7. tablazat Hajtas - Fordulatszam meres eredmenyek
.............................. 104

================================================================================
PDF OLDAL: 116
================================================================================


================================================================================
PDF OLDAL: 117
================================================================================

                      14 IRODALOMJEGYZEK

[1] R.W. van Gils, "Control of the Ball on the Wheel System,"
Technische Universiteit Eindhoven, 2007

[2] Lantos Bela, "Szabalyozasi rendszerek tervezese allapotterbem,"
Iranyitastechnikai rendszerek elmelete es tervezese I.
Akademiai kiado, 2001.

[3] Halasz Sandor, "Egyenaramu hajtasok," Villamos hajtasok, Muegyetemi
kiado, 1999.

[4] Microchip. (2011). AN957
[Online]. Available: http://www.microchip.com

[5] Prof. Bokor Jozsef, "BLDC motor PID szabalyozasa," Iranyitastechnikai
gyakorlatok, Muegyetemi kiado, 2003

[6] Manolo Venturin, Silvia Poles (2012)
Controller for an inverted pendulum
[Online]. Available: www.openeering.com

[7] Hurst Motor
Datasheet of DMB0224C10002
[Online]. Available:
http://ww1.microchip.com/downloads/en/DeviceDoc/Hurst%20Motor%20D
ataSheet.pdf

[8] Michael Baudin (2010)
Floating point numbers in Scilab
[Online]. Available: forge.scilab.org

================================================================================
PDF OLDAL: 118
================================================================================

[9] X2C documentation (v1028), "Introduction to X2C," Linz Center of
Mechatronic, 2016
[Online]. Available: http://www.mechatronic-simulation.org/

[10] Gilber, Solyom, Kocsanyi, "Merev testek kinematikai leirasa," Fizika
Mernokoknek, Muegyetemi kiado
[Online]. Available:
http://merlin2.bgk.bme.hu/files/h/0/7efa5a4c579a67b3277313e20f608804.pdf

[11] Bauer Peter, Repulogepek egyszeru referenciajel koveto szabalyozoinak
tervezese, BME Kozlekedesautomatikai Tanszek, 2009

[12] Lantos, Kiss, Harmati, Szabalyozastechnika gyakorlatok, BME IIT, 2009

[13]
Unknown
How to setup the simulation parameters in Xcos (Scilab);
[Online]. Available: www.x-engineer.org

[14] Barsh-Ruth, "14.3.5 Kozvetlen digitalis szabalyozasok," Automaitka,
Akademiai kiado, 1980, pp. 535.

[15] Fodor Gyorgy, "1.7-2.2. Minimalfazisu rendszer," Halozatok es
rendszerek, Muegyetemi kiado, 2003

[16] Keviczky, Barsh, Ruth, Szabalyozastecnika, 1999

[17] Scilab dokumentacio
[Online] Available: Scilab.org

================================================================================
PDF OLDAL: 119
================================================================================

[18] Dr. Kopjak Jozsef, "2.3. Multitaszkon alapulo megoldasok," Vezerles
feladatokat ellato programmodellek bemutatas es az esemenyvezerelt
vezerlesi halo, Obudai Egyetem, Kando Kalman Villamosmernok Kar,
Automatika Intezet

[19] Microchip
AN: DS70183, A/D
[Online] Available:
http://ww1.microchip.com/downloads/en/DeviceDoc/70225B.pdf

[20] Microchip
AN: DS70214C; Interrupts
[Online] Available:
http://ww1.microchip.com/downloads/en/DeviceDoc/70214C.pdf

[21] Microchip
AN: DS70205, Timers
[Online] Available:
http://ww1.microchip.com/downloads/en/DeviceDoc/70205B.pdf

[22] Microchip
AN: DS70198, Input captures
[Online] Available:
http://ww1.microchip.com/downloads/en/DeviceDoc/70198D.pdf

[23] Microchip
AN: DS70187, PWM
[Online] Available:
http://ww1.microchip.com/downloads/en/DeviceDoc/70187E.pdf

================================================================================
PDF OLDAL: 120
================================================================================

[24] Microchip
AN: DS70283J, dsPIC33FJ32MC202/204
[Online] Available:
http://ww1.microchip.com/downloads/en/DeviceDoc/70283J.pdf

[25] Richard H. Welch Jr., George W. Younkin, How Temperature Affects a
Servomotor's Electrical and Mechanical Time Constants;

[26] Kasowitz Artur, Instabil poziciora szabalyozas, 2007

[27] Farago Istvan, Horvath Robert, "2.1 Modellalkotas," Numerikus
modszerek, BME TTK, 2013

[28] Schmidt, Istvan, Veszpremi, Karoly, BMEVIVEM175, 2011 2012

[29]Dr. Hack Frigyes, Negyjegyu fuggvenytablazat, 2010

[30]: Maxima
Maxima documentation
[Online] Avaliable: maxima.org
