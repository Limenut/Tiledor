#pragma once

#include "Selector.h"
#include "Tilemap.h"
#include "Console.h"
#include "Texturemap.h"
#include "Window.h"

#include <vector>
#include <map>

#include <SDL2/SDL.h>
#pragma comment (lib, "SDL2.lib")

using namespace std;

extern map<Uint32, Window*> windowMap;

extern Window mainWindow;
extern Window tileWindow;
extern Texturemap texturemap;

extern Tilemap gTilemap;

extern Console console;

extern bool quit;

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 1024;