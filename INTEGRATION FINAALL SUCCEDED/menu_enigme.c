#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <stdbool.h>
#include "menu_enigme.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Variables globales (définies ici UNE SEULE FOIS)
TTF_Font *font = NULL;
time_t start_time;

// Fonctions du menu
bool isMouseOver(SDL_Rect pos, int mouseX, int mouseY) {
    return (mouseX >= pos.x && mouseX <= pos.x + 250 &&
            mouseY >= pos.y && mouseY <= pos.y + 100);
}

void init_menu(Menu *menu) {
    menu->background = IMG_Load("background.png");
    menu->btnquizNormal = IMG_Load("bouton_quiz.png");
    menu->btnquizHover = IMG_Load("bouton_quiz_hover.png");
    menu->btnpuzzleNormal = IMG_Load("bouton_puzzle.png");
    menu->btnpuzzleHover = IMG_Load("bouton_puzzle_hover.png");

    if (!menu->background || !menu->btnquizNormal || !menu->btnquizHover ||
        !menu->btnpuzzleNormal || !menu->btnpuzzleHover) {
        printf("Erreur de chargement d'image : %s\n", SDL_GetError());
    }

    menu->font = TTF_OpenFont("Digitizer.ttf", 32);
    if (!menu->font) {
        printf("Erreur chargement police : %s\n", TTF_GetError());
    }

    SDL_Color textColor = {0, 0, 0};  // noir
    menu->quizText = TTF_RenderText_Blended(menu->font, "QUIZ", textColor);
    menu->puzzleText = TTF_RenderText_Blended(menu->font, "PUZZLE", textColor);

    menu->quizPos.x = 400; menu->quizPos.y = 200;
    menu->puzzlePos.x = 400; menu->puzzlePos.y = 350;
    menu->quizTextPos.x = menu->quizPos.x + 80; menu->quizTextPos.y = menu->quizPos.y + 20;
    menu->puzzleTextPos.x = menu->puzzlePos.x + 80; menu->puzzleTextPos.y = menu->puzzlePos.y + 20;
}

void afficher_menu(SDL_Surface *screen, Menu *menu) {
    int mx, my;
    SDL_GetMouseState(&mx, &my);

    SDL_BlitSurface(menu->background, NULL, screen, NULL);

    if (isMouseOver(menu->quizPos, mx, my))
        SDL_BlitSurface(menu->btnquizHover, NULL, screen, &menu->quizPos);
    else
        SDL_BlitSurface(menu->btnquizNormal, NULL, screen, &menu->quizPos);

    SDL_BlitSurface(menu->quizText, NULL, screen, &menu->quizTextPos);

    if (isMouseOver(menu->puzzlePos, mx, my))
        SDL_BlitSurface(menu->btnpuzzleHover, NULL, screen, &menu->puzzlePos);
    else
        SDL_BlitSurface(menu->btnpuzzleNormal, NULL, screen, &menu->puzzlePos);

    SDL_BlitSurface(menu->puzzleText, NULL, screen, &menu->puzzleTextPos);

    SDL_Flip(screen);
}

void liberer_menu(Menu *menu) {
    SDL_FreeSurface(menu->background);
    SDL_FreeSurface(menu->btnquizNormal);
    SDL_FreeSurface(menu->btnquizHover);
    SDL_FreeSurface(menu->btnpuzzleNormal);
    SDL_FreeSurface(menu->btnpuzzleHover);
    SDL_FreeSurface(menu->quizText);
    SDL_FreeSurface(menu->puzzleText);
    TTF_CloseFont(menu->font);
}

int gerer_clic_menu(Menu *menu, SDL_Event event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX = event.button.x;
        int mouseY = event.button.y;

        if (isMouseOver(menu->quizPos, mouseX, mouseY)) {
            return 1; // Code pour le quiz
        }
        else if (isMouseOver(menu->puzzlePos, mouseX, mouseY)) {
            return 2; // Code pour le puzzle
        }
    }
    return 0; // Aucun clic sur les boutons
}

