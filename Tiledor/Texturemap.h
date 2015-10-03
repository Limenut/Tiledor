#pragma once

class Texturemap
{
public:
	~Texturemap();
	void makeMap(const string &_file, int tileRes);

	SDL_Texture* fullTex; //this exists on renderer two
	vector<SDL_Texture*> map; // and this on renderer one
	SDL_Rect rect;
};

