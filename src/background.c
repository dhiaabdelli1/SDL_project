#include "background.h"

void initialiser_background(background *b)
{
	b->image = IMG_Load("../img/background/x2.jpg");
	b->background_mask = IMG_Load("../img/background/x2_mask.jpg");

	b->posCamera.x = 400;
	b->posCamera.y = 900;
	b->posCamera.w = SCREEN_WIDTH;
	b->posCamera.h = SCREEN_HEIGHT;

	b->position_background_mask.x = 400;
	b->position_background_mask.y = 900;
	b->position_background_mask.w = SCREEN_WIDTH;
	b->position_background_mask.h = SCREEN_HEIGHT;

	//optimized image loading
	//b->image = SDL_DisplayFormat(b->image);
	//b->background_mask = SDL_DisplayFormat(b->background_mask);
}

void afficher_background(background *b, SDL_Surface *screen)
{

	SDL_BlitSurface(b->background_mask, &b->position_background_mask, screen, NULL);
	SDL_BlitSurface(b->image, &b->posCamera, screen, NULL);
}
void free_background(background *b)
{
	SDL_FreeSurface(b->image);
	SDL_FreeSurface(b->background_mask);
}

void initialiser_platforme(platforme *p, int x, int y)
{
	p->image = IMG_Load("../img/background/platform.png");
	p->position.x = x;
	p->position.y = y;
	p->image = SDL_DisplayFormat(p->image);
}

void animer_platforme(platforme *p, int x)
{
	static int tempsActuel = 0;
	static int tempsPrecedent = 0;
	static int sens = 1;
	if (x == 0)
	{
		tempsActuel = SDL_GetTicks();
		if (tempsActuel - tempsPrecedent > 10)
		{
			p->position.x += 1 * sens;
			tempsPrecedent = tempsActuel;
		}
		if (p->position.x >= 1380)
			sens = -1;
		if (p->position.x <= 1180)
			sens = 1;
	}
	else if (x == 1)
	{
		tempsActuel = SDL_GetTicks();
		if (tempsActuel - tempsPrecedent > 10)
		{
			p->position.y += 1 * sens;
			tempsPrecedent = tempsActuel;
		}
		if (p->position.y >= 1380)
			sens = -1;
		if (p->position.y <= 1180)
			sens = 1;
	}
}

void afficher_platforme(platforme p, background b, SDL_Surface *ecran)
{
	SDL_Rect pos;
	pos.x = p.position.x - b.posCamera.x;
	pos.y = p.position.y - b.posCamera.y;
	SDL_BlitSurface(p.image, NULL, ecran, &pos);
}

void initialiser_text(text *i, char message[40], int x, int y, int size)
{
	TTF_Init();
	i->color.r = 255;
	i->color.g = 255;
	i->color.b = 255;
	i->font = TTF_OpenFont("../fonts/chalk_2.ttf", size);
	i->text = TTF_RenderText_Blended(i->font, message, i->color);
	i->position.x = x;
	i->position.y = y;
}
void afficher_text(text i, background b, SDL_Surface *ecran)
{
	SDL_Rect pos;
	pos.x = i.position.x - b.posCamera.x;
	pos.y = i.position.y - b.posCamera.y;
	SDL_BlitSurface(i.text, NULL, ecran, &pos);
}

void initialiser_text_2(text *i, int x, int y, int size)
{
	TTF_Init();
	i->color.r = 255;
	i->color.g = 255;
	i->color.b = 255;
	i->position.x = x;
	i->position.y = y;
	i->font = TTF_OpenFont("../fonts/chalk_2.ttf", size);
}
void afficher_text_2(text i, SDL_Surface *ecran, char message[20])
{

	i.text = TTF_RenderText_Blended(i.font, message, i.color);
	SDL_BlitSurface(i.text, NULL, ecran, &i.position);
}

void initialiser_instructions(text instructions[], int n)
{

	TTF_Init();
	int i;
	for (i = 0; i < n; i++)
	{
		instructions[i].color.r = 255;
		instructions[i].color.g = 255;
		instructions[i].color.b = 255;
		instructions[i].font = TTF_OpenFont("../fonts/chalk_2.ttf", 30);
	}

	instructions[0].text = TTF_RenderText_Blended(instructions[0].font, "Press the arrow keys to move around", instructions[0].color);
	instructions[0].position.x = 488;
	instructions[0].position.y = 1360;
	instructions[1].text = TTF_RenderText_Blended(instructions[1].font, "Press x to punch and c to kick", instructions[1].color);
	instructions[1].position.x = 1400;
	instructions[1].position.y = 1300;
	instructions[2].text = TTF_RenderText_Blended(instructions[2].font, "Jump to get more coins!", instructions[2].color);
	instructions[2].position.x = 1800;
	instructions[2].position.y = 1540;
	instructions[3].text = TTF_RenderText_Blended(instructions[2].font, "Just kidding lol!", instructions[2].color);
	instructions[3].position.x = 2050;
	instructions[3].position.y = 2300;
}
void afficher_instructions(text instructions[], int n, background b, SDL_Surface *ecran)
{
	int i;
	SDL_Rect pos;
	for (i = 0; i < n; i++)
	{
		pos.x = instructions[i].position.x - b.posCamera.x;
		pos.y = instructions[i].position.y - b.posCamera.y;
		SDL_BlitSurface(instructions[i].text, NULL, ecran, &pos);
	}
}

