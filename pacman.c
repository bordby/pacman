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

struct Tiles {
	SDL_FRect rects[(RESX / TILESIZE) * (RESY / TILESIZE)];
	int count;
};

struct Fruits {
	SDL_FRect rects[(RESX / TILESIZE) * (RESY / TILESIZE)];
	int count;
};

char collision(struct Pacman *pacman, char direction, struct Tiles *tiles, struct Fruits *fruits){
	float x = pacman->rect.x, y = pacman->rect.y;
	switch(direction){
		case 'l':
			x += MOVE;
			for(int i = 0; i < tiles->count; i++){
				if(((x + TILESIZE) > (tiles->rects + i)->x) && (x < (tiles->rects +i)->x))
					if(((y + TILESIZE) > (tiles->rects + i)->y ) && (y < ((tiles->rects + i)->y + TILESIZE)))
						return 1;
			}
			for(int i = 0; i < fruits->count; i++){
				if(((x + TILESIZE) > (fruits->rects + i)->x) && (x < (fruits->rects +i)->x))
					if(((y + TILESIZE) > (fruits->rects + i)->y ) && (y < ((fruits->rects + i)->y + TILESIZE)))
						return 2;
			}
			break;
		case 'k':
			y -= MOVE;
			for(int i = 0; i < tiles->count; i++){
				if((y < ((tiles->rects + i)->y + TILESIZE)) && (y > (tiles->rects + i)->y))
					if(((x + TILESIZE) > (tiles->rects + i)->x) && (x < ((tiles->rects +i)->x + TILESIZE)))
						return 1;
			}
			for(int i = 0; i < fruits->count; i++){
				if((y < ((fruits->rects + i)->y + TILESIZE)) && (y > (fruits->rects + i)->y))
					if(((x + TILESIZE) > (fruits->rects + i)->x) && (x < ((fruits->rects +i)->x + TILESIZE)))
						return 2;
			}
			break;
		case 'j':
			y += MOVE;
			for(int i = 0; i < tiles->count; i++){
				if(((y + TILESIZE) > (tiles->rects + i)->y ) && (y < ((tiles->rects + i)->y)))
					if(((x + TILESIZE) > (tiles->rects + i)->x) && (x < ((tiles->rects +i)->x + TILESIZE)))
						return 1;
			}
			for(int i = 0; i < fruits->count; i++){
				if(((y + TILESIZE) > (fruits->rects + i)->y ) && (y < ((fruits->rects + i)->y)))
					if(((x + TILESIZE) > (fruits->rects + i)->x) && (x < ((fruits->rects +i)->x + TILESIZE)))
						return 2;
			}
			break;
		case 'h':
			x -= MOVE;
			for(int i = 0; i < tiles->count; i++){
				if((x < ((tiles->rects + i)->x + TILESIZE)) && (x > (tiles->rects +i)->x))
					if(((y + TILESIZE) > (tiles->rects + i)->y ) && (y < ((tiles->rects + i)->y + TILESIZE)))
						return 1;
			}
			for(int i = 0; i < fruits->count; i++){
				if((x < ((fruits->rects + i)->x + TILESIZE)) && (x > (fruits->rects +i)->x))
					if(((y + TILESIZE) > (fruits->rects + i)->y ) && (y < ((fruits->rects + i)->y + TILESIZE)))
						return 2;
			}
			break;
	}
	return 0;
}

char addTile(struct Tiles *tiles, int coordinates){
	float x, y;
	x = coordinates % 1000;
	y = coordinates / 1000;
	for(int i = 0; i < tiles->count; i++){
		if( ((tiles->rects + i)->x == x) && ((tiles->rects + i)->y == y)){
			return 1;
		}
	}
	(tiles->rects + tiles->count)->x = x;
	(tiles->rects + tiles->count)->y = y;
	(tiles->rects + tiles->count)->w = TILESIZE;
	(tiles->rects + tiles->count)->h = TILESIZE;
	(tiles->count)++;
	return 0;
}

void saveTiles(struct Tiles *tiles){
	FILE *fptr = NULL;

	fptr = fopen("tiles.by", "w");

	if(fptr == NULL){
		printf("File cannot be opened\n");
		return;
	}

	for(int i = 0; i < tiles->count; i++){
		fprintf(fptr, "%d\n", (int)((tiles->rects + i)->y * 1000 + (tiles->rects + i)->x));
	}
	fprintf(fptr, "%d\n", -1);

	fclose(fptr);
}

void loadTiles(struct Tiles *tiles){
	FILE *fptr = NULL;

	fptr = fopen("tiles.by", "r");

	if(fptr == NULL){
		printf("File cannot be opened\n");
		return;
	}

	int coordinates;
	tiles->count = 0;
	for(int i = 0; i < (RESX / TILESIZE) * (RESY / TILESIZE); i++){
		fscanf(fptr, "%d", &coordinates);
		if(coordinates < 0)
			break;

		(tiles->rects + i)->x = coordinates % 1000;
		(tiles->rects + i)->y = coordinates / 1000;
		(tiles->rects + i)->w = TILESIZE;
		(tiles->rects + i)->h = TILESIZE;
		(tiles->count)++;
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


	struct Tiles *tiles = malloc(sizeof(struct Tiles)); 
	tiles->count = 0;

	struct Fruits *fruits = malloc(sizeof(struct Fruits)); 
	fruits->count = 0;

	loadTiles(tiles);


	fruits->rects->x = 720; fruits->rects->y = 40; 
	// + ( TILESIZE * 3 / 8)         + (TILESIZE * 3 / 8) 
	fruits->rects->w = TILESIZE / 4; fruits->rects->h = TILESIZE / 4;
	fruits->count = 1;

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
						saveTiles(tiles);
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
				addTile(tiles, mouseY * 1000 + mouseX);
			}
		}
		if(speedIndex > speed){
			if(collision(pacman, bufferDirection, tiles, fruits) != 1)
				pacman->direction = bufferDirection;
			
			switch(collision(pacman, pacman->direction, tiles, fruits)){
				case 0:
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
					break;
				case 2:
					printf("Hit the fruit!\n");
					break;
			}
			speedIndex = 0;
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);
		SDL_RenderFillRect(renderer, &(pacman->rect));

		for(int i = 0; i < tiles->count; i++){
			SDL_SetRenderDrawColor(renderer, 20, 100, 255, 0);
			SDL_RenderFillRect(renderer, tiles->rects + i);
		}
		for(int i = 0; i < fruits->count; i++){
			SDL_SetRenderDrawColor(renderer, 200, 200, 255, 0);
			SDL_RenderFillRect(renderer, fruits->rects + i);
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
