#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define RESX 800
#define RESY 800

#define TILESIZE 40
#define MOVE 10

struct Pacman {
	SDL_FRect rect;
	char direction;
};

char collision(struct Pacman *pacman, SDL_FRect *tiles, int numberOfTiles){
	float x = pacman->rect.x, y = pacman->rect.y;
	switch(pacman->direction){
		case 'l':
			x += MOVE;
			for(int i = 0; i < numberOfTiles; i++){
				if(((x + TILESIZE) > (tiles + i)->x) && (x < (tiles +i)->x))
					if(((y + TILESIZE) > (tiles + i)->y ) && (y < ((tiles + i)->y + TILESIZE)))
						return 1;
			}
			break;
		case 'k':
			y -= MOVE;
			for(int i = 0; i < numberOfTiles; i++){
				if((y < ((tiles + i)->y + TILESIZE)) && (y > (tiles + i)->y))
					if(((x + TILESIZE) > (tiles + i)->x) && (x < ((tiles +i)->x + TILESIZE)))
						return 1;
			}
			break;
		case 'j':
			y += MOVE;
			for(int i = 0; i < numberOfTiles; i++){
				if(((y + TILESIZE) > (tiles + i)->y ) && (y < ((tiles + i)->y)))
					if(((x + TILESIZE) > (tiles + i)->x) && (x < ((tiles +i)->x + TILESIZE)))
						return 1;
			}
			break;
		case 'h':
			x -= MOVE;
			for(int i = 0; i < numberOfTiles; i++){
				if((x < ((tiles + i)->x + TILESIZE)) && (x > (tiles +i)->x))
					if(((y + TILESIZE) > (tiles + i)->y ) && (y < ((tiles + i)->y + TILESIZE)))
						return 1;
			}
			break;
	}
	return 0;
}

int main(void){
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window      *window = NULL;
	SDL_Renderer  *renderer = NULL;

	window = SDL_CreateWindow("Pacman", RESX, RESY, 0);
	if(window == NULL){
		printf("Could not create a window\n");
		return 0;
	}
	renderer = SDL_CreateRenderer(window, NULL);
	if(renderer == NULL){
		printf("Could not create a renderer\n"); 
		return 0;
	}

	struct Pacman *pacman;
	pacman = malloc(sizeof(struct Pacman));
	pacman->rect.x = 0;        pacman->rect.y = 0; 
	pacman->rect.w = TILESIZE; pacman->rect.h = TILESIZE;
	pacman->direction = 'o';


	SDL_FRect tiles[(RESX * RESY) / (TILESIZE * TILESIZE)];
	int numberOfTiles = 0;

	tiles->x = 200; tiles->y = 200; tiles->w = TILESIZE; tiles->h = TILESIZE;
	numberOfTiles++;
	

	SDL_Event event;
	char loopRun = 1, speed = 3, speedIndex = 0;
	while(loopRun){
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_EVENT_QUIT){
				loopRun = 0;
			}
			else if(event.type == SDL_EVENT_KEY_DOWN)
				switch(event.key.key){
					case SDLK_L:
						pacman->direction = 'l';
						break;
					case SDLK_K:
						pacman->direction = 'k';
						break;
					case SDLK_J:
						pacman->direction = 'j';
						break;
					case SDLK_H:
						pacman->direction = 'h';
						break;
				}
		}
		if(speedIndex > speed){
			if(!collision(pacman, tiles, numberOfTiles))
			switch(pacman->direction){
				case 'l':
					pacman->rect.x += MOVE;
					break;
				case 'k':
					pacman->rect.y -= MOVE;
					break;
				case 'j':
					pacman->rect.y += MOVE;
					break;
				case 'h':
					pacman->rect.x -= MOVE;
					break;
			}
			speedIndex = 0;
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);
		SDL_RenderFillRect(renderer, &(pacman->rect));

		for(int i = 0; i < numberOfTiles; i++){
			SDL_SetRenderDrawColor(renderer, 20, 100, 255, 0);
			SDL_RenderFillRect(renderer, tiles + i);
		}


		SDL_RenderPresent(renderer);
		SDL_Delay(16);
		speedIndex++;
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
