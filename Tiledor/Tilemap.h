#pragma once

#include <vector>
#include <string>

using namespace std;

class Tilemap
{
public:
	Tilemap();
	~Tilemap();

	char tileRes;
	int vertiTiles;
	int horiTiles;
	string bitMapName;
	vector<char> tiles;
	SDL_Texture* fullTex;

	void loadFile(const string &_file);
	void saveFile(const string &_file);
	void create(char _tileRes, unsigned _vertiTiles, unsigned _horiTiles, const string& _bitMapName);
	void render();
	void update();
};