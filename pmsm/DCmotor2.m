

%v20250417  octave --persist DCmotor2.m
%v20250410a, motorB elindit-->torque_pid hangolas(ts=10ms, 2%overshoot)
%v20250404a, motorApid 100ms, motorBpid 10ms,
%v20250332a
%v20250326a
clear;

#target:kul. szabalyozas tanulmanyozasa

#date:  2025 marc

#setup: MotorA es MotorB DC motorok egymassal szembe forditva
#       MotorA szabalyozo a vizsgalt rendszer
#       MotorB a fek, kul nyomatek profilok beallitasa, pl step func

#DC Motor Parameters: (same for A and B for simplicity)
#       MotorA tipus: N20E-12-4000: 12V 4mNm encoder
#       MotorB tipus: N20 -12-4000 (enkoder nelkul)

Vcc = 4;

#********************************************
#motor parametrizalas:
#1, fizikai modellezes --> motor egyenlet
#2, identifikacio:
# Vinput egysegugrast adni ra es nezni az idofuggo valtozokat
#  i(t)--> elektrodinamikai allando tau_e = L/R ,
#  w(t)--> mechanikusdinamikai allando tau_m = J/surlodas


#********************************************
# MotorA parameters
#********************************************

#***** 1, R meres *****
#DC multimeter
RrawA=7.47#vezetek nelkul

#full load: lefogtam a motort:#3.693Vavg(87%)/233mA=13.73ohm??????????????
RwireA=12.5;%vezetek, csatlakozok, uzemi homerseklet??????????

RmotorA=RrawA+RwireA#vezetekkel


#***** 2, L meres *****
#LRC mero,Agilent 4263B, sorosRL,1khz,1V --> 7ohm, 1.6mH
L = 1.6e-3;         % Henry


#***** 3, Ke, Kt motor konstans meres
#Ke es Kt a DC motornal megegyezik
#fordulatszam merovel:
n_measFmero = 875;
#rotatary senzor: egy negyed fordulat 15db felfuto el --> *4 = 60pulse/rot
#1ms periodusido * 60pulse = 60msec/rot --> 937rpm
n_meas = 937;     % rpm
w_meas = n_meas / 60 * 2 *pi
#motorA szabadon forog, motorBvel:  U0=3.700Vavg(90%)/85mA
#motorB kapocsfesz V_meas=2.7Vpeak, 2.338Vavg
#U0-Ubemf=UR  --> UR/iR=R
#3.7-2.3=1.4V //1.5V/200/100mohm=75mA --> 1.4V/75mA=18.6ohm  =? ~7ohm
V_meas = 2.338    % V dc average
Ke = V_meas / w_meas
%---PMSM, FOC---
%MesaSP = 1000;%measured rotor speed in RPM
%MeasVllpk = 0.387;%measured line to line voltage peak % from Marco actually the double...0.774V!!!
%Ke = (MeasVllpk/sqrt(3))/(MesaSP*Rev_Min_to_Rad_Sec);
%Fl = Ke / Npp
%Kt = 3/2 * Ke
%Kt = 3/2 * Fl * Npp;       % V·s/rad
%MeasVllpk = Fl*sqrt(3)*(MesaSP*Rev_Min_to_Rad_Sec)*Npp;
%-------
#~0.5N*10mm=5Nmm, 233mA --> 5Nmm/233mA
Kt = 5e-3/233e-3       % N·m/A
#Ke hasznalva Kt-re
Kt=Ke

#***** 4, Friction guess *****
#steady: n=937rpm, w=98rad/s, ia=75mA, V=3.77V
#teht beallitom a tapfeszt es nezem az aramot es az w-t
B_fric = 0.00002;    % N·m·s

%aprox. cilinder jrot = m*r^2 = 0.5[kg]*0.05m2 = 0.025kgm2
J = 0.000001;        % kg·m^2


#********************************************
# MotorB parameters
#********************************************
#DC multimeter
RrawB=7.47#vezetek nelkul

#full load: lefogtam a motort:#3.693Vavg(87%)/233mA=13.73ohm??????????????
RwireB=12.5;%vezetek, csatlakozok, uzemi homerseklet??????????

RmotorB=RrawB+RwireB#vezetekkel


#L, K, J, B ua elso korben mindket motorra


%********************************************
% PID Gains
#********************************************
#{
Start with Only P (Proportional Control)
    Set I and D to zero: Start with only the proportional gain.
    Increase P gain gradually: Increase KpKp​ until the system starts oscillating.
    Identify critical gain KcrKcr​ and period TcrTcr​: The value at which the system oscillates continuously is called the critical gain KcrKcr​, and the oscillation period is TcrTcr​.
    Reduce P slightly: If the system oscillates too much, reduce KpKp​ until oscillations dampen slightly.

