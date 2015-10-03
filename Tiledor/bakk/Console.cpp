#include "Globals.h"
#include <iostream>
#include <sstream>

Console::Console()
{
	isOpen = false;
	backRect.w = 600;
	backRect.h = 300;
	backRect.x = SCREEN_WIDTH / 2 - backRect.w / 2;
	backRect.y = SCREEN_HEIGHT / 2 - backRect.h / 2;
}
Console::~Console()
{
	SDL_DestroyTexture(tex);
	//TTF_CloseFont(font);
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
	SDL_StartTextInput();
	isOpen = true;
	updateSave();
}

void Console::close()
{
	SDL_StopTextInput();
	isOpen = false;
}

void Console::render()
{
	//pienempi rendausalue ja musta väri
	SDL_RenderSetViewport(mainWindow.ren, &backRect);
	SDL_SetRenderDrawColor(mainWindow.ren, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(mainWindow.ren, NULL);

	SDL_RenderCopy(mainWindow.ren, tex, NULL, &textRect);

	//oletusten palautus
	SDL_RenderSetViewport(mainWindow.ren, NULL);
}

void Console::handleEvents()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		//Special key input
		if (e.type == SDL_KEYDOWN)
		{
			if (e.key.keysym.sym == SDLK_BACKSPACE && input.length() > 0)
			{
				//lop off character
				input.pop_back();
				updateSave();
			}
			//enter: konsoli sammutetaan
			if (e.key.keysym.sym == SDLK_RETURN)
			{
				gTilemap.saveFile(input+".map");
				console.close();
				return;
			}
			if (e.key.keysym.sym == SDLK_ESCAPE)
			{
				console.close();
				return;
			}
		}
		//Special text input event
		else if (e.type == SDL_TEXTINPUT)
		{
			//Append character
			input += e.text.text[0];
			updateSave();
		}
	}
}

void Console::updateTexture()
{
	if (!font)
	{
		return;
	}

	std::stringstream ss(text);
	std::vector<SDL_Surface*> lineSurfs;
	std::string line;

	int lineHeight = TTF_FontLineSkip(font);

	textHeight = 0;
	textWidth = 0;
	while (std::getline(ss, line))
	{
		auto surf = TTF_RenderText_Blended(font, line.c_str(), color);

		textHeight += lineHeight;
		if (surf->w > textWidth) textWidth = surf->w;

		lineSurfs.push_back(surf);
	}

	auto finalSurf = SDL_CreateRGBSurface(
		0,
		textWidth,
		textHeight,
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

	//updateTexture();

	surf = TTF_RenderText_Solid(font, text.c_str(), { 0xFF, 0xFF, 0xFF, 0xFF });
	tex = SDL_CreateTextureFromSurface(mainWindow.ren, surf);

	textRect.w = surf->w;
	textRect.h = surf->h;

	SDL_FreeSurface(surf);
}