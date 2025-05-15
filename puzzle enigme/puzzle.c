#include "puzzle.h"
#include <stdlib.h>
#include <time.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>

TTF_Font *font = NULL;
time_t start_time;

void loadPieces(Piece pieces[], const char *filenames[], int count) {
    for (int i = 0; i < count; i++) {
        pieces[i].image = IMG_Load(filenames[i]);
        if (!pieces[i].image) {
            fprintf(stderr, "Erreur chargement image %s: %s\n", filenames[i], IMG_GetError());
        }
        pieces[i].correctIndex = i;
        pieces[i].currentIndex = i;
    }
}

void AfficherTemps(SDL_Surface *ecran) {
    // Essayer plusieurs polices système si arial n'est pas trouvée
    if (!font) {
        const char* fonts[] = {
            "arial.ttf",
            "/usr/share/fonts/truetype/freefont/FreeSans.ttf",
            "/usr/share/fonts/truetype/ubuntu/Ubuntu-R.ttf",
            NULL
        };
        
        for (int i = 0; fonts[i]; i++) {
            font = TTF_OpenFont(fonts[i], 36);
            if (font) break;
        }
        
        if (!font) {
            fprintf(stderr, "ERREUR: Impossible de charger aucune police\n");
            return;
        }
    }

    time_t now = time(NULL);
    int seconds = (int)difftime(now, start_time);
    char time_str[20];
    snprintf(time_str, sizeof(time_str), "Temps: %02d:%02d", seconds/60, seconds%60);

    SDL_Color color = {255, 255, 255, 255}; // Blanc sur fond noir
    SDL_Surface* text = TTF_RenderUTF8_Solid(font, time_str, color);
    
    if (!text) {
        fprintf(stderr, "Erreur création surface texte\n");
        return;
    }

    SDL_Rect pos = {20, 20, text->w, text->h};
    SDL_BlitSurface(text, NULL, ecran, &pos);
    SDL_FreeSurface(text);
}

void shufflePieces(Piece pieces[], int count) {
    SDL_Rect positions[4] = {
        {0, 0, 0, 0},
        {960, 0, 0, 0},
        {0, 540, 0, 0},
        {960, 540, 0, 0}};
    
    srand(time(NULL));
    for (int i = 3; i > 0; i--) {
        int j = rand() % (i + 1);
        SDL_Rect temp = positions[i];
        positions[i] = positions[j];
        positions[j] = temp;
    }
    
    for (int i = 0; i < count; i++) {
        pieces[i].position.x = positions[i].x;
        pieces[i].position.y = positions[i].y;
        if (pieces[i].image) {
            pieces[i].position.w = pieces[i].image->w;
            pieces[i].position.h = pieces[i].image->h;
        }
        pieces[i].isDragging = 0;
    }
}

void drawPieces(Piece pieces[], SDL_Surface *screen, int count) {
    // D'abord les pièces non déplacées
    for (int i = 0; i < count; i++) {
        if (!pieces[i].isDragging && pieces[i].image) {
            SDL_BlitSurface(pieces[i].image, NULL, screen, &pieces[i].position);
        }
    }
    // Ensuite les pièces déplacées (au-dessus)
    for (int i = 0; i < count; i++) {
        if (pieces[i].isDragging && pieces[i].image) {
            SDL_BlitSurface(pieces[i].image, NULL, screen, &pieces[i].position);
        }
    }
}

bool checkWinCondition(Piece pieces[], int count) {
    for (int i = 0; i < count; i++) {
        if (pieces[i].currentIndex != pieces[i].correctIndex)
            return false;
    }
    return true;
}

void initBackground(Background *bg, const char *path) {
    bg->image = IMG_Load(path);
    if (!bg->image) {
        fprintf(stderr, "Erreur chargement background: %s\n", IMG_GetError());
    }
}

void blitBackground(Background *bg, SDL_Surface *screen) {
    if (bg && bg->image && screen) {
        SDL_BlitSurface(bg->image, NULL, screen, NULL);
    }
}

void freeBackground(Background *bg) {
    if (bg && bg->image) {
        SDL_FreeSurface(bg->image);
        bg->image = NULL;
    }
}
