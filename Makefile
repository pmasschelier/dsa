SRCDIR=./src
INCLUDEDIR=./include
OBJDIR=./obj
EXT=

CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c11 -fno-common -fno-builtin -I$(INCLUDEDIR)
LDFLAGS=
SRC= $(wildcard $(SRCDIR)/*.c)
HEADERS= $(wildcard $(INCLUDEDIR)/*.h)

# $@ est le nom de la cible
# $^ est la liste des dépendances
# $< est la première dépendance

# rajouter @ devant une commande pour la rendre silencieuse

ifeq ($(DEBUG),y)
	CFLAGS += -DDEBUG -g3 -O0
	EXT=-d
else ifeq($(RETRO), y)
	CFLAGS += -O0
	EXT=retro
else
	CFLAGS += -O3 -DNDEBUG
endif

EXEC=struct$(EXT)
OBJ=$(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%$(EXT).o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

# Construction automatique des .o à partir des .c
$(OBJDIR)/%$(EXT).o:$(SRCDIR)/%.c $(HEADERS)
	$(CC) -o $@ -c $< $(CFLAGS)


# .PHONY est la cible qui est systématiquement reconstruite (utile si il existe déjà des fichiers clean ou mrproper dans le répertoire courant, il serait alors forcément plus récent que ses dépendances et la règle ne serait alors jamais exécutée.
.PHONY: clean

clean:
	rm -rf $(OBJDIR)/*.o $(EXEC)

rebuild: clean all
