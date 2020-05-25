#include "background.h"
#include "entite_secondaire.h"
#include "hero.h"
#include "collision.h"
#include "defs.h"
#include "enigme.h"
#include "structs.h"
#include "menu.h"

void jeu(SDL_Surface *ecran, etat *etat, hero safwen, parameter *p, character c, background background)
{
	SDL_Event event;
	int Jcontinuer = 1;
	int verif = 0;

	entite enemie;
	enigme enigme_m;
	power_up coin_1, coin_2;
	platforme platforme;
	text game_over_txt;
	dialogue dialogue;
	text instructions[4];
	power_up coins[2];
	heart hearts[2];

	SDL_Surface *black = IMG_Load("../img/black.jpg");

	SDL_Rect position_black;
	position_black.x = 0;
	position_black.y = 0;

	initialiser_dialogue(&dialogue,ecran,c);
	initialiser_background(&background);
	initialiser_entite(&enemie);
	initenigme(&enigme_m);
	initialiser_platforme(&platforme, 1180, 1400);

	initialiser_instructions(instructions,4);
	initialiser_coins(coins,2);
	initialiser_hearts(hearts,1);
	
	initialiser_text(&game_over_txt, "GAME OVER", SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT / 2, 90);


	p->music = Mix_LoadMUS("../sfx/alter-ego.mp3");
	if (!p->mute)
		Mix_PlayMusic(p->music, -1);
	else
		Mix_PauseMusic();
	if (p->fullscreen)
		SDL_WM_ToggleFullScreen(ecran);
	int passage_boucle = 0;


	while (Jcontinuer)
	{
		deplacer_hero(&safwen, &background, &Jcontinuer, c, platforme);
		CollisionParfaite(&safwen, background, platforme);

		//save_game(safwen, background, c);

		if (abs(safwen.position.x - enemie.position.x) <= 250 && abs(safwen.position.y - enemie.position.y) >= 0 && abs(safwen.position.y - enemie.position.y) <= 50)
			attack_entite(&enemie, &safwen);
		else
			deplacer_alea(&enemie);

		coins_interaction(coins,2,&safwen);
		hearts_interaction(hearts,1,&safwen);

		playing_dialogue(&dialogue,safwen,ecran);

		animer_entite(&enemie);
		animer_hero(&safwen, safwen.state, c);
		animer_coins(coins,2);
		animer_hearts(hearts,1);
		animer_platforme(&platforme, 0);

		afficher_background(&background, ecran);
		afficher_platforme(platforme, background, ecran);

		afficher_coins(coins,2,background,ecran);
		afficher_hearts(hearts,1,background,ecran);
		afficher_entite(&enemie, ecran, background);

		afficher_instructions(instructions,4,background,ecran);

		if (Jcontinuer == 0 && safwen.vie_hero.nb_vie != 0)
			(*etat) = EXIT;

		if (safwen.vie_hero.nb_vie == 0 || abs(safwen.position.y + safwen.sprite.image->h - background.image->h - background.posCamera.h) < 170)
		{
			animer_hero(&safwen, DIE, c);
			passage_boucle++;
			SDL_SetAlpha(black, SDL_SRCALPHA, 120);
			SDL_BlitSurface(black, NULL, ecran, &position_black);
			SDL_BlitSurface(game_over_txt.text, NULL, ecran, &game_over_txt.position);

			if (passage_boucle == 15)
			{
				safwen.sprite.image = NULL;
				SDL_Delay(2000);
				(*etat) = GAME_OVER;
				Jcontinuer = 0;
				dialogue.done=1;
			}
		}
		afficher_hero(safwen, ecran, background);
		afficher_dialogue(dialogue,ecran);
		SDL_Flip(ecran);

		printf("%d\t%d\n", safwen.vie_hero.nb_vie, safwen.score_hero.valeur_score);
		printf("camera x: \t%d \n", background.posCamera.x);
		printf("safwen:%d.%d\t%d\tomar:%d.%d\n", safwen.position.x, safwen.position.y, collision(&enemie, &safwen), enemie.position.x, enemie.position.y);
		printf("%d%d%d%d\n", safwen.collision_UP, safwen.collision_DOWN, safwen.collision_RIGHT, safwen.collision_LEFT);
		printf("diff: %d\n", abs(safwen.position.x - enemie.position.x));
	}
	free_background(&background);
	free_entite(&enemie);
	freeenigme(&enigme_m);
	free_hero(&safwen);
	free_pu(&coins[0]);
	free_pu(&coins[1]);
	SDL_FreeSurface(game_over_txt.text);
}