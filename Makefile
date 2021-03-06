CC=gcc
CFLAGS=-Wall
SDLFLAGS=-lSDL2
MATHFLAGS=-lm
NCURSESFLAGS=-lncurses
EXEC=exec

all: $(EXEC)

run:
	./$(EXEC)


exec: main.o game.o display.o vehicule.o couleur.o sound.o list_car.o
	$(CC) -o $@ $^ $(CFLAGS) $(SDLFLAGS) $(MATHFLAGS) $(NCURSESFLAGS)

main.o: game.h
game.o: display.h vehicule.h couleur.h sound.h list_car.h
vehicule.o: couleur.h display.h
couleur.o: couleur.h
sound.o: sound.h
display.o: display.h vehicule.h couleur.h list_car.h game.h
list_car.o: list_car.h vehicule.h
%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(SDLFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
