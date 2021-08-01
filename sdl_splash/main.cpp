#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <thread>

const int W = 800;
const int H = 600;
SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Renderer* renderer = NULL;

void LoadIcon(const char* file) {
	SDL_Surface* imageSurface = NULL;
	imageSurface = IMG_Load(file);
	if (!imageSurface) {
		printf("IMG_Load fail\n");
		return;
	}
	SDL_SetWindowIcon(window, imageSurface);
	//texture = SDL_CreateTextureFromSurface(renderer, imageSurface);
	SDL_FreeSurface(imageSurface);
	return;
}

SDL_Texture* LoadImage(const char* file) {
	return IMG_LoadTexture(renderer, file);
}

void loading() {
	SDL_RenderClear(renderer);
	SDL_Texture* bg = SDL_CreateTextureFromSurface(renderer, screenSurface);
	SDL_RenderCopy(renderer, bg, NULL, NULL);

	SDL_Texture* texture = LoadImage("rotate.png");
	SDL_Rect srect = { 0 };
	SDL_Rect drect = { 0 };
	SDL_QueryTexture(texture, NULL, NULL, &srect.w, &srect.h);
	drect.x = (W - srect.w) / 2;
	drect.y = (H - srect.h) / 2;
	drect.w = srect.w;
	drect.h = srect.h;
	//printf("%d %d - %d %d\n", srect.w, srect.h, drect.x, drect.y);
	SDL_RenderCopy(renderer, texture, NULL, &drect);
	SDL_Point point = { 0 };
	point.x = srect.x + srect.w / 2;
	point.y = srect.y + srect.h / 2;
	SDL_RenderPresent(renderer);

	int angel = 0;
	while (true) {
		angel = (angel + 10) % 360;
		SDL_RenderCopy(renderer, bg, NULL, NULL);
		SDL_RenderCopyEx(renderer, texture, NULL, &drect, (double)angel, &point, SDL_FLIP_NONE);
		SDL_Delay(50);
		SDL_RenderPresent(renderer);
	}
}

int SDL_main(int argc, char* argv[]) {

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("SDL_Init fail\n");
	}
	window = SDL_CreateWindow("Snapmaker Luban", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, W, H, SDL_WINDOW_SHOWN);
	LoadIcon("snap-luban-logo-64x64.png");
	if (!window) {
		printf("SDL_CreateWindow fail\n");
	}
	else {
		screenSurface = SDL_GetWindowSurface(window);
		SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xf5, 0xf5, 0xf7));
		SDL_UpdateWindowSurface(window);
		//SDL_Delay(0);
		//SDL_Rect rect = { 0, 0, imageSurface->w, imageSurface->h };
		//SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0xff, 0xff, 0x00));
		//SDL_BlitSurface(imageSurface, &rect, screenSurface, &rect);
		//SDL_UpdateWindowSurface(window);
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		printf("SDL_CreateRenderer fail\n");
	}

	std::thread loadingThread(loading);
	
	bool quit = false;
	while (!quit) {
		SDL_Event event;
		while (SDL_WaitEvent(&event)) {
			if (event.type == SDL_QUIT) {
				loadingThread.detach();
				quit = true;
				break;
			}
		}
	}
	//SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_FreeSurface(screenSurface);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}