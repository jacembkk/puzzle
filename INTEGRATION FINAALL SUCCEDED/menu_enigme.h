#ifndef ENIGME_H
#define ENIGME_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <stdbool.h>
#include <time.h>

// Structures du menu
typedef struct {
    SDL_Surface *background;
    SDL_Surface *btnquizNormal, *btnquizHover;
    SDL_Surface *btnpuzzleNormal, *btnpuzzleHover;
    SDL_Surface *quizText, *puzzleText;
    SDL_Rect quizPos, puzzlePos;
    SDL_Rect quizTextPos, puzzleTextPos;
    TTF_Font *font;
} Menu;

// Structures du puzzle
typedef struct {
    SDL_Surface *image;
    SDL_Rect position;
    int correctIndex;
    int currentIndex;
    int isDragging;
    SDL_Rect dragOffset;
} Piece;

typedef struct {
    SDL_Surface *image;
} Background;

// Variables globales
extern TTF_Font *font;
extern time_t start_time;

// Fonctions du menu
bool isMouseOver(SDL_Rect pos, int mouseX, int mouseY);
void init_menu(Menu *menu);
void afficher_menu(SDL_Surface *screen, Menu *menu);
void liberer_menu(Menu *menu);
int gerer_clic_menu(Menu *menu, SDL_Event event);

// Fonctions du puzzle
void AfficherTemps(SDL_Surface* ecran);
void loadPieces(Piece pieces[], const char *filenames[], int count);
void shufflePieces(Piece pieces[], int count);
void drawPieces(Piece pieces[], SDL_Surface *screen, int count);
bool checkWinCondition(Piece pieces[], int count);
void initBackground(Background *bg, const char *path);
void blitBackground(Background *bg, SDL_Surface *screen);
void freeBackground(Background *bg);

// Structures et fonctions du quizz
typedef struct {
    SDL_Surface *images[9];
    SDL_Rect position;
    int frame_actuelle;
    Uint32 dernier_temps;
    Uint32 intervalle;
} Chrono;

typedef struct {
    char question[256];
    char reponses[3][256];
    int num_reponse;

    SDL_Surface *texte;
    SDL_Surface *reponse_txt[3];
    SDL_Surface *image_bouton;
    SDL_Surface *background;
    SDL_Rect pos_bouton[3];
    Mix_Music *musique_enigme;
    Mix_Chunk *son_bonne;
    Mix_Chunk *son_mauvaise;
} Enigme;

int initEnigme(Enigme *e, char *fichier_enigmes);
void afficherEnigme(SDL_Surface *screen, Enigme *e);
int genererEnigmeAleatoire(Enigme *e, int *questions_posees, int total_questions);
int verifierClic(SDL_Event event, Enigme *e);
int resoudreEnigme(Enigme *e, int choix_joueur);
void gestionScoreViesLevel(int *score, int *vies, int resultat);
void libererEnigme(Enigme *e);
int initChrono(Chrono *c, const char *chemins[], SDL_Rect position, Uint32 intervalle);
void afficherChrono(SDL_Surface *screen, Chrono *c);
void mettreAJourChrono(Chrono *c);
void libererChrono(Chrono *c);
void initialiser_sons(Enigme *e);
void liberer_sons(Enigme *e);
void jouer_son_resultat(Enigme *e, int reponse_juste);
void resetChrono(Chrono *c);

#endif
