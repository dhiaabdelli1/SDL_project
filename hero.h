#ifndef HERO_H
#define HERO_H
#include <stdio.h>
#include <string.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_ttf.h"

#include "structs.h"
#include "background.h"

#include "defs.h"

typedef enum State
{
    IDLE,
    DIE,
    ATTACK,
    DAMAGE,
    JUMP,
    WALK_RIGHT,
    WALK_LEFT,
    PUNCH,
    KICK
} state;
typedef enum Direction
{
    RIGHT,
    LEFT
} direction;
typedef struct Sprite
{
    SDL_Surface *image;
    SDL_Rect frame;
    int curframe, maxframe;
} sprite;

typedef struct Vie
{
    SDL_Surface *heart;
    SDL_Rect position_heart_a;
    SDL_Rect position_heart_b;
    SDL_Rect position_heart_c;
    int nb_vie;
} vie;

typedef struct Score
{
    SDL_Surface *texte_score;
    SDL_Rect position_texte;
    TTF_Font *score_font;
    SDL_Color couleurNoire;
    int valeur_score;
} score;

typedef struct Hero
{
    SDL_Rect position;
    sprite sprite;
    state state;
    int collision_UP, collision_DOWN, collision_RIGHT, collision_LEFT;
    int current_ground_position;
    direction direction;
    vie vie_hero;
    score score_hero;

} hero;

void initialiser_hero(hero *h, char name[20]);
void deplacer_hero(hero *h, background *b,int *Jcontinuer,character c);
void animer_hero(hero *h, state movement,character c);
void afficher_hero(hero h, SDL_Surface *screen,background b);
void free_hero(hero *h);
#endif