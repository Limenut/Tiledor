#pragma once

#include <SDL2/SDL.h>

class Selector
{
public:
	Selector();
	~Selector();
	void init(Uint32 _id);
	void makeTex();
	void makeTex2();

	//Window *currentWin;
	char winIndex;

	//int x;
	//int y;
	SDL_Rect rect;
	SDL_Texture *tex;
	int currentTile;
};