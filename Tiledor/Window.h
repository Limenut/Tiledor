#pragma once

#include <SDL2\SDL.h>

//each window has its own renderer and event handler for window-related events
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

