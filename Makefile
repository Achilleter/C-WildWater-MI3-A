# Nom de l'exécutable
PROG = C-WILDWATER-MI3-A

# Fichiers sources
SRC = main.c leaks.c

# Options de compilation
CFLAGS = -Wall -Wextra -g
LDFLAGS = 

# Règle par défaut : compile puis exécute
all: $(PROG)
	./$(PROG)

# Compilation de l'exécutable
$(PROG): $(SRC) Principal.h
	gcc $(CFLAGS) -o $(PROG) $(SRC) $(LDFLAGS)

# Nettoyage des fichiers générés
clean:
	rm -f $(PROG)