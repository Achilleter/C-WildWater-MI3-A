# Nom de l'exécutable
PROG = C-WILDWATER-MI3-A

# Fichiers sources
SRC = main.c leaks.c histo.c utilitaire.c

# Options de compilation
CFLAGS = -Wall -Wextra -g 

# Règle par défaut : compile puis exécute
all: $(PROG)

run: $(PROG)
	./$(PROG) $(ARGS)
# Compilation de l'exécutable
$(PROG): $(SRC) Principal.h
	gcc $(CFLAGS) -o $(PROG) $(SRC)

# Nettoyage des fichiers générés
clean:
	rm -f $(PROG)