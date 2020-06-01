#include "background.h"
#include "entite_secondaire.h"
#include "hero.h"
#include "collision.h"
#include "enigme.h"
#include "menu.h"
#include "structs.h"
#include "defs.h"

void main(int argc, char *argv[])
{
    SDL_Surface *tab[190];
    etat etat = MENU;
    if (argc == 2)
    {
        etat = INTRO;
        load_intro(tab);
    }

    int continuer = 1;

    SDL_Surface *screen;

    hero safwen;
    background background;
    parameter parameter;
    dialogue d;

    initialiser_parameters(&parameter);

    character c;

    SDL_Event event;

    SDL_Init(SDL_INIT_EVERYTHING);

    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

    while (continuer)
    {
        switch (etat)
        {
        case INTRO:
            play_intro(tab, screen, &etat, &parameter);
            break;
        case MENU:
            menu(screen, &etat, &parameter);
            break;
        case GAME:
            jeu(screen, &etat, &safwen, &parameter, c, background, d);
            break;
        case SETTING:
            settings(screen, &parameter, &etat);
            break;
        case GAME_OVER:
            game_over(screen, &etat, &parameter, &safwen);
            break;
        case CHARACTER:
            character_choice(&safwen, &etat, screen, &parameter, &c);
            break;
        case GAME_LOAD:
            game_load(&safwen, &background, &etat, screen, &parameter, &c, &d);
            break;
        case CHEAT:
            cheat(screen, &etat, parameter);
            break;
        case CREDS:
            play_credits(screen, &etat, parameter);
            break;
        case EXIT:
            continuer = 0;
            break;
        }
    }
    Mix_FreeMusic(parameter.music);
    Mix_FreeChunk(parameter.click);
    SDL_FreeSurface(screen);
    SDL_Quit();
}
