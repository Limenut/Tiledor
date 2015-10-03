#include "Globals.h"

Selector::Selector()
{
	//init();
}

Selector::~Selector()
{
	SDL_DestroyTexture(tex);
}

void Selector::init(Uint32 _id)
{
	winIndex = _id;
	//currentWin = &mainWindow;
	currentTile = 0;
	tex = nullptr;
	rect.x = rect.y = 0;
	rect.w = rect.h = gTilemap.tileRes;
	makeTex();
}

void Selector::makeTex()
{
	SDL_Renderer **ren = &windowMap[winIndex]->ren;

	//destroy last tex and make new
	SDL_DestroyTexture(tex);
	SDL_Texture *texTarget = SDL_CreateTexture(*ren, SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET, gTilemap.tileRes, gTilemap.tileRes);

	//copy of currently selected tile
	SDL_Texture *current = texturemap.map[currentTile];

	//transparent tile
	SDL_SetTextureBlendMode(texTarget, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(current, 128);

	//render to texture
	SDL_SetRenderTarget(*ren, texTarget);

	SDL_SetRenderDrawColor(*ren, 0, 0, 0, 0);
	SDL_RenderClear(*ren);

	//draw background texture
	SDL_RenderCopy(*ren, current, NULL, NULL);

	//draw red selection thing
	int res = gTilemap.tileRes;
	SDL_SetRenderDrawColor(*ren, 255, 0, 0, 255);

	SDL_RenderDrawLine(*ren, 0, 0, res / 3, 0);
	SDL_RenderDrawLine(*ren, 0, res - 1, res / 3, res - 1);

	SDL_RenderDrawLine(*ren, res * 2 / 3, 0, res - 1, 0);
	SDL_RenderDrawLine(*ren, res * 2 / 3, res - 1, res - 1, res - 1);

	SDL_RenderDrawLine(*ren, 0, 0, 0, res / 3);
	SDL_RenderDrawLine(*ren, res - 1, 0, res - 1, res / 3);

	SDL_RenderDrawLine(*ren, 0, res * 2 / 3, 0, res - 1);
	SDL_RenderDrawLine(*ren, res - 1, res * 2 / 3, res - 1, res - 1);

	//change back to normal
	SDL_SetRenderTarget(*ren, NULL);

	tex = texTarget;
}

void Selector::makeTex2()
{
	SDL_Renderer **ren = &windowMap[winIndex]->ren;

	//destroy last tex and make new
	SDL_DestroyTexture(tex);
	SDL_Texture *texTarget = SDL_CreateTexture(*ren, SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET, gTilemap.tileRes, gTilemap.tileRes);


	//transparent tile
	SDL_SetTextureBlendMode(texTarget, SDL_BLENDMODE_BLEND);

	//render to texture
	SDL_SetRenderTarget(*ren, texTarget);

	SDL_SetRenderDrawColor(*ren, 0, 0, 0, 0);
	SDL_RenderClear(*ren);

	//draw red selection thing
	int res = gTilemap.tileRes;
	SDL_SetRenderDrawColor(*ren, 255, 0, 0, 255);

	SDL_RenderDrawLine(*ren, 0, 0, res / 3, 0);
	SDL_RenderDrawLine(*ren, 0, res - 1, res / 3, res - 1);

	SDL_RenderDrawLine(*ren, res * 2 / 3, 0, res - 1, 0);
	SDL_RenderDrawLine(*ren, res * 2 / 3, res - 1, res - 1, res - 1);

	SDL_RenderDrawLine(*ren, 0, 0, 0, res / 3);
	SDL_RenderDrawLine(*ren, res - 1, 0, res - 1, res / 3);

	SDL_RenderDrawLine(*ren, 0, res * 2 / 3, 0, res - 1);
	SDL_RenderDrawLine(*ren, res - 1, res * 2 / 3, res - 1, res - 1);

	//change back to normal
	SDL_SetRenderTarget(*ren, NULL);

	tex = texTarget;
}