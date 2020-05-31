#ifndef ES_H
#define ES_H
#include <stdio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_ttf.h"
#include <time.h>
#include "hero.h"
#include "defs.h"

typedef struct Sprite_entite
{
    SDL_Surface *image;
    SDL_Rect frame;
    int curframe, maxframe;
} sprite_entite;

typedef enum State_entite
{
    WALK_entite,
    DIE_entite,
    ATTACK_entite,
    FOLLOW_entite
} state_entite;


typedef enum Type
{
    HERO,
    ENTITE,
    COIN
} type;

typedef struct Entite
{
    SDL_Rect posMax;
    SDL_Rect posMin;

    SDL_Rect position;
    sprite_entite sprite_entite;
    state_entite state_entite;
    int direction_entite;

    type type;
    int vitesse;
} entite;

typedef struct Power_up
{
    sprite sprite;
    SDL_Rect position;
    Mix_Chunk *click;
    SDL_Rect pos_init;
}power_up;

typedef struct Heart
{
    SDL_Surface *image;
    SDL_Rect position;
    Mix_Chunk *click;
    SDL_Rect pos_init;
}heart;
void input_ennemi(entite *E, hero *h);
void update_entite(entite *E ,hero *h);
void attack_entite(entite *E,hero *h);
void initialiser_entite(entite *E, int x, int y);
void deplacer_alea(entite *E);
void animer_entite(entite *E);
void afficher_entite(entite *E, SDL_Surface *screen,background b);
void free_entite(entite *E);


void free_pu(power_up p[],int n);
void free_hearts(heart hearts[], int n);

void initialiser_coins(power_up coins[], int n);
void coins_interaction(power_up coins[], int n, hero *h);
void animer_coins(power_up coins[], int n);
void afficher_coins(power_up coins[], int n, background b, SDL_Surface *ecran);

void init_heart(heart *h, int x, int y);
void initialiser_hearts(heart hearts[], int n);
void animer_hearts(heart hearts[], int n);
void hearts_interaction(heart hearts[],int n,hero *h);
void afficher_hearts(heart hearts[], int n, background b, SDL_Surface *ecran);

void initialiser_ennemies(entite tab[], int n);
void animer_ennemies(entite tab[], int n);
void deplacer_alea_ennemies(entite tab[], int n);
void attack_ennemies(entite tab[], int n, hero *h);
void free_ennemies(entite tab[],int n);
void afficher_ennemies(entite tab[],int n,SDL_Surface *screen,background b);

#endif