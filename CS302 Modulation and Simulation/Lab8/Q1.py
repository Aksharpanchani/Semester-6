import matplotlib.pyplot as plt
import numpy as np

# Define parameters
a = 0.0106
j = 0.0544
J0 = 18274
A0 = 66454
dt = 1  # Time step (days)

# Initialize lists for storing troop numbers
J = [J0]
A = [A0]
t = [0]

# Stopping condition (one army falls below 1 troop)
while min(J[-1], A[-1]) > 1:
    # Update troop numbers using Euler's method
    J_new = J[-1] - a * A[-1] * dt
    A_new = A[-1] - j * J[-1] * dt

    # Append to lists
    J.append(J_new)
    A.append(A_new)
    t.append(t[-1] + dt)

# Calculate number of days and victorious army
days = len(t) - 1
victorious_army = "American" if A[-1] > J[-1] else "Japanese"  # Assuming victory for remaining army

# Calculate casualties (difference between initial and final troop numbers)
american_casualties = A0 - A[-1]
japanese_casualties = J0 - J[-1]

# Significance of casualty numbers
#The high casualty numbers on both sides highlight the ferocity of the Battle of Iwo Jima. Despite the American victory, the losses were significant, underscoring the human cost of war.

# Plot J and A with linear and logarithmic y-axes
plt.figure(figsize=(10, 6))

plt.subplot(121)
plt.plot(t, J, label='Japanese Troops')
plt.plot(t, A, label='American Troops')
plt.xlabel('Time (days)')
plt.ylabel('Troop Count')
plt.title('Linear Scale')
plt.legend()

plt.subplot(122)
plt.semilogy(t, J, label='Japanese Troops')
plt.semilogy(t, A, label='American Troops')
plt.xlabel('Time (days)')
plt.ylabel('Troop Count (Logarithmic)')
plt.title('Logarithmic Scale')
plt.legend()

plt.tight_layout()
plt.show()
