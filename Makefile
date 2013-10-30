CC=gcc
LDFLAGS=-lSDL -lSDL_ttf -lncurses
# -lmingw32 => needed under Windows
CFLAGS=-W -Wall -Wextra -pedantic -std=c99
EXEC=zPlus
SRC= $(wildcard src/*.c)
OBJ= $(SRC:src/%.c=obj/%.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	@echo "====	Édition des liens ($^)"
	$(CC) -o "$@" $^ $(LDFLAGS)

obj/%.o: src/%.c
	@echo "====	Compilation de '$<' ($?)"
	$(CC) -o "$@" -c "$<" $(CFLAGS)
