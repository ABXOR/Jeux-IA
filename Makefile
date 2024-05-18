# Nom de l'exécutable final
EXEC = puissance4

# Options du compilateur
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Fichiers sources
SRCS = board.c list.c puissance4.c
HEADERS = board.h list.h item.h puissance4.h

# Fichiers objets générés
OBJS = $(SRCS:.c=.o)

# Règle principale
all: $(EXEC)

# Règle pour générer l'exécutable final
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Règle générique pour générer les fichiers objets
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

# Règle pour nettoyer les fichiers objets et l'exécutable
clean:
	rm -f $(OBJS) $(EXEC)

# Règle pour exécuter le jeu
run: $(EXEC)
	./$(EXEC)