// Fonctions du puzzle (inchangées, à compléter selon ton code d'origine)
// ... (garde ici toutes tes fonctions de gestion du puzzle) ...
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

    SDL_Color color = {255, 255, 255, 255};
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
    for (int i = 0; i < count; i++) {
        if (!pieces[i].isDragging && pieces[i].image) {
            SDL_BlitSurface(pieces[i].image, NULL, screen, &pieces[i].position);
        }
    }
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

// Fonctions du quizz (inchangées, à compléter selon ton code d'origine)
// ... (garde ici toutes tes fonctions de gestion du quizz) ...

void initialiser_sons(Enigme *e) {
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    e->musique_enigme = Mix_LoadMUS("son_enigme.mp3");
    e->son_bonne = Mix_LoadWAV("son_bonne.wav");
    e->son_mauvaise = Mix_LoadWAV("son_mauvaise.wav");

    if (!e->musique_enigme || !e->son_bonne || !e->son_mauvaise) {
        printf("Erreur chargement sons : %s\n", Mix_GetError());
    }
}

int initChrono(Chrono *c, const char *chemins[], SDL_Rect position, Uint32 intervalle) {
    for (int i = 0; i < 9; i++) {
        c->images[i] = IMG_Load(chemins[i]);
    }
    c->position.x = 700;
    c->position.y = 500;
    c->frame_actuelle = 0;
    c->dernier_temps = SDL_GetTicks();
    c->intervalle = intervalle;
    return 0 ;
}

int initEnigme(Enigme *e, char *fichier_enigmes) {
    FILE *f = fopen(fichier_enigmes, "r");
    if (!f) {
        printf("Erreur ouverture fichier enigmes.txt\n");
        return -1;
    }

    srand(SDL_GetTicks());
    int n = rand() % 12; // 12 énigmes 

    char buffer[256];
    int ligne_cible = n * 5; // chaque énigme a 5 lignes
    int ligne_actuelle = 0;

    while (fgets(buffer, 256, f)) {
        if (ligne_actuelle == ligne_cible) {
            strncpy(e->question, buffer, 256);
            e->question[strcspn(e->question, "\n")] = 0;
        } else if (ligne_actuelle == ligne_cible + 1) {
            strncpy(e->reponses[0], buffer, 256);
            e->reponses[0][strcspn(e->reponses[0], "\n")] = 0;
        } else if (ligne_actuelle == ligne_cible + 2) {
            strncpy(e->reponses[1], buffer, 256);
            e->reponses[1][strcspn(e->reponses[1], "\n")] = 0;
        } else if (ligne_actuelle == ligne_cible + 3) {
            strncpy(e->reponses[2], buffer, 256);
            e->reponses[2][strcspn(e->reponses[2], "\n")] = 0;
        } else if (ligne_actuelle == ligne_cible + 4) {
            e->num_reponse = atoi(buffer); // 0, 1 ou 2
            break;
        }
        ligne_actuelle++;
    }

    fclose(f);

    e->image_bouton = IMG_Load("bouton.png");
    if (!e->image_bouton) {
        printf("Erreur chargement image bouton\n");
        return -1;
    }

    e->background = IMG_Load("background.png");
    if (!e->background) {
        printf("Erreur chargement background: %s\n", IMG_GetError());
        return -1;
    }

    for (int i = 0; i < 3; i++) {
        e->pos_bouton[i].x = 50;
        e->pos_bouton[i].y = 150 + i * 100;
    }

    return 0;
}

