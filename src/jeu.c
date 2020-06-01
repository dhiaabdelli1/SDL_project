#include "background.h"
#include "entite_secondaire.h"
#include "hero.h"
#include "collision.h"
#include "defs.h"
#include "enigme.h"
#include "structs.h"
#include "menu.h"

void jeu(SDL_Surface *ecran, etat *etat, hero *safwen, parameter *p, character c, background background, dialogue dial)
{
	SDL_Event event;
	int Jcontinuer = 1;
	int verif = 0;


	//enigme enigme_m;
	int nb_platformes = 5;
	int nb_coins = 5;
	int nb_hearts = 1;
	int nb_instructions = 6;
	int nb_ennemies = 2;

	int tempsActuel = 0;
	int tempsPrecedent = 0;
	int mini = 1;
	int idk = 1;

	SDL_Rect pos_rel;

	entite ennemies[nb_ennemies];
	platforme platformes[nb_platformes];
	text game_over_txt;
	dialogue dialogue = dial;
	text instructions[nb_instructions];
	power_up coins[nb_coins];
	heart hearts[nb_hearts];
	timer timer;
	minimap minimap;
	portal portal;

	int plat_coll;

	SDL_Surface *black = NULL;

	int portal_number = 0;

	SDL_Rect position_black;
	position_black.x = 0;
	position_black.y = 0;

	init_timer(&timer);
	initialiser_dialogue(&dialogue, ecran, c);
	initialiser_background(&background);
	initialiser_ennemies(ennemies, nb_ennemies);
	initialiser_minimap(&minimap, background, *safwen);

	initialiser_portal(&portal);

	initialiser_plats(platformes, nb_platformes);

	initialiser_instructions(instructions, nb_instructions);
	initialiser_coins(coins, nb_coins);
	initialiser_hearts(hearts, nb_hearts);

	initialiser_text(&game_over_txt, "", SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT / 2, 90);

	p->music = Mix_LoadMUS("../sfx/alter-ego.mp3");
	if (!p->mute)
		Mix_PlayMusic(p->music, -1);
	else
		Mix_PauseMusic();
	if (p->fullscreen)
		SDL_WM_ToggleFullScreen(ecran);
	int passage_boucle = 0;
	int once = 0;

	start_timer(&timer);
	int i;
	int saving = 0;
	SDL_Surface *save_screen = NULL;
	SDL_Rect pos_save_screen;
	pos_save_screen.x = 280;
	pos_save_screen.y = 280;

	text save_text;
	initialiser_text(&save_text, "", 315, 320, 10);
	save_text.color.r = 0;
	save_text.color.g = 0;
	save_text.color.b = 0;

	while (Jcontinuer)
	{

		deplacer_hero(safwen, &background, &Jcontinuer, c, platformes, &saving, nb_platformes, &mini);

		CollisionParfaite(safwen, background);

		plat_coll = collision_platforme(safwen, platformes, nb_platformes);

		for (i = 0; i < nb_platformes; i++)
		{
			tempsActuel = SDL_GetTicks();
			if (safwen->collision_DOWN_PLAT && safwen->state == IDLE)
			{
				if (tempsActuel - tempsPrecedent > 10)
				{
					safwen->position.x += 1 * platformes[plat_coll].sens;
					tempsPrecedent = tempsActuel;
				}
			}
		}

		//portal
		if (safwen->position.x >= 5100 && safwen->position.x <= 5340 && safwen->position.y >= 1070 && safwen->position.y <= 1120)
		{
			safwen->position.x += 1200;
			safwen->position.y += 600;
		}
		//save
		if (saving == 0)
		{

			save_screen = NULL;
			black = NULL;
			once = 0;
			save_text.text = TTF_RenderText_Blended(save_text.font, "", save_text.color);
		}
		else
		{
			if (!once)
			{
				save_screen = IMG_Load("../img/menu/objects/text_field.png");
				black = IMG_Load("../img/black.jpg");
				SDL_SetAlpha(black, SDL_SRCALPHA, 120);
				save_text.text = TTF_RenderText_Blended(save_text.font, "Press y to confirm and q to quit", save_text.color);
				once = 1;
			}

			if (saving == 2)
			{
				save_game(*safwen, background, c, dialogue);
				printf("SAVED\n");
				saving = 0;
				once = 0;
			}
		}

		if (safwen->state == IDLE)
		{
			resume_timer(&timer);
		}
		else
		{
			pause_timer(&timer);
			start_timer(&timer);
		}

		for (i = 0; i < nb_ennemies; i++)
		{
			if (abs(safwen->position.x - ennemies[i].position.x) <= 250 && abs(safwen->position.y - ennemies[i].position.y) >= 0 && abs(safwen->position.y - ennemies[i].position.y) <= 50)
				attack_entite(&ennemies[i], safwen);
			else
				deplacer_alea(&ennemies[i]);
		}

		coins_interaction(coins, nb_coins, safwen);
		hearts_interaction(hearts, nb_hearts, safwen);

		playing_dialogue(&dialogue, *safwen, ecran, timer);

		animer_ennemies(ennemies, nb_ennemies);
		animer_hero(safwen, safwen->state, c);
		animer_coins(coins, nb_coins);
		animer_hearts(hearts, nb_hearts);
		animer_platformes(platformes, nb_platformes);
		animer_portal(&portal);

		afficher_background(&background, ecran);
		afficher_platformes(platformes, background, ecran, nb_platformes);

		afficher_coins(coins, nb_coins, background, ecran);
		afficher_hearts(hearts, nb_hearts, background, ecran);
		afficher_ennemies(ennemies, nb_ennemies, ecran, background);

		afficher_instructions(instructions, nb_instructions, background, ecran);

		if (Jcontinuer == 0 && safwen->vie_hero.nb_vie != 0)
			(*etat) = EXIT;

		if (safwen->vie_hero.nb_vie == 0 || abs(safwen->position.y + safwen->sprite.image->h - background.image->h - background.posCamera.h) < 170)
		{
			animer_hero(safwen, DIE, c);
			passage_boucle++;
			black = IMG_Load("../img/black.jpg");
			SDL_SetAlpha(black, SDL_SRCALPHA, 120);
			game_over_txt.text = TTF_RenderText_Blended(game_over_txt.font, "GAME OVER", game_over_txt.color);

			if (passage_boucle == 15)
			{
				safwen->sprite.image = NULL;
				SDL_Delay(2000);
				(*etat) = GAME_OVER;
				Jcontinuer = 0;
				dialogue.talking = 0;
			}
		}
		afficher_hero(*safwen, ecran, background);
		afficher_portal(portal,background,*safwen,ecran);


		afficher_dialogue(dialogue, ecran);
		show_time(&timer, ecran);
		SDL_BlitSurface(black, NULL, ecran, &position_black);
		SDL_BlitSurface(save_screen, NULL, ecran, &pos_save_screen);
		SDL_BlitSurface(save_text.text, NULL, ecran, &save_text.position);
		SDL_BlitSurface(game_over_txt.text, NULL, ecran, &game_over_txt.position);

		if (mini == 1 && idk != 1)
		{
			minimap.image = IMG_Load("../img/background/test2.jpg");
			minimap.hero = IMG_Load("../img/background/rouge.png");
			idk = 1;

			minimap.pos_image.x = (SCREEN_WIDTH / 2) - (minimap.image->w / 2);
			minimap.pos_image.y = 0;

			minimap.pos_hero.x = (SCREEN_WIDTH / 2) - (minimap.image->w / 2) + safwen->position.x / 12.5;
		}
		if (mini == -1 && idk != 2)
		{
			minimap.image = IMG_Load("../img/background/test1.jpg");
			idk = 2;
			minimap.pos_image.x = 0;
			minimap.pos_image.y = 0;
		}


		afficher_minimap(&minimap, *safwen, ecran, mini);
		SDL_Flip(ecran);

		printf("safwen: (%d;%d)\n", safwen->position.x, safwen->position.y);
	}
	free_hero(safwen);
	free_background(&background);
	free_ennemies(ennemies, nb_ennemies);
	free_pu(coins, nb_coins);
	free_hearts(hearts, nb_hearts);
	free_instructions(instructions, nb_instructions);
	free_dialogue(&dialogue);
	free_platformes(platformes, nb_platformes);
	free_minimap(&minimap);
	free_portal(&portal);
}