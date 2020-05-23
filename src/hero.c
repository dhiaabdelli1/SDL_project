#include "hero.h"
#include "background.h"
#include "collision.h"

void initialiser_hero(hero *h, char name[20])
{
	if (TTF_Init() == -1)
	{
		fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
	}

	char image_load[40] = "";
	char score_str[10] = "";
	h->position.x = 500;
	h->position.y = 1565;

	h->current_ground_position = h->position.y;

	h->direction = RIGHT;
	h->state = IDLE;

	//load function
	strcpy(image_load, "../img/hero/");
	strcat(image_load, name);
	if (h->direction == RIGHT)
		strcat(image_load, "_right");
	else if (h->direction == LEFT)
		strcat(image_load, "_left");
	strcat(image_load, ".png");
	h->sprite.image = IMG_Load(image_load);

	printf("%s\n", image_load);
	//end load function
	h->sprite.frame.x = 0;
	h->sprite.frame.y = 0;

	h->sprite.frame.w = h->sprite.image->w / 5;
	h->sprite.frame.h = h->sprite.image->h / 7;

	h->sprite.curframe = 0;

	h->vie_hero.nb_vie = 3;
	h->score_hero.valeur_score = 0;

	h->vie_hero.heart = IMG_Load("../img/hero/heart1.png");

	h->vie_hero.position_heart_a.x = 0;
	h->vie_hero.position_heart_a.y = 0;
	h->vie_hero.position_heart_b.x = 50;
	h->vie_hero.position_heart_b.y = 0;
	h->vie_hero.position_heart_c.x = 100;
	h->vie_hero.position_heart_c.y = 0;

	h->score_hero.couleurNoire.b = 255;
	h->score_hero.couleurNoire.g = 255;
	h->score_hero.couleurNoire.r = 255;
	h->score_hero.score_font = TTF_OpenFont("../fonts/8bit.ttf", 15);
	snprintf(score_str, 10, "%d", h->score_hero.valeur_score);
	h->score_hero.texte_score = TTF_RenderText_Blended(h->score_hero.score_font, score_str, h->score_hero.couleurNoire);
	h->score_hero.position_texte.x = SCREEN_WIDTH - h->vie_hero.heart->w;
	h->score_hero.position_texte.y = 10;
}

