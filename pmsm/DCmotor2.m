


%v20250326a

#setup: MotorA es MotorB egymassal szembe forditva

#********************************************
# Motor parameters
#********************************************
% DC Motor Parameters (same for A and B for simplicity)
#MotorA tipus: N20E-12-4000: 12V 4mNm encoder
#MotorB tipus: N20 -12-4000 (enkoder nelkul

#***** 1, R meres *****
#DC multimeter
Rraw=7.47#vezetek nelkul

#full load: lefogtam a motort:#3.693Vavg(87%)/233mA=13.73ohm??????????????
Rwire=12.5;%vezetek, csatlakozok, uzemi homerseklet??????????

RmotorA=Rraw+Rwire#vezetekkel

#DC multimeter:
RmotorB=6.28#brake motor, ezen nincs csati, ehhez nem adom hozza a 0.4ohm-ot


#***** 2, L meres *****
#LRC mero, 1khz, !!! max 100mA !!!
L = 0.07;         % Henry

 
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
%---FOC---
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

 
 
 
% Simulation Parameters
dt1 = 1e-4;  % Motor simulation time step
dt2 = 1e-3;  % PID control update time step
t_end = 1.0;     % end time (s)
 
% Initial Conditions
i_A = 0.0; omega_A = 0.0;
i_B = 0.0; omega_B = 0.0;  % May not be necessary if B is brake only
#i_A = 0.0; omega_A = 0.0;
#i_B = 0.0; % omega_B not used (coupled to A)
 
% Control Setpoints
omega_ref = 10.0;  % rad/s desired speed for motor A
torque_ref_B = 0.0; % Nm braking torque (can step this)
 
%***************************************************************
% PID Gains
#{
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
torque_B_vec = [];
#--
torque_ref_B_vec = [];
error_torque_B_vec = [];
int_error_torque_B
 
% Time counters
t = 0.0;
t_pid = 0.0;

% Simulation loop
while t <= t_end
    % --- Step change in Motor B braking torque at t = 1.0s ---
    if 0#t >= 0.5
        torque_ref_B = 3e-3;  % Nm after step
    end
 
    % Update PID every td2
    if t >= t_pid
	    % --- Motor A Speed PID Controller ---
	    error_speed = omega_ref - omega_A;
	    int_error_speed += error_speed * dt2;
	    torque_ref_A = Kp_speed * error_speed + Ki_speed * int_error_speed;
	    torque_ref_A = 3e-3;
	 
	    % --- Motor A Torque PID Controller ---
	    torque_A = Kt * i_A;
	    error_torque_A = torque_ref_A - torque_A;
	    int_error_torque_A += error_torque_A * dt2;
	    V_input_A = Kp_torque_A * error_torque_A + Ki_torque_A * int_error_torque_A;
	    if 1#V_input_A > 5
	      V_input_A = 3.7;
	    endif
	 
	    % --- Motor B Torque PID Controller ---
	    torque_ref_A = 3e-3;
	    torque_B = Kt * i_B;
	    error_torque_B = torque_ref_B - torque_B;
	    int_error_torque_B += error_torque_B * dt2;
	    V_input_B = Kp_torque_B * error_torque_B + Ki_torque_B * int_error_torque_B;
	    if V_input_B > 5
	      V_input_B = 5;
	    endif
 
	    % Update PID execution time
	    t_pid += dt2;
    end

    % --- Motor A Dynamics ---
    diA_dt = (V_input_A - RmotorA*i_A - Ke*omega_A) / L;
    torque_B = 0;
    domegaA_dt = (Kt*i_A - B_fric*omega_A - torque_B) / J;
 
    i_A += diA_dt * dt1;
    omega_A += domegaA_dt * dt1;
 
    % --- Motor B Dynamics ---
    diB_dt = (V_input_B - RmotorB*i_B - Ke*omega_A) / L;
    i_B += diB_dt * dt1;
 
    % Store data
    time_vec(end+1) = t;
    omega_A_vec(end+1) = omega_A;
    i_A_vec(end+1) = i_A;
    i_B_vec(end+1) = i_B;
    torque_B_vec(end+1) = torque_B;
 
 
    error_speed_vec(end+1) = error_speed;
    int_error_speed_vec(end+1) = int_error_speed;
    torque_ref_A_vec(end+1) = torque_ref_A;
    torque_A_vec(end+1) = torque_A;
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
 
% Plot results
figure;
subplot(3,1,1);
plot(time_vec, omega_A_vec, 'b');
xlabel('Time (s)'); ylabel('Speed A (rad/s)');
title('Motor A Speed'); grid on;
 
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






 
