CFLAGS = -g
LIBS = -lSDL -lSDL_image -lSDL_ttf -lSDL_gfx  # Important:  -lSDL_ttf doit être inclus !
EXEC_NAME = puzzle  # Nom de l'exécutable

$(EXEC_NAME): main.o puzzle.o  # Dépendances et commande de liaison
	gcc $(CFLAGS) main.o puzzle.o -o $(EXEC_NAME) $(LIBS)

main.o: main.c puzzle.h  # Dépendances et commande de compilation de main.c
	gcc -c main.c $(CFLAGS)

puzzle.o: puzzle.c puzzle.h  # Dépendances et commande de compilation de puzzle.c
	gcc -c puzzle.c $(CFLAGS)

clean:  # Commande pour nettoyer les fichiers intermédiaires
	rm -f $(EXEC_NAME) main.o puzzle.o


