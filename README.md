# 💧 C-WildWater-MI3-A 
Synthèse de données d’un système de distribution d’eau

Projet d’algorithmique en C — Pré-Ingénieur CY Tech, Semestre 3

## 👥 Collaborateurs

• Bernard Alexandre  
• Dumaire Alexis  
• Therin Achille  

## 📄 Description du projet
## 📌 Objectifs

Ce projet consiste à créer un programme Shell permettant de réaliser des opérations de calcul sur le fichier de données c-wildwater_v3.dat, ou sur tout autre fichier de format similaire.

## 📊 Génération d’histogrammes

Dans un premier temps, le programme permet la génération d’histogrammes basée sur :

🌲 Un AVL, permettant de traiter les usines dans un temps raisonnable.

À partir de cet AVL, des informations sont extraites afin de construire trois histogrammes :

• histo max : volume maximal de traitement de l’usine  
• histo src : volume total capté par les sources  
• histo real : volume total réellement traité  

## 💧 Calcul des fuites

Dans un second temps, le programme permet le calcul des fuites, basé sur :

🌳 Un arbre collectant les usines ainsi que l’ensemble de leur réseau aval

🌲 Indexé par un AVL, garantissant des recherches efficaces en O(log n)

## 📂 Format du fichier d’entrée

Le fichier d’entrée est un fichier texte de type CSV, de la forme :

ID_USINE;ID_AMONT;ID_AVAL;VOLUME;FUITES


Lorsque certaines valeurs sont absentes, le caractère - est utilisé.

## 🛠️ Compilation et exécution

Tout d’abord, utilisez le Makefile fourni :
make

Ensuite, accordez les permissions d’exécution au script Shell :  
chmod u+x script_shell.sh

Exécutez le script selon l’opération souhaitée :

💧 Calcul des fuites
./script_shell.sh "c-wildwater_v3.dat" leaks "nom_usine"  
👉 nom_usine correspond à l’identifiant de l’usine à analyser.  

📊 Génération des histogrammes  
./script_shell.sh "c-wildwater_v3.dat" histo max  
./script_shell.sh "c-wildwater_v3.dat" histo src  
./script_shell.sh "c-wildwater_v3.dat" histo real  

⏳ Il ne vous reste plus qu’à patienter et observer les résultats !
