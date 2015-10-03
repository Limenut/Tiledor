#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Console
{
public:
	Console();
	~Console();
	Console(SDL_Renderer *ren);
	void open();
	void close();
	void render();
	void handleEvents();
	void updateTexture();
	void updateSave();

	bool isOpen;
	SDL_Rect backRect;
	SDL_Rect textRect;
	SDL_Renderer** renderer;
	TTF_Font* font;
	string text;
	string input;
	SDL_Texture *tex;
	SDL_Color color = { 0xFF,0xFF,0xFF,0xFF };

	int textHeight;
	int textWidth;
};