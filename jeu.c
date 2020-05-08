#include "background.h"
#include "entite_secondaire.h"
#include "hero.h"
#include "colision.h"
#include "defs.h"
#include "enigme.h"
#include "structs.h"
#include "menu.h"
void jeu(SDL_Surface *ecran, etat *etat, hero safwen, parameter *p, character c) //etat
{
	//hero safwen;
	background background;
	entite enemie;
	enigme enigme_m;
	power_up coin_1, coin_2;

	int Jcontinuer = 1;
	int verif = 0;

	//initialiser_hero(&safwen, "safwen");
	initialiser_background(&background);
	initialiser_entite(&enemie);
	initenigme(&enigme_m);
	initialiser_pu(&coin_1, 1065, 1460);
	initialiser_pu(&coin_2, 1696, 1400);
	SDL_Event event;

	TTF_Init();
	SDL_Surface *instruction_1, *instruction_2, *game_over_txt;
	SDL_Rect position_instruction_1, position_instruction_2, pos_game_over_txt;
	TTF_Font *instructions_font,*go_font;
	SDL_Color couleurBlanche,couleurRouge;
	SDL_Rect pos_relative;

	couleurBlanche.r = 255;
	couleurBlanche.g = 255;
	couleurBlanche.b = 255;

	couleurRouge.r = 255;
	couleurRouge.g = 0;
	couleurRouge.b = 0;

	instructions_font = TTF_OpenFont("./fonts/chalk_2.ttf", 30);
	go_font = TTF_OpenFont("./fonts/chalk_2.ttf", 90);
	instruction_1 = TTF_RenderText_Blended(instructions_font, "press the arrow keys to move", couleurBlanche);
	instruction_2 = TTF_RenderText_Blended(instructions_font, "press x to punch and c to kick", couleurBlanche);
	game_over_txt = TTF_RenderText_Blended(go_font, "GAME OVER", couleurBlanche);
	position_instruction_1.x = 488;
	position_instruction_1.y = 1360;
	position_instruction_2.x = 1400;
	position_instruction_2.y = 1300;
	pos_game_over_txt.x = SCREEN_WIDTH/2-250;
	pos_game_over_txt.y = SCREEN_HEIGHT/2;

	p->music = Mix_LoadMUS("./img/menu/alter-ego.mp3");

	if (!p->mute)
		Mix_PlayMusic(p->music, -1);
	else
		Mix_PauseMusic();

	int nb = 0;
	while (Jcontinuer)
	{

		deplacer_hero(&safwen, &background, &Jcontinuer, c);
		CollisionParfaite(&safwen, background);

		//CollisionParfaite(&safwen, background);
		/*if (safwen.position.y == 380 && safwen.position.x >= 670 && verif == 0)
		{
			enigme_math(ecran, &enigme_m);
			if (enigme_m.resolution == 1)
				safwen.score_hero.valeur_score += 50;
			else
				safwen.score_hero.valeur_score -= 50;
			verif = 1;
		}*/

		save_game(safwen, background, c);

		printf("%d\t%d\n", safwen.vie_hero.nb_vie, safwen.score_hero.valeur_score);
		printf("camera x: \t%d \n", background.posCamera.x);
		printf("safwen:%d.%d\t%d\tomar:%d.%d\n", safwen.position.x, safwen.position.y, collision(&enemie, &safwen), enemie.position.x, enemie.position.y);
		printf("%d%d%d%d\n", safwen.collision_UP, safwen.collision_DOWN, safwen.collision_RIGHT, safwen.collision_LEFT);
		printf("diff: %d\n", abs(safwen.position.x - enemie.position.x));

		if (abs(safwen.position.x - enemie.position.x) <= 250 && abs(safwen.position.y - enemie.position.y) >= 0 && abs(safwen.position.y - enemie.position.y) <= 50)
			attack_entite(&enemie, &safwen);
		else
			deplacer_alea(&enemie);

		if (coin_1.sprite.image != NULL && safwen.position.x >= coin_1.position.x && safwen.position.x <= coin_1.position.x + coin_1.sprite.frame.w && safwen.position.y >= coin_1.position.y && safwen.position.y <= coin_1.position.y + coin_1.sprite.frame.h)
		{
			safwen.score_hero.valeur_score += 20;
			Mix_PlayChannel(-1, coin_1.click, 0);
			coin_1.sprite.image = NULL;
		}

		if (coin_2.sprite.image != NULL && safwen.position.x >= coin_2.position.x && safwen.position.x <= coin_2.position.x + coin_2.sprite.frame.w && safwen.position.y >= coin_2.position.y && safwen.position.y <= coin_2.position.y + coin_2.sprite.frame.h)
		{
			safwen.score_hero.valeur_score += 20;
			Mix_PlayChannel(-1, coin_1.click, 0);
			coin_2.sprite.image = NULL;
		}

		animer_entite(&enemie);
		animer_hero(&safwen, safwen.state, c);
		animer_pu(&coin_1, &coin_2);

		afficher_background(&background, ecran);

		afficher_pu(coin_1, ecran, background);
		afficher_pu(coin_2, ecran, background);
		afficher_entite(&enemie, ecran, background);

		//instructions
		pos_relative.x = position_instruction_1.x - background.position_background.x;
		pos_relative.y = position_instruction_1.y - background.position_background.y;

		SDL_BlitSurface(instruction_1, NULL, ecran, &pos_relative);
		pos_relative.x = position_instruction_2.x - background.position_background.x;
		pos_relative.y = position_instruction_2.y - background.position_background.y;
		SDL_BlitSurface(instruction_2, NULL, ecran, &pos_relative);

		if (Jcontinuer == 0 && safwen.vie_hero.nb_vie != 0)
			(*etat) = EXIT;
		if (safwen.vie_hero.nb_vie == 0 || safwen.position.y >= 2600)
		{
			animer_hero(&safwen, DIE, c);
			SDL_BlitSurface(game_over_txt, NULL, ecran, &pos_game_over_txt);

			if (safwen.state != DIE)
			{
				safwen.sprite.image = NULL;


				SDL_Delay(2000);

				(*etat) = GAME_OVER;
				Jcontinuer = 0;
			}
		}
		afficher_hero(safwen, ecran, background);
		SDL_Flip(ecran);
	}
	free_background(&background);
	free_entite(&enemie);
	freeenigme(&enigme_m);
	free_hero(&safwen);
	free_pu(&coin_1);
	free_pu(&coin_2);
	TTF_CloseFont(instructions_font);
	SDL_FreeSurface(instruction_1);
	SDL_FreeSurface(instruction_2);
	SDL_FreeSurface(game_over_txt);
}