#include "Globals.h"
#include <SDL2/SDL_image.h>
#include <iostream>


Texturemap::~Texturemap()
{
	SDL_DestroyTexture(fullTex);
	for (unsigned i = 0; i < map.size(); i++)
	{
		SDL_DestroyTexture(map[i]);
	}
}

void Texturemap::makeMap(const string &_file, int tileRes)
{
	map.clear();

	if (!mainWindow.ren)
	{
		cout << "No renderer! Can't load texture!" << endl;
		return;
	}

	cout << "Loading " << _file.c_str() << "... ";

	SDL_Surface *fullSurf = IMG_Load(_file.c_str());
	if (!fullSurf)
	{
		cout << "Loading failed" << endl;
		return;
	}

	//save whole image
	fullTex = SDL_CreateTextureFromSurface(tileWindow.ren, fullSurf);


	SDL_Rect fullRect;
	fullRect.x = fullRect.y = 0;
	fullRect.w = fullSurf->w;
	fullRect.h = fullSurf->h;

	rect.x = rect.y = 0;
	rect.w = fullSurf->w;
	rect.h = fullSurf->h;

	SDL_Texture *tex;
	SDL_Surface *surf = SDL_CreateRGBSurface(0, tileRes, tileRes, 24, 0, 0, 0, 0);
	SDL_Rect rect;
	rect.w = rect.h = tileRes;
	rect.x = rect.y = 0;

	//chop into tiles
	for (rect.y = 0; rect.y < fullRect.h; rect.y += tileRes)
	{
		for (rect.x = 0; rect.x < fullRect.w; rect.x += tileRes)
		{
			SDL_BlitSurface(fullSurf, &rect, surf, NULL);
			tex = SDL_CreateTextureFromSurface(mainWindow.ren, surf);
			map.push_back(tex);
		}
	}


	SDL_FreeSurface(surf);
	SDL_FreeSurface(fullSurf);

	cout << "Ok" << endl;
}