void afficherEnigme(SDL_Surface *screen, Enigme *e) {
    TTF_Font *font = TTF_OpenFont("Digitizer.ttf", 32);
    SDL_Color color = {0, 0, 0};
    SDL_Surface *textSurface = NULL;
    SDL_Rect textPos;

    // affichage du background
    SDL_BlitSurface(e->background, NULL, screen, NULL);

    // affichage question
    textSurface = TTF_RenderText_Blended(font, e->question, color);
    textPos.x = 50;
    textPos.y = 50;
    SDL_BlitSurface(textSurface, NULL, screen, &textPos);
    SDL_FreeSurface(textSurface);

    //  Affichage  réponses + boutons
    for (int i = 0; i < 3; i++) {
        textSurface = TTF_RenderText_Blended(font, e->reponses[i], color);
        textPos.x = 70;
        textPos.y = 150 + i * 100;

        e->pos_bouton[i].x = textPos.x-850;
        e->pos_bouton[i].y = textPos.y-500;

        //  Affichage  bouton
        SDL_BlitSurface(e->image_bouton, NULL, screen, &e->pos_bouton[i]);

        //  Affichage texte au dessus du bouton
        SDL_BlitSurface(textSurface, NULL, screen, &textPos);
        SDL_FreeSurface(textSurface);
    }
    SDL_Flip(screen);
    TTF_CloseFont(font);
}
int genererEnigmeAleatoire(Enigme *e, int *questions_posees, int total_questions) {
    int index;
    do {
        index = rand() % total_questions;
    } while (questions_posees[index] == 1);

    questions_posees[index] = 1;

    char fichier[] = "enigmes.txt";
    return initEnigme(e, fichier); 
}

int verifierClic(SDL_Event event, Enigme *e) {
    for (int i = 0; i < 3; i++) {
        SDL_Rect pos = e->pos_bouton[i];
        int w = e->image_bouton->w;
        int h = e->image_bouton->h;

        if (event.button.x >= pos.x &&
            event.button.x <= pos.x + w &&
            event.button.y >= pos.y &&
            event.button.y <= pos.y + h) {
            return i; 
        }
    }
    return -1; 
}

int resoudreEnigme(Enigme *e, int choix_joueur) {
    
    return (choix_joueur == e->num_reponse);
}

void gestionScoreViesLevel(int *score, int *vies, int resultat) {
    if (resultat == 1) {
        (*score)++;
        (*vies)++;
        
    } else {
        (*score)--;
        if (*score <= 0)
            (*vies) = 0;
            
    }
}

void libererEnigme(Enigme *e) {
    if (e->texte) SDL_FreeSurface(e->texte);
    for (int i = 0; i < 3; i++)
        if (e->reponse_txt[i]) SDL_FreeSurface(e->reponse_txt[i]);
}


void afficherChrono(SDL_Surface *screen, Chrono *c) {
    SDL_BlitSurface(c->images[c->frame_actuelle], NULL, screen, &c->position);
}

void mettreAJourChrono(Chrono *c) {
    Uint32 temps_actuel = SDL_GetTicks();
    if (temps_actuel - c->dernier_temps >= c->intervalle) {
        c->frame_actuelle++;
        c->dernier_temps = temps_actuel;
        if (c->frame_actuelle >= 9) {
            c->frame_actuelle = 8; 
        }
    }
}

void libererChrono(Chrono *c) {
    for (int i = 0; i < 9; i++) {
        SDL_FreeSurface(c->images[i]);
    }
}
void jouer_son_resultat(Enigme *e, int reponse_juste) {
    Mix_HaltMusic();

    if (reponse_juste)
        Mix_PlayChannel(-1, e->son_bonne, 0);
    else
        Mix_PlayChannel(-1, e->son_mauvaise, 0);
}

void resetChrono(Chrono *c) {
    c->frame_actuelle = 0;
    c->dernier_temps = SDL_GetTicks();
}

void liberer_sons(Enigme *e) {
    Mix_FreeMusic(e->musique_enigme);
    Mix_FreeChunk(e->son_bonne);
    Mix_FreeChunk(e->son_mauvaise);
    Mix_CloseAudio();
}

