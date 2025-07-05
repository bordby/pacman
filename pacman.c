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

char collision(struct Pacman *pacman, char direction, SDL_FRect *tiles, int numberOfTiles){
	float x = pacman->rect.x, y = pacman->rect.y;
	switch(direction){
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

void addTile(SDL_FRect tiles[], int *numberOfTiles, int coordinates){
	(tiles + *numberOfTiles)->x = coordinates % 1000;
	(tiles + *numberOfTiles)->y = coordinates / 1000;
	(tiles + *numberOfTiles)->w = TILESIZE;
	(tiles + *numberOfTiles)->h = TILESIZE;
	(*numberOfTiles)++;
}

void initTiles(SDL_FRect tiles[], int *numberOfTiles){
	int tileLocations[] = {
		0, 40, 80, 120
	};
	*numberOfTiles = 0;
	int size = sizeof(tileLocations) / sizeof(tileLocations[0]);
	for(int i = 0; i < size; i++){
		(tiles + *numberOfTiles)->x = tileLocations[i] % 1000;
		(tiles + *numberOfTiles)->y = tileLocations[i] / 1000;
		(tiles + *numberOfTiles)->w = TILESIZE;
		(tiles + *numberOfTiles)->h = TILESIZE;
		(*numberOfTiles)++;
	}
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
	pacman->rect.x = 40;        pacman->rect.y = 40; 
	pacman->rect.w = TILESIZE; pacman->rect.h = TILESIZE;
	pacman->direction = 'o';


	SDL_FRect tiles[(RESX * RESY) / (TILESIZE * TILESIZE)];
	int numberOfTiles = 0;

	initTiles(tiles, &numberOfTiles);

	SDL_Event event;
	char loopRun = 1, speed = 3, speedIndex = 0;
	char bufferDirection = 'o';
	float mouseX = 0, mouseY = 0;
	int IMouseX = 0, IMouseY = 0;
	while(loopRun){
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_EVENT_QUIT){
				loopRun = 0;
			}
			else if(event.type == SDL_EVENT_KEY_DOWN)
				switch(event.key.key){
					case SDLK_L:
						bufferDirection = 'l';
						break;
					case SDLK_K:
						bufferDirection = 'k';
						break;
					case SDLK_J:
						bufferDirection = 'j';
						break;
					case SDLK_H:
						bufferDirection = 'h';
						break;
				}
			else if(event.type == SDL_EVENT_MOUSE_BUTTON_UP){
				SDL_GetMouseState(&mouseX, &mouseY);
				IMouseX = mouseX / TILESIZE;
				IMouseY = mouseY / TILESIZE;

				IMouseX *= TILESIZE;
				IMouseY *= TILESIZE;
				printf("x: %d, y: %d\n", IMouseX, IMouseY);
				addTile(tiles, &numberOfTiles, IMouseY * 1000 + IMouseX);
			}
		}
		if(speedIndex > speed){
			if(!collision(pacman, bufferDirection, tiles, numberOfTiles))
				pacman->direction = bufferDirection;
			
			if(!collision(pacman, pacman->direction, tiles, numberOfTiles))
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
