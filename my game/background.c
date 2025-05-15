#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "background.h"
/**
* @file background.c
* @brief Testing Program.
* @author C Team
* @version 0.1
* @date Apr 08, 2025
*
* Testing program for background scrollilng
*
*/

/**
* @struct Background
* @brief struct for background
*/
struct background
{
SDL_Rect bckg; /*!< Rectangle*/
SDL_Surface *img; /*!< Surface. */
}
typedef struct background background;


void initBckg (background * b, char url[]){
b->img = IMG_Load (url);
b->bckg.x = 0;
b->bckg.y = 0;
b->bckg.w = b->img->w;
b->bckg.h = b->img->h;
}
void showBckg (SDL_Surface * screen, background b){
SDL_BlitSurface (b.img, &(b.bckg), screen, NULL); }
void scrollToLeft (background * b){
if (b->bckg.x != 0)
b->bckg.x--;
}
void scrollToRight (background * b){
if (b->bckg.w - b->bckg.x != CAMERA_W)
b->bckg.x++;
}

/**
* @file background.c
*/
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "background.h"
/**
* @brief To initialize the background b .
* @param b the background
* @param url the url of the image
* @return Nothing
*/
void initBckg (background * b, char url[]){
b->img = IMG_Load (url);
b->bckg.x = 0;
b->bckg.y = 0;
b->bckg.w = b->img->w;
b->bckg.h = b->img->h;
}
/**
* @brief To show the background b .
* @param scren the screen
* @param b the background
* @return Nothing
*/
void showBckg (SDL_Surface * screen, background b){
SDL_BlitSurface (b.img, &(b.bckg), screen, NULL);
}
/**
* @brief To scroll the background to the left .
* @param b the background
* @return Nothing
*/
void scrollToLeft (background * b){
if (b->bckg.x != 0)
{ b->bckg.x--; }
}
/**
* @brief To scroll the background to the right .
* @param b the background
* @return Nothing
*/
void scrollToRight (background * b){
if (b->bckg.w - b->bckg.x != CAMERA_W)
b->bckg.x++;
}

