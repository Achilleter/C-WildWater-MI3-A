#!/bin/bash

# $4 correspond au quatrième argument dans la commande, $1 le premier ect.
FICHIER="$1" 

if [[ -f $FICHIER ]];then
	echo "Le fichier en argument n'est pas correct"
	exit 1
fi

# On identifie la demande pour renvoyer le résultat souhaité
if [[ "$2" == "histo" ]]; then
	if [[ "$3" == "max" ]]; then
		echo "on est dans histo max" #volume maximale de traitement de l’usine
	elif [[ "$4" == "src" ]]; then
		echo "on est dans histo src" #volume total capté par les sources
	elif [[ "$4" == "real" ]]; then
		echo "on est dans histo real" #volume total réellement traité
	else
		echo "L'argument après 'histo' n'a pas été compris"
		exit 1
	fi
elif [[ "$2" == "leaks" ]]; then 
	echo "on est dans leaks" # Pertes d'une usine passé en paramètre 3 ( $3 )
    

elif [[ "$2" == "HELP" ]]; then
	echo "	- histo max : volume maximale de traitement de l’usine
 	- histo src : volume total capté par les sources
	- histo real : volume total réellement trait
	- leaks 'REF_USINE' : mesure des pertes d’une usine  sur son réseau"
else
	echo "Aucun argument coresspondant, pour en savoir plus; exécuter avec comme premier argument: HELP"
	exit 1
fi

#cat $FICHIER >> notes.txt # CECI EST TEMPORAIRE
