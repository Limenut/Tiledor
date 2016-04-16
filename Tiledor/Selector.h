#pragma once

#include <SDL2/SDL.h>

//a "cursor" for selecting tiles on either window
class Selector
{
public:
	Selector();
	~Selector();
	void init(Uint32 _id);
	void makeTex();	//draw the cursor

	char winIndex;

	SDL_Rect rect;
	SDL_Texture *tex;
	int currentTile;
};