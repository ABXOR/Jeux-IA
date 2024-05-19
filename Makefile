# Noms des exécutables finaux
EXEC_CONSOLE = puissance4_console
EXEC_GUI = puissance4_gui

# Options du compilateur
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Fichiers sources communs
COMMON_SRCS = board.c list.c puissance4.c

# Fichiers sources spécifiques
CONSOLE_SRCS = mainconsole.c
GUI_SRCS = mainguiv2.c gui.c

# Fichiers objets générés
COMMON_OBJS = $(COMMON_SRCS:.c=.o)
CONSOLE_OBJS = $(CONSOLE_SRCS:.c=.o)
GUI_OBJS = $(GUI_SRCS:.c=.o)

# Fichiers d'en-tête
HEADERS = board.h list.h item.h puissance4.h ig.h

# Règle principale
all: $(EXEC_CONSOLE) $(EXEC_GUI)

# Règle pour générer l'exécutable pour la console
$(EXEC_CONSOLE): $(COMMON_OBJS) $(CONSOLE_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Règle pour générer l'exécutable pour l'interface graphique
$(EXEC_GUI): $(COMMON_OBJS) $(GUI_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lSDL2 -lSDL2_ttf

# Règle générique pour générer les fichiers objets
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

# Règle pour nettoyer les fichiers objets et les exécutables
clean:
	rm -f $(COMMON_OBJS) $(CONSOLE_OBJS) $(GUI_OBJS) $(EXEC_CONSOLE) $(EXEC_GUI)

# Règle pour exécuter le jeu en console
run_console: $(EXEC_CONSOLE)
	./$(EXEC_CONSOLE)

# Règle pour exécuter le jeu en interface graphique
run_gui: $(EXEC_GUI)
	./$(EXEC_GUI)
