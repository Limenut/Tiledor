#include "Globals.h"

Window mainWindow;
Window tileWindow;

map<Uint32, Window*> windowMap;
Texturemap texturemap;

Tilemap gTilemap;

Console console(mainWindow.ren);

bool quit = false;