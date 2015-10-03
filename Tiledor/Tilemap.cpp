#include "Globals.h"
#include <fstream>
#include <iostream>


using namespace std;

Tilemap::Tilemap()
{
	tileRes = 0;
	vertiTiles = 0;
	horiTiles = 0;

	fullTex = nullptr;
}

Tilemap::~Tilemap()
{
	SDL_DestroyTexture(fullTex);
}

void Tilemap::loadFile(const string &_file)
{
	fstream file;
	file.open(_file.c_str(), ios::in | ios::binary);

	file.read((char *)&tileRes, sizeof(tileRes));
	file.read((char *)&vertiTiles, sizeof(vertiTiles));
	file.read((char *)&horiTiles, sizeof(horiTiles));

	char tempChar;

	file.read((char *)&tempChar, sizeof(tempChar));
	while (tempChar)
	{
		bitMapName += tempChar;
		file.read((char *)&tempChar, sizeof(tempChar));
	}

	for (int i = 0; i < vertiTiles; i++)
	{
		for (int j = 0; j < horiTiles; j++)
		{
			file.read((char *)&tempChar, sizeof(tempChar));
			tiles.push_back(tempChar);
		}
	}

	file.close();
}

void Tilemap::saveFile(const string &_file)
{
	fstream file;
	file.open(_file.c_str(), ios::out | ios::binary);

	file.write((char *)&tileRes, sizeof(tileRes));
	file.write((char *)&vertiTiles, sizeof(vertiTiles));
	file.write((char *)&horiTiles, sizeof(horiTiles));
	file << bitMapName << '\0';

	for (auto it = begin(tiles); it != end(tiles); ++it)
	{
		file.write((char *)&*it, sizeof(char));
	}

	file.close();
}

void Tilemap::update()
{
	/*vector<char>::iterator it = tiles.begin();
	for (int i = 0; i < gTilemap.vertiTiles; i++)
	{
		for (int j = 0; j < gTilemap.horiTiles; j++)
		{
			SDL_Texture *tex;
			tex = texturemap.map[*it];

			++it;

			SDL_Rect rect;
			rect.y = i*gTilemap.tileRes;
			rect.x = j*gTilemap.tileRes;
			rect.w = rect.h = gTilemap.tileRes;
			SDL_RenderCopy(gRenderer, tex, NULL, &rect);
		}
	}*/

	SDL_Texture *tempTex = SDL_CreateTexture(mainWindow.ren, SDL_PIXELFORMAT_RGBX8888, SDL_TEXTUREACCESS_TARGET, mainWindow.area.w, mainWindow.area.h);
	SDL_SetRenderTarget(mainWindow.ren, tempTex);

	for (int i = 0; i < mainWindow.area.h / tileRes; i++)
	{
		int y = i + mainWindow.offsetY;
		if (y < 0 || y >= vertiTiles) continue;
		for (int j = 0; j < mainWindow.area.w / tileRes; j++)
		{
			
			int x = j + mainWindow.offsetX;
			if (x < 0 || x >= horiTiles) continue;

			int in = y*horiTiles + x;

			SDL_Texture *tex;
			tex = texturemap.map[tiles[in]];

			SDL_Rect rect;
			rect.y = i*gTilemap.tileRes;
			rect.x = j*gTilemap.tileRes;
			rect.w = rect.h = gTilemap.tileRes;
			SDL_RenderCopy(mainWindow.ren, tex, NULL, &rect);
		}
	}
	fullTex = tempTex;
	SDL_SetRenderTarget(mainWindow.ren, NULL);
}

void Tilemap::render()
{
	SDL_RenderCopy(mainWindow.ren, fullTex, NULL, NULL);
}

void Tilemap::create(char _tileRes, unsigned _vertiTiles, unsigned _horiTiles, const string& _bitMapName)
{
	tileRes = _tileRes;
	vertiTiles = _vertiTiles;
	horiTiles = _horiTiles;
	bitMapName = _bitMapName;
	tiles.resize(_vertiTiles*_horiTiles);
}