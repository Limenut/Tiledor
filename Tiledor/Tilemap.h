#pragma once

#include <vector>
#include <string>

using namespace std;

//this contains the types (indices) of tiles in the game level
//is analogous to the .map format
class Tilemap
{
public:
	Tilemap();
	~Tilemap();

	char tileRes;			//vertical and horizontal resolution of tiles
	int vertiTiles;			//size of map in tiles
	int horiTiles;
	string bitMapName;		//which image the tile textures are fetched from
	vector<char> tiles;		//type of tile
	SDL_Texture* fullTex;	//texture to be rendered

	void loadFile(const string &_file);
	void saveFile(const string &_file);
	void create(char _tileRes, unsigned _vertiTiles, unsigned _horiTiles, const string& _bitMapName);
	void render();
	void update();
};