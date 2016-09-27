CC=gcc
CFLAGS=-Wall
LDFLAGS=
EXEC=exec

all: $(EXEC)

exec: main.o keys.o
	$(CC) -o $@ $^ $(LDFLAGS)

main.o: keys.h
vehicule.o: couleur.h
%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
