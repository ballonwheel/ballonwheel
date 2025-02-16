% PMSM Simulation with PID Control in Octave

clear; clc; close all;

% PMSM Parameters
R = 0.5;          % Stator resistance (Ohm)
Ld = 0.01;        % d-axis inductance (H)
Lq = 0.01;        % q-axis inductance (H)
J = 0.01;         % Moment of inertia (kg.m^2)
B = 0.001;        % Friction coefficient (N.m.s)
P = 4;            % Pole pairs
lambda_m = 0.1;   % PM Flux linkage (Wb)
Ts = 1e-4;        % Sampling time (s)
Tsim = 1;         % Total simulation time (s)

% State-space matrices
A = [-R/Ld,  -1/Ld,      0;
      1/Lq, -R/Lq, lambda_m/Lq;
      0,    (3/2)*P*lambda_m/J, -B/J];

B = [1/Ld,  0;
     0,  1/Lq;
     0,  0];

% Discretization using Euler method
Ad = eye(3) + A * Ts;
Bd = B * Ts;

% Initial conditions
id = 0; iq = 0; omega = 0;
x = [id; iq; omega];

% Reference values
id_ref = 0;  % Set to 0 for maximum torque control
iq_ref = 5;  % Desired q-axis current
omega_ref = 100;  % Target speed in rad/s

% PID Controller Gains
Kp_id = 10; Ki_id = 100; Kd_id = 0.1;
Kp_iq = 10; Ki_iq = 100; Kd_iq = 0.1;
Kp_w  = 1;  Ki_w  = 50;  Kd_w  = 0.1;

% Initialize PID states
e_id_int = 0; e_iq_int = 0; e_w_int = 0;
e_id_prev = 0; e_iq_prev = 0; e_w_prev = 0;

% Time vector
time = 0:Ts:Tsim;
x_log = zeros(length(time), 3);
Vd_log = zeros(length(time), 1);
Vq_log = zeros(length(time), 1);

% Simulation loop
for k = 1:length(time)
    % Save state for plotting
    x_log(k, :) = x';
    
    % Calculate errors
    e_id = id_ref - x(1);
    e_iq = iq_ref - x(2);
    e_w  = omega_ref - x(3);
    
    % PID for i_d
    e_id_int += e_id * Ts;
    Vd = Kp_id * e_id + Ki_id * e_id_int + Kd_id * (e_id - e_id_prev) / Ts;
    e_id_prev = e_id;
    
    % PID for i_q
    e_iq_int += e_iq * Ts;
    Vq_iq = Kp_iq * e_iq + Ki_iq * e_iq_int + Kd_iq * (e_iq - e_iq_prev) / Ts;
    e_iq_prev = e_iq;

    % PID for Speed Control (sets iq_ref)
    e_w_int += e_w * Ts;
    iq_ref = Kp_w * e_w + Ki_w * e_w_int + Kd_w * (e_w - e_w_prev) / Ts;
    e_w_prev = e_w;
    
    % Apply iq limit (avoiding excessive current)
    iq_ref = max(-10, min(10, iq_ref));  
    
    % Set final Vq
    Vq = Vq_iq;
    
    % Store voltages for plotting
    Vd_log(k) = Vd;
    Vq_log(k) = Vq;
    
    % Update PMSM states
    u = [Vd; Vq];
    x = Ad * x + Bd * u;
end

% Plot results
figure;
subplot(3,1,1);
plot(time, x_log(:,1), 'r', 'LineWidth', 1.5);
xlabel('Time (s)'); ylabel('i_d (A)'); title('d-axis Current');
grid on;

subplot(3,1,2);
plot(time, x_log(:,2), 'b', 'LineWidth', 1.5);
xlabel('Time (s)'); ylabel('i_q (A)'); title('q-axis Current');
grid on;

subplot(3,1,3);
plot(time, x_log(:,3), 'g', 'LineWidth', 1.5);
xlabel('Time (s)'); ylabel('\omega (rad/s)'); title('Motor Speed');
grid on;

figure;
subplot(2,1,1);
plot(time, Vd_log, 'r', 'LineWidth', 1.5);
xlabel('Time (s)'); ylabel('V_d (V)'); title('d-axis Voltage');
grid on;

subplot(2,1,2);
plot(time, Vq_log, 'b', 'LineWidth', 1.5);
xlabel('Time (s)'); ylabel('V_q (V)'); title('q-axis Voltage');
grid on;

