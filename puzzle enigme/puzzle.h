#ifndef PUZZLE_H
#define PUZZLE_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <stdbool.h>
#include <time.h>

extern TTF_Font *font;
extern time_t start_time;

typedef struct {
    SDL_Surface *image;
    SDL_Rect position;
    int correctIndex;
    int currentIndex;
    int isDragging;
    SDL_Rect dragOffset;
} Piece;

void AfficherTemps(SDL_Surface* ecran);
void loadPieces(Piece pieces[], const char *filenames[], int count);
void shufflePieces(Piece pieces[], int count);
void drawPieces(Piece pieces[], SDL_Surface *screen, int count);
bool checkWinCondition(Piece pieces[], int count);

typedef struct {
    SDL_Surface *image;
} Background;

void initBackground(Background *bg, const char *path);
void blitBackground(Background *bg, SDL_Surface *screen);
void freeBackground(Background *bg);

#endif
