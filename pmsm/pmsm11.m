% PMSM Simulation with Vectorized PID Control in Octave

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

% PMSM State-space Matrices
A = [-R/Ld,  -1/Ld,      0;
      1/Lq, -R/Lq, lambda_m/Lq;
      0,    (3/2)*P*lambda_m/J, -B/J];

B = [1/Ld,  0;
     0,  1/Lq;
     0,  0];

% Discretization using Euler method
Ad = eye(3) + A * Ts;
Bd = B * Ts;

% Initial state
x = [0; 0; 0];  % [id; iq; omega]

% Reference values
x_ref = [0; 5; 100];  % [id_ref; iq_ref; omega_ref]

% PID Controller Gains
Kp = diag([10, 10, 1]);   % Proportional gains
Ki = diag([100, 100, 50]); % Integral gains
Kd = diag([0.1, 0.1, 0.1]); % Derivative gains

% PID State-space Matrices
A_pid = eye(9);
A_pid(1:3, 4:6) = Ts * eye(3);  % Integrator states

B_pid = zeros(9, 3);
B_pid(1:3, :) = eye(3);
B_pid(7:9, :) = eye(3) / Ts;  % Derivative states

C_pid = [Kp, Ki, Kd];

% Initial PID states
x_pid = zeros(9, 1);

% Time vector
time = 0:Ts:Tsim;
x_log = zeros(length(time), 3);
Vd_log = zeros(length(time), 1);
Vq_log = zeros(length(time), 1);

% Simulation loop
for k = 1:length(time)
    % Save state for plotting
    x_log(k, :) = x';
    
    % Compute error
    e = x_ref - x;
    
    % PID state update
    x_pid = A_pid * x_pid + B_pid * e;
    
    % Compute control output (voltages)
    u = C_pid * x_pid;
    Vd = u(1);
    Vq = u(2);
    
    % Store voltages for plotting
    Vd_log(k) = Vd;
    Vq_log(k) = Vq;
    
    % Update PMSM states
    x = Ad * x + Bd * [Vd; Vq];
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