void afficher_hero(hero h, SDL_Surface *screen, background b)
{
	if (h.sprite.image != NULL)
	{
		SDL_Rect pos;
		if (TTF_Init() == -1)
		{
			fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
			exit(EXIT_FAILURE);
		}
		char score_str[10];
		switch (h.vie_hero.nb_vie)
		{
		case 2:
			h.vie_hero.position_heart_c.x = 0;
			break;
		case 1:
			h.vie_hero.position_heart_b.x = 0;
			break;
		case 0:
			h.vie_hero.heart = NULL;
			break;
		}

		pos.x = h.position.x - b.posCamera.x;
		pos.y = h.position.y - b.posCamera.y;

		SDL_BlitSurface(h.sprite.image, &h.sprite.frame, screen, &pos);

		SDL_BlitSurface(h.vie_hero.heart, NULL, screen, &h.vie_hero.position_heart_a);
		SDL_BlitSurface(h.vie_hero.heart, NULL, screen, &h.vie_hero.position_heart_b);
		SDL_BlitSurface(h.vie_hero.heart, NULL, screen, &h.vie_hero.position_heart_c);

		snprintf(score_str, 10, "%d", h.score_hero.valeur_score);
		h.score_hero.texte_score = TTF_RenderText_Blended(h.score_hero.score_font, score_str, h.score_hero.couleurNoire);
		SDL_BlitSurface(h.score_hero.texte_score, NULL, screen, &h.score_hero.position_texte);
	}
}
void animer_hero(hero *h, state movement, character c)
{
	if (h->sprite.image != NULL)
	{
		static int x = 0;
		static int tempsActuel = 0;
		static int tempsPrecedent = 0;

		switch (movement)
		{
		case IDLE:
			h->sprite.frame.y = 0;
			h->sprite.maxframe = 3;
			break;
		case WALK_RIGHT:
			h->sprite.frame.y = h->sprite.frame.h;
			h->sprite.maxframe = 4;
			h->state = WALK_RIGHT;
			h->direction = RIGHT;
			break;
		case WALK_LEFT:
			h->sprite.frame.y = h->sprite.frame.h;
			h->sprite.maxframe = 4;
			h->state = WALK_LEFT;
			h->direction = LEFT;
			break;
		case PUNCH:
			h->sprite.frame.y = 3 * h->sprite.frame.h;
			h->sprite.maxframe = 4;
			h->state = PUNCH;
			break;
		case KICK:
			h->sprite.frame.y = 4 * h->sprite.frame.h;
			h->sprite.maxframe = 4;
			h->state = KICK;
			break;
		case DAMAGE:
			h->sprite.frame.y = 5 * h->sprite.frame.h;
			h->sprite.maxframe = 4;
			h->state = DAMAGE;
			break;
		case DIE:
			h->sprite.frame.y = 6 * h->sprite.frame.h;
			h->sprite.maxframe = 3;
			h->state = DIE;
			break;
		case JUMP:
			h->sprite.frame.y = 2 * h->sprite.frame.h;
			h->sprite.frame.x = h->sprite.frame.w;
			h->state = JUMP;
			break;
		case FALLING:
			h->sprite.frame.y = 2 * h->sprite.frame.h;
			h->sprite.frame.x = 2 * h->sprite.frame.w;
			h->state = FALLING;
			break;
		}

		if (c == SAFWEN)
		{
			if (h->direction == RIGHT && x != 1)
			{
				h->sprite.image = IMG_Load("../img/hero/safwen_right.png");
				x = 1;
			}

			else if (h->direction == LEFT && x != 2)
			{
				h->sprite.image = IMG_Load("../img/hero/safwen_left.png");
				x = 2;
			}
		}
		else if (c == OMAR)
		{
			if (h->direction == RIGHT && x != 3)
			{
				h->sprite.image = IMG_Load("../img/hero/omar_right.png");
				x = 3;
			}

			if (h->direction == LEFT && x != 4)
			{
				h->sprite.image = IMG_Load("../img/hero/omar_left.png");
				x = 4;
			}
		}

		tempsActuel = SDL_GetTicks();
		if (movement != JUMP && movement != FALLING)
		{
			if (tempsActuel - tempsPrecedent > 200)
			{
				if (h->sprite.curframe >= h->sprite.maxframe)
				{
					h->sprite.curframe = 0;
					if (h->state != WALK_RIGHT && h->state != WALK_LEFT)
						h->state = IDLE; //to not interrupt animation (but can be interrupted with SDL_KEYUP)
				}

				h->sprite.frame.x = h->sprite.curframe * h->sprite.frame.w;
				tempsPrecedent = tempsActuel;
				h->sprite.curframe += 1;
			}
		}
	}
}

void free_hero(hero *h)
{
	SDL_FreeSurface(h->sprite.image);
}

