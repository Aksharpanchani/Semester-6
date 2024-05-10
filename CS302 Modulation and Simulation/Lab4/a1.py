import matplotlib.pyplot as plt
import numpy as np

# Define the Gompertz equation
def gompertz(x, a, b):
  return -a * np.log(b * x)

# Rescale variables
def rescale(x, a, b):
  X = x / b**(-1)
  T = a * t
  return X, T

# Parameter-free differential equation
def dXdt(X):
  return -X * np.log(X)

# Initial values
X0_list = [0.1, 0.5, 0.9]

# Plot Xdot vs X for different initial values
for X0 in X0_list:
  # Define time range
  t = np.linspace(0, 5, 1000)

  # Solve the differential equation
  X = np.zeros_like(t)
  X[0] = X0
  for i in range(1, len(t)):
    X[i] = X[i-1] + dXdt(X[i-1]) * (t[i] - t[i-1])

  # Plot
  plt.plot(X, dXdt(X), label=f"X0 = {X0}")

# Label and show the plot
plt.xlabel("X")
plt.ylabel("Xdot")
plt.title("Xdot vs X for different initial values")
plt.legend()
plt.grid(True)
plt.show()