Add I (Integral Control)
    Set an initial I gain: Start with a small KiKi​, typically 1/10th of KpKp​.
    Increase KiKi​ slowly: This will reduce steady-state error but can introduce oscillations if too high.
    Watch for instability: If the system becomes unstable, decrease KiKi​.
    Adjust until steady-state error is minimal: Ensure the system reaches the target value with minimal error.

Add D (Derivative Control)
    Set a small D gain: Start with a small KdKd​, around 1/10th of KpKp​.
    Increase KdKd​ slowly: This helps reduce overshoot and oscillations.
    Monitor system behavior: If KdKd​ is too high, it can cause excessive noise or instability.
    Stop when overshoot is minimized: The goal is to improve stability without making the system too sluggish.

Fine-Tuning
    Adjust Kp,Ki,KdKp​,Ki​,Kd​ iteratively until you achieve the desired balance of response speed, stability, and minimal steady-state error.
    If the response is:
        Too slow: Increase KpKp​.
        Oscillating too much: Reduce KpKp​ or increase KdKd​.
        Drifting away from the setpoint: Increase KiKi​.
Alternative: Ziegler-Nichols Method (Empirical Tuning)

If you identified the critical gain KcrKcr​ and critical period TcrTcr​, use the table below for tuning:
Type	Kp	Ki​	  Kd​
P	0.5Kcr  0	  0
PI	0.45Kcr 1.2Kp/Tcr​ 0
PID	0.6Kcr  2Kp/Tcr​	  Kp​Tcr​/8

%control related parameters
%damp = 2.0;%damping factor with 2% overshoot
damp = 1.0;%damping factor with no overshoot

KiI = R/(6.0*Ts*damp*damp);
KpI = L/(6.0*Ts*damp*damp);

%speed controller
tset_w = 0.1/4.0;%settling time divided by 4 to acheive 2%
Kiw = (FF)/(tset_w);
Kpw = Jrot/(tset_w);
#}

% Motor A - Speed Loop
Kp_speed = 3;
Ki_speed = 15.0;

% Motor A - Torque Loop
Kp_torque_A = 1000.0;
Ki_torque_A = 0.0;

% Motor B - Torque Loop
Kp_torque_B = 1000.0;
Ki_torque_B = 0.0;



#********************************************
% Simulation loop prep
#********************************************
dt1 = 1e-4;  % Motor simulation time step
dtA = 10e-3;  % PID control update time step
dtB = 1e-3
t_end = 1.0;     % end time (s)

i_A = 0.0; omega_A = 0.0;
i_B = 0.0; omega_B = 0.0;  % May not be necessary if B is brake only

% Control Setpoints
omega_ref = 10.0;  % rad/s desired speed for motor A
torque_ref_B = 0.0; % Nm braking torque (can step this)

% Integrators
int_error_speed = 0.0;
int_error_torque_A = 0.0;
int_error_torque_B = 0.0;

% Data storage
time_vec = [];

omega_A_vec = [];
i_A_vec = [];
#--
error_speed_vec = [];
int_error_speed_vec = [];
torque_ref_A_vec = [];
torque_A_vec = [];
error_torque_A_vec = [];
int_error_torque_A_vec = [];
V_input_A_vec = [];

i_B_vec = [];
omega_B_vec = [];
torque_B_vec = [];
#--
torque_ref_B_vec = [];
error_torque_B_vec = [];
int_error_torque_B_vec = [];
V_input_B_vec = [];


