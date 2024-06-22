SRCDIR=./src
INCLUDEDIR=./include
OBJDIR=./obj
EXT=

CC=clang
# -Wstrict-prototypes
CFLAGS=-Wall -Wextra -Wunreachable-code -Wwrite-strings -pedantic -std=c11 -fno-common -fno-builtin -ferror-limit=0 -I$(INCLUDEDIR)
LDFLAGS=
SRC= $(wildcard $(SRCDIR)/*.c)
HEADERS= $(wildcard $(INCLUDEDIR)/*.h)

# $@ est le nom de la cible
# $^ est la liste des dépendances
# $< est la première dépendance

# rajouter @ devant une commande pour la rendre silencieuse

ifeq ($(DEBUG),y)
	CFLAGS += -DDEBUG -g3 -O1
	EXT=-d
else ifeq ($(RETRO), y)
	CFLAGS += -O0
	EXT=retro
else
	CFLAGS += -O3 -DNDEBUG
endif

EXEC=struct$(EXT)
OBJ=$(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%$(EXT).o)

all: $(EXEC)

$(EXEC): $(OBJ) depend
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

# Construction automatique des .o à partir des .c
$(OBJDIR)/%$(EXT).o:$(SRCDIR)/%.c
	$(CC) -o $@ -c $< $(CFLAGS)

depend: .depend

.depend: $(SRC)
	$(CC) -MM $(SRC) -I $(INCLUDEDIR) > $@

include .depend

# .PHONY est la cible qui est systématiquement reconstruite (utile si il existe déjà des fichiers clean ou mrproper dans le répertoire courant, il serait alors forcément plus récent que ses dépendances et la règle ne serait alors jamais exécutée.
.PHONY: clean

clean:
	rm -rf $(OBJDIR)/*.o $(EXEC) .depend

rebuild: clean
	bear --force-wrapper -- make
