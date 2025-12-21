# ---------------------------------------------------------------------------------------
# Script pour générer des histogrammes à partir du fichier histo.dat
# Affiche les meilleures et les pires usines selon les volumes calculés
# ---------------------------------------------------------------------------------------

import matplotlib.pyplot as plt

# -----------------------------------
# PREMIER HISTOGRAMME : Meilleures Usines (MAX)
# -----------------------------------

plants = []  # Liste pour stocker les identifiants des usines
values = []  # Liste pour stocker les volumes correspondants

with open("histo.dat", "r") as f:
    for line in f:
        # Chaque ligne a trois colonnes séparées par des virgules : id, volume, type (max/min)
        col1, col2, col3 = line.strip().split(",")
        # On ne garde que les lignes avec type "max"
        if col3 == "max":
            plants.append(col1)                 # Ajout de l'identifiant de l'usine
            values.append(float(col2)/1000)     # Conversion du volume en M.m3 et ajout à la liste


# Création du graphique
plt.figure()
plt.title("Meilleures Usines (MAX)") # Titre du graphique
plt.xlabel("Plant IDs")
plt.ylabel("Volumes (M.m3)")
plt.bar(plants, values)
plt.xticks(rotation=45, ha="right")
plt.tight_layout()
plt.savefig("meilleures_usines_max.png", dpi=300) # Sauvegarde du graphique en PNG
plt.show() # Affichage du graphique à l'écran

# -----------------------------------
# DEUXIÈME HISTOGRAMME : Pires Usines (MIN)
# -----------------------------------

plants = [] # Réinitialisation des listes
values = []

with open("histo.dat", "r") as f:
    for line in f:
        col1, col2, col3 = line.strip().split(",")
        # On ne garde que les lignes avec type "min"
        if col3 == "min":
            plants.append(col1)                 # Ajout de l'identifiant de l'usine
            values.append(float(col2)/1000)     # Conversion du volume en M.m3 et ajout à la liste

plt.figure()
plt.title("Pires Usines (MIN)") # Titre du graphique
plt.xlabel("Plant IDs")
plt.ylabel("Volumes (M.m3)")
plt.bar(plants, values, color="orange")
plt.xticks(rotation=45, ha="right")
plt.savefig("pires_usines_min.png", dpi=300) # Sauvegarde du graphique en PNG
plt.tight_layout()
plt.show() # Affichage du graphique à l'écran
