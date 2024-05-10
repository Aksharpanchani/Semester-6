import numpy as np
import matplotlib.pyplot as plt
import pandas

df2 = pandas.read_csv("data1.csv", names=["size", "runs", "sizeXruns", "throughput"])

#plt.plot(df["size"].values, df["throughput"].values, "r", label="O1")
plt.plot(df2["size"].values, df2["throughput"].values/(1024**3), "k.-", label="O2")
plt.xscale("log", basex=2)
plt.grid(True)
plt.ylabel("Throughput (GFlOPs)")
plt.xlabel(r"$N$")
plt.savefig("vtriad.png", format="png", dpi=300)
