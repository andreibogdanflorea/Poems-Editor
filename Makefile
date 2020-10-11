CC = gcc
CFLAG = -Wall -Wextra -ltask_helper -L ./util
EXE = poems_editor
OBJ = poems_editor.o
DEPS = util/compare.h

build: $(EXE)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAG)

$(EXE): $(OBJ)
	$(CC) $^ $(CFLAG) -o $(EXE)

run:
	./poems_editor

.PHONY: clean

clean:
	rm -f *.o $(EXE)
