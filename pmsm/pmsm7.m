clear; clc; close all;
graphics_toolkit("gnuplot")
setenv("GNUTERM", "dumb");  % Use ASCII plotting mode

% PMSM Parameters
R_s = 0.5;        % Stator resistance (ohms)
L_d = 0.008;      % d-axis inductance (H)
L_q = 0.008;      % q-axis inductance (H)
lambda_m = 0.175; % Permanent magnet flux linkage (Wb)
P = 4;            % Number of poles
J = 0.01;         % Inertia (kg.m^2)
B = 0.001;        % Damping coefficient
T_L = 0;          % Load torque (Nm)

% Simulation parameters
dt = 1e-3;        % Time step (s)
t_end = 0.5;      % Total simulation time (s)
t = 0:dt:t_end;   % Time vector

% Initial conditions
i_d = 0;
i_q = 0;
omega = 0;

% Input voltages (step input for simplicity)
v_d = 0;         % Assume v_d = 0 for simplification
v_q = 100;       % Apply voltage in q-axis

% Storage for animation
i_d_log = zeros(size(t));
i_q_log = zeros(size(t));
omega_log = zeros(size(t));

% Simulation loop
for k = 1:length(t)
    % Electrical dynamics (Euler discretization)
    di_d = (v_d - R_s * i_d + omega * L_q * i_q) / L_d;
    di_q = (v_q - R_s * i_q - omega * L_d * i_d - omega * lambda_m) / L_q;
    
    i_d = i_d + di_d * dt;
    i_q = i_q + di_q * dt;
    
    % Compute torque
    T_e = (3/2) * (P/2) * (lambda_m * i_q + (L_d - L_q) * i_d * i_q);
    
    % Mechanical dynamics (Euler discretization)
    domega = (T_e - T_L - B * omega) / J;
    omega = omega + domega * dt;
    
    % Store results
    i_d_log(k) = i_d;
    i_q_log(k) = i_q;
    omega_log(k) = omega;
    
    % --- Real-time ASCII plotting ---
    clf;  % Clear the figure before redrawing
    subplot(3,1,1);
    plot(t(1:k), i_d_log(1:k), 'b');
    xlabel('Time (s)'); ylabel('i_d (A)');
    title('d-axis Current');
    grid on;
    
    subplot(3,1,2);
    plot(t(1:k), i_q_log(1:k), 'r');
    xlabel('Time (s)'); ylabel('i_q (A)');
    title('q-axis Current');
    grid on;
    
    subplot(3,1,3);
    plot(t(1:k), omega_log(1:k), 'k');
    xlabel('Time (s)'); ylabel('\omega (rad/s)');
    title('Rotor Speed');
    grid on;
    
    drawnow();  % Force Octave to update the plot
    pause(0.01);  % Pause to simulate real-time effect
end



