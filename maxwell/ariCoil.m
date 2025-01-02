



#air Core coil design

# mekkora aram kell?
# vezetek vastagsag / homerseklet, melegedes optimalizalas
# 5A 1mm 18awg 075mm2
# https://www.engineeringtoolbox.com/wire-square-mm-diameter-d_1874.html
# https://www.engineeringtoolbox.com/awg-wire-gauge-d_731.html


# befoglalo meretek?
# szelesseg / tekercs hossz jatek, optimalizalas
# https://www.circuits.dk/calculator_multi_layer_aircore.htm
# https://www.circuits.dk/single-layer-air-core-inductor-calculator/
# https://m0ukd.com/calculators/air-cored-inductor-calculator/
# https://k7mem.com/Ind_Coil_Design.html

# Multi layer air core inductor calculator
# Formula used in this calculation is from Wheelers approximations which is accurate to <1% if the cross section is near square shaped.:

# L (uH) =31.6*N^2* r1^2 / 6*r1+ 9*L + 10*(r2-r1)

# where....
# L(uH)= Inductance in microHenries
# N = Total Number of turns
# r1 = Radius of the inside of the coil in meters
# r2 = Radius of the outside of the coil in meters
# L = Length of the coil in meters

# This formula applies at 'low' frequencies (<3MHz) using enameled copper wire tightly wound.

#Inductance (L):?mH
#Coil Inner Diameter (d):?mm
#Coil Length (l):?mm
#Wire Gauge: ? AWG
#---> calc
#Number of Turns (N):db
#Turns per Layer:db
#Number of Layers: db
#Coil Outer Diameter (D): mm
#Wire Diameter: mm
#Wire Length:meter
#DC Resistance (R): Ohms (at 20'C,



# a vasarolhato tekercsek egyben hagyva mekkora induktivitasuak?

#1. 4mH, 1ohm
# d1mm,18awg huzalhossz56m, A=(30mm/2)^2*pi l=50mm --> ~ 4mH, 1ohm
# 430menet, 47menet/layer, 9layer
# rendeles RS, https://hu.rs-online.com/web/p/vorosrez-huzal/7790735
# Block Egymagos vörösréz huzal 18 AWG, hossz: 56m, külső Ø: 1mm
# RS raktári szám:    779-0735 Gyártó cikkszáma: CUL 500/1,00 Gyártó: Block


#2. - 37mH, 7ohm
# Block Egymagos vörösréz huzal 22 AWG, hossz: 140m, külső Ø: 0.63mm
# RS raktári szám:    779-0729 Gyártó cikkszáma:    CUL 500/0,63 Gyártó:  Block
# https://hu.rs-online.com/web/p/vorosrez-huzal/7790729?gb=s
# awg22 0,63mm 0,33mm2, 37mH, 7ohm, 140m hossz
# 1054 menet, 44 menet/layer, 24 layer

#3, - 3mH, 0,75ohm
# Block Egymagos vörösréz huzal 17 AWG, hossz: 44m, külső Ø: 1.12mm
# RS raktári szám: 779-0744 Gyártó cikkszáma: CUL 500/1,12 Gyártó: Block
# 3mH 0.75ohm, 44m-hossz, 368 menet, 42menet/layer, 9 layer 

