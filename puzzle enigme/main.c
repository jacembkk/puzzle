#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_rotozoom.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "puzzle.h"

#define GAME_OVER_TIMEOUT 40000
#define PI 3.14159265

// Variables pour l'animation de rotoscope
float rotation_angle = 0.0f;
float scale = 1.0f;
float scale_direction = 0.01f;

int main(int argc, char *argv[]) {
    // Initialisation SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Erreur SDL_Init: %s\n", SDL_GetError());
        return 1;
    }

    // Initialisation SDL_ttf
    if (TTF_Init() != 0) {
        fprintf(stderr, "Erreur TTF_Init: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    // Création fenêtre
    SDL_Surface *screen = SDL_SetVideoMode(1920, 1080, 32, SDL_HWSURFACE);
    if (!screen) {
        fprintf(stderr, "Erreur SetVideoMode: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    SDL_WM_SetCaption("Puzzle Game", NULL);

    // Initialisation temps
    start_time = time(NULL);

    // Chargement des pièces
    const char *pieces_files[4] = {
        "piece0.png",
        "piece1.png",
        "piece2.png",
        "piece3.png"
    };

    Piece pieces[4];
    loadPieces(pieces, pieces_files, 4);
    shufflePieces(pieces, 4);

    // Variables jeu
    bool running = true;
    bool game_over = false;
    int selected_piece = -1;
    Uint32 game_start = SDL_GetTicks();

    // Boucle principale
    SDL_Event event;
    while (running) {
        // Gestion événements
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            if (!game_over) {
                if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                    for (int i = 0; i < 4; i++) {
                        SDL_Rect pos = pieces[i].position;
                        if (event.button.x >= pos.x && event.button.x <= pos.x + pos.w &&
                            event.button.y >= pos.y && event.button.y <= pos.y + pos.h) {
                            selected_piece = i;
                            pieces[i].isDragging = 1;
                            pieces[i].dragOffset.x = event.button.x - pos.x;
                            pieces[i].dragOffset.y = event.button.y - pos.y;
                            break;
                        }
                    }
                }

                if (event.type == SDL_MOUSEMOTION && selected_piece != -1) {
                    pieces[selected_piece].position.x = event.motion.x - pieces[selected_piece].dragOffset.x;
                    pieces[selected_piece].position.y = event.motion.y - pieces[selected_piece].dragOffset.y;
                }

                if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT && selected_piece != -1) {
                    pieces[selected_piece].isDragging = 0;
                    selected_piece = -1;
                }
            }
        }

        // Vérification fin de jeu
        if (!game_over && (SDL_GetTicks() - game_start >= GAME_OVER_TIMEOUT)) {
            game_over = true;
        }

        // RENDU VISUEL - ORDRE CRUCIAL
        // 1. Effacer l'écran
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

        if (!game_over) {
            // 2. Dessiner les pièces
            drawPieces(pieces, screen, 4);

            // 3. Dessiner le temps PAR-DESSUS les pièces
            AfficherTemps(screen);

            if (checkWinCondition(pieces, 4)) {
                SDL_WM_SetCaption("Gagné!", NULL);
            }
        } else {
            // Écran Game Over avec animation de rotoscope
            SDL_Color red = {255, 0, 0, 255};
            SDL_Surface* text = TTF_RenderUTF8_Solid(font, "TEMPS ECOULE!", red);
            if (text) {
                // Mise à jour des paramètres d'animation
                rotation_angle += 2.0f;
                if (rotation_angle >= 360.0f) rotation_angle = 0.0f;
                
                scale += scale_direction;
                if (scale > 1.2f || scale < 0.8f) scale_direction = -scale_direction;

                // Création d'une surface temporaire pour la rotation
                SDL_Surface* rotated = rotozoomSurface(text, rotation_angle, scale, 1);
                if (rotated) {
                    SDL_Rect pos = {
                        (screen->w - rotated->w)/2,
                        (screen->h - rotated->h)/2,
                        rotated->w,
                        rotated->h
                    };
                    SDL_BlitSurface(rotated, NULL, screen, &pos);
                    SDL_FreeSurface(rotated);
                }
                SDL_FreeSurface(text);
            }
        }

        SDL_Flip(screen);
        SDL_Delay(16); // Limite à ~60 FPS
    }

    // Nettoyage
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    return 0;
}

