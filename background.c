#include "background.h"

void initialiser_background(background *b)
{
	b->image = IMG_Load("./img/background/x2.jpg");
	b->background_mask = IMG_Load("./img/background/x2_mask.jpg");
	b->position_background.x = 400;
	b->position_background.y = 900;
	b->position_background.w = SCREEN_WIDTH;
	b->position_background.h = SCREEN_HEIGHT;

	b->position_background_mask.x = 400;
	b->position_background_mask.y = 900;
	b->position_background_mask.w = SCREEN_WIDTH;
	b->position_background_mask.h = SCREEN_HEIGHT;


	b->posCamera.x = 0; //b->posCamera.x = ( h->position.x + h->sprite.frame.w / 2) - SCREEN_WIDTH / 4
	b->posCamera.y = 0; // - SCREEN_HEIGHT / 2;
	b->posCamera.w = b->image->w;
	b->posCamera.h = b->image->h;

	//optimized image loading
	b->image = SDL_DisplayFormat(b->image);
	//b->background_mask = SDL_DisplayFormat(b->background_mask);
}

void afficher_background(background *b, SDL_Surface *screen)
{

	SDL_BlitSurface(b->background_mask, &b->position_background_mask, screen, NULL);
	SDL_BlitSurface(b->image, &b->position_background, screen, NULL);
}
void free_background(background *b)
{
	SDL_FreeSurface(b->image);
	SDL_FreeSurface(b->background_mask);
}