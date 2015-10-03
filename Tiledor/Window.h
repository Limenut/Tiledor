#pragma once

#include <SDL2\SDL.h>

class Window
{
public:
	Window();
	~Window();
	bool init(const string &_name);

	void handleEvents(SDL_Event *e);

	string name;
	Uint32 id;

	SDL_Window* win;
	SDL_Renderer* ren;
	SDL_Rect area;

	Selector sel;

	int offsetX;
	int offsetY;

	bool mouseFocus;
	bool keyFocus;
};

