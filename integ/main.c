#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>

int main(int argc, char** argv) {
    SDL_Surface *ecran = NULL; 
    SDL_Surface *image = NULL; 
    SDL_Surface *image_1 = NULL; 
    SDL_Surface *image_2 = NULL;
    SDL_Surface *image_3 = NULL; 
    SDL_Surface *image_4 = NULL; 
    SDL_Surface *image_5 = NULL; 
    SDL_Surface *image_6 = NULL; 
    SDL_Surface *image_7 = NULL; 
    SDL_Surface *image_8 = NULL; 
    SDL_Surface *image_9 = NULL; 
    SDL_Surface *image_10 = NULL; 
    SDL_Surface *image_r = NULL;
    SDL_Surface *image_j = NULL; 
    SDL_Surface *image_v = NULL; 
    SDL_Surface *image_tt = NULL;
    

    SDL_Surface *button2 = NULL;
    SDL_Surface *button3 = NULL; 
    SDL_Surface *button4 = NULL; 
    SDL_Surface *button5 = NULL; 
    

    SDL_Surface *button2Hover = NULL; 
    SDL_Surface *button3Hover = NULL; 
    SDL_Surface *button4Hover = NULL; 
    SDL_Surface *button5Hover = NULL; 
    SDL_Surface *page1Image = NULL;
    
    SDL_Rect posecranimg;
    SDL_Rect poseimage1;
    SDL_Rect poseimage3; 
    SDL_Rect poseimage5; 
    SDL_Rect poseimage7; 
    SDL_Rect poseimage9; 
    SDL_Rect posett;
    
    SDL_Rect poseImage_r; 
    SDL_Rect poseImage_j; 
    SDL_Rect poseImage_v; 
    SDL_Rect positiontext; 

    SDL_Rect button2Pos; 
    SDL_Rect button3Pos; 
    SDL_Rect button4Pos;
    SDL_Rect button5Pos;
    TTF_Font *font = NULL;
    SDL_Color textColor = {0, 0, 0};
    SDL_Event event;
    Mix_Music *musique = NULL;
    Mix_Chunk *sound = NULL;

    int quitter = 1, play = 0, imagechoisie = 0;
    int state = 0; // 0: Main screen, 1: Options screen
    int volume = MIX_MAX_VOLUME / 2;
    int fullscreen = 0;
    int hoverButton1 = 0, hoverButton2 = 0, hoverButton3 = 0, hoverButton4 = 0, hoverButton5 = 0;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0 || TTF_Init() == -1 || Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
        printf("Erreur d'initialisation: %s\n", SDL_GetError());
        return 1;
    }

    // Create window
    ecran = SDL_SetVideoMode(1165, 728, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (!ecran) {
        printf("Erreur de création de fenêtre: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Load images
    image = IMG_Load("back_n.png");
    image_1 = IMG_Load("jouer1.png");
    image_2 = IMG_Load("jouer2.png");
    image_3 = IMG_Load("option1.png");
    image_4 = IMG_Load("option2.png");
    image_5 = IMG_Load("ms1.png");
    image_6 = IMG_Load("ms2.png");
    image_7 = IMG_Load("hist1.png");
    image_8 = IMG_Load("hist2.png");
    image_9 = IMG_Load("quitt1.png");
    image_10 = IMG_Load("quitt2.png");
    image_r = IMG_Load("rouge.png");
    image_v = IMG_Load("vert.png");
    image_j = IMG_Load("jaune.png");
    image_tt = IMG_Load("tt.png");
    
    page1Image = IMG_Load("imagevv.jpeg");

    button2 = IMG_Load("minus1.jpeg");
    button3 = IMG_Load("plus1.jpeg");
    button4 = IMG_Load("grow .jpeg");
    button5 = IMG_Load("retour.jpeg");
   
    button2Hover = IMG_Load("minus1 hover.jpeg");
    button3Hover = IMG_Load("plus1 hover.jpeg");
    button4Hover = IMG_Load("grow hover.jpeg");
    button5Hover = IMG_Load("retour hover.jpeg");

    if (!image || !image_1 || !image_2 || !image_3 || !image_4 || !image_5 || !image_6 || !image_7 || !image_8 || !image_9 || !image_10 || !page1Image || !button2 || !button3 || !button4 || !button5  || !button2Hover || !button3Hover || !button4Hover || !button5Hover) {
        printf("Erreur de chargement des images: %s\n", IMG_GetError());
        SDL_FreeSurface(image); SDL_FreeSurface(image_1); SDL_FreeSurface(image_2); SDL_FreeSurface(image_3); SDL_FreeSurface(image_4); SDL_FreeSurface(image_5); SDL_FreeSurface(image_6); SDL_FreeSurface(image_7); SDL_FreeSurface(image_8); SDL_FreeSurface(image_9); SDL_FreeSurface(image_10); SDL_FreeSurface(page1Image);  SDL_FreeSurface(button2); SDL_FreeSurface(button3); SDL_FreeSurface(button4); SDL_FreeSurface(button5); SDL_FreeSurface(button2Hover); SDL_FreeSurface(button3Hover); SDL_FreeSurface(button4Hover); SDL_FreeSurface(button5Hover);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Set positions
    posecranimg.x = 0; posecranimg.y = 0; posecranimg.w = image->w; posecranimg.h = image->h;
    poseimage1.x = 480; poseimage1.y = 200;
    poseimage3.x = 480; poseimage3.y = 300;
    poseimage5.x = 480; poseimage5.y = 400;
    poseimage7.x = 480; poseimage7.y = 500;
    poseimage9.x = 480; poseimage9.y = 600;
    posett.x = 250; posett.y = 25;
    poseImage_r.x = 0; poseImage_r.y = 0;
    poseImage_v.x = 0; poseImage_v.y = 0;
    poseImage_j.x = 0; poseImage_j.y = 0;

    button2Pos.x = 700; button2Pos.y = 260;
    button3Pos.x = 540; button3Pos.y = 260;
    button4Pos.x = 610; button4Pos.y = 380;
    button5Pos.x = 610; button5Pos.y = 500;

    // Load font
    font = TTF_OpenFont("arial.ttf", 24);
    if (!font) {
        printf("Erreur de chargement de la police: %s\n", TTF_GetError());
        SDL_FreeSurface(image); SDL_FreeSurface(image_1); SDL_FreeSurface(image_2); SDL_FreeSurface(image_3); SDL_FreeSurface(image_4); SDL_FreeSurface(image_5); SDL_FreeSurface(image_6); SDL_FreeSurface(image_7); SDL_FreeSurface(image_8); SDL_FreeSurface(image_9); SDL_FreeSurface(image_10); SDL_FreeSurface(image_r); SDL_FreeSurface(image_v); SDL_FreeSurface(image_j); SDL_FreeSurface(image_tt); SDL_FreeSurface(page1Image); SDL_FreeSurface(button2); SDL_FreeSurface(button3); SDL_FreeSurface(button4); SDL_FreeSurface(button5);SDL_FreeSurface(button2Hover); SDL_FreeSurface(button3Hover); SDL_FreeSurface(button4Hover); SDL_FreeSurface(button5Hover);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Load music
    musique = Mix_LoadMUS("song.mp3");
    if (!musique) {
        printf("Erreur de chargement de la musique: %s\n", Mix_GetError());
        SDL_FreeSurface(image); SDL_FreeSurface(image_1); SDL_FreeSurface(image_2); SDL_FreeSurface(image_3); SDL_FreeSurface(image_4); SDL_FreeSurface(image_5); SDL_FreeSurface(image_6); SDL_FreeSurface(image_7); SDL_FreeSurface(image_8); SDL_FreeSurface(image_9); SDL_FreeSurface(image_10); SDL_FreeSurface(image_r); SDL_FreeSurface(image_v); SDL_FreeSurface(image_j); SDL_FreeSurface(image_tt); SDL_FreeSurface(page1Image); SDL_FreeSurface(button2); SDL_FreeSurface(button3); SDL_FreeSurface(button4); SDL_FreeSurface(button5); SDL_FreeSurface(button2Hover); SDL_FreeSurface(button3Hover); SDL_FreeSurface(button4Hover); SDL_FreeSurface(button5Hover);
        TTF_CloseFont(font);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }



    Mix_PlayMusic(musique, -1);
    Mix_VolumeMusic(volume);

    // Main loop
    while (quitter) {
        if (state == 0) {
            SDL_BlitSurface(image, NULL, ecran, &posecranimg);
            SDL_BlitSurface(image_1, NULL, ecran, &poseimage1);
            SDL_BlitSurface(image_3, NULL, ecran, &poseimage3);
            SDL_BlitSurface(image_5, NULL, ecran, &poseimage5);
            SDL_BlitSurface(image_7, NULL, ecran, &poseimage7);
            SDL_BlitSurface(image_9, NULL, ecran, &poseimage9);
            SDL_BlitSurface(image_tt, NULL, ecran, &posett);

            if (imagechoisie == 1) {
                SDL_BlitSurface(image_r, NULL, ecran, &poseImage_r);
            } else if (imagechoisie == 2) {
                SDL_BlitSurface(image_v, NULL, ecran, &poseImage_v);
            } else if (imagechoisie == 3) {
                SDL_BlitSurface(image_j, NULL, ecran, &poseImage_j);
            }
        } else if (state == 1) {
            SDL_BlitSurface(page1Image, NULL, ecran, &posecranimg);

            SDL_BlitSurface(hoverButton2 ? button2Hover : button2, NULL, ecran, &button2Pos);
            SDL_BlitSurface(hoverButton3 ? button3Hover : button3, NULL, ecran, &button3Pos);
            SDL_BlitSurface(hoverButton4 ? button4Hover : button4, NULL, ecran, &button4Pos);
            SDL_BlitSurface(hoverButton5 ? button5Hover : button5, NULL, ecran, &button5Pos);

            SDL_Surface *surfaceTexte = TTF_RenderText_Solid(font, "Volume", textColor);
            if (surfaceTexte) {
                positiontext.x = 625;
                positiontext.y = 100;
                SDL_BlitSurface(surfaceTexte, NULL, ecran, &positiontext);
                SDL_FreeSurface(surfaceTexte);
            }
        }

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                quitter = 0;
            } else if (event.type == SDL_MOUSEMOTION) {
                int x = event.motion.x, y = event.motion.y;
                if (state == 0) {
                    if (x >= poseimage1.x && x <= poseimage1.x + image_1->w && y >= poseimage1.y && y <= poseimage1.y + image_1->h) {
                        SDL_BlitSurface(image_2, NULL, ecran, &poseimage1);
                        Mix_PlayChannel(-1, sound, 0);
                    } else if (x >= poseimage3.x && x <= poseimage3.x + image_3->w && y >= poseimage3.y && y <= poseimage3.y + image_3->h) {
                        SDL_BlitSurface(image_4, NULL, ecran, &poseimage3);
                    } else if (x >= poseimage5.x && x <= poseimage5.x + image_5->w && y >= poseimage5.y && y <= poseimage5.y + image_5->h) {
                        SDL_BlitSurface(image_6, NULL, ecran, &poseimage5);
                    } else if (x >= poseimage7.x && x <= poseimage7.x + image_7->w && y >= poseimage7.y && y <= poseimage7.y + image_7->h) {
                        SDL_BlitSurface(image_8, NULL, ecran, &poseimage7);
                    } else if (x >= poseimage9.x && x <= poseimage9.x + image_9->w && y >= poseimage9.y && y <= poseimage9.y + image_9->h) {
                        SDL_BlitSurface(image_10, NULL, ecran, &poseimage9);
                    }
                } else if (state == 1) {
                    
                    hoverButton2 = (x >= button2Pos.x && x <= button2Pos.x + button2->w && y >= button2Pos.y && y <= button2Pos.y + button2->h);
                    hoverButton3 = (x >= button3Pos.x && x <= button3Pos.x + button3->w && y >= button3Pos.y && y <= button3Pos.y + button3->h);
                    hoverButton4 = (x >= button4Pos.x && x <= button4Pos.x + button4->w && y >= button4Pos.y && y <= button4Pos.y + button4->h);
                    hoverButton5 = (x >= button5Pos.x && x <= button5Pos.x + button5->w && y >= button5Pos.y && y <= button5Pos.y + button5->h);
                }
            } else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int x = event.button.x, y = event.button.y;
                if (state == 0) {
                    if (x >= poseimage1.x && x <= poseimage1.x + image_1->w && y >= poseimage1.y && y <= poseimage1.y + image_1->h && imagechoisie == 0) {
                        imagechoisie = 1;
                    } else if (x >= poseimage3.x && x <= poseimage3.x + image_3->w && y >= poseimage3.y && y <= poseimage3.y + image_3->h && imagechoisie == 0) {
                        state = 1;
                    } else if (x >= poseimage5.x && x <= poseimage5.x + image_5->w && y >= poseimage5.y && y <= poseimage5.y + image_5->h && imagechoisie == 0) {
                        imagechoisie = 3;
                    } else if (x >= poseimage9.x && x <= poseimage9.x + image_9->w && y >= poseimage9.y && y <= poseimage9.y + image_9->h) {
                        quitter = 0;
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
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_j) {
                    imagechoisie = 1;
                } else if (event.key.keysym.sym == SDLK_o) {
                    imagechoisie = 2;
                } else if (event.key.keysym.sym == SDLK_m) {
                    imagechoisie = 3;
                } else if (event.key.keysym.sym == SDLK_ESCAPE) {
                    quitter = 0;
                }
            }
        }

        SDL_Flip(ecran);
    }

    // Cleanup
    SDL_FreeSurface(image); 
    SDL_FreeSurface(image_1); 
    SDL_FreeSurface(image_2); 
    SDL_FreeSurface(image_3); 
    SDL_FreeSurface(image_4); 
    SDL_FreeSurface(image_5); 
    SDL_FreeSurface(image_6); 
    SDL_FreeSurface(image_7); 
    SDL_FreeSurface(image_8); 
    SDL_FreeSurface(image_9); 
    SDL_FreeSurface(image_10); 
    SDL_FreeSurface(image_r); 
    SDL_FreeSurface(image_v); 
    SDL_FreeSurface(image_j);
     SDL_FreeSurface(image_tt);

    SDL_FreeSurface(button2); 
    SDL_FreeSurface(button3); 
    SDL_FreeSurface(button4); 
    SDL_FreeSurface(button5); 
    SDL_FreeSurface(page1Image);

    SDL_FreeSurface(button2Hover); 
    SDL_FreeSurface(button3Hover); 
    SDL_FreeSurface(button4Hover); 
    SDL_FreeSurface(button5Hover);
    Mix_FreeChunk(sound); 
    Mix_FreeMusic(musique);
    TTF_CloseFont(font);
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();

    return 0;
}