% Time counters
t = 0.0;
t_pidA = 0.0;
t_pidB = 0.0;
#********************************************
% Simulation loop
#********************************************
while t <= t_end
    % --- Step change in Motor B braking torque at t = 1.0s ---
    if 0#t >= 0.5
        torque_ref_B = 3e-3;  % Nm after step
    end

    % Update PID every td2
    if t >= t_pidA
	    % --- Motor A Speed PID Controller ---
	    error_speed = omega_ref - omega_A;
	    int_error_speed += error_speed * dtA;
	    torque_ref_A = Kp_speed * error_speed + Ki_speed * int_error_speed;
	    torque_ref_A = 3e-3;

	    % --- Motor A Torque PID Controller ---
	    torque_A = Kt * i_A;
	    error_torque_A = torque_ref_A - torque_A;
	    int_error_torque_A += error_torque_A * dtA;
	    V_input_A = Kp_torque_A * error_torque_A + Ki_torque_A * int_error_torque_A;
	    if 1#V_input_A > Vcc
	      V_input_A = 3.7;#Vcc
	    end
	    % Update PID execution time
	    t_pidA += dtA;
    end

    if t >= t_pidB
	    % --- Motor B Torque PID Controller ---
	    torque_ref_B = 3e-3;
	    torque_B = Kt * i_B;
	    error_torque_B = torque_ref_B - torque_B;
	    int_error_torque_B += error_torque_B * dtB;
	    V_input_B = Kp_torque_B * error_torque_B + Ki_torque_B * int_error_torque_B;
	    if 1#V_input_B > Vcc
	      V_input_B = 3.7;#Vcc
	    end
	    % Update PID execution time
	    t_pidB += dtB;
    end

    % --- Motor A Dynamics ---
    diA_dt = (V_input_A - RmotorA*i_A - Ke*omega_A) / L;
    torque_B = 0;
    domegaA_dt = (Kt*i_A - B_fric*omega_A - torque_B) / J;

    i_A += diA_dt * dt1;
    omega_A += domegaA_dt * dt1;

    % --- Motor B Dynamics ---
    diB_dt = (V_input_B - RmotorB*i_B - Ke*omega_B) / L;
    torque_A = 0;
    domegaB_dt = (Kt*i_B - B_fric*omega_B - torque_A) / J;

    i_B += diB_dt * dt1;
    omega_B += domegaB_dt * dt1;

    % Store data
    time_vec(end+1) = t;

    omega_A_vec(end+1) = omega_A;
    i_A_vec(end+1) = i_A;
    torque_A_vec(end+1) = torque_A;

    omega_B_vec(end+1) = omega_B;
    i_B_vec(end+1) = i_B;
    torque_B_vec(end+1) = torque_B;



    error_speed_vec(end+1) = error_speed;
    int_error_speed_vec(end+1) = int_error_speed;
    torque_ref_A_vec(end+1) = torque_ref_A;

    error_torque_A_vec(end+1) = error_torque_A;
    int_error_torque_A_vec(end+1) = int_error_torque_A;
    V_input_A_vec(end+1) = V_input_A;


    torque_ref_B_vec(end+1) = torque_ref_B;
    error_torque_B_vec(end+1) = error_torque_B;
    int_error_torque_B_vec(end+1) = int_error_torque_B;
    V_input_B_vec(end+1) = V_input_B;





    % Update time
    t += dt1;
end


#********************************************
% plot
#********************************************

plot = 0 #gui plot
#plot = 1 #gnuplot

if plot == 0

figure;
subplot(3,1,1);
plot(time_vec, omega_A_vec, 'r', time_vec, omega_B_vec, 'g');
xlabel('Time (s)'); ylabel('Speed A,B (rad/s)');
title('Motor A,B Speed'); grid on;

subplot(3,1,2);
plot(time_vec, i_A_vec, 'b', time_vec, i_B_vec, 'b');
xlabel('Time (s)'); ylabel('Current (A)');
legend('Motor A', 'Motor B');
title('Currents A and B'); grid on;

subplot(3,1,3);
plot(time_vec, torque_B_vec, 'b');
xlabel('Time (s)'); ylabel('Braking Torque (Nm)');
title('Motor B (Brake) Torque'); grid on;


% Plot results2
figure;
subplot(3,1,1);
plot(time_vec, error_speed_vec, 'r', time_vec, int_error_speed_vec, 'g');
xlabel('Time (s)'); ylabel('error_speed');
grid on;

subplot(3,1,2);
plot(time_vec, torque_ref_A_vec, 'r', time_vec, torque_A_vec, 'g');
xlabel('Time (s)'); ylabel('torque_A');
grid on;

subplot(3,1,3);
plot(time_vec, error_torque_A_vec, 'r', time_vec, int_error_torque_A_vec, 'g');
xlabel('Time (s)'); ylabel('error_torque_A');
grid on;

% Plot results3
figure;
subplot(3,1,1);
plot(time_vec, error_torque_B_vec, 'r', time_vec, int_error_torque_B_vec, 'g');
xlabel('Time (s)'); ylabel('error_torque_B');
grid on;

subplot(3,1,2);
plot(time_vec, torque_ref_B_vec, 'r');
xlabel('Time (s)'); ylabel('torque_B');
grid on;


subplot(3,1,3);
plot(time_vec, V_input_A_vec, 'r', time_vec, V_input_B_vec, 'g');
xlabel('Time (s)'); ylabel('V_input');
grid on;


else #gnu plot


% --- Save Data for GNUPLOT ---
save -ascii "time.dat" time_vec;
save -ascii "omega_A.dat" omega_A_vec;
save -ascii "current_A.dat" i_A_vec;
save -ascii "current_B.dat" i_B_vec;
save -ascii "torque_B.dat" torque_B_vec;

% --- GNUPLOT Plot Commands ---
printf("GNUPLOT command example:\n");
printf("plot 'time.dat' using 1:2 with lines title 'Motor A Speed'\n");
printf("plot 'time.dat' using 1:3 with lines title 'Motor A Current'\n");
printf("plot 'time.dat' using 1:4 with lines title 'Motor B Current'\n");
printf("plot 'time.dat' using 1:5 with lines title 'Braking Torque'\n");

end



