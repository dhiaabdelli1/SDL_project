#include "entite_secondaire.h"
#include "collision.h"
#include "background.h"

void initialiser_entite(entite *E)
{
	E->sprite_entite.image = IMG_Load("../img/es/walk.png");
	E->sprite_entite.maxframe = 5;
	E->sprite_entite.frame.x = 0;
	E->sprite_entite.frame.y = 0; //2=Nb de ligne(g/d)
	E->sprite_entite.frame.w = E->sprite_entite.image->w / E->sprite_entite.maxframe;
	E->sprite_entite.frame.h = E->sprite_entite.image->h / 2; //2=Nb de ligne(g/d)
	E->type = ENTITE;
	E->state_entite = WALK_entite;
	E->position.x = 2100;
	E->position.y = 1265;
	E->direction_entite = 1;
	E->type = 0;
	E->sprite_entite.curframe = 0; //unused
	srand(time(NULL));
	E->posMin.x = rand() % 200 + 100 + E->position.x; //+ position Hero
	E->posMax.x = rand() % 100 + E->posMin.x;
}
void animer_entite(entite *E)
{
	if (E->sprite_entite.image != NULL)
	{
		static int tempsActuel = 0;
		static int tempsPrecedent = 0;

		switch (E->state_entite)
		{
		case (DIE_entite):
		{
			E->sprite_entite.image = IMG_Load("../img/es/Die.png");
			E->sprite_entite.maxframe = 4;
			break;
		}
		case (WALK_entite):
		{
			E->sprite_entite.image = IMG_Load("../img/es/walk.png");
			E->sprite_entite.maxframe = 5;
			break;
		}
		case (FOLLOW_entite):
		{
			E->sprite_entite.image = IMG_Load("../img/es/attack.png");
			E->sprite_entite.maxframe = 3;
			break;
		}

			//opt
			//E->sprite_entite.image = SDL_DisplayFormat(E->sprite_entite.image);
			/*case (ATTACK_entite):
	{
		E->sprite_entite.image = IMG_Load("img/es/attack.png");
		E->sprite_entite.maxframe = 3;
		break;
	}*/
		}
		E->sprite_entite.frame.w = E->sprite_entite.image->w / E->sprite_entite.maxframe;
		E->sprite_entite.frame.y = E->direction_entite * E->sprite_entite.frame.h; // nb =  E->direction * E->sprite_entite.frame.h
		tempsActuel = SDL_GetTicks();
		if (tempsActuel - tempsPrecedent > 200)
		{
			if (E->sprite_entite.frame.x == E->sprite_entite.image->w - E->sprite_entite.frame.w)
				E->sprite_entite.frame.x = 0;
			else
				E->sprite_entite.frame.x += E->sprite_entite.frame.w;
			tempsPrecedent = tempsActuel;
		}
	}
}
void deplacer_alea(entite *E)
{
	if (E->sprite_entite.image != NULL)
	{
		if (E->position.x > E->posMax.x)
			E->direction_entite = 0;
		if (E->position.x < E->posMin.x)
			E->direction_entite = 1;
		if (E->direction_entite == 0)
			(E->position.x)--;
		if (E->direction_entite == 1)
			(E->position.x)++;
	}
}
void attack_entite(entite *E, hero *h)
{
	if (E->sprite_entite.image != NULL)
	{
		//E->state_entite = FOLLOW_entite;
		if (E->position.x > h->position.x)
		{
			E->direction_entite = 0;
			E->position.x -= 2;
		}
		else if (E->position.x < h->position.x)
		{
			E->direction_entite = 1;
			E->position.x += 2;
		}
		else if (E->position.x == h->position.x)
			E->direction_entite = 0;
		if (collision(E, h) == 1)
		{
			static int tempsActuel = 0;
			static int tempsPrecedent = -2000;
			if ((h->state != KICK && h->state != PUNCH))
			{
				tempsActuel = SDL_GetTicks();
				h->state = DAMAGE;
				if (tempsActuel - tempsPrecedent >= 2000)
				{
					h->vie_hero.nb_vie--;
					tempsPrecedent = tempsActuel;
				}
			}
			if (h->state == PUNCH || h->state == KICK)
			{
				E->state_entite = DIE_entite;
				E->sprite_entite.image = NULL;
			}
			switch (h->vie_hero.nb_vie)
			{
			case 2:
				h->vie_hero.position_heart_c.x = 0;
				break;
			case 1:
				h->vie_hero.position_heart_b.x = 0;
				break;
			case 0:
				h->vie_hero.heart = NULL;
				break;
			}
		}
	}
}

void afficher_entite(entite *E, SDL_Surface *screen, background b)
{
	SDL_Rect pos;
	pos.x = E->position.x - b.posCamera.x;
	pos.y = E->position.y - b.posCamera.y;
	SDL_BlitSurface(E->sprite_entite.image, &E->sprite_entite.frame, screen, &pos);
}
void free_entite(entite *E)
{
	SDL_FreeSurface(E->sprite_entite.image);
}

void free_pu(power_up *p)
{
	SDL_FreeSurface(p->sprite.image);
}

void initialiser_coins(power_up coins[], int n)
{
	int i;
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
	{
		printf("%s", Mix_GetError());
	}
	for (i = 0; i < n; i++)
	{
		coins[i].click = Mix_LoadWAV("../sfx/coin_pick.wav");
		coins[i].sprite.image = IMG_Load("../img/es/coin_small.png");
		coins[i].sprite.curframe = 0;
		coins[i].sprite.maxframe = 5;
		coins[i].sprite.frame.x = 0;
		coins[i].sprite.frame.y = 0;
		coins[i].sprite.frame.w = coins[i].sprite.image->w / 6;
		coins[i].sprite.frame.h = coins[i].sprite.image->h / 3;
	}

	coins[0].position.x = 1065;
	coins[0].position.y = 1460;
	coins[1].position.x = 1696;
	coins[1].position.y = 1400;
}

void coins_interaction(power_up coins[], int n, hero *h)
{
	int i;
	for (i = 0; i < n; i++)
	{
		if (coins[i].sprite.image != NULL && h->position.x >= coins[i].position.x && h->position.x <= coins[i].position.x + coins[i].sprite.frame.w && h->position.y >= coins[i].position.y && h->position.y <= coins[i].position.y + coins[i].sprite.frame.h)
		{
			h->score_hero.valeur_score += 20;
			Mix_PlayChannel(-1, coins[i].click, 0);
			coins[i].sprite.image = NULL;
		}
	}
}
void animer_coins(power_up coins[], int n)
{
	static int tempsActuel = 0;
	static int tempsPrecedent = 0;
	int i;

	tempsActuel = SDL_GetTicks();
	if (tempsActuel - tempsPrecedent > 100)
	{
		for (i = 0; i < n; i++)
		{
			if (coins[i].sprite.curframe >= coins[i].sprite.maxframe)
			{
				coins[i].sprite.curframe = 0;
				coins[i].sprite.curframe = 0;
			}

			coins[i].sprite.frame.x = coins[i].sprite.curframe * coins[i].sprite.frame.w;
			coins[i].sprite.curframe += 1;
		}

		tempsPrecedent = tempsActuel;
	}
}

void afficher_coins(power_up coins[], int n, background b, SDL_Surface *ecran)
{
	SDL_Rect pos;
	int i;
	for (i = 0; i < n; i++)
	{
		pos.x = coins[i].position.x - b.posCamera.x;
		pos.y = coins[i].position.y - b.posCamera.y;

		SDL_BlitSurface(coins[i].sprite.image, &coins[i].sprite.frame, ecran, &pos);
	}
}
