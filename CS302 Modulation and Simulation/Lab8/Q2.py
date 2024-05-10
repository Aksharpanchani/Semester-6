import numpy as np
import matplotlib.pyplot as plt

# Define parameters
b = 0.05
f = 0.05
reserve_ships = 14
small_french_force = 3
initial_british_force = 27

# Time step
dt = 1

# Function to integrate Lanchester equations using Euler's method
def integrate_lanchester_equations(b, f, F0, B0):
    # Initial values
    F = F0
    B = B0
    
    # Lists to store values for plotting
    time_points = [0]
    French_ships = [F]
    British_ships = [B]
    
    # Euler's method iteration
    while F > 0 and B > 0:
        # Update forces using Lanchester equations
        dF = -b * B
        dB = -f * F
        F += dF * dt
        B += dB * dt
        
        # Append values to lists
        
        time_points.append(time_points[-1] + dt)
        French_ships.append(F)
        British_ships.append(B)
    
    return time_points, French_ships, British_ships

# Stage A: Initial stage of Lord Nelson's battle plan
F0_stage_A = 33 - small_french_force
B0_stage_A = 27 - small_french_force
time_A, French_A, British_A = integrate_lanchester_equations(b, f, F0_stage_A, B0_stage_A)

plt.plot(time_A, French_A, label='French Ships (Stage A)')
plt.plot(time_A, British_A, label='British Ships (Stage A)')

# Stage B: First stage of Lord Nelson's battle plan
F0_stage_B = French_A[-1]
B0_stage_B = British_A[-1]
time_B, French_B, British_B = integrate_lanchester_equations(b, f, F0_stage_B, B0_stage_B)

plt.plot(time_B, French_B, label='French Ships (Stage B)')
plt.plot(time_B, British_B, label='British Ships (Stage B)')

# Stage C: Second stage of Lord Nelson's battle plan
F0_stage_C = French_B[-1] + 17  # Surviving French ships from first stage + 17 larger force
B0_stage_C = British_B[-1] + reserve_ships  # Surviving British ships from first stage + reserve ships
time_C, French_C, British_C = integrate_lanchester_equations(b, f, F0_stage_C, B0_stage_C)

plt.plot(time_C, French_C, label='French Ships (Stage C)')
plt.plot(time_C, British_C, label='British Ships (Stage C)')

# Stage D: Third stage of Lord Nelson's battle plan
F0_stage_D = French_C[-1] + 13  # Surviving French ships from second stage + last group
B0_stage_D = British_C[-1]  # Remaining British ships from second stage
time_D, French_D, British_D = integrate_lanchester_equations(b, f, F0_stage_D, B0_stage_D)

plt.plot(time_D, French_D, label='French Ships (Stage D)')
plt.plot(time_D, British_D, label='British Ships (Stage D)')
print(British_C[-1])


# Full fleet engagement scenario
b_full_fleet = 0.1
f_full_fleet = 0.1
F0_full_fleet = 33
B0_full_fleet = 27
time_points_full_fleet, French_ships_full_fleet, British_ships_full_fleet = integrate_lanchester_equations(b_full_fleet, f_full_fleet, F0_full_fleet, B0_full_fleet)

# Plotting
plt.figure(figsize=(12, 8))

# Stages A, B, C, and D




# Full fleet engagement scenario
plt.plot(time_points_full_fleet, French_ships_full_fleet, '--', label='French Ships (Full Fleet Engagement)', color='black')
plt.plot(time_points_full_fleet, British_ships_full_fleet, '--', label='British Ships (Full Fleet Engagement)', color='red')

plt.xlabel('Time')
plt.ylabel('Number of Ships')
plt.title('Battle of Trafalgar: Lord Nelson\'s Battle Plan and Full Fleet Engagement')
plt.legend()
plt.grid(True)
plt.show()

# Print the number of British ships at the end of each stage
print("Number of British ships at the end of Stage A:", British_A[-1])
print("Number of British ships at the end of Stage B:", British_B[-1])
print("Number of British ships at the end of Stage C:", British_C[-1])
print("Number of British ships at the end of Stage D:", time_D[-1])
