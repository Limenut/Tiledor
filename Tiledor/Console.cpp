#include "Globals.h"
#include <iostream>
#include <sstream>

Console::Console()
{
	isOpen = false;
	inputOn = false;
	backRect.w = 600;
	backRect.h = 300;
	backRect.x = SCREEN_WIDTH / 2 - backRect.w / 2;
	backRect.y = SCREEN_HEIGHT / 2 - backRect.h / 2;
}
Console::~Console()
{
	SDL_DestroyTexture(tex);
}

Console::Console(SDL_Renderer *ren)
{
	renderer = &ren;
	isOpen = false;
	backRect.w = 600;
	backRect.h = 300;
	backRect.x = SCREEN_WIDTH / 2 - backRect.w / 2;
	backRect.y = SCREEN_HEIGHT / 2 - backRect.h / 2;
}

void Console::open()
{
	selection = 0;
	isOpen = true;
	updateTexture();
}

void Console::close()
{
	isOpen = false;
	inputOn = false;
}

void Console::startInput()
{
	SDL_StartTextInput();
	inputOn = true;
}

void Console::stopInput()
{
	SDL_StopTextInput();
	inputOn = false;
	input = "";
}

void Console::render()
{
	//pienempi rendausalue ja musta väri
	SDL_RenderSetViewport(mainWindow.ren, &backRect);
	SDL_SetRenderDrawColor(mainWindow.ren, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(mainWindow.ren, NULL);

	SDL_SetRenderDrawColor(mainWindow.ren, 0x55, 0x55, 0x55, 0xFF);
	SDL_RenderFillRect(mainWindow.ren, &highRect);

	SDL_RenderCopy(mainWindow.ren, tex, NULL, &textRect);

	//oletusten palautus
	SDL_RenderSetViewport(mainWindow.ren, NULL);
}

SDL_Keycode Console::handleEvents()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_KEYDOWN)
		{
			if (e.key.keysym.sym == SDLK_BACKSPACE && inputOn && input.length() > 0)
			{
				//lop off character
				input.pop_back();
				updateTexture();
			}
			
			//enter: konsoli sammutetaan
			if (e.key.keysym.sym == SDLK_RETURN)
			{
				return SDLK_RETURN;
			}
			if (e.key.keysym.sym == SDLK_ESCAPE)
			{
				return SDLK_ESCAPE;
			}
			if (e.key.keysym.sym == SDLK_UP)
			{
				if (selection > 0) selection--;
				else selection = lines.size() - 1;
				highlight();
			}
			if (e.key.keysym.sym == SDLK_DOWN)
			{
				if (selection < lines.size() - 1) selection++;
				else selection = 0;
				highlight();
			}
			if (e.key.keysym.sym == SDLK_LEFT)
			{
				return SDLK_LEFT;
			}
			if (e.key.keysym.sym == SDLK_RIGHT)
			{
				return SDLK_RIGHT;
			}
		}
		//Special text input event
		else if (e.type == SDL_TEXTINPUT && inputOn && input.length() < 20)
		{
			//Append character
			input += e.text.text[0];
			updateTexture();
		}
	}
	return 0;
}

void Console::updateTexture()
{
	if (!font)
	{
		return;
	}

	std::vector<SDL_Surface*> lineSurfs;

	lineHeight = TTF_FontLineSkip(font);

	textRect.w = textRect.h = 0;

	for (unsigned i = 0; i < lines.size(); i++)
	{
		auto surf = TTF_RenderText_Blended(font, lines[i].c_str(), color);

		textRect.h += lineHeight;
		if (surf->w > textRect.w) textRect.w = surf->w;

		lineSurfs.push_back(surf);
	}


	auto finalSurf = SDL_CreateRGBSurface(
		0,
		textRect.w,
		textRect.h,
		32,
		0xff000000,
		0x00ff0000,
		0x0000ff00,
		0x000000ff
		);

	if (!finalSurf)
	{
		printf("Couldn't create the destination surface!\n");
		return;
	}

	int i = 0;
	for (auto surf : lineSurfs)
	{
		SDL_Rect rect;
		rect.x = 0;
		rect.y = lineHeight * i++;
		rect.w = surf->w;
		rect.h = surf->h;

		SDL_BlitSurface(surf, nullptr, finalSurf, &rect);
		SDL_FreeSurface(surf);
	}

	lineSurfs.clear();


	if (tex)
	{
		SDL_DestroyTexture(tex);
	}

	tex = SDL_CreateTextureFromSurface(mainWindow.ren, finalSurf);

	SDL_FreeSurface(finalSurf);
}

void Console::updateSave()
{
	SDL_Surface *surf = NULL;

	text = "Save as: " + input + ".map";

	surf = TTF_RenderText_Solid(font, text.c_str(), { 0xFF, 0xFF, 0xFF, 0xFF });
	tex = SDL_CreateTextureFromSurface(mainWindow.ren, surf);

	textRect.w = surf->w;
	textRect.h = surf->h;

	SDL_FreeSurface(surf);
}

void Console::addLine(const string& _line)
{
	lines.push_back(_line);
	updateTexture();
}

void Console::clear()
{
	lines.clear();
	SDL_DestroyTexture(tex);
	tex = nullptr;
}

void Console::highlight()
{
	highRect.y = lineHeight*selection;
	highRect.x = 0;
	highRect.w = backRect.w;
	highRect.h = lineHeight;
}

string Console::saveMapText()
{
	return "Save as: " + input + ".map";
}

string Console::loadMapText()
{
	return "Load map: " + input + ".map";
}

string Console::loadBitText()
{
	return "Load bitmap: " + input;
}