void deplacer_hero(hero *h, background *b, int *Jcontinuer, character c, platforme p)
{
	if (h->sprite.image != NULL)
	{
		int timeStepMs = 10; //if 20 it ruins mvt for some reason
		static int timeLastMs, timeAccumulatedMs, timeDeltaMs, timeCurrentMs = 0;
		static int current_ground_position;
		static float accel = 0;
		static int tanguiza = -1;

		static int sur = 0;

		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
		{
			printf("%s", Mix_GetError());
		}
		Mix_Chunk *punch_sound;
		punch_sound = Mix_LoadWAV("../sfx/punch.wav");

		SDL_Event event;

		timeLastMs = timeCurrentMs;
		timeCurrentMs = SDL_GetTicks();
		timeDeltaMs = timeCurrentMs - timeLastMs;
		timeAccumulatedMs += timeDeltaMs;

		if (h->collision_DOWN)
		{
			h->current_ground_position = h->position.y;
			if (h->state != KICK && h->state != PUNCH)
				h->state = IDLE;
		}

		if (timeAccumulatedMs >= timeStepMs)
		{
			if (b->posCamera.x < b->image->w - SCREEN_WIDTH)
			{
				b->posCamera.x = (h->position.x + 5 * h->sprite.frame.w) - SCREEN_WIDTH;
			}
			if (b->posCamera.y < b->image->h - SCREEN_HEIGHT)
			{
				b->posCamera.y = (h->position.y + 4 * h->sprite.frame.h) - SCREEN_HEIGHT;
			}
			if (b->posCamera.x < 0)
				b->posCamera.x = 1;
			if (b->posCamera.y < 0)
				b->posCamera.y = 1;

			Uint8 *keystates = SDL_GetKeyState(NULL);
			if (h->position.y > h->current_ground_position - JUMP_HEIGHT && tanguiza == 0 && !h->collision_UP)
			{
				animer_hero(h, JUMP, c);
				h->position.y -= JUMP_SPEED;
			}
			if (h->position.y == h->current_ground_position - JUMP_HEIGHT || h->collision_UP)
				tanguiza = 1;
			if (tanguiza == 1 && !h->collision_DOWN)
			{

				animer_hero(h, FALLING, c);
				h->position.y += JUMP_SPEED;
			}
			if (keystates[SDLK_RIGHT] || (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT) && (event.motion.x > h->position.x)))
			{
				h->direction = RIGHT;
				CollisionParfaite(h, *b, p);

				if (!h->collision_DOWN && !h->collision_RIGHT)
				{
					h->position.x += 4 + accel;
				}
				else if (h->collision_DOWN && !h->collision_RIGHT)
				{

					h->position.x += 4 + accel;
					animer_hero(h, WALK_RIGHT, c);

					//animer_hero(h, WALK_RIGHT,c);
					/*if (accel < 5)
					accel += 0.1;*/
				}
			}
			if (keystates[SDLK_LEFT] || (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT) && (event.motion.x < h->position.x)))
			{
				h->direction = LEFT;
				CollisionParfaite(h, *b, p);
				if (!h->collision_DOWN && !h->collision_LEFT)
				{

					h->position.x -= 4 + accel;
					/*if (accel < 5)
					accel += 0.1;*/
					if (h->position.x < 0)
						h->position.x = 0;
				}
				else if (h->collision_DOWN && !h->collision_LEFT)
				{
					h->position.x -= 4 + accel;
					animer_hero(h, WALK_LEFT, c);
					//animer_hero(h, WALK_LEFT,c);
					/*if (accel < 5)
					accel += 0.1;*/
					if (h->position.x < 0)
						h->position.x = 0;
				}
			}
			printf("SUR: %d\n", sur);
			SDL_EnableKeyRepeat(2, 2);
			while (SDL_PollEvent(&event))
			{
				switch (event.type)
				{
				case SDL_QUIT:
					(*Jcontinuer) = 0;
					break;
				case SDL_MOUSEBUTTONDOWN:

					if (event.button.button == SDL_BUTTON_RIGHT)
					{
						if (h->collision_DOWN)
						{
							tanguiza = 0;
						}
					}
					break;
				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_RIGHT)
					{
						if (h->collision_UP || h->position.y == h->current_ground_position - JUMP_HEIGHT)
						{
							tanguiza = 1;
						}
					}
					else if (event.button.button == SDL_BUTTON_LEFT)
						accel = 0;
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						(*Jcontinuer) = 0;
						break;
					case SDLK_UP:
						if (h->collision_DOWN)
						{
							tanguiza = 0;
						}

						break;
					case SDLK_x:
						Mix_PlayChannel(-1, punch_sound, 0);
						animer_hero(h, PUNCH, c);
						break;
					case SDLK_c:
						Mix_PlayChannel(-1, punch_sound, 0);
						animer_hero(h, KICK, c);
						break;
					}
					break;
				case SDL_KEYUP:
					switch (event.key.keysym.sym)
					{
					case SDLK_UP:
						if (h->collision_UP || h->position.y == h->current_ground_position - JUMP_HEIGHT)
						{
							tanguiza = 1;
						}
						break;
					case SDLK_RIGHT:
					case SDLK_LEFT:
					{
						accel = 0;
					}
					break;
					}
					if (h->state == WALK_RIGHT || h->state == WALK_LEFT)
						h->state = IDLE;
					break;
				}
			}
			timeAccumulatedMs -= timeStepMs;
		}
	}
}
void initialiser_dialogue(dialogue *d, SDL_Surface *ecran)
{
	int i=0;
	FILE *f=NULL;
	f = fopen("dialogue.txt", "r");
	if (f == NULL)
	{
		fprintf(stderr, "Failed to open load file\n");
		exit(EXIT_FAILURE);
	}
	while(!feof(f))
	{
		fgets(d->script[i],30,f);
		i++;
	}
	fclose(f);

	initialiser_text(&d->text, d->script[0], 100, 80, 15);

	d->line = 0;
	d->text.text = NULL;

	d->hero_dialogue = IMG_Load("../img/hero/safwen_choice_active.png");
	d->dialogue_box = SDL_CreateRGBSurface(SDL_HWSURFACE, 220, 180, 32, 0, 0, 0, 0);
	SDL_FillRect(d->dialogue_box, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
	
	d->pos_dialogue_box.x = 0;
	d->pos_dialogue_box.y = SCREEN_HEIGHT - d->hero_dialogue->h;
	d->pos_hero_dialogue.x = 100;
	d->pos_hero_dialogue.y = SCREEN_HEIGHT - d->hero_dialogue->h;
}

void playing_dialogue(dialogue *d, hero h, SDL_Surface *ecran)
{
	static int tempsActuel = 0;
	static int tempsPrecedent = 0;
	d->text.position.x = 300;
	d->text.position.y = 530;
	if (h.position.x >= 600 && h.position.x < 2000 && d->line < 5)
	{
		tempsActuel = SDL_GetTicks();
		if (tempsActuel - tempsPrecedent > 3000)
		{
			d->text.font = TTF_OpenFont("../fonts/chalk_2.ttf", 30);
			d->text.text = TTF_RenderText_Blended(d->text.font, d->script[d->line], d->text.color);
			d->line++;
			tempsPrecedent = tempsActuel;
		}
		d->hero_dialogue = IMG_Load("../img/hero/safwen_choice_active.png");
		d->dialogue_box = SDL_CreateRGBSurface(SDL_HWSURFACE, SCREEN_WIDTH, 180, 32, 0, 0, 0, 0);
		SDL_FillRect(d->dialogue_box, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
	}

	else if (h.position.x >= 2100 && h.position.x < 2600 && d->line < 8)
	{
		tempsActuel = SDL_GetTicks();
		if (tempsActuel - tempsPrecedent > 3000)
		{
			d->text.font = TTF_OpenFont("../fonts/chalk_2.ttf", 30);
			d->text.text = TTF_RenderText_Blended(d->text.font, d->script[d->line], d->text.color);
			d->line++;
			tempsPrecedent = tempsActuel;
		}
		d->hero_dialogue = IMG_Load("../img/hero/safwen_choice_active.png");
		d->dialogue_box = SDL_CreateRGBSurface(SDL_HWSURFACE, SCREEN_WIDTH, 180, 32, 0, 0, 0, 0);
		SDL_FillRect(d->dialogue_box, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
	}
	else
	{
		d->text.text = NULL;
		tempsActuel = 0;
		tempsPrecedent = 0;
		d->dialogue_box = NULL;
		d->hero_dialogue = NULL;
	}
}

void afficher_dialogue(dialogue d, SDL_Surface *ecran)
{
	SDL_BlitSurface(d.dialogue_box, NULL, ecran, &d.pos_dialogue_box);
	SDL_BlitSurface(d.hero_dialogue, NULL, ecran, &d.pos_hero_dialogue);
	SDL_BlitSurface(d.text.text, NULL, ecran, &d.text.position);
}