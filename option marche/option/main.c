#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>

int main(int argc, char** argv) {
    SDL_Surface *ecran, *image, *page1Image, *surfaceTexte;
    SDL_Surface *button1, *button2, *button3, *button4, *button5 ,*button1Hover ,*button2Hover,*button3Hover,*button4Hover,*button5Hover;
    SDL_Rect posecranimg, positiontext, button1Pos, button2Pos, button3Pos, button4Pos, button5Pos;
    TTF_Font *font;
    SDL_Color textColor = {0, 0, 0};
    SDL_Event event;
    Mix_Music *musique;


    int quitter = 1, play = 0;
    int state = 0; // 0: Main screen, 1: Page 1
    int volume = MIX_MAX_VOLUME / 2;
    int fullscreen = 0;
    int hoverButton1 = 0;
    int hoverButton2 = 0;
    int hoverButton3 = 0;
    int hoverButton4 = 0;
    int hoverButton5 = 0;


    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0 || TTF_Init() == -1 || Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
        printf("Erreur d'initialisation: %s\n", SDL_GetError());
        return 1;
    }

    // Create window
    ecran = SDL_SetVideoMode(1400, 700, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (!ecran) {
        printf("Erreur de création de fenêtre: %s\n", SDL_GetError());
        return 1;
    }

    // Load images
    image = IMG_Load("imagev.jpg");
    page1Image = IMG_Load("imagevv.jpg");
    button1 = IMG_Load("option hover.png");
    button2 = IMG_Load("minus1.png");
    button3 = IMG_Load("plus1.png");
    button4 = IMG_Load("grow.png");
    button5 = IMG_Load("retour.png");

    button1Hover = IMG_Load("option.png");
    button2Hover = IMG_Load("minus1 hover.png");
    button3Hover = IMG_Load("plus1 hover.png");
    button4Hover = IMG_Load("grow hover.png");
    button5Hover = IMG_Load("retour hover.png");

    if (!image || !button1 || !button2 || !button3 || !page1Image|| !button4 || !button5 || !button1Hover || !button2Hover|| !button3Hover|| !button4Hover|| !button5Hover) {
        printf("Erreur de chargement des images: %s\n", IMG_GetError());
        return 1;
    }

    // Set positions
    posecranimg.x = 0; posecranimg.y = 0;
    button1Pos.x = 590; button1Pos.y = 250;
    button2Pos.x = 750; button2Pos.y = 290;
    button3Pos.x = 600; button3Pos.y = 290;
    button4Pos.x = 675; button4Pos.y = 410;
    button5Pos.x = 680; button5Pos.y = 545;


    // Load font
    font = TTF_OpenFont("arial.ttf", 20);
    if (!font) {
        printf("Erreur de chargement de la police: %s\n", TTF_GetError());
        return 1;
    }

    // Load music
    musique = Mix_LoadMUS("hiha.mp3");
    if (!musique) {
        printf("Erreur de chargement de la musique: %s\n", Mix_GetError());
        return 1;
    }

    Mix_PlayMusic(musique, -1);
    Mix_VolumeMusic(volume);

    // Main loop
    while (quitter) {
        if (state == 0) {
    SDL_BlitSurface(image, NULL, ecran, &posecranimg);
} else if (state == 1) {
    SDL_BlitSurface(page1Image, NULL, ecran, &posecranimg);
}


        if (state == 0) {
            SDL_BlitSurface(hoverButton1 ? button1Hover : button1, NULL, ecran, &button1Pos);


            
        } else if (state == 1) {
            SDL_BlitSurface(button2, NULL, ecran, &button2Pos);
            SDL_BlitSurface(button3, NULL, ecran, &button3Pos);
            SDL_BlitSurface(button5, NULL, ecran, &button5Pos);
  	    SDL_BlitSurface(button4, NULL, ecran, &button4Pos);
            SDL_BlitSurface(hoverButton2 ? button2Hover : button2, NULL, ecran, &button2Pos);
            SDL_BlitSurface(hoverButton3 ? button3Hover : button3, NULL, ecran, &button3Pos);
            SDL_BlitSurface(hoverButton4 ? button4Hover : button4, NULL, ecran, &button4Pos);
            SDL_BlitSurface(hoverButton5 ? button5Hover : button5, NULL, ecran, &button5Pos);


            // Render "Volume" text
            surfaceTexte = TTF_RenderText_Solid(font, "Volume", textColor);
            if (surfaceTexte) {
                positiontext.x = 625;
                positiontext.y = 100;
                //SDL_BlitSurface(surfaceTexte, NULL, ecran, &positiontext);
                //SDL_FreeSurface(surfaceTexte);
            }
	
        }

        while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
        quitter = 0;
    } else if (event.type == SDL_MOUSEMOTION) {
        int x = event.motion.x, y = event.motion.y;
        hoverButton1 = (x >= button1Pos.x && x <= button1Pos.x + button1->w && y >= button1Pos.y && y <= button1Pos.y + button1->h);
        hoverButton2 = (x >= button2Pos.x && x <= button2Pos.x + button2->w && y >= button2Pos.y && y <= button2Pos.y + button2->h);
        hoverButton3 = (x >= button3Pos.x && x <= button3Pos.x + button3->w && y >= button3Pos.y && y <= button3Pos.y + button3->h);
        hoverButton4 = (x >= button4Pos.x && x <= button4Pos.x + button4->w && y >= button4Pos.y && y <= button4Pos.y + button4->h);
        hoverButton5 = (x >= button5Pos.x && x <= button5Pos.x + button5->w && y >= button5Pos.y && y <= button5Pos.y + button5->h);
   

    } else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        int x = event.button.x, y = event.button.y;
        hoverButton1 = (x >= button1Pos.x && x <= button1Pos.x + button1->w && y >= button1Pos.y && y <= button1Pos.y + button1->h);
        hoverButton2 = (x >= button2Pos.x && x <= button2Pos.x + button2->w && y >= button2Pos.y && y <= button2Pos.y + button2->h);
        hoverButton3 = (x >= button3Pos.x && x <= button3Pos.x + button3->w && y >= button3Pos.y && y <= button3Pos.y + button3->h);
        hoverButton4 = (x >= button4Pos.x && x <= button4Pos.x + button4->w && y >= button4Pos.y && y <= button4Pos.y + button4->h);
        hoverButton5 = (x >= button5Pos.x && x <= button5Pos.x + button5->w && y >= button5Pos.y && y <= button5Pos.y + button5->h);

        if (state == 0) {
          	  if (x >= button1Pos.x && x <= button1Pos.x + button1->w && y >= button1Pos.y && y <= button1Pos.y + button1->h) {
     		   	state = 1;
   		  } 
       } else if (state == 1) {
            if (x >= button2Pos.x && x <= button2Pos.x + button2->w && y >= button2Pos.y && y <= button2Pos.y + button2->h) {
                volume = (volume - 10 < 0) ? 0 : volume - 10;
                Mix_VolumeMusic(volume);
            } else if (x >= button3Pos.x && x <= button3Pos.x + button3->w && y >= button3Pos.y && y <= button3Pos.y + button3->h) {
                volume = (volume + 10 > MIX_MAX_VOLUME) ? MIX_MAX_VOLUME : volume + 10;
                Mix_VolumeMusic(volume);
            } else if (x >= button5Pos.x && x <= button5Pos.x + button5->w && y >= button5Pos.y && y <= button5Pos.y + button5->h) {
                state = 0;
            } else if (x >= button4Pos.x && x <= button4Pos.x + button4->w && y >= button4Pos.y && y <= button4Pos.y + button4->h) {
                fullscreen = !fullscreen;
                if (fullscreen) {
                    ecran = SDL_SetVideoMode(1400, 700, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
                } else {
                    ecran = SDL_SetVideoMode(1400, 700, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
                }
            }
        }



    }
}


        SDL_Flip(ecran);
    }

    SDL_FreeSurface(image);
    SDL_FreeSurface(button1);
    SDL_FreeSurface(button2);
    SDL_FreeSurface(button3);
    SDL_FreeSurface(button4);
    SDL_FreeSurface(button5);
    SDL_FreeSurface(page1Image);
    Mix_FreeMusic(musique);
    TTF_CloseFont(font);
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();






    return 0;
}


