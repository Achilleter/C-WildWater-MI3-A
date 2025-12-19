#!/bin/bash

# $4 correspond au quatrième argument dans la commande, $1 le premier ect.
FICHIER="$1" 

if [[ ! -f $FICHIER ]];then
	echo "Le fichier en argument n'est pas correct"
	exit 1
fi

# On identifie la demande pour renvoyer le résultat souhaité
if [[ "$2" == "histo" ]]; then
	if [[ "$3" == "max" ]]; then
		make run ARGS="\"$FICHIER\" histo max"
		#volume maximale de traitement de l’usine
	elif [[ "$3" == "src" ]]; then
		make run ARGS="\"$FICHIER\" histo src"
		#volume total capté par les sources
	elif [[ "$3" == "real" ]]; then
		make run ARGS="\"$FICHIER\" histo real"
		#volume total réellement traité
	else
		echo "L'argument après 'histo' n'a pas été compris"
		exit 1
	fi
	
	if [[ -f histo.dat ]]; then
		gnuplot gnuplot.gnu
		echo "Graphique généré : histo.png"
	else
		echo "Fichier histo.gnu introuvable"
	fi
	
	
	
	
	
	
elif [[ "$2" == "leaks" ]]; then 
	if [[ -z "$3" ]]; then
		echo "Usage: $0 "$FICHIER" leaks <USINE>"
		exit 1
	fi
	make run ARGS="\"$FICHIER\" leaks \"$3\""
	# Pertes d'une usine passé en paramètre 3 ( $3 )
    

elif [[ "$1" == "HELP" || "$2" == "HELP" ]]; then
	echo "	- histo max : volume maximale de traitement de l’usine
 	- histo src : volume total capté par les sources
	- histo real : volume total réellement trait
	- leaks 'REF_USINE' : mesure des pertes d’une usine  sur son réseau
	Et ne pas oublier que le premier argument doit être le nom du fichier que vous voulez analyser "
else
	echo "Aucun argument coresspondant, pour en savoir plus; exécuter avec comme premier argument: HELP"
	exit 1
fi