#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_rotozoom.h>
#include "menu_enigme.h"

#define GAME_OVER_TIMEOUT 40000

// Variables globales
time_t start_time;

// Déclaration des fonctions
void lancer_puzzle(SDL_Surface *screen); // À compléter si tu veux le puzzle
void lancer_quizz(SDL_Surface *screen);
float rotation_angle = 0.0f;
float scale = 1.0f;
float scale_direction = 0.01f;
int main(int argc, char *argv[]) {
    SDL_Surface *screen = NULL;
    SDL_Event event;
    bool running = true;
    Menu menu;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    screen = SDL_SetVideoMode(1920, 1080, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Menu", NULL);

    init_menu(&menu);

    while (running) {
        afficher_menu(screen, &menu);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            
            int choix = gerer_clic_menu(&menu, event);
            if (choix == 1) {
                printf("Bouton Quiz cliqué\n");
                lancer_quizz(screen);
            } else if (choix == 2) {
                printf("Bouton Puzzle cliqué\n");
                lancer_puzzle(screen);
            }
        }

        SDL_Delay(16);
    }

    liberer_menu(&menu);
    TTF_Quit();
    IMG_Quit();
    Mix_CloseAudio();
    SDL_Quit();
    return 0;
}

// ----------- FONCTION QUIZZ -----------
void lancer_quizz(SDL_Surface *screen) {
    Enigme e;
    Chrono c;
    int score = 0, vies = 3;
    int questions_posees[53] = {0};
    int total_questions = 53;
    SDL_Rect chrono_pos = {800, 50};
    const char *chemins[] = {
        "chrono0.png", "chrono1.png", "chrono2.png",
        "chrono3.png", "chrono4.png", "chrono5.png",
        "chrono6.png", "chrono7.png", "chrono8.png"
    };
    initialiser_sons(&e);
    Mix_PlayMusic(e.musique_enigme, -1);
    initChrono(&c, chemins, chrono_pos, 10000); // 10 secs
    SDL_Event event;
    int continuer = 1;
    while (continuer && vies > 0) {
        if (genererEnigmeAleatoire(&e, questions_posees, total_questions) == -1) {
            printf("Erreur chargement énigme\n");
            break;
        }
        int chrono_fini = 0, resolved = 0;
        int choix = -1;
        while (!chrono_fini && !resolved) {
            SDL_PollEvent(&event);
            if (event.type == SDL_QUIT)
                continuer = 0;
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                continuer = 0;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                choix = verifierClic(event, &e);
                if (choix != -1)
                    resolved = 1;
            }
            mettreAJourChrono(&c);
            afficherEnigme(screen, &e);
            afficherChrono(screen, &c);
            SDL_Flip(screen);
            if (c.frame_actuelle >= 8) {
                chrono_fini = 1;
            }
        }
        int resultat = 0;
        if (resolved) {
            resultat = resoudreEnigme(&e, choix);
        }
        gestionScoreViesLevel(&score, &vies, resultat);
        SDL_Surface *message;
        TTF_Font *font = TTF_OpenFont("Digitizer.ttf", 32);
        SDL_Color color = {0, 0, 0};
        if (resultat){
            message = TTF_RenderText_Blended(font, "Bonne reponse !", color);
            int reponse_juste = (choix == e.num_reponse);
            jouer_son_resultat(&e, reponse_juste);}
        else{
            message = TTF_RenderText_Blended(font, "Mauvaise reponse...", color);
            int reponse_juste = (choix == e.num_reponse);
            jouer_son_resultat(&e, reponse_juste);}
        SDL_Rect pos = {screen->w / 2 - message->w / 2, 500};
        afficherEnigme(screen, &e);
        afficherChrono(screen, &c);
        SDL_BlitSurface(message, NULL, screen, &pos);
        SDL_Flip(screen);
        SDL_Delay(2000);
        SDL_FreeSurface(message);
        TTF_CloseFont(font);
        libererEnigme(&e);
    }
    libererChrono(&c);
    liberer_sons(&e);
}

// ----------- FONCTION PUZZLE (à compléter) -----------
void lancer_puzzle(SDL_Surface *screen) {
    // ... ton code du puzzle ici ...
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

    // Boucle principale du puzzle
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

            // Retour au menu si on appuie sur Échap
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            }
        }

        // Vérification fin de jeu
        if (!game_over && (SDL_GetTicks() - game_start >= GAME_OVER_TIMEOUT)) {
            game_over = true;
        }

        // RENDU VISUEL
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

        if (!game_over) {
            drawPieces(pieces, screen, 4);
            AfficherTemps(screen);

            if (checkWinCondition(pieces, 4)) {
                SDL_WM_SetCaption("Gagné!", NULL);
            }
        } else {
            // Écran Game Over avec animation
            SDL_Color red = {255, 0, 0, 255};
            SDL_Surface* text = TTF_RenderUTF8_Solid(font, "TEMPS ECOULE!", red);
            if (text) {
                rotation_angle += 2.0f;
                if (rotation_angle >= 360.0f) rotation_angle = 0.0f;
                
                scale += scale_direction;
                if (scale > 1.2f || scale < 0.8f) scale_direction = -scale_direction;

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
        SDL_Delay(16);
    }

    // Nettoyage des pièces
    for (int i = 0; i < 4; i++) {
        if (pieces[i].image) {
            SDL_FreeSurface(pieces[i].image);
        }
    }
}
