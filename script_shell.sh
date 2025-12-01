#!/bin/bash

# $4 correspond au quatrième argument dans la commande, $1 le premier ect.
FICHIER="$4" 

# On identifie la demande pour renvoyer le résultat souhaité
if [[ "$1" == "histo" ]]; then
	if [[ "$2" == "max" ]]; then
		echo "on est dans histo max" #volume maximale de traitement de l’usine
	elif [[ "$2" == "src" ]]; then
		echo "on est dans histo src" #volume total capté par les sources
	elif [[ "$2" == "real" ]]; then
		echo "on est dans histo real" #volume total réellement traité
	fi
elif [[ "$1" == "leaks" ]]; then 
	echo "on est dans leaks" # Pertes d'une usine passé en paramètre 3 ( $3 )
    

elif [[ "$1" == "HELP" ]]; then
	echo "	- histo max : volume maximale de traitement de l’usine
 	- histo src : volume total capté par les sources
	- histo real : volume total réellement trait
	- leaks 'REF_USINE' : mesure des pertes d’une usine  sur son réseau"
else
	echo "Aucun argument coresspondant, pour en savoir plus; exécuter avec comme premier argument: HELP"
fi

#cat $FICHIER >> notes.txt # CECI EST TEMPORAIRE
