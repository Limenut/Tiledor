#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>  
#include <string>

#include "Globals.h"

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#pragma comment (lib, "SDL2_image.lib")
#pragma comment (lib, "SDL2_ttf.lib")

#ifdef main
#undef main
#endif

struct Mouse
{
	int x;
	int y;
} mouse;

void conSave();

bool init()
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	//Set texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		printf("Warning: Linear texture filtering not enabled!");
	}

	if (!mainWindow.init("Map editor")) return false;
	if (!tileWindow.init("Tile selector")) return false;

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return false;
	}

	//Initialize SDL_TTF
	if (TTF_Init())
	{
		printf("SDL_TTF could not initialize! SDL_TTF Error: %s\n", TTF_GetError());
		return false;
	}

	return true;
}

void close()
{
		IMG_Quit();
		TTF_Quit();
		SDL_Quit();
}

SDL_Texture* loadBitmap(const string &_file)
{
	if (!mainWindow.ren)
	{
		cout << "No renderer! Can't load texture!" << endl;
		return nullptr;
	}

	cout << "Loading " << _file.c_str() << "... ";

	SDL_Surface *surf = IMG_Load(_file.c_str());
	if (!surf)
	{
		cout << "Loading failed" << endl;
		return nullptr;
	}

	SDL_Rect rect;
	SDL_Texture *tex;

	rect.w = surf->w;
	rect.h = surf->h;

	tex = SDL_CreateTextureFromSurface(mainWindow.ren, surf);
	SDL_FreeSurface(surf);
	if (!tex)
	{
		cout << "Creating texture failed" << endl;
		return nullptr;
	}

	cout << "Ok" << endl;

	return tex;
}

/*void makeTexmap(const string &_file, int tileRes)
{
	texturemap.clear();

	if (!gRenderer)
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


	SDL_Rect fullRect;
	fullRect.x = fullRect.y = 0;
	fullRect.w = fullSurf->w;
	fullRect.h = fullSurf->h;


	SDL_Texture *tex;
	SDL_Surface *surf = SDL_CreateRGBSurface(0,tileRes,tileRes, 24, 0, 0, 0, 0);
	SDL_Rect rect;
	rect.w = rect.h = tileRes;
	rect.x = rect.y = 0;

	for (rect.y = 0; rect.y < fullRect.h; rect.y += tileRes)
	{
		for (rect.x = 0; rect.x < fullRect.w; rect.x += tileRes)
		{
			SDL_BlitSurface(fullSurf, &rect, surf, NULL);
			tex = SDL_CreateTextureFromSurface(gRenderer, surf);
			texturemap.push_back(tex);
		}
	}

	SDL_FreeSurface(surf);
	SDL_FreeSurface(fullSurf);

	cout << "Ok" << endl;
}*/

void handleEvents()
{
	SDL_Event e;
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);

	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_WINDOWEVENT)
		{
			windowMap[e.window.windowID]->handleEvents(&e);
		}
		if (e.type == SDL_QUIT)
		{
			quit = true;
		}
		//If mouse event happened
		if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
		{
			//Get mouse position
			SDL_GetMouseState(&mouse.x, &mouse.y);
			int xi = mouse.x / gTilemap.tileRes;
			int yi = mouse.y / gTilemap.tileRes;
			
	
			if (windowMap[e.window.windowID] == &mainWindow)
			{
				mainWindow.sel.rect.x = xi*gTilemap.tileRes;
				mainWindow.sel.rect.y = yi*gTilemap.tileRes;
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					yi += mainWindow.offsetY;
					xi += mainWindow.offsetX;
					if (yi < 0 || yi >= gTilemap.vertiTiles) continue;
					if (xi < 0 || xi >= gTilemap.horiTiles) continue;
					unsigned i = (yi)*gTilemap.horiTiles + xi;
					gTilemap.tiles[i] = mainWindow.sel.currentTile;
					gTilemap.update();
				}
			}
			else if (windowMap[e.window.windowID] == &tileWindow)
			{
				if (e.button.button == SDL_BUTTON_LEFT)
				{	
					unsigned i = (yi)*texturemap.rect.w / gTilemap.tileRes + xi;
					tileWindow.sel.rect.x = xi*gTilemap.tileRes;
					tileWindow.sel.rect.y = yi*gTilemap.tileRes;
					if (i >= texturemap.map.size()) break;
					mainWindow.sel.currentTile = i;
					mainWindow.sel.makeTex();
				}
			}
		}

		if (e.type == SDL_KEYDOWN)
		{
			unsigned tile = 0;
			switch (e.key.keysym.sym)
			{
			case SDLK_0:
			case SDLK_1:
			case SDLK_2:
			case SDLK_3:
			case SDLK_4:
			case SDLK_5:
			case SDLK_6:
			case SDLK_7:
			case SDLK_8:
			case SDLK_9:
			{
				tile = e.key.keysym.sym - SDLK_0;
				if (tile < texturemap.map.size())
				{
					mainWindow.sel.currentTile = tile;
					mainWindow.sel.makeTex();
				}
				break;
			}
			case SDLK_s:
			{
				if (keystate[SDL_SCANCODE_LCTRL])
				{
					conSave();
				}
				break;
			}
			case SDLK_UP: mainWindow.offsetY--; gTilemap.update(); break;
			case SDLK_DOWN: mainWindow.offsetY++; gTilemap.update(); break;
			case SDLK_LEFT: mainWindow.offsetX--; gTilemap.update(); break;
			case SDLK_RIGHT: mainWindow.offsetX++; gTilemap.update(); break;
			}
		}
	}
}

