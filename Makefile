CC=gcc
CFLAGS=-Wall
LDFLAGS=
EXEC=exec

all: $(EXEC)

exec: main.o vehicule.o couleur.o sound.o
	$(CC) -o $@ $^ $(LDFLAGS)

main.o: vehicule.h couleur.h sound.h
vehicule.o: couleur.h
couleur.o: couleur.h
sound.o: sound.h	
%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
