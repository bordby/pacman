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

char addTile(SDL_FRect tiles[], int *numberOfTiles, int coordinates){
	float x, y;
	x = coordinates % 1000;
	y = coordinates / 1000;
	for(int i = 0; i < *numberOfTiles; i++){
		if( ((tiles + i)->x == x) && ((tiles + i)->y == y)){
			return 0;
		}
	}
	(tiles + *numberOfTiles)->x = x;
	(tiles + *numberOfTiles)->y = y;
	(tiles + *numberOfTiles)->w = TILESIZE;
	(tiles + *numberOfTiles)->h = TILESIZE;
	(*numberOfTiles)++;
	return 1;
}

void saveTiles(SDL_FRect tiles[], int *numberOfTiles){
	FILE *fptr = NULL;

	fptr = fopen("tiles.by", "w");

	if(fptr == NULL){
		printf("File cannot be opened\n");
		return;
	}

	for(int i = 0; i < *numberOfTiles; i++){
		fprintf(fptr, "%d\n", (int)((tiles + i)->y * 1000 + (tiles + i)->x));
	}
	fprintf(fptr, "%d\n", -1);

	fclose(fptr);
}

void loadTiles(SDL_FRect tiles[], int *numberOfTiles){
	FILE *fptr = NULL;

	fptr = fopen("tiles.by", "r");

	if(fptr == NULL){
		printf("File cannot be opened\n");
		return;
	}

	int coordinates;
	*numberOfTiles = 0;
	for(int i = 0; i < (RESX / TILESIZE) * (RESY / TILESIZE); i++){
		fscanf(fptr, "%d", &coordinates);
		if(coordinates < 0)
			break;

		(tiles + i)->x = coordinates % 1000;
		(tiles + i)->y = coordinates / 1000;
		(tiles + i)->w = TILESIZE;
		(tiles + i)->h = TILESIZE;
		(*numberOfTiles)++;
	}

	fclose(fptr);
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


	SDL_FRect tiles[(RESX / TILESIZE) * (RESY / TILESIZE)];
	int numberOfTiles = 0;

	loadTiles(tiles, &numberOfTiles);

	SDL_FRect fruits[(RESX / TILESIZE) * (RESY / TILESIZE)];
	int numberOfFruits = 0;

	fruits->x = 720 + ( TILESIZE * 3 / 8); fruits->y = 40 + (TILESIZE * 3 / 8); 
	fruits->w = TILESIZE / 4; fruits->h = TILESIZE / 4;
	numberOfFruits++;

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
					case SDLK_S:
						saveTiles(tiles, &numberOfTiles);
						break;
					case SDLK_X:
						loopRun = 0;
						break;
				}
			else if(event.type == SDL_EVENT_MOUSE_BUTTON_DOWN){
				SDL_GetMouseState(&mouseX, &mouseY);
				mouseX = ((int)mouseX / TILESIZE);
				mouseY = ((int)mouseY / TILESIZE);

				mouseX *= TILESIZE;
				mouseY *= TILESIZE;
				addTile(tiles, &numberOfTiles, mouseY * 1000 + mouseX);
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
		for(int i = 0; i < numberOfFruits; i++){
			SDL_SetRenderDrawColor(renderer, 200, 200, 255, 0);
			SDL_RenderFillRect(renderer, fruits + i);
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