TTF_Font* loadFont(const string &_file, int size)
{
	TTF_Font* font;
	printf("Loading %s... ", _file.c_str());
	font = TTF_OpenFont(_file.c_str(), size);
	if (font)
	{
		printf("success\n");
		return font;
	}
	else 
	{ 
		printf("error!\n");
		return nullptr;
	}
}

bool testFile(const string &_file)
{
	std::ifstream infile(_file.c_str());
	return infile.good();
}

bool conNew();
bool conLoad();

void conBegin()
{
	console.addLine("New map");
	console.addLine("Load map");
	console.highlight();
	console.open();

	while (true)
	{
		if (console.handleEvents() == SDLK_RETURN)
		{
			if (console.selection == 0)
			{
				conNew();
				break;
			}
			else
			{
				conLoad();
				break;
			}
		}
		console.render();
		SDL_RenderPresent(mainWindow.ren);
	}
	console.close();
}

bool conNew()
{
	char tileRes = 0;
	int vertiTiles = 0;
	int horiTiles = 0;
	string bitMapName;
	//vector<char> tiles;


	console.clear();

	console.selection = 0;
	console.highlight();

	console.startInput();

	while (true)
	{
		console.clear();
		console.addLine(console.loadBitText());

		if (console.handleEvents() == SDLK_RETURN && testFile(console.input) && console.input.length())
		{
			bitMapName = console.input;
			console.stopInput();
			break;
		}
		console.render();
		SDL_RenderPresent(mainWindow.ren);
	}

	console.selection = 0;
	console.highlight();

	while (true)
	{
		console.clear();
		console.addLine("Tile size: " + to_string((int)tileRes));
		console.addLine("Vertical tiles: " + to_string(vertiTiles));
		console.addLine("Horizontal tiles: " + to_string(horiTiles));
		console.addLine("Create map");

		switch (console.handleEvents())
		{
		case SDLK_LEFT:
		{
			switch (console.selection)
			{
			case 0: 
			{
				if (tileRes > 0) tileRes--;
				break; 
			}
			case 1:
			{
				if (vertiTiles > 0) vertiTiles--;
				break;
			}
			case 2:
			{
				if (horiTiles > 0) horiTiles--;
				break;
			}
			default: break;
			}
			break;
		}
		case SDLK_RIGHT:
		{
			switch (console.selection)
			{
			case 0:
			{
				if (tileRes < 255) tileRes++;
				break;
			}
			case 1:
			{
				if (vertiTiles < 10000) vertiTiles++;
				break;
			}
			case 2:
			{
				if (horiTiles < 10000) horiTiles++;
				break;
			}
			default: break;
			}
			break;
		}
		case SDLK_RETURN:
		{
			if (console.selection == 3)
				gTilemap.create(tileRes, vertiTiles, horiTiles, bitMapName);
			return true;
		}
		default: break;
		}

		console.render();
		SDL_RenderPresent(mainWindow.ren);
	}

	return false;
}

bool conLoad()
{
	console.selection = 0;
	console.highlight();
	console.startInput();

	while (true)
	{
		console.clear();
		console.addLine(console.loadMapText());
		if (console.handleEvents() == SDLK_RETURN)
		{
			if (testFile(console.input + ".map") && console.input.length())
			{
				gTilemap.loadFile(console.input + ".map");
				console.stopInput();
				console.close();
				return true;
			}
		}
		console.render();
		SDL_RenderPresent(mainWindow.ren);
	}
}

void conSave()
{
	
	console.selection = 0;
	console.highlight();
	console.open();
	console.startInput();

	while (true)
	{
		console.clear();
		console.addLine(console.saveMapText());
		if (console.handleEvents() == SDLK_RETURN && console.input.length())
		{
			gTilemap.saveFile(console.input + ".map");
			break;
		}
		console.render();
		SDL_RenderPresent(mainWindow.ren);
	}
	console.close();
}

int main()
{

	printf("Initializing SDL... ");
	if (init())
	{
		printf("success\n");
	}
	else
	{
		close();
		return 1;
	}

	console.font = loadFont("OCR_A_Std.ttf", 32);

	SDL_SetWindowSize(tileWindow.win, 1,1);
	conBegin();
	

	//gTilemap.loadFile("mappi.map");
	

	texturemap.makeMap(gTilemap.bitMapName, gTilemap.tileRes);
	SDL_SetWindowSize(tileWindow.win, texturemap.rect.w, texturemap.rect.h);

	mainWindow.sel.init(mainWindow.id);
	tileWindow.sel.init(tileWindow.id);
	tileWindow.sel.makeTex();

	gTilemap.update();
	

	while (!quit)
	{
		//if (console.isOpen) console.handleEvents();
		handleEvents();
		

		SDL_SetRenderDrawColor(mainWindow.ren, 0, 0, 0, 255);
		SDL_RenderFillRect(mainWindow.ren, NULL);

		gTilemap.render();

		//render selection
		SDL_RenderCopy(mainWindow.ren, mainWindow.sel.tex, NULL, &mainWindow.sel.rect);

		//if (console.isOpen) console.render();

		SDL_RenderPresent(mainWindow.ren);

		SDL_SetRenderDrawColor(tileWindow.ren, 0, 0, 0, 255);
		SDL_RenderFillRect(tileWindow.ren, NULL);

		SDL_RenderCopy(tileWindow.ren, texturemap.fullTex, NULL, &texturemap.rect);

		//render selection
		SDL_RenderCopy(tileWindow.ren, tileWindow.sel.tex, NULL, &tileWindow.sel.rect);

		SDL_RenderPresent(tileWindow.ren);
	}

	close();
}

//päivityspöh