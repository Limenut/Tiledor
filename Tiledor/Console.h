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
	void startInput();
	void stopInput();
	void render();
	SDL_Keycode handleEvents();
	void updateTexture();
	void updateSave();
	void addLine(const string& _line);
	void clear();
	void highlight();
	string saveMapText();
	string loadMapText();
	string loadBitText();

	bool isOpen;
	bool inputOn;
	SDL_Rect backRect;
	SDL_Rect textRect;
	SDL_Rect highRect;
	SDL_Renderer** renderer;
	TTF_Font* font;
	string text;
	string input;
	SDL_Texture *tex;
	SDL_Color color = { 0xFF,0xFF,0xFF,0xFF };

	vector<string> lines;

	//int textHeight;
	//int textWidth;
	int lineHeight;
	unsigned selection = 0;
};