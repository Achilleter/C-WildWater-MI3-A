import matplotlib.pyplot as plt

plants = []
values = []

with open("histo.dat", "r") as f:
    for line in f:
        col1, col2, col3 = line.strip().split(",")
        if col3 == "max":
            plants.append(col1)
            values.append(float(col2)/1000)

plt.figure()
plt.title("Meilleures Usines (MAX)")
plt.xlabel("Plant IDs")
plt.ylabel("Volumes (M.m3)")
plt.bar(plants, values)
plt.xticks(rotation=45, ha="right")
plt.tight_layout()
plt.savefig("meilleures_usines_max.png", dpi=300)
plt.show()

plants = []
values = []

with open("histo.dat", "r") as f:
    for line in f:
        col1, col2, col3 = line.strip().split(",")
        if col3 == "min":
            plants.append(col1)
            values.append(float(col2)/1000)

plt.figure()
plt.title("Pires Usines (MIN)")
plt.xlabel("Plant IDs")
plt.ylabel("Volumes (M.m3)")
plt.bar(plants, values, color="orange")
plt.xticks(rotation=45, ha="right")
plt.savefig("pires_usines_min.png", dpi=300)
plt.tight_layout()
plt.show()
