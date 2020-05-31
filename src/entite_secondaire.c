#include "entite_secondaire.h"
#include "collision.h"
#include "background.h"

void initialiser_entite(entite *E, int x, int y)
{
	E->sprite_entite.image = IMG_Load("../img/es/walk.png");
	E->sprite_entite.maxframe = 5;
	E->sprite_entite.frame.x = 0;
	E->sprite_entite.frame.y = 0; //2=Nb de ligne(g/d)
	E->sprite_entite.frame.w = E->sprite_entite.image->w / E->sprite_entite.maxframe;
	E->sprite_entite.frame.h = E->sprite_entite.image->h / 2; //2=Nb de ligne(g/d)
	E->type = ENTITE;
	E->state_entite = WALK_entite;
	E->position.x = x; // 2100;
	E->position.y = y; //1570;
	E->direction_entite = 1;
	E->type = 0;
	E->sprite_entite.curframe = 0; //unused
	srand(time(NULL));
	E->posMin.x = rand() % 200 + 100 + E->position.x; //+ position Hero
	E->posMax.x = rand() % 100 + E->posMin.x;
}
void initialiser_ennemies(entite tab[], int n)
{
	initialiser_entite(&tab[0],2100,1570);
	initialiser_entite(&tab[1],1242,1055);
}

//this fonction needs tweaking. Apply same idea as animer_coins/animer_hearts
void animer_ennemies(entite tab[], int n)
{
	static int i;
	for (i = 0; i < n; i++)
	{
		animer_entite(&tab[i]);
	}
}

void deplacer_alea_ennemies(entite tab[], int n)
{
	static int i;
	for (i = 0; i < n; i++)
	{
		deplacer_alea(&tab[i]);
	}
}
void attack_ennemies(entite tab[], int n, hero *h)
{
	static int i;
	for (i = 0; i < n; i++)
	{
		attack_entite(&tab[i], h);
	}
}
void free_ennemies(entite tab[], int n)
{
	int i;
	for (i = 0; i < n; i++)
	{
		free_entite(&tab[i]);
	}
}

void afficher_ennemies(entite tab[], int n, SDL_Surface *screen, background b)
{
	static int i;
	for (i = 0; i < n; i++)
	{
		afficher_entite(&tab[i], screen, b);
	}
}
void animer_entite(entite *E)
{
	static int dead = 0, walking = 0, following = 0;
	if (E->sprite_entite.image != NULL)
	{
		static int tempsActuel = 0;
		static int tempsPrecedent = 0;

		switch (E->state_entite)
		{
		case (DIE_entite):
		{
			if (!dead)
			{
				E->sprite_entite.image = IMG_Load("../img/es/Die.png");
				E->sprite_entite.maxframe = 4;
				dead = 1;
				walking = 0;
				following = 0;
			}

			break;
		}
		case (WALK_entite):
		{
			if (!walking)
			{
				E->sprite_entite.image = IMG_Load("../img/es/walk.png");
				E->sprite_entite.maxframe = 5;
				walking = 1;
				dead = 0;
				following = 0;
			}

			break;
		}
		case (FOLLOW_entite):
		{
			if (!following)
			{
				E->sprite_entite.image = IMG_Load("../img/es/attack.png");
				E->sprite_entite.maxframe = 3;
				following = 1;
				dead = 0;
				walking = 0;
			}

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

	coins[0].position.x = 1445;
	coins[0].position.y = 1305;
	coins[1].position.x = 1815;
	coins[1].position.y = 1475;
	coins[2].position.x = 2268;
	coins[2].position.y = 900;
	coins[3].position.x = 2900;
	coins[3].position.y = 1065;
	coins[4].position.x = 320;
	coins[4].position.y = 700;
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
	static SDL_Rect pos;
	int i;
	for (i = 0; i < n; i++)
	{
		pos.x = coins[i].position.x - b.posCamera.x;
		pos.y = coins[i].position.y - b.posCamera.y;

		SDL_BlitSurface(coins[i].sprite.image, &coins[i].sprite.frame, ecran, &pos);
	}
}

void init_heart(heart *h, int x, int y)
{

	h->image = IMG_Load("../img/hero/heart1.png");
	h->click = Mix_LoadWAV("../sfx/coin_pick.wav");
	h->position.x = x;
	h->position.y = y;
	h->pos_init.x = x;
	h->pos_init.y = y;
}

void initialiser_hearts(heart hearts[], int n)
{

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
	{
		printf("%s", Mix_GetError());
	}

	init_heart(&hearts[0], 2870, 1455);
}

void animer_hearts(heart hearts[], int n)
{
	static int tempsActuel = 0;
	static int tempsPrecedent = 0;
	static int sens = 1;
	static int i;

	tempsActuel = SDL_GetTicks();
	if (tempsActuel - tempsPrecedent > 30)
	{
		for (i = 0; i < n; i++)
		{
			hearts[i].position.y += 1 * sens;
			tempsPrecedent = tempsActuel;
		}
	}

	if (hearts[0].position.y >= hearts[0].pos_init.y)
		sens = -1;
	if (hearts[0].position.y <= hearts[0].pos_init.y - 20)
		sens = 1;
}

void hearts_interaction(heart hearts[], int n, hero *h)
{
	static int i;
	for (i = 0; i < n; i++)
	{
		if (hearts[i].image != NULL && h->position.x >= hearts[i].position.x && h->position.x <= hearts[i].position.x + hearts[i].image->w && h->position.y >= hearts[i].position.y && h->position.y <= hearts[i].position.y + hearts[i].image->h)
		{
			h->vie_hero.nb_vie++;
			Mix_PlayChannel(-1, hearts[i].click, 0);
			hearts[i].image = NULL;
		}
	}
}
void afficher_hearts(heart hearts[], int n, background b, SDL_Surface *ecran)
{

	static SDL_Rect pos;
	int i;
	for (i = 0; i < n; i++)
	{
		pos.x = hearts[i].position.x - b.posCamera.x;
		pos.y = hearts[i].position.y - b.posCamera.y;

		SDL_BlitSurface(hearts[i].image, NULL, ecran, &pos);
	}
}

void free_pu(power_up p[], int n)
{
	int i;
	for (i = 0; i < n; i++)
	{
		SDL_FreeSurface(p[i].sprite.image);
	}
}

void free_hearts(heart hearts[], int n)
{
	int i;
	for (i = 0; i < n; i++)
	{
		SDL_FreeSurface(hearts[i].image);
		Mix_FreeChunk(hearts[i].click);
	}
}
