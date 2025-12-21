#!/bin/bash
# $1 : chemin du fichier de données
# $2 : commande principale (histo / leaks / HELP)
# $3 : option (max / src / real)

# Vérification de la présence du fichier en argument
if [[ $# -gt 3 ]]; then
	echo "Trop d'argument"
	exit 1
fi

FICHIER="$1" 

if [[ ! -f $FICHIER ]];then
	echo "Le fichier en argument n'est pas correct"
	exit 1
fi

# On identifie la demande pour renvoyer le résultat souhaité
if [[ "$2" == "histo" ]]; then

	# Sous-options autorisées : max / src / real
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
		# Cas d’erreur :
		echo "L'argument après 'histo' n'a pas été compris"
		exit 1
	fi
	
	if [[ -f histo.dat ]]; then
		# Génération du graphique via Python
		python3 HISTOGRAM.py
		echo "Graphique généré : histo.png"
	else
		# Cas d’erreur :
		echo "Fichier histo.dat introuvable"
		exit 1
	fi
	
elif [[ "$2" == "leaks" ]]; then 
	# Vérification de la présence de l’identifiant usine
	if [[ -z "$3" ]]; then
		echo "Usage: $0 "$FICHIER" leaks <USINE>"
		exit 1
	fi
	# Appel du programme C pour calculer les pertes
	make run ARGS="\"$FICHIER\" leaks \"$3\""
    

elif [[ "$1" == "HELP" || "$2" == "HELP" ]]; then
	# Affichage des commandes disponibles
	echo "	- histo max : volume maximale de traitement de l’usine
 	- histo src : volume total capté par les sources
	- histo real : volume total réellement trait
	- leaks 'REF_USINE' : mesure des pertes d’une usine  sur son réseau
	Et ne pas oublier que le premier argument doit être le nom du fichier que vous voulez analyser "
else
	echo "Aucun argument coresspondant, pour en savoir plus; exécuter avec comme premier argument: HELP"
	exit 1
fi
