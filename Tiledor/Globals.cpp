#include "Globals.h"

//SDL_Window* gWindow;
//SDL_Renderer* gRenderer;

//SDL_Window* tileWindow;
//SDL_Renderer* tileRenderer;
Window mainWindow;
Window tileWindow;

map<Uint32, Window*> windowMap;
//vector<SDL_Texture*> texturemap;
Texturemap texturemap;

Tilemap gTilemap;

//Selector selector;

Console console(mainWindow.ren);

bool quit